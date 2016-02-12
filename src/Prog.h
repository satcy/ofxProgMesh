//
//  ProgMesh.h
//
//  Created by satcy on 2015/07/28.
//
//
#pragma once
#include <stdio.h>

#include <vector>
#include <list>
#include "Vertex.h"
#include "Triangle.h"
#include "Mesh.h"

namespace ofx { namespace ProgMesh {
    
    struct EdgeCollapse
    {
        int _vfrom;
        int _vto;
        set<int> _trisRemoved;
        set<int> _trisAffected;
        
        
        // Used for debugging
        void dumpEdgeCollapse()
        {
            std::cout << "**** Edge Collapse Dump ****" << std::endl;
            
            std::cout << "\tFrom Vert# " << _vfrom << " to Vert# " << _vto << std::endl;
            cout << "\tTris removed:";
            set<int>::iterator pos;
            for (pos = _trisRemoved.begin(); pos != _trisRemoved.end(); ++pos)
            {
                std::cout << " " << *pos;
            }
            cout << std::endl << "\tTris affected:";
            for (pos = _trisAffected.begin(); pos != _trisAffected.end(); ++pos)
            {
                std::cout << " " << *pos;
            }
            std::cout  << std::endl << "**** End of Edge Collapse Dump ****" << std::endl;
        }
    };
    
    // This is a "pointer" to a vertex in a given mesh
    struct VertexPtr
    {
        Mesh* _meshptr;
        int _index; // ptr to vertex position in mesh
        
        bool operator<(const VertexPtr& vp) const
        {
            return (_meshptr->getProgVertex(_index) < vp._meshptr->getProgVertex(vp._index));
        }
    };
    
    
    typedef multiset<VertexPtr, less<VertexPtr> > vertexPtrSet;
    
    class Prog{
    public:
        // Type of progress mesh algorithm
        enum EdgeCost {SHORTEST, MELAX, QUADRIC, QUADRICTRI, MAX_EDGECOST};
        Prog() {percent = 100;};
        Prog(Mesh* mesh, EdgeCost ec = QUADRICTRI);
        
        void setup(Mesh* mesh, EdgeCost ec = QUADRICTRI);
        
        // Collapse one vertex to another.
        bool collapseEdge();
        
        // One vertex will be split into two vertices -- this
        // is the opposite of a collapse
        bool splitVertex();
        
        // number of edge collapses
        int numCollapses() {return _edgeCollList.size();}
        int numEdgeCollapses() {return _edgeCollList.size();}
        
        // number of triangles, and visible triangles in mesh
        int numTris() {return _newmesh.getNumTriangles();}
        int numVisTris() {return _nVisTriangles;}
        
        bool getTri(int i, Triangle& t) {
            t = _newmesh.getTri(i);
            return true;
        }
        
        // Return a short text description of the current Edge Cost method
        char* getEdgeCostDesc();
    public:
        
        void setProgress(float percent);
        float getProgress() { return percent; }
        void readToMesh(ofMesh & mesh);
    
    protected:
        
        float percent;
    
    protected:
        ofVec3f unitcross(ofVec3f a, ofVec3f v) {
            ofVec3f vr(a.y * v.z - a.z * v.y,
                 a.z * v.x - a.x * v.z,
                 a.x * v.y - a.y * v.x);
            vr.normalize();
            return vr;
        }
    protected:
        
        Mesh* _mesh; // original mesh - not changed
        Mesh _newmesh; // we change this one
        
        EdgeCost _cost; // Type of progressive mesh algorithm
        
        list<EdgeCollapse> _edgeCollList; // list of edge collapses
        list<EdgeCollapse>::iterator _edgeCollapseIter;
        
        // functions used to calculate edge collapse costs.  Different
        // methods can be used, depending on user preference.
        double shortEdgeCollapseCost(Mesh& m, Vertex& v);
        double melaxCollapseCost(Mesh& m, Vertex& v);
        double quadricCollapseCost(Mesh& m, Vertex& v);
        
        int _nVisTriangles; // # of triangles, after we collapse edges
        
        // Create the list of the edge collapses used
        // to simplify the mesh.
        void createEdgeCollapseList();
        
        // Used in the QEM edge collapse methods.
        void calcAllQMatrices(Mesh& mesh, bool bUseTriArea); // used for quadric method
        double calcQuadricError(double Qsum[4][4], Vertex& v, double triArea); // used for quadric method
        
