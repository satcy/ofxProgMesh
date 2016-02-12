//
//  Triangle.cpp
//
//  Created by satcy on 2015/07/28.
//
//

#include "Triangle.h"

#include <assert.h>
#include "Triangle.h"
#include "Mesh.h"

namespace ofx { namespace ProgMesh {

// retrieve vertices as an array of floats
const float* Triangle::getVert1() {return (_mesh->getProgVertex(_vert1)).getArrayVerts();}
const float* Triangle::getVert2() {return (_mesh->getProgVertex(_vert2)).getArrayVerts();}
const float* Triangle::getVert3() {return (_mesh->getProgVertex(_vert3)).getArrayVerts();}

// retrieve vertices as a vertex object
const Vertex& Triangle::getVert1vertex() const {return _mesh->getProgVertex(_vert1);};
const Vertex& Triangle::getVert2vertex() const {return _mesh->getProgVertex(_vert2);};
const Vertex& Triangle::getVert3vertex() const {return _mesh->getProgVertex(_vert3);};

// Calculate normal of Triangle
void
Triangle::calcNormal()
{
    assert(_mesh);
    
    ofVec3f vec1 = (_mesh->getProgVertex(_vert1)).getXYZ();
    ofVec3f vec2 = (_mesh->getProgVertex(_vert2)).getXYZ();
    ofVec3f vec3 = (_mesh->getProgVertex(_vert3)).getXYZ();
    ofVec3f veca = vec2 - vec1;
    ofVec3f vecb = vec3 - vec2;
    
    _normal = unitcross(veca, vecb);
    // Note that if the Triangle is degenerate (all vertices lie in a line),
    // the normal will be <0,0,0>
    
    // This is the "d" from the plane equation ax + by + cz + d = 0;
    
    _d = -_normal.dot(vec1);
}

// Calculate area of Triangle
float
Triangle::calcArea()
{
    assert(_mesh);
    
    // If a Triangle is defined by 3 points, say p, q and r, then
    // its area is 0.5 * length of ((p - r) cross (q - r))
    // See Real-Time Rendering book, Appendix A
    ofVec3f vec1 = (_mesh->getProgVertex(_vert1)).getXYZ();
    ofVec3f vec2 = (_mesh->getProgVertex(_vert2)).getXYZ();
    ofVec3f vec3 = (_mesh->getProgVertex(_vert3)).getXYZ();
    ofVec3f vecA = vec1 - vec2;
    ofVec3f vecB = vec3 - vec2;
    
    ofVec3f cross = vecA.cross(vecB);
    float area = float(0.5 * cross.length());
    
    return area;
}


// Used for output
std::ostream&
operator<<(std::ostream& os, const Triangle& to)
{
    os << "vert1: " << to._vert1 << " vert2: " << to._vert2 << " vert3: " << to._vert3; // for some reason this isn't working as a friend function, not sure why
    os << " Normal: " << to._normal << " Active? " << to.isActive();
    os << " Index: " << to._index;
    
    // it is pulling ostream from the STL typedef, not the regular ostream, though.
    return os;
}

}}
