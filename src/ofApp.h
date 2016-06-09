#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxJSON.h"
#include "controller.hpp"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    int width, height;
    int OF_WIDTH, OF_HEIGHT;
    
    ofCamera cam;
//    float extrusionAmount;
    ofVboMesh mainMesh;
    ofVideoGrabber vidGrabber;
    
    ofxPanel gui;
//    ofParameter<float> extrusionAmount;
    ofParameter<float> camZoom;
    float extrusionAmount;
    // JSON requests
    GSTController controller;
    
    ofTrueTypeFont font;
    
};
