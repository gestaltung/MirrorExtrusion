#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofBackground(0,0,0);
    
    //initialize the video grabber
    vidGrabber.setVerbose(true);
    vidGrabber.setup(406,540);
    
    //store the width and height for convenience
    width = vidGrabber.getWidth();
    height = vidGrabber.getHeight();
    
    OF_WIDTH = ofGetScreenWidth();
    OF_HEIGHT = ofGetScreenHeight();
    
    // start the controller thread
    controller.start();
    font.load("futura.ttf", 24);
    
    // Setup GUI panel
    gui.setup("panel");
//    gui.add(extrusionAmount.set("amplitude", 70., 0., 150.));
    gui.add(camZoom.set("camZoom", 455., 255., 1255.));
    
    //add one vertex to the mesh for each pixel
    for (int y = 0; y < height; y+=1){
        for (int x = 0; x<width; x+=1){
            mainMesh.addVertex(ofPoint(x,y,0));	// mesh index = x + y*width
												// this replicates the pixel array within the camera bitmap...
            mainMesh.addColor(ofFloatColor(0,0,0));  // placeholder for colour data, we'll get this from the camera
        }
    }
    
    for (int y = 0; y<height-1; y+=1){
        for (int x=0; x<width-1; x+=1){
            mainMesh.addIndex(x+y*width);				// 0
            mainMesh.addIndex((x+1)+y*width);			// 1
            mainMesh.addIndex(x+(y+1)*width);			// 10
            
            mainMesh.addIndex((x+1)+y*width);			// 1
            mainMesh.addIndex((x+1)+(y+1)*width);		// 11
            mainMesh.addIndex(x+(y+1)*width);			// 10
        }
    }
    
    //this is an annoying thing that is used to flip the camera
    cam.setScale(1,-1,1);
    
    
    //this determines how much we push the meshes out
//    extrusionAmount = 70.0;

}

//--------------------------------------------------------------
void ofApp::update(){
    extrusionAmount = ofMap(controller.totalSteps, 10000, 0, 0, 200.0, true);
    //grab a new frame
    vidGrabber.update();
    
    //update the mesh if we have a new frame
    if (vidGrabber.isFrameNew()){
        //this determines how far we extrude the mesh
        for (int i=0; i<vidGrabber.getWidth()*vidGrabber.getHeight(); i++){
            
            ofFloatColor sampleColor(vidGrabber.getPixels()[i*3]/255.f,				// r
                                     vidGrabber.getPixels()[i*3+1]/255.f,			// g
                                     vidGrabber.getPixels()[i*3+2]/255.f);			// b
            
            //now we get the vertex aat this position
            //we extrude the mesh based on it's brightness
            ofVec3f tmpVec = mainMesh.getVertex(i);
            tmpVec.z = sampleColor.getBrightness() * extrusionAmount;
            mainMesh.setVertex(i, tmpVec);
            
            mainMesh.setColor(i, sampleColor);
        }
    }
    
    //let's move the camera when you move the mouse
    float rotateAmount = ofMap(ofGetMouseY(), 0, ofGetHeight(), 0, 360);
    
    
    //move the camera around the mesh
    ofVec3f camDirection(0,0,1);
    ofVec3f centre(vidGrabber.getWidth()/2.f,vidGrabber.getHeight()/2.f, camZoom/2.f);
    ofVec3f camDirectionRotated = camDirection.getRotated(0, ofVec3f(1,0,0));
    ofVec3f camPosition = centre + camDirectionRotated * 120;
//    ofVec3f camPosition = centre + 120;
    
    cam.setPosition(camPosition);
    cam.lookAt(centre);
}

//--------------------------------------------------------------
void ofApp::draw(){
    gui.draw();
    //we have to disable depth testing to draw the video frame
    ofDisableDepthTest();
    //	vidGrabber.draw(20,20);
    
    //but we want to enable it to show the mesh
    ofEnableDepthTest();
    cam.begin();
    
    //You can either draw the mesh or the wireframe
    // mainMesh.drawWireframe();
    mainMesh.drawFaces();
    cam.end();
    controller.drawMetrics(width,height);
}

//--------------------------------------------------------------
void ofApp::exit() {
    controller.stop();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key=='f') {
        ofToggleFullscreen();
    }
    
    if (key==OF_KEY_LEFT) {
        controller.setDate("20160528");
        controller.getMetricsForDay();
    }
    
    if (key==OF_KEY_RIGHT) {
        controller.setDate("20160601");
        controller.getMetricsForDay();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
