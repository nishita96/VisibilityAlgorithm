//
//  segment.hpp
//  PointVisibilityComputationalGeometry
//
//  Created by Nishita Kharche on 4/4/23.
//

//#ifndef segment_hpp
//#define segment_hpp
//
//
//
//#endif /* segment_hpp */
#include <stdio.h>
#include <utility>
#include "ofMain.h"

using namespace std;

class segment {
    public:
//        Segment(pair<float, float> e0, pair<float, float> e1) :
//            p0(e0.first, e0.second), p1(e1.first, e1.second) {}
        segment(ofVec2f e0, ofVec2f e1) :
            p0(e0.x, e0.y), p1(e1.x, e1.y) {}
    
//        pair<float, float> getP0() const { return p0; }
//        pair<float, float> getP1() const { return p1; }
    
        ofVec2f p0;
        ofVec2f p1;
    
        void setup();
        void update();
        void draw();
        
};

