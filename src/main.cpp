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
            segment other(ofVec2f(0.0f, 0.0f), ofVec2f(1500.0f * cos(thetaRad), 1500.0f * sin(thetaRad))); // x axis as segment
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


// void test_preprocessing() {

//     vector<int> all_time;
//     int iteration = 500;
//     double timeDuration = 0.0;
//     for(int i=0;i<iteration;i++) {
//         vector<segment> listSegments = {
//             segment(ofVec2f(600.0f, 550.0f), ofVec2f(650.0f, 400.0f)),  // right small, line to split at 0 degree
//             segment(ofVec2f(100.0f, 500.0f), ofVec2f(350.0f, 300.0f)),  // connected pair above
//             segment(ofVec2f(550.0f, 700.0f), ofVec2f(100.0f, 500.0f)),  // connected pair bottom
//             segment(ofVec2f(600.0f, 600.0f), ofVec2f(800.0f, 150.0f)),  // right side long
//             segment(ofVec2f(200.0f, 300.0f), ofVec2f(700.0f, 100.0f)),  // top horizontal

//     //        segment(ofVec2f(800.0f, 500.0f), ofVec2f(800.0f, 500.01f)),   // the line for ending
//             segment(ofVec2f(450.0f, 450.0f), ofVec2f(400.0f, 400.0f))   // the collinear line
//         };
//         ofVec2f pointQ(500,400);
//         clock_t begin = clock();
        

//     // PREPROCESS
//    // ---- translate all segments to q (q becomes origin)
//    for(int i=0; i< setOfSegments.size(); i++){
//        setOfSegments.at(i).translateToQ(q);
//    }
// //    cout << "\n size after transalation to Q " << listSegments.size() ;

//    // ---- removing points collinear (checking - if area of triangle Q p0 p1 = 0 then they are collinear)
//    for (vector<segment>::iterator it = setOfSegments.begin(); it != setOfSegments.end();){
//        if(it->collinearWithQ() == 0.0f){
//            setOfSegments.erase(it); // automatically iterates to next item after erasing
//        }
//        else{
//            ++it;
//        }
//    }
// //    cout << "\n size after removing collinear segments " << listSegments.size() ;

//    // ---- divde segment into 2 is cuts xaxis properly (slope != 0)
// //    vector<segment>::iterator it = listSegments.begin(); // TODO has some issue, removes the wrong segment
// //    vector<segment> listSegmentsCopy; // TODO find way to not have to make this copy
//    listSegmentsCopy.clear();
//    bool splitsAtX = false;
//    for (auto seg: setOfSegments){
//        if(seg.possibleIntersectionTestXAxis()){
//            ofVec2f splitPoint = seg.splitSegmentInto2();
//            if(splitPoint.x != -1.0f){ // there is intersection, HENCE split it in 2 segments
//                // TODO check if you need to check which has smaller angle
//                splitsAtX = true;
//                listSegmentsCopy.push_back(segment(seg.p0, splitPoint));
//                listSegmentsCopy.push_back(segment(seg.p1, splitPoint));
//            }
//            else{
//                listSegmentsCopy.push_back(segment(seg.p0, seg.p1));
//            }
//        }
//        else{
//            listSegmentsCopy.push_back(segment(seg.p0, seg.p1));
//        }
//    }
// //    cout << "\n size after spliting xaxis segments(copy) " << listSegmentsCopy.size() << "\n" ;







//         clock_t end = clock();
//         double duration = double(end-begin) / CLOCKS_PER_SEC;
//         // auto duration = std::chrono::duration_cast<std::chr
//         timeDuration = timeDuration + duration;
//     }
//     //return timeDuration/iteration;//(float)std::reduce(all_time.begin(), all_time.end())/(float) all_time.size();
//     cout << "\n Preprocessing in CPU: " << timeDuration/iteration;
// }

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
        cout << x0 << y0 << y0 << y1<< endl;
        ret.push_back(segment(ofVec2f(x0, y0), ofVec2f(x1, y1)));
    }
    return ret;
}

