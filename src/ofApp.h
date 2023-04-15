#pragma once
#include "ofMain.h"
#include "segment.hpp"


class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
//        vector<ofVec2f> pointsToDraw;
        ofVec2f q;
        vector<segment> setOfSegmentsToDraw;
        vector<segment> setOfSegments;
        vector<segment> setOfSegmentsOriginal;
        vector<segment> listSegmentsCopy;
        vector<ofVec2f> pointsForTriangle;
        vector<vray> vrayForMerge;
        vector<vray> mergeVrays(vector<vray> l1, vector<vray> l2);
        vector<ofVec2f> calculatePointsForTriangles(vector<vray> vrayForMerge, ofVec2f pointQ);
        void doEverything();
        void mergeSequantially(int minValueR);
        void drawPolygon();
};
