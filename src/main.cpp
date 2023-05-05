#include "ofMain.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include "ofApp.h"
#include "segment.hpp"
#include "vray.hpp"



using namespace std;
vector<vray> mergeVrays(vector<vray> l1, vector<vray> l2);
vector<vray> mergeVraysAll(vector<vray> l1, vector<vray> l2);
void printAllSegments(vector<segment> listSegments);
void printAllVrays(vector<vray> vrays);
bool myComparator(vray a, vray b);
const float infinity = 9999.0f;

void divideVrays(vector<vray> v){
    if(v.size() == 1){
//        cout << "\n size one " << v.size();
        return;
    }
    int mid = v.size()/2;
    vector<vray> left;
    for(int i=0; i<mid; i++){
        left.push_back(v.at(i));
    }
//    cout << "\n size left " << left.size();
    vector<vray> right;
    for(int i=mid; i<v.size(); i++){
        right.push_back(v.at(i));
    }
//    cout << "\n size right " << right.size();
    
    divideVrays(left);
    divideVrays(right);
    mergeVrays(left, right);
    return;
}

vector<vray> mergeVrays(vector<vray> l1, vector<vray> l2){
    vector<vray> l;
    
    int n = l1.size() + l2.size();
    int n1 = l1.size();
    int n2 = l2.size();
//    cout <<  "\n size l1:" << l1.size() << " l2:" << l2.size() << " l:" << l.size();
//    cout << "\n l1";
//    printAllVrays(l1);
//    cout << "\n l2";
//    printAllVrays(l2);
    
    int i = 0; // diff from paper
    int i1 = 0;
    int i2 = 0;
    int k = 0;
    int t = 0;
    
    vector<vray> lk;
    vector<vray> lt;
    int ik=0;
    int it=0;
    

    while(i < n){
        if(i2 >= n2){
            k = 1;
            ik = i1;
            lk = l1;
            t = 2;
            it = i2;
            lt = l2;
        }
        else if(i1 >= n1){
            k = 2;
            ik = i2;
            lk = l2;
            t = 1;
            it = i1;
            lt = l1;
        }
        else if(l1.at(i1).theta <= l2.at(i2).theta){
            k = 1;
            ik = i1;
            lk = l1;
            t = 2;
            it = i2;
            lt = l2;
        }
        else{
            k = 2;
            ik = i2;
            lk = l2;
            t = 1;
            it = i1;
            lt = l1;
        }

        l.push_back(lk.at(ik));

        if(0 < it && it < lt.size() && lt.at(it).r < infinity ){
            segment s = segment(lt.at(it-1).unitVec * lt.at(it-1).l, lt.at(it).unitVec * lt.at(it).r);
            float thetaRad = l.at(i).theta * PI / 180.0;
            segment other(ofVec2f(0.0f, 0.0f), ofVec2f(99999.9 * cos(thetaRad), 1500.0f * sin(thetaRad))); // x axis as segment
            ofVec2f p = s.intersectionWithGivenSegment(other);
//            if(p != ofVec2f(0.0f, 0.0f)){
//                cout << "\n point" << p.x << " " << p.y ;
//            }
//            else{
//                cout << "\n it is null - did not find an intersection ";
//            }
            

            l.at(i).l = min(l.at(i).l, p.length());
            l.at(i).r = min(l.at(i).r, p.length());
        }

        if(k == 1){
            i1 = i1 + 1; // can assign k = i1 or i2 initially based on condition so that we  have to chec value of k again n again
        }
        else{
            i2 = i2 + 1;
        }
        i = i + 1;
    }
    
//    cout << "\n l";
//    printAllVrays(l);
    return l;
}

void printAllSegments(vector<segment> listSegments){
    // cout << "\n DISPLAYING ALL SEGMENTS";
    // for (auto seg: listSegments){
    //     cout << "\n seg (" << seg.p0.x << ", " << seg.p0.y << "), (" << seg.p1.x << ", " << seg.p1.y << ") ";
    // }
    // cout << "\n";
}

void printAllVrays(vector<vray> vrays){
    // cout << "\n DISPLAY VRAYS";
    // for (auto ray : vrays) {
    //     cout << "\n theta:" << ray.theta << ", unitvec(" << ray.unitVec.x << "," << ray.unitVec.y << "), r:" << ray.r << ", l:" << ray.l ;
    // }
    // cout << "\n";
}

bool myComparator(vray a, vray b){
    return a.theta < b.theta;
}

vector<ofVec2f> calculatePointsForTriangles(vector<vray> vrayForMerge, ofVec2f pointQ){
    vector<ofVec2f> pointsPolygon;
    vray v = vrayForMerge.at(0);
    float xx = 0.0f;
    float yy = 0.0f;
    
    pointsPolygon.push_back(ofVec2f(infinity, pointQ.y));
//    cout << "\n - : 1500.0 " << pointQ.y ;
    for(int i=0; i<vrayForMerge.size(); i++){
        v = vrayForMerge.at(i);
        
        if(v.r == 0.0 && v.l == 0.0){ // takes care of that null issue which I am not able to pass
            continue;
        }
        else if(v.theta == 360.0 && i != vrayForMerge.size()-1){
            continue;
        }
        
        xx = (v.unitVec * v.r).x + pointQ.x;
        yy = (-(v.unitVec * v.r).y) + pointQ.y;
        pointsPolygon.push_back(ofVec2f(xx, yy)); // adding point
//        cout << "\n r : " << xx << " " << yy ;
        pointsPolygon.push_back(ofVec2f(pointQ.x, pointQ.y)); // adding point
//        cout << "\n - : " << pointQ.x << " " << pointQ.y ;
//        cout << "\n ----------------------------------" ;
        xx = (v.unitVec * v.l).x + pointQ.x;
        yy = (-(v.unitVec * v.l).y) + pointQ.y;
        pointsPolygon.push_back(ofVec2f(xx, yy)); // adding point
//        cout << "\n l : " << xx << " " << yy ;
    };
    pointsPolygon.push_back(ofVec2f(infinity, pointQ.y));
//    cout << "\n - : 1500.0 " << pointQ.y ;
    pointsPolygon.push_back(ofVec2f(pointQ.x, pointQ.y));
//    cout << "\n - : " << pointQ.x << " " << pointQ.y ;
//    cout << "\n size:" << pointsPolygon.size();
//    ofAppNew.pointsToDraw = pointsPolygon;
    
    return pointsPolygon;
}