int main( ){

    // test_preprocessing();


    // do all processing
    
    ofApp ofAppNew;
    
    
    
    // ISSUE the lines have to be non intersecting - how to generate them dynamically?
    
    // ---- define the point Q
    ofVec2f pointQ(500,400); //= ofGetWindowSize() / 2;
    ofAppNew.q = pointQ;

    int numSegments = 1000;
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

    int iteration = 500;
    double timeCpu = 0.00;
    for(int i=0; i<iteration; i++){
        time_t start = time(NULL);

        ofAppNew.doPreprocessing();

        time_t end = time(NULL);
        // double duration = double(end-begin) / CLOCKS_PER_SEC;
        double duration = double(end-start);
        timeCpu = timeCpu + duration;
    }
    cout.precision(17);
    cout << fixed << "\n Preprocessing in CPU: " << timeCpu/iteration;



    timeCpu = 0.00;
    for(int i=0; i<iteration; i++){
        time_t start = time(NULL);

        ofAppNew.mergeSequantially();

        time_t end = time(NULL);
        // double duration = double(end-begin) / CLOCKS_PER_SEC;
        double duration = double(end-start);
        timeCpu = timeCpu + duration;
    }
    cout.precision(17);
    cout << fixed << "\n Merge Sequential time in CPU: " << timeCpu/iteration;




    
    
//    // PREPROCESS
//    // ---- translate all segments to q (q becomes origin)
//    for(int i=0; i< listSegments.size(); i++){
//        listSegments.at(i).translateToQ(pointQ);
//    }
////    cout << "\n size after transalation to Q " << listSegments.size() ;
//
//    // ---- removing points collinear (checking - if area of triangle Q p0 p1 = 0 then they are collinear)
//    for (vector<segment>::iterator it = listSegments.begin(); it != listSegments.end();){
//        if(it->collinearWithQ() == 0.0f){
//            listSegments.erase(it); // automatically iterates to next item after erasing
//        }
//        else{
//            ++it;
//        }
//    }
////    cout << "\n size after removing collinear segments " << listSegments.size() ;
//
//    // ---- divde segment into 2 is cuts xaxis properly (slope != 0)
////    vector<segment>::iterator it = listSegments.begin(); // TODO has some issue, removes the wrong segment
//    vector<segment> listSegmentsCopy; // TODO find way to not have to make this copy
//    bool splitsAtX = false;
//    for (auto seg: listSegments){
//        if(seg.possibleIntersectionTestXAxis()){
//            ofVec2f splitPoint = seg.splitSegmentInto2();
//            if(splitPoint.x != -1.0f){ // there is intersection, HENCE split it in 2 segments
//                // TODO check if you need to check which has smaller angle
//                splitsAtX = true;
//                listSegmentsCopy.push_back(segment(seg.p0, splitPoint));
//                listSegmentsCopy.push_back(segment(seg.p1, splitPoint));
//            }
//            else{
//                listSegmentsCopy.push_back(segment(seg.p0, seg.p1));
//            }
//        }
//        else{
//            listSegmentsCopy.push_back(segment(seg.p0, seg.p1));
//        }
//    }
////    cout << "\n size after spliting xaxis segments(copy) " << listSegmentsCopy.size() << "\n" ;
//
//
//
    
//    // MAKE THE V RAYS // TODO like the pseudocode
//    vector<vray> vrays;
//    ofVec2f xAxisVec(1,0);
//    for (auto seg : listSegmentsCopy){
//        vrays.push_back(seg.generateVray(seg).at(0));
//        vrays.push_back(seg.generateVray(seg).at(1));
//    }
////    cout << "\n all vrays: ";
////    printAllVrays(vrays);
//    
//    // to handle the edge case where more than 1 line cuts at xaxis
//    int minValueR = 9999.0f;
//    if(splitsAtX){
//        for(auto vray : vrays){
//            if(vray.theta == 360.0f && vray.r < minValueR){
//                minValueR = vray.r;
//            }
//        }
//    }
    
//    // MERGE, TODO currently sequential
//    vector<vray> vrayForMerge;
//    vrayForMerge.push_back(listSegmentsCopy.at(0).generateVray(listSegmentsCopy.at(0)).at(0));
//    vrayForMerge.push_back(listSegmentsCopy.at(0).generateVray(listSegmentsCopy.at(0)).at(1));
//    vector<vray> vrayNewPair;
//    for (int i=1; i<listSegmentsCopy.size(); i++){
//        segment seg = listSegmentsCopy.at(i);
//        vrayNewPair.clear();
//        vrayNewPair.push_back(seg.generateVray(seg).at(0));
//        vrayNewPair.push_back(seg.generateVray(seg).at(1));
//        vrayForMerge = mergeVrays(vrayForMerge, vrayNewPair);
//    }
//
//    vrayForMerge.push_back(vray(360.0, ofVec2f(1.0f, 0.0f), minValueR, infinity));
//
//    cout << "\n after merge vrays: ";
//    printAllVrays(vrayForMerge);

    
    // TODO ignore the point 0.0f,0.0f , might make some cases bad
    

    cout << "\n end of processing.";
    
    
//    // POINTS TO DRAW THE POLYGON
//    // ---- points to draw polygon
//    vector<ofVec2f> pointsPolygon;
//    pointsPolygon = calculatePointsForTriangles(vrayForMerge, pointQ);
//    ofAppNew.pointsForTriangle = pointsPolygon;

    
    // TODO take care of when angle is 180, the trianlge looks like a line
    

	ofSetupOpenGL(1024,768, OF_WINDOW);			// <-------- setup the GL context
    cout << "\n sajdhfgiuashgfuywasgifuhasdgkjhsadfgkjhdsgkjsfdhg";
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	// ofRunApp( &ofAppNew);

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
