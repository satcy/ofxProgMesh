#include "ofMain.h"
#include "ofxProgMesh.h"

class ofApp : public ofBaseApp{
    
public:
    void setup(){
        ofBackground(0);
        
        ofxProgMesh::Mesh ori;
        ori.load("bun_zipper.ply");
        ori.Normalize();
        
        ofxProgMesh::Prog prog;
        prog.setup(&ori);
        
        
        for ( int i=0; i<5; i++ ) {
            ofVboMesh mesh;
            prog.setProgress(50 - i*12);
            prog.readToMesh(mesh);
            meshes.push_back(mesh);
        }
        
        light.setDirectional();
        light.setOrientation(ofVec3f(ofRandom(360), ofRandom(360)));
    }
    void update(){
        
    }
    
    void draw(){
        ofEnableDepthTest();
        light.enable();
        cam.begin();
        ofScale(200, 200, 200);
        ofPushMatrix();
        ofTranslate(-3.5, 0);
        for ( int i=0; i<meshes.size(); i++ ) {
            ofSetColor(155);
            meshes[i].draw();
            ofSetColor(0,255,255);
            meshes[i].drawWireframe();
            ofTranslate(2, 0);
        }
        ofPopMatrix();
        cam.end();
        light.disable();
    }
    
    
    vector<ofVboMesh> meshes;
    ofEasyCam cam;
    ofLight light;
};


//========================================================================
int main( ){
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
