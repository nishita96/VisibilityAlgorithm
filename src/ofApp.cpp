#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Set the background color to white
    ofBackground(255);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // POLYGON
    // Define the points of the concave polygon
    std::vector<ofVec2f> points = {
        ofVec2f(100, 100),
        ofVec2f(300, 100),
        ofVec2f(250, 250),
        ofVec2f(150, 250)
    };
    // Set the color to blue
    ofSetColor(0, 0, 255);
    // Draw the polygon and fill it with the specified color
    ofBeginShape();
    for (auto p : points) {
        ofVertex(p);
    }
    ofEndShape(true);
    
    
//    cout << " x" << ofGetWindowSize().x << " y" << ofGetWindowSize().y;
    
    // CIRCLE
    // Set the color to magenta
    ofSetColor(255, 0, 255);
    // Draw a circle with a radius of 50 pixels at the center of the screen
    ofVec2f center = ofGetWindowSize() / 2;
    center.set(0.0f, 0.0f);
    float radius = 50.0f;
    ofDrawCircle(center, radius);
    
    // LINE SEGMENT
    // Set the line color to green
    ofSetColor(0, 255, 0); // Red color
    // Set the line width
    ofSetLineWidth(3); // Set line width to 3 pixels
    // Draw a line segment between two points
    ofDrawLine(100, 100, 200, 200);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

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
