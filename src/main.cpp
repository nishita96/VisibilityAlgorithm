#include "ofMain.h"
#include <iostream>
#include <vector>
#include <cmath>

#include "ofApp.h"
#include "segment.hpp"
#include "vray.hpp"



using namespace std;
int main( ){
    // do all processing
    
    ofApp ofAppNew;
    
    // -- points to draw polygon
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

    // -- made a set of segments covering cases 
    vector<segment> listSegments = {
        segment(ofVec2f(600.0f, 550.0f), ofVec2f(650.0f, 400.0f)),  // right small, line to split at 0 degree
        segment(ofVec2f(100.0f, 500.0f), ofVec2f(350.0f, 300.0f)),  // connected pair above
        segment(ofVec2f(550.0f, 700.0f), ofVec2f(100.0f, 500.0f)),  // connected pair bottom
        segment(ofVec2f(600.0f, 600.0f), ofVec2f(800.0f, 150.0f)),  // right side long
        segment(ofVec2f(200.0f, 300.0f), ofVec2f(700.0f, 100.0f)),  // top horizontal
        segment(ofVec2f(450.0f, 450.0f), ofVec2f(400.0f, 400.0f))   // the collinear line
    };
    
    // -- add segments to draw
    ofAppNew.setOfSegments = listSegments;
    
    // PREPROCESS
    // ---- translate all segments to q (q becomes origin)
    for(int i=0; i< 6; i++){ // for (auto seg : listSegments) {
//        segment& seg = listSegments.at(i); //by reference, not a memory address, wont have illegal values like null
//        seg.p0.set(seg.p0.x - pointQ.x, seg.p0.y - pointQ.y);
//        seg.p1.set(seg.p1.x - pointQ.x, seg.p1.y - pointQ.y);
        listSegments.at(i).translateToQ(pointQ);
//        listSegments.at(i).p0.set(seg.p0.x - pointQ.x, seg.p0.y - pointQ.y); // check if .at() performs better or worse later
//        cout << "\n here" << seg->p0.x << ", " << seg->p0.y << ", " << seg->p1.x << ", " << seg->p1.y << ", ";
    }
//    cout << "\n here outside " << listSegments.at(5).p0.x << ", " << listSegments.at(5).p0.y ;
//    cout << "\n q " << pointQ.x << " " << pointQ.y ;
    cout << "\n size after transalation to Q " << listSegments.size() ;
    cout << "\n DISPLAY ALL SEGMENTS";
    for (auto seg: listSegments){
        cout << "\n seg (" << seg.p0.x << ", " << seg.p0.y << "), (" << seg.p1.x << ", " << seg.p1.y << ") ";
    }
    
    // ---- removing points collinear (checking - if area of triangle Q p0 p1 = 0 then they are collinear)
    for (vector<segment>::iterator it = listSegments.begin(); it != listSegments.end();){
//        cout << "\n area " << area ;//<< " points " << seg.p0.x << ", " << seg.p0.y << ", " << seg.p1.x << ", " << seg.p1.y << " \n";
        if(it->collinearWithQ() == 0.0f){
            listSegments.erase(it); // automatically iterates to next item after erasing
        }
        else{
            ++it;
        }
    }
    cout << "\n size after removing collinear segments " << listSegments.size() ;
    cout << "\n DISPLAY ALL SEGMENTS";
    for (auto seg: listSegments){
        cout << "\n seg (" << seg.p0.x << ", " << seg.p0.y << "), (" << seg.p1.x << ", " << seg.p1.y << ") ";
    }
    
    // ---- divde segment into 2 is cuts xaxis properly (slope != 0)
//    vector<segment>::iterator it = listSegments.begin(); // has some issue, removes the wrong segment
    vector<segment> listSegmentsCopy; // TODO find way to not have to make this copy
//    int i=0;
    for (auto seg: listSegments){
//        cout << "\n here" << seg.p0.x << ", " << seg.p0.y << ", " << seg.p1.x << ", " << seg.p1.y << ", ";
//        cout << "\n possible: " << seg.possibleIntersectionTestXAxis();
        if(seg.possibleIntersectionTestXAxis()){
            ofVec2f splitPoint = seg.splitSegmentInto2();
//            cout << "\n \t value: x " << splitPoint.x << " y " << splitPoint.y;
            if(splitPoint.x != -1.0f){ // there is intersection, HENCE split it in 2 segments
//                listSegments.erase(it);
//                listSegments.push_back(segment(seg.p0, splitPoint));
//                listSegments.push_back(segment(seg.p1, splitPoint));
                
                listSegmentsCopy.push_back(segment(seg.p0, splitPoint));
                listSegmentsCopy.push_back(segment(seg.p1, splitPoint));
            }
            else{
                listSegmentsCopy.push_back(segment(seg.p0, seg.p1));
//                ++it;
            }
        }
        else{
            listSegmentsCopy.push_back(segment(seg.p0, seg.p1));
//            ++it;
        }
    }
    cout << "\n size after spliting xaxis segments " << listSegmentsCopy.size() ;
    cout << "\n DISPLAY ALL SEGMENTS";
    for (auto seg: listSegmentsCopy){
        cout << "\n seg (" << seg.p0.x << ", " << seg.p0.y << "), (" << seg.p1.x << ", " << seg.p1.y << ") ";
    }

//    // -- add segments to draw
//    ofAppNew.setOfSegments = listSegments;
    
    // MAKE THE V RAYS
    vector<vray> vrays;
    for (auto seg : listSegments){
        ofVec2f findAngle = seg.p0;
        float angle = atan2(-findAngle.y, findAngle.x);
//        cout << "\n " << findAngle.x << " " << findAngle.y << " angle 1 : " << angle* ( 180 / PI );
        findAngle = seg.p1;
        angle = atan2(-findAngle.y, findAngle.x);
//        cout << "\n " << findAngle.x << " " << findAngle.y << " angle 2 : " << angle* ( 180 / PI );
    }
     
    
    
    cout << "\n ";
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
