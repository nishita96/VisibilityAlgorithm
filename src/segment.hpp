//
//  segment.hpp
//  VisibilityAlgorithm
//
//  Created by Nishita Kharche on 4/4/23.
//
#pragma once
#include <stdio.h>
#include <utility>
#include "ofMain.h"
#include "vray.h"


using namespace std;

class segment {
    public:
        segment(ofVec2f e0, ofVec2f e1) :
            p0(e0.x, e0.y), p1(e1.x, e1.y) {}
    
        ofVec2f p0;
        ofVec2f p1;
        
        int signum(float f);
        void translateToQ(ofVec2f q);
        bool possibleIntersectionTestXAxis();
        ofVec2f splitSegmentInto2();
        ofVec2f intersectionWithGivenSegment(segment other);
        bool collinearWithQ(); // q is always 0 after translation
        vector<vray> generateVray(segment seg);
};
