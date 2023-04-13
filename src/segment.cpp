//
//  segment.cpp
//  VisibilityAlgorithm
//
//  Created by Nishita Kharche on 4/4/23.
//

#include "segment.hpp"


void segment::translateToQ(ofVec2f pointQ){
    p0.set(p0.x - pointQ.x, -(p0.y - pointQ.y)); // because display needs oroginal coordinates but geomterically the y direction is opposite
    p1.set(p1.x - pointQ.x, -(p1.y - pointQ.y));
}


int segment::signum(float f) {
   if (f > 0) return 1;
   if (f < 0) return -1;
   return 0;
}


bool segment::possibleIntersectionTestXAxis(){
    // TODO: Implement A Method To Find The Intersection Between 2 Axis Aligned Bounding Boxes
    if(p0.x < 0 && p1.x < 0){
        return false;
    }
    if(p0.y < 0 && p1.y < 0){
        return false;
    }
    if(p0.y > 0 && p1.y > 0){
        return false;
    }
    return true;
  }


ofVec2f segment::splitSegmentInto2(){
    // TODO: Implement A Fast Method To Find The Edge Intersection Point.
    // Should return the intersection point or null, if no intersection exists.
    //  Care should be taken to make the implementation CORRECT, but SPEED MATTERS.
     
    segment other(ofVec2f(0.0f, 0.0f), ofVec2f(1500.0f, 0.0f)); // x axis as segment
    //do the p1 - p0 and use that so that number of multiplication is less
    float p0x = other.p0.x;
    float p0y = other.p0.y;
    float p1x = other.p1.x;
    float p1y = other.p1.y;
    float p2x = p0.x;
    float p2y = p0.y;
    float p3x = p1.x;
    float p3y = p1.y;
    float s = (p2y*p1x - p0y*p1x - p2y*p0x + p0y*p0x - p2x*p1y + p0x*p1y + p2x*p0y - p0x*p0y)/(p3x*p1y - p2x*p1y - p3x*p0y + p2x*p0y - p3y*p1x + p2y*p1x + p3y*p0x - p2y*p0x);
    if(0 < s && s < 1){ // does not take the end points
        float t = (p2x + ((p3x - p2x) * s) - p0x)/(p1x - p0x);
        if(0 < t && t < 1){ // does not take the end points
             float pointX = p0x + ((p1x - p0x) * t);
             float pointY = p0y + ((p1y - p0y) * t);
             return ofVec2f(pointX, pointY);
        }
    }
    return ofVec2f(-1.0f, 0.0f); // if no intersection THEN returned point has x<0
}


ofVec2f segment::intersectionWithGivenSegment(segment other){
    // TODO: Implement A Fast Method To Find The Edge Intersection Point.
    // Should return the intersection point or null, if no intersection exists.
    //  Care should be taken to make the implementation CORRECT, but SPEED MATTERS.
     
//    segment other(ofVec2f(0.0f, 0.0f), ofVec2f(1500.0f, 0.0f)); // x axis as segment
    //do the p1 - p0 and use that so that number of multiplication is less
    float p0x = other.p0.x;
    float p0y = other.p0.y;
    float p1x = other.p1.x;
    float p1y = other.p1.y;
    float p2x = p0.x;
    float p2y = p0.y;
    float p3x = p1.x;
    float p3y = p1.y;
    float s = (p2y*p1x - p0y*p1x - p2y*p0x + p0y*p0x - p2x*p1y + p0x*p1y + p2x*p0y - p0x*p0y)/(p3x*p1y - p2x*p1y - p3x*p0y + p2x*p0y - p3y*p1x + p2y*p1x + p3y*p0x - p2y*p0x);
    if(0 < s && s < 1){ // does not take the end points
        float t = (p2x + ((p3x - p2x) * s) - p0x)/(p1x - p0x);
        if(0 < t && t < 1){ // does not take the end points
             float pointX = p0x + ((p1x - p0x) * t);
             float pointY = p0y + ((p1y - p0y) * t);
             return ofVec2f(pointX, pointY);
        }
    }
    return ofVec2f(0.0f, 0.0f); // if no intersection THEN returned point has x<0, TODO how to pass null
}


bool segment::collinearWithQ(){
    return abs(p0.x * p1.y - p0.y * p1.x)/2;
}


vector<vray> segment::generateVray(segment seg){
    vector<vray> bothVray;
    ofVec2f xAxisVec(1,0);
    float infinity = 1500.0f;
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
    endPoint = ofVec2f(seg.p0.x, seg.p0.y); // coz coordinate system is downward +y
    theta = xAxisVec.angle(endPoint);
    if(p0isleft && theta == 0.0f){
        theta = 360.0f;
    }
    theta = theta < 0? theta + 360.0f : theta;
    r = p0isleft? endPoint.length() : infinity;
    l = !p0isleft? endPoint.length() : infinity;
//        cout << "\n p0 (" << endPoint.x << " " << endPoint.y << ") theta:" << theta << ", r:" << r << ", l:" << l ;
    bothVray.push_back(vray(theta, endPoint.getNormalized(), r, l)); //theta,unitvec,right,left

    
    // making ray from p1
    endPoint = ofVec2f(seg.p1.x, seg.p1.y); // coz coordinate system is downward +y
    theta = xAxisVec.angle(endPoint);
    if(!p0isleft && theta == 0.0f){
        theta = 360.0f;
    }
    theta = theta < 0? theta + 360.0f : theta;
    r = !p0isleft? endPoint.length() : infinity;
    l = p0isleft? endPoint.length() : infinity;
//        cout << "\n p1 (" << endPoint.x << " " << endPoint.y << ") theta:" << theta << ", r:" << r << ", l:" << l ;
    bothVray.push_back(vray(theta, endPoint.getNormalized(), r, l));
    
    return bothVray;
}
