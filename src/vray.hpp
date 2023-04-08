//
//  vray.hpp
//  PointVisibilityComputationalGeometry
//
//  Created by Nishita Kharche on 4/5/23.
//

#ifndef vray_hpp
#define vray_hpp

#include <stdio.h>
#include "ofMain.h"

#endif /* vray_hpp */



class vray {
    public:
        vray(float _theta, ofVec2f _unitVec, float _r, float _l) :
            theta(_theta), unitVec(_unitVec.x, _unitVec.y), r(_r), l(_l) {}
        
        float theta;
        ofVec2f unitVec;
        float r;
        float l;
        
        void setup();
        void update();
        void draw();
};
