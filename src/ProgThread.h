//
//  ProgThread.h
//
//  Created by satcy on 2015/07/28.
//
//

#pragma once

#include "ofThread.h"

#include "Mesh.h"
#include "Prog.h"

namespace ofx { namespace ProgMesh {
    

class ProgThread : public ofThread{
protected:
    // Triangle model
    Mesh g_pMesh;
    // Progressive Mesh
    Prog* g_pProgMesh = NULL;
    
    string filepath;
    
    float target_percent;
    mutable ofMesh * target;
    
public:
    
    ProgThread(){
        target_percent = 100.0;
    }
    
    void loadPLY(string path){
        filepath = path;
        if ( isThreadRunning() ) {
            stop();
        }
        
        delete g_pProgMesh;
        g_pProgMesh = NULL;
        
        startThread();
    }
    
    void setFromMesh(ofMesh & mesh){
        
        delete g_pProgMesh;
        g_pProgMesh = NULL;
        
        g_pMesh = mesh;
        g_pMesh.Normalize();
        
        g_pProgMesh = new Prog(&g_pMesh, Prog::QUADRICTRI );
    }
    
    void setTargetMesh(ofMesh & _target){
        target = &_target;
    }
    
    void setProgress(float percent) {
        if ( target_percent == percent ) return;
        target_percent = percent;
        if ( isThreadRunning() ) return;
        startThread();
    }
    
    
    void stop() {
        stopThread();
    }
    
    void threadedFunction() {
        if(lock()) {
            
            if ( g_pProgMesh == NULL ) {
                g_pMesh.load(filepath);
                g_pMesh.Normalize();
                
                g_pProgMesh = new Prog(&g_pMesh, Prog::QUADRICTRI );
            }
            
            if (g_pProgMesh) {
                //cout << g_pProgMesh->numVisTris() << endl;
                g_pProgMesh->setProgress(target_percent);
                
                if ( target ) {
                    ofVboMesh mesh;
                    g_pProgMesh->readToMesh(mesh);
                    target->clear();
                    target->append(mesh);
                    //target->mergeDuplicateVertices();
                }
            }
            
            unlock();
            
            stop();
        }
    }
    
};
}}