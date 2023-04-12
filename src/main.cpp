#include "ofMain.h"
#include <iostream>
#include <vector>
#include <cmath>

#include "ofApp.h"
#include "segment.hpp"
#include "vray.hpp"



using namespace std;

void printAllSegments(vector<segment> listSegments){
    cout << "\n DISPLAYING ALL SEGMENTS";
    for (auto seg: listSegments){
        cout << "\n seg (" << seg.p0.x << ", " << seg.p0.y << "), (" << seg.p1.x << ", " << seg.p1.y << ") ";
    }
}

const float infinity = 1500.0f;

int main( ){
    // do all processing
//    INFINITY = 1500.0f;
    
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
        listSegments.at(i).translateToQ(pointQ);
    }
    cout << "\n size after transalation to Q " << listSegments.size() ;
    
    // ---- removing points collinear (checking - if area of triangle Q p0 p1 = 0 then they are collinear)
    for (vector<segment>::iterator it = listSegments.begin(); it != listSegments.end();){
        if(it->collinearWithQ() == 0.0f){
            listSegments.erase(it); // automatically iterates to next item after erasing
        }
        else{
            ++it;
        }
    }
    cout << "\n size after removing collinear segments " << listSegments.size() ;
    
    // ---- divde segment into 2 is cuts xaxis properly (slope != 0)
//    vector<segment>::iterator it = listSegments.begin(); // has some issue, removes the wrong segment
    vector<segment> listSegmentsCopy; // TODO find way to not have to make this copy
    for (auto seg: listSegments){
        if(seg.possibleIntersectionTestXAxis()){
            ofVec2f splitPoint = seg.splitSegmentInto2();
            if(splitPoint.x != -1.0f){ // there is intersection, HENCE split it in 2 segments
                listSegmentsCopy.push_back(segment(seg.p0, splitPoint));
                listSegmentsCopy.push_back(segment(seg.p1, splitPoint));
            }
            else{
                listSegmentsCopy.push_back(segment(seg.p0, seg.p1));
            }
        }
        else{
            listSegmentsCopy.push_back(segment(seg.p0, seg.p1));
        }
    }
    cout << "\n size after spliting xaxis segments(copy) " << listSegmentsCopy.size() << "\n" ;
    
    
    
    
    // MAKE THE V RAYS
    vector<vray> vrays;
    ofVec2f xAxisVec(1,0);
    for (auto seg : listSegmentsCopy){
        bool p0isleft = false;
        if((seg.p0.x * seg.p1.y - seg.p0.y * seg.p1.x)<0){ // p0 X p1
            // finds out which side is this point so that accordingly we can decide r and l
            p0isleft = true;
        }

        float theta;
        ofVec2f endPoint;
        float r;
        float l;

        // making ray from p0
        endPoint = ofVec2f(seg.p0.x, -seg.p0.y); // coz coordinate system is downward +y
        theta = xAxisVec.angle(endPoint);
        theta = theta < 0? theta + 360.0f : theta;
        r = p0isleft? endPoint.length() : infinity;
        l = !p0isleft? endPoint.length() : infinity;
//        cout << "\n p0 (" << endPoint.x << " " << endPoint.y << ") theta:" << theta << ", r:" << r << ", l:" << l ;
        vrays.push_back(vray(theta, endPoint.getNormalized(), r, l)); //theta,unitvec,right,left

        // making ray from p1
        endPoint = ofVec2f(seg.p1.x, -seg.p1.y); // coz coordinate system is downward +y
        theta = xAxisVec.angle(endPoint);
        theta = theta < 0? theta + 360.0f : theta;
        r = !p0isleft? endPoint.length() : infinity;
        l = p0isleft? endPoint.length() : infinity;
//        cout << "\n p1 (" << endPoint.x << " " << endPoint.y << ") theta:" << theta << ", r:" << r << ", l:" << l ;
        vrays.push_back(vray(theta, endPoint.getNormalized(), r, l));
    }
    for (auto ray : vrays) {
        cout << "\n theta:" << ray.theta << ", r:" << ray.r << ", l:" << ray.l ;
    }
    
    
    
    cout << "\n ";
	ofSetupOpenGL(1024,768, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( &ofAppNew);

}

/* class SEGMENT
 
 point = 2 float values for X and Y
 segment = set of 2 points
 rays = point (for start) and direction (vector with 2 components X and Y
 output = region (set rays or bounded areas)
 
 input = point Q AND set of line segment (point p0, point p1)

*/


/*
 SCRAP CODE WITH GOOD CPP OPTIONS/IDEAS
 
 //        segment& seg = listSegments.at(i); //by reference-not a memory address,wont have illegal values (null)
 //        seg.p0.set(seg.p0.x - pointQ.x, seg.p0.y - pointQ.y);
 //        seg.p1.set(seg.p1.x - pointQ.x, seg.p1.y - pointQ.y);
 
 //        float angle = atan2(-findAngle.y, findAngle.x);
 //        float theta = angle * ( 180 / PI );
 //        theta = (theta < 0)? theta + 360.0f : theta;
 
 
 
 */
