//
//  Triangle.h
//
//  Created by satcy on 2015/07/28.
//
//
#pragma  once

#include <stdio.h>
#include "ofVec3f.h"




namespace ofx { namespace ProgMesh {
    class Vertex;
    
    class Mesh;
    
    class Triangle{
    public:
        // Constructors
        Triangle() :
        _vert1(-1), _vert2(-1), _vert3(-1) ,
        _mesh(0), bActive(false), _index(-1) {};
        
        Triangle(int v1, int v2, int v3) :
        _vert1(v1), _vert2(v2), _vert3(v3), _mesh(0), bActive(true),
        _index(-1) {};
        
        Triangle(Mesh* mp, int v1, int v2, int v3) :
        _vert1(v1), _vert2(v2), _vert3(v3), _mesh(mp), bActive(true),
        _index(-1)
        {
            assert(mp);
            
            calcNormal();
            
        };
        
        
        // copy ctor
        Triangle(const Triangle& t) :
        _vert1(t._vert1), _vert2(t._vert2), _vert3(t._vert3),
        _mesh(t._mesh), bActive(t.bActive),
        _index(t._index)
        {
            calcNormal();
        };
        
        // assignment operator
        Triangle& operator=(const Triangle& t)
        {
            if (&t == this) return *this; // check for assignment to self
            _vert1 = t._vert1;
            _vert2 = t._vert2;
            _vert3 = t._vert3;
            _mesh = t._mesh;
            _normal = t._normal;
            bActive = t.bActive;
            _index = t._index;
            return *this;
        }
        
        // assumes pointing to same list of verts
        bool operator==(const Triangle& t)
        {
            return (_vert1 == t._vert1 &&
                    _vert2 == t._vert2 &&
                    _vert3 == t._vert3 &&
                    _mesh == t._mesh
                    );
        }
        
        // reset the normal for the triangle
        void calcNormal();
        
        void changeMesh(Mesh* mp) {_mesh = mp;};
        
        // if the triangle is not active, we set a flag.
        bool isActive() const {return bActive;};
        void setActive(bool b) {bActive = b;};
        
        bool hasVertex(int vi) {
            return	(vi == _vert1 ||
                     vi == _vert2 ||
                     vi == _vert3);}
        
        // When we collapse an edge, we may change the
        // vertex of a triangle.
        void changeVertex(int vFrom, int vTo)
        {
            assert(vFrom != vTo);
            assert(vFrom == _vert1 || vFrom == _vert2 || vFrom == _vert3);
            if (vFrom == _vert1) {
                _vert1 = vTo;
            }
            else if (vFrom == _vert2) {
                _vert2 = vTo;
            }
            else if (vFrom == _vert3) {
                _vert3 = vTo;
            }
            else {
                //!FIX error
            }
        }
        
        void getVerts(int& v1, int& v2, int& v3) {v1=_vert1;v2=_vert2;v3=_vert3;}
        
        const float* getVert1();
        const float* getVert2();
        const float* getVert3();
        
        const Vertex& getVert1vertex() const;
        const Vertex& getVert2vertex() const;
        const Vertex& getVert3vertex() const;
        
        float* getNormal() {_normArray[0]=_normal.x;
            _normArray[1]=_normal.y;
            _normArray[2]=_normal.z;
            return _normArray;}
        
        float calcArea(); // returns area of triangle
        
        friend std::ostream& operator<<(std::ostream& os, const Triangle& to);
        
        int getVert1Index() const {return _vert1;}
        int getVert2Index() const {return _vert2;}
        int getVert3Index() const {return _vert3;}
        const ofVec3f& getNormalVec3() const {return _normal;}
        
        int getIndex() const {return _index;}
        void setIndex(int i) {_index = i;}
        
        // 'd' is from the plane equation ax + by + cz + d = 0
        float getD() const {return _d;}
    protected:
        ofVec3f unitcross(ofVec3f a, ofVec3f v) {
            ofVec3f vr(a.y * v.z - a.z * v.y,
                       a.z * v.x - a.x * v.z,
                       a.x * v.y - a.y * v.x);
            vr.normalize();
            return vr;
        }
    protected:
        
        int _vert1;
        int _vert2;
        int _vert3;
        
        ofVec3f _normal; // normal to plane
        float _normArray[3];
        
        float _d;	// This parameter is the "d" in the
        // plane equation ax + by + cz + d = 0
        // The plane equation of this triangle is used.
        
        bool bActive; // active flag
        
        Mesh* _mesh;
        
        int _index; // index in list of triangles w/in mesh
    };

}}