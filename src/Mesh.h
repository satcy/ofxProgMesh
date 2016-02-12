//
//  Mesh.h
//
//  Created by satcy on 2015/07/28.
//
//

#pragma once

#include <stdio.h>
#include "ofVboMesh.h"
#include "Vertex.h"
#include "Triangle.h"

namespace ofx { namespace ProgMesh {
    
    class Mesh : public ofVboMesh{
    public:
        // Constructors and Destructors
        ~Mesh();
        Mesh(){_numTriangles = 0;};
        Mesh(const Mesh&); // copy ctor
        Mesh& operator=(const Mesh&); // assignment op
        Mesh& operator=(const ofMesh&); // assignment op
        
        // Get list of vertices, triangles
        Vertex& getProgVertex(int index) {return _vlist[index];};
        const Vertex& getProgVertex(int index) const {return _vlist[index];};
        Triangle& getTri(int index) {return _plist[index];};
        const Triangle& getTri(int index) const {return _plist[index];};
        
        void load(string path);
        
        //int getNumVerts() {return _numVerts;};
        //void setNumVerts(int n) {_numVerts = n;};
        int getNumTriangles() {return _numTriangles;};
        void setNumTriangles(int n) {_numTriangles = n;};
        
        void configure();
        
        void Normalize();// center mesh around the origin & shrink to fit in [-1, 1]
        
        void calcOneVertNormal(unsigned vert); // recalc normal for one vertex
        
        void dump(); // print mesh state to cout
        
    protected:
        vector<Vertex> _vlist; // list of vertices in mesh
        vector<Triangle> _plist; // list of triangles in mesh
        
        //int _numVerts;
        int _numTriangles;
        
        bool operator==(const Mesh&); // don't allow op== -- too expensive
        
        void ChangeStrToLower(char* pszUpper)
        {
            for(char* pc = pszUpper; pc < pszUpper + strlen(pszUpper); pc++) {
                *pc = (char)tolower(*pc);
            }
        }
        
        // get bounding box for mesh
        void setMinMax(ofVec3f & min, ofVec3f & max);
        
        void calcAllQMatrices(Mesh& mesh); // used for Quadrics method
        
        void calcVertNormals(); // Calculate the vertex normals after loading the mesh
        
    };
}}