vector<segment> generateSegments(int n){
    vector<segment> ret;
    // int full = n%
    float r = 5.0f;
    float theta = 0.1f;
    for (int i=0; i<n; i++) {
        r = r + 3.0;
        theta = theta + 0.1;
        float x0 = r * cos(theta) + 500;
        float y0 = r * sin(theta) + 400;
        r = r + 3.0;
        theta = theta + 0.1;
        float x1 = r * cos(theta)+ 500;
        float y1 = r * sin(theta)+ 400;
//        cout << x0 << y0 << y0 << y1<< endl;
        ret.push_back(segment(ofVec2f(x0, y0), ofVec2f(x1, y1)));
    }
    return ret;
}


void test_performance() {

    int segment_count = 450;
    int iteration = 500;

    vector<segment> listSegments = generateSegments(segment_count);
    cout<<"TESTING "<<segment_count<< " in "<<iteration<<" iterations"<<endl;

    ofApp ofAppNew;
    ofVec2f pointQ(500,400);
    ofAppNew.q = pointQ;
    ofAppNew.setOfSegmentsOriginal = listSegments;
    ofAppNew.setOfSegmentsToDraw = listSegments;
    ofAppNew.setOfSegments = listSegments;

    
    
    

    // GPU_V1 *gpu = new GPU_V1();
    
    double timeCpu = 0.00;
    vector<segment> updated_segments;
    for(int i=0; i<iteration; i++){
        auto begin = chrono::high_resolution_clock::now();    
        ofAppNew.doPreprocessing();
        auto end = chrono::high_resolution_clock::now();    
        auto dur = end - begin;
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
        
        timeCpu = timeCpu + ms;

        
    }
    cout.precision(17);
    cout << fixed << "\n Preprocessing in CPU: " << timeCpu<<endl;

    // vector<vray> initial_vrays;
    // for(int i =0; i<updated_segments.size(); i++){
    //     segment s = updated_segments.at(i);
    //     initial_vrays.push_back(s.generateVray(s).at(0));
    //     initial_vrays.push_back(s.generateVray(s).at(1));
    // }
    
    timeCpu = 0.00;
    for(int i=0; i<iteration; i++){
        auto begin = chrono::high_resolution_clock::now();    
        ofAppNew.mergeSequantially(9999.0);
        auto end = chrono::high_resolution_clock::now();    
        auto dur = end - begin;
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
        timeCpu = timeCpu + ms;
    }
    cout.precision(17);
    cout << fixed << "\n Merge Sequential time in CPU: " << timeCpu<<endl;
}

int main( ){

//    test_performance();


    // do all processing
    
     ofApp ofAppNew;
    
    
    
     // ISSUE the lines have to be non intersecting - how to generate them dynamically?
    
     // ---- define the point Q
     ofVec2f pointQ(500,400); //= ofGetWindowSize() / 2;
     ofAppNew.q = pointQ;

     int numSegments = 100;
     vector<segment> listSegmentsGenerated = generateSegments(numSegments);
    
     // ---- made a set of segments covering cases
     vector<segment> listSegments = listSegmentsGenerated; //= {
         // segment(ofVec2f(600.0f, 550.0f), ofVec2f(650.0f, 400.0f)),  // right small, line to split at 0 degree
         // segment(ofVec2f(100.0f, 500.0f), ofVec2f(350.0f, 300.0f)),  // connected pair above
         // segment(ofVec2f(550.0f, 700.0f), ofVec2f(100.0f, 500.0f)),  // connected pair bottom
         // segment(ofVec2f(600.0f, 600.0f), ofVec2f(800.0f, 150.0f)),  // right side long
         // segment(ofVec2f(200.0f, 300.0f), ofVec2f(700.0f, 100.0f)),  // top horizontal

 // //        segment(ofVec2f(800.0f, 500.0f), ofVec2f(800.0f, 500.01f)),   // the line for ending
 //         segment(ofVec2f(450.0f, 450.0f), ofVec2f(400.0f, 400.0f))   // the collinear line
 //     };
    
     // ---- add segments to draw
     ofAppNew.setOfSegmentsOriginal = listSegments;
     ofAppNew.setOfSegmentsToDraw = listSegments;
     ofAppNew.setOfSegments = listSegments;


    // TODO ignore the point 0.0f,0.0f , might make some cases bad
    

    cout << "\n End of processing.";
    
    
    // POINTS TO DRAW THE POLYGON
//    // ---- points to draw polygon
//    vector<ofVec2f> pointsPolygon;
//    pointsPolygon = calculatePointsForTriangles(vrayForMerge, pointQ);
//    ofAppNew.pointsForTriangle = pointsPolygon;

    
    // TODO take care of when angle is 180, the trianlge looks like a line
    

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

 
