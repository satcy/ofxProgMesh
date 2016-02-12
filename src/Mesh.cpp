//
//  Mesh.cpp
//
//  Created by satcy on 2015/07/28.
//
//

#include "Mesh.h"

namespace ofx { namespace ProgMesh {

Mesh::Mesh(const Mesh& m)
{
    ofVboMesh::operator=(m);
    _numTriangles = m._numTriangles;
    _vlist = m._vlist; // NOTE: triangles are still pointing to original mesh
    _plist = m._plist;
    // NOTE: should reset tris in _vlist, _plist
}

Mesh& Mesh::operator=(const Mesh& m)
{
    if (this == &m) return *this; // don't assign to self
    ofVboMesh::operator=(m);
    _numTriangles = m._numTriangles;
    _vlist = m._vlist; // NOTE: triangles are still pointing to original mesh
    _plist = m._plist;
    // NOTE: should reset tris in _vlist, _plist
    return *this;
}

Mesh& Mesh::operator=(const ofMesh& m)
{
    if (this == &m) return *this; // don't assign to self
    ofVboMesh::operator=(m);
    configure();
    return *this;
}
    
Mesh::~Mesh() {
    _numTriangles = 0;
    _vlist.erase(_vlist.begin(), _vlist.end());
    _plist.erase(_plist.begin(), _plist.end());
}

void Mesh::load(string path){
    ofMesh::load(path);
    configure();
}

void Mesh::configure(){
    int numVerts = getNumVertices();
    _numTriangles = getNumIndices() / 3;
    
    for ( int i=0; i<numVerts; i++ ) {
        ofVec3f vec = getVertex(i);
        Vertex v(vec.x, vec.y, vec.z);
        v.setIndex(i);
        _vlist.push_back(v);
    }
    
    for ( int i = 0; i < _numTriangles; i++)
    {
        int v1, v2, v3;
        v1 = getIndex(i*3);
        v2 = getIndex(i*3+1);
        v3 = getIndex(i*3+2);
        
        
        // make sure verts in correct range
        assert(v1 < numVerts && v2 < getNumVertices() && v3 < numVerts);
        
        Triangle t(this, v1, v2, v3);
        t.setIndex(i);
        
        _plist.push_back(t); // push_back puts a *copy* of the element at the end of the list
        
        // update each vertex w/ its neighbors (vertrices & triangles)
        _vlist[v1].addTriNeighbor(i);
        _vlist[v1].addVertNeighbor(v2);
        _vlist[v1].addVertNeighbor(v3);
        
        _vlist[v2].addTriNeighbor(i);
        _vlist[v2].addVertNeighbor(v1);
        _vlist[v2].addVertNeighbor(v3);
        
        _vlist[v3].addTriNeighbor(i);
        _vlist[v3].addVertNeighbor(v1);
        _vlist[v3].addVertNeighbor(v2);
        
    }
}


// Recalculate the normal for one vertex
void Mesh::calcOneVertNormal(unsigned vert)
{
    Vertex& v = getProgVertex(vert);
    const set<int>& triset = v.getTriNeighbors();
    
    set<int>::iterator iter;
    
    ofVec3f vec;
    
    for (iter = triset.begin(); iter != triset.end(); ++iter)
    {
        // get the triangles for each vertex & add up the normals.
        vec += getTri(*iter).getNormalVec3();
    }
    
    vec.normalize(); // normalize the vertex
    v.setVertNomal(vec);
}


// Calculate the vertex normals after loading the mesh.
void Mesh::calcVertNormals()
{
    // Iterate through the vertices
    for (unsigned i = 0; i < _vlist.size(); ++i)
    {
        calcOneVertNormal(i);
    }
}


// Used for debugging
void Mesh::dump()
{
    std::cout << "*** Mesh Dump ***" << std::endl;
    std::cout << "# of vertices: " << getNumVertices() << std::endl;
    std::cout << "# of triangles: " << _numTriangles << std::endl;
    for (unsigned i = 0; i < _vlist.size(); ++i)
    {
        std::cout << "\tVertex " << i << ": " << _vlist[i] << std::endl;
    }
    std::cout << std::endl;
    for (int i = 0; i < _plist.size(); ++i)
    {
        std::cout << "\tTriangle " << i << ": " << _plist[i] << std::endl;
    }
    std::cout << "*** End of Mesh Dump ***" << std::endl;
    std::cout << std::endl;
}

// Get min, max values of all verts
void Mesh::setMinMax(ofVec3f & min, ofVec3f & max)
{
    max.x = max.y = max.z = -FLT_MAX;
    min.x = min.y = min.z = FLT_MAX;
    
    for (unsigned int i = 0; i < _vlist.size(); ++i)
    {
        const float* pVert = _vlist[i].getArrayVerts();
        if (pVert[0] < min.x) min.x = pVert[0];
        if (pVert[1] < min.y) min.y = pVert[1];
        if (pVert[2] < min.z) min.z = pVert[2];
        if (pVert[0] > max.x) max.x = pVert[0];
        if (pVert[1] > max.y) max.y = pVert[1];
        if (pVert[2] > max.z) max.z = pVert[2];
    }
}

// Center mesh around origin.
// Fit mesh in box from (-1, -1, -1) to (1, 1, 1)
void Mesh::Normalize()
{
    float Scale;
    ofVec3f minv, maxv;
    
    setMinMax(minv, maxv);
    
    ofVec3f dimv = maxv - minv;
    
    if (dimv.x >= dimv.y && dimv.x >= dimv.z) Scale = 2.0f/dimv.x;
    else if (dimv.y >= dimv.x && dimv.y >= dimv.z) Scale = 2.0f/dimv.y;
    else Scale = 2.0f/dimv.z;
    
    ofVec3f transv = minv + maxv;
    
    transv *= 0.5f;
    
    for (unsigned int i = 0; i < _vlist.size(); ++i)
    {
        _vlist[i].getXYZ() -= transv;
        _vlist[i].getXYZ() *= Scale;
    }
}

}}