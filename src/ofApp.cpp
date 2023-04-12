#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Set the background color to white
    ofBackground(50, 50, 50);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofVec2f pp1;
    pp1.set(50.0f, 50.0f);
    
//    cout << " here" << seg1.p0.x << " " << seg1.p0.y;
    
    
    // POLYGON
    // Define the points of the concave polygon - pointsToDraw
    ofSetColor(0, 0, 255); // Set the color to blue
    ofBeginShape(); // Draw the polygon and fill it with the specified color
    for (auto seg : pointsToDraw) {
        ofVertex(seg);
    }
    ofEndShape(true);
//    cout << " x" << ofGetWindowSize().x << " y" << ofGetWindowSize().y;
    
    
    //  - the center point ( CIRCLE )
    ofSetColor(255, 0, 0);
    float radiusP = 3.0f;
    ofDrawCircle(q, radiusP);
    
    // line segments ( LINE SEGMENT )
    ofSetColor(0, 255, 0); // Set the line color to Red color
    ofSetLineWidth(3); // Set line width to 3 pixels
    for (auto s : setOfSegments) {
        ofDrawLine(s.p0.x, s.p0.y, s.p1.x, s.p1.y); // Draw a line segment between two points
    }
    
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
