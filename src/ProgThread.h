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
    ofMesh target;
    
    ofThreadChannel<float> toProgress;
    ofThreadChannel<ofMesh> fromProgress;
    
    bool newFrame;
    
public:
    ProgThread(){
        setProgress(100.0);
        
        startThread();
    }
    
    ~ProgThread(){
        toProgress.close();
        fromProgress.close();
        stopThread();
    }
    
    void loadPLY(string path){
        filepath = path;
        
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
    
    void update(ofMesh & mesh){
        newFrame = false;
        while(fromProgress.tryReceive(target)){
            newFrame = true;
        }
        if ( newFrame ) {
            mesh = target;
        }
    }
    
    void setProgress(float percent) {
        if ( toProgress.empty() ) toProgress.send(percent);
    }
    
    void stop() {
        stopThread();
    }
    
    void threadedFunction() {
        while(toProgress.receive(target_percent)){
            if ( g_pProgMesh == NULL ) {
                g_pMesh.load(filepath);
                g_pMesh.Normalize();
                
                g_pProgMesh = new Prog(&g_pMesh, Prog::QUADRICTRI );
                
                cout << "ProgThread: " << g_pProgMesh->numVisTris() << endl;
            }
            if (g_pProgMesh) {
                g_pProgMesh->setProgress(target_percent);
                //if ( target ) {
                    ofMesh mesh;
                    g_pProgMesh->readToMesh(mesh);
                    if (fromProgress.empty() ) fromProgress.send(mesh);
                //}
            }
        }
    }
    
};
}}