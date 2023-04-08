#include "ofMain.h"
#include <iostream>
#include "ofApp.h"
#include "segment.hpp"


using namespace std;
int main( ){
    // do all processing
    
    ofApp ofAppNew;
    
    vector<ofVec2f> pointsPolygon = {
        ofVec2f(50, 50),
        ofVec2f(100, 50),
        ofVec2f(150, 100),
        ofVec2f(50, 150)
    };
    ofAppNew.pointsToDraw = pointsPolygon;
    
    ofVec2f pointQ(500,500); //= ofGetWindowSize() / 2;
    ofAppNew.q = pointQ;
    
//    ofVec2f pp1;
//    pp1.set(50.0f, 50.0f);
//    ofVec2f pp2;
//    pp2.set(250.0f, 250.0f);
//    segment seg1(pp1, pp2);
    
    vector<segment> listSegments = {
        segment(ofVec2f(300.0f, 600.0f), ofVec2f(400.0f, 450.0f)),
        segment(ofVec2f(100.0f, 500.0f), ofVec2f(300.0f, 300.0f)),
        segment(ofVec2f(550.0f, 700.0f), ofVec2f(100.0f, 500.0f)),
        segment(ofVec2f(600.0f, 600.0f), ofVec2f(800.0f, 200.0f)),
        segment(ofVec2f(200.0f, 300.0f), ofVec2f(800.0f, 100.0f))
    };
//    ofAppNew.setOfSegments = listSegments;
    
     
	ofSetupOpenGL(1024,768, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( &ofAppNew);

}

/* class segment
 
 point = 2 float values for X and Y
 segment = set of 2 points
 rays = point (for start) and direction (vector with 2 components X and Y
 output = region (set rays or bounded areas)
 
 input = point Q AND set of line segment (point p0, point p1)

*/