        enum {BOUNDARY_WEIGHT = 1000}; // used to weight border edges so they don't collapse
        void applyBorderPenalties(set<Border> &borderSet, Mesh &mesh);
        
        Prog(const Prog&); // don't allow copy ctor -- too expensive
        Prog& operator=(const Prog&); // don't allow assignment op.
        bool operator==(const Prog&); // don't allow op==
        
#ifndef NDEBUG
        // used in debugging
        void assertEveryVertActive(int nVerts, int nTri, Mesh &mesh);
#endif
        // helper function for edge collapse costs
        void calcEdgeCollapseCosts(vertexPtrSet &vertSet, vector<vertexPtrSet::iterator> &vertSetVec,
                                   int nVerts, Mesh &mesh, EdgeCost &cost);
        
        // Calculate the QEM matrices used to computer edge
        // collapse costs.
        void calcQuadricMatrices(EdgeCost &cost, Mesh &mesh);
        
        // We can't collapse Vertex1 to Vertex2 if Vertex2 is invalid.
        // This can happen if Vertex2 was previously collapsed to a
        // separate vertex.
        void insureEdgeCollapseValid(EdgeCollapse &ec, Vertex &vc, Mesh &mesh,
                                     const EdgeCost &cost, bool &bBadVertex);
        
        // Calculate the QEM for the "to vertex" in the edge collapse.
        void setToVertexQuadric(Vertex &to, Vertex &from, const EdgeCost &cost);
        
        // At this point, we have an edge collapse.  We're collapsing the "from vertex"
        // to the "to vertex."  For all the surrounding triangles which use this edge,
        // update "from vertex" to the "to vertex".  Also keep track of the vertices
        // in the surrounding triangles.
        void updateTriangles(EdgeCollapse &ec, Vertex &vc, set<int> &affectedVerts, Mesh &mesh);
        
        
        // These affected vertices are not in the current collapse,
        // but are in the triangles which share the collapsed edge.
        void updateAffectedVertNeighbors(Vertex &vert, const EdgeCollapse &ec,
                                         const set<int> &affectedVerts);
        
        // Reset the edge collapse costs of vertices which were
        // affected by a previous edge collapse.
        void resetAffectedVertCosts(const EdgeCost &cost, Mesh &newmesh, Vertex &vert);
        
        // If this vertex has no active triangles (i.e. triangles which have
        // not been removed from the mesh) then set it to inactive.
        void removeVertIfNecessary(Vertex &vert, vertexPtrSet &vertSet,
                                   vector<vertexPtrSet::iterator> &vertSetVec,
                                   Mesh &mesh, const EdgeCost &cost,
                                   set<int> &affectedQuadricVerts);
        
        // Update the vertices affected by the most recent edge collapse
        void updateAffectedVerts(Mesh &_newmesh, vector<vertexPtrSet::iterator> &vertSetVec,
                                 vertexPtrSet &vertSet, const EdgeCollapse &ec,
                                 set<int> &affectedVerts, const EdgeCost &cost,
                                 set<int> &affectedQuadricVerts);
        
        // Recalculate the QEM matrices (yeah, that's redundant) if we're
        // using the Quadrics to calculate edge collapse costs.
        void recalcQuadricCollapseCosts(set<int> &affectedQuadricVerts, 
                                        Mesh &mesh, const EdgeCost &cost);
        
        // Calculate the list of edge collapses.  Each edge collapse
        // consists of two vertices:  a "from vertex" and a "to vertex".
        // The "from vertex" is collapsed to the "to vertex".  The
        // "from vertex" is removed from the mesh.
        void buildEdgeCollapseList(Mesh &mesh, const EdgeCost &cost, 
                                   list<EdgeCollapse> &_edgeCollList,
                                   vertexPtrSet &vertSet, 
                                   vector<vertexPtrSet::iterator> &vertSetVec);
        
        // Helper function for melaxCollapseCost().  This function
        // will loop through all the triangles to which this vertex
        // belongs.
        void calcMelaxMaxValue(Mesh &mesh, set<int> &adjfaces, 
                               Vertex &v, set<int> &tneighbors,
                               float &retmaxValue, 
                               bool &bMaxValueFound);
    };
}}