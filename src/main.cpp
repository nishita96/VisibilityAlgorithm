#include "ofMain.h"
#include <iostream>
#include <vector>
#include <cmath>

#include "ofApp.h"
#include "segment.hpp"
#include "vray.hpp"



using namespace std;
vector<vray> mergeVrays(vector<vray> l1, vector<vray> l2);
vector<vray> mergeVraysAll(vector<vray> l1, vector<vray> l2);
void printAllSegments(vector<segment> listSegments);
void printAllVrays(vector<vray> vrays);
bool myComparator(vray a, vray b);
const float infinity = 1500.0f;

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
    mergeVraysAll(left, right);
    return;
}


vector<vray> mergeVraysAll(vector<vray> l1, vector<vray> l2){
    vector<vray> l;
    
    int n = l1.size() + l2.size();
    int n1 = l1.size();
    int n2 = l2.size();
    cout <<  "\n size l1:" << l1.size() << " l2:" << l2.size() << " l:" << l.size();
    cout << "\n l1";
    printAllVrays(l1);
    cout << "\n l2";
    printAllVrays(l2);
    
    int i = 0; // diff from paper
    int i1 = 0;
    int i2 = 0;
    int k = 0;
    int t = 0;
    
    vector<vray> lk;
    vector<vray> lt;
    int ik;
    int it;
    

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
        cout << "\n here: " << l.at(i).theta << " " << l.at(i).r << " " << l.at(i).l;

        
//        if(0 < it && it <= lt.size() && lt.at(it-1).r < infinity ){
        if(0 < it && it < lt.size() && lt.at(it).r < infinity ){
            cout << "\n entering:";
            segment s = segment(lt.at(it-1).unitVec * lt.at(it-1).l, lt.at(it).unitVec * lt.at(it).r);
            float thetaRad = l.at(i).theta * PI / 180.0;
            segment other(ofVec2f(0.0f, 0.0f), ofVec2f(1500.0f * cos(thetaRad), 1500.0f * sin(thetaRad))); // x axis as segment
            ofVec2f p = s.intersectionWithGivenSegment(other);
            if(p != ofVec2f(0.0f, 0.0f)){
                cout << "\n point" << p.x << " " << p.y ;
//                l.at(i).l = min(l.at(i).l, p.length());
//                l.at(i).r = min(l.at(i).r, p.length());
            }
            else{
                cout << "\n it is null - did not find an intersection ";
            }
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
    
    cout << "\n l";
    printAllVrays(l);
    return l;
}

vector<vray> mergeVrays(vector<vray> l1, vector<vray> l2){
    vector<vray> l;
    
    int n = l1.size() + l2.size();
    int n1 = l1.size();
    int n2 = l2.size();
    cout <<  "\n size l1:" << l1.size() << " l2:" << l2.size() << " l:" << l.size();
    cout << "\n l1";
    printAllVrays(l1);
    cout << "\n l2";
    printAllVrays(l2);
    
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
            if(p != ofVec2f(0.0f, 0.0f)){
                cout << "\n point" << p.x << " " << p.y ;
            }
            else{
                cout << "\n it is null - did not find an intersection ";
            }
            

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
    
    cout << "\n l";
    printAllVrays(l);
    return l;
}

void printAllSegments(vector<segment> listSegments){
    cout << "\n DISPLAYING ALL SEGMENTS";
    for (auto seg: listSegments){
        cout << "\n seg (" << seg.p0.x << ", " << seg.p0.y << "), (" << seg.p1.x << ", " << seg.p1.y << ") ";
    }
    cout << "\n";
}

void printAllVrays(vector<vray> vrays){
    cout << "\n DISPLAY VRAYS";
    for (auto ray : vrays) {
        cout << "\n theta:" << ray.theta << ", unitvec(" << ray.unitVec.x << "," << ray.unitVec.y << "), r:" << ray.r << ", l:" << ray.l ;
    }
    cout << "\n";
}

bool myComparator(vray a, vray b){
    return a.theta < b.theta;
}

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
    
    
    
    
    // ---- define the point Q
    ofVec2f pointQ(500,500); //= ofGetWindowSize() / 2;
    ofAppNew.q = pointQ;
    
    // ---- made a set of segments covering cases
    vector<segment> listSegments = {
//        segment(ofVec2f(600.0f, 550.0f), ofVec2f(650.0f, 400.0f)),  // right small, line to split at 0 degree
        segment(ofVec2f(100.0f, 500.0f), ofVec2f(350.0f, 300.0f)),  // connected pair above
        segment(ofVec2f(550.0f, 700.0f), ofVec2f(100.0f, 500.0f)),  // connected pair bottom
//        segment(ofVec2f(600.0f, 600.0f), ofVec2f(800.0f, 150.0f)),  // right side long
//        segment(ofVec2f(200.0f, 300.0f), ofVec2f(700.0f, 100.0f)),  // top horizontal
        segment(ofVec2f(450.0f, 450.0f), ofVec2f(400.0f, 400.0f))   // the collinear line
    };
    
    // ---- add segments to draw
    ofAppNew.setOfSegments = listSegments;
    
    // PREPROCESS
    // ---- translate all segments to q (q becomes origin)
    for(int i=0; i< listSegments.size(); i++){
        listSegments.at(i).translateToQ(pointQ);
    }
//    cout << "\n size after transalation to Q " << listSegments.size() ;
    
    // ---- removing points collinear (checking - if area of triangle Q p0 p1 = 0 then they are collinear)
    for (vector<segment>::iterator it = listSegments.begin(); it != listSegments.end();){
        if(it->collinearWithQ() == 0.0f){
            listSegments.erase(it); // automatically iterates to next item after erasing
        }
        else{
            ++it;
        }
    }
//    cout << "\n size after removing collinear segments " << listSegments.size() ;
    
    // ---- divde segment into 2 is cuts xaxis properly (slope != 0)
//    vector<segment>::iterator it = listSegments.begin(); // TODO has some issue, removes the wrong segment
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
//    cout << "\n size after spliting xaxis segments(copy) " << listSegmentsCopy.size() << "\n" ;
    
    
    
    
    // MAKE THE V RAYS // TODO like the pseudocode
    vector<vray> vrays;
    ofVec2f xAxisVec(1,0);
    for (auto seg : listSegmentsCopy){
        vrays.push_back(seg.generateVray(seg).at(0));
        vrays.push_back(seg.generateVray(seg).at(1));
    }
    cout << "\n all vrays: ";
    printAllVrays(vrays);
    
    // ---- sorting based on angels
//    sort(vrays.begin(), vrays.end(), myComparator); // sorting based on angle, TODO check if this is the best way
//    // TODO check if you need handling vrays with same theta
//    cout << "\n after sorting: ";
//    printAllVrays(vrays);

    
    // MERGE
//    vector<vray> ll;
//    vray start(0.0, ofVec2f(1500.0, 0.0), 1500.0, 1500.0);
//    vray end(360.0, ofVec2f(1500.0, 0.0), 1500.0, 1500.0);
//    ll.push_back(start);
//    for(int i=0; i<vrays.size(); i=i+1){
//        ll.push_back(vrays.at(i));
//    }
//    ll.push_back(end);
//    cout << "\n before merging: ";
//    printAllVrays(ll);
//    divideVrays(ll);
//    cout << "\n after merging: ";
//    printAllVrays(ll);
    
    
    
//    divideVrays(vrays);
//    cout << "\n after merging: ";
//    printAllVrays(vrays);
    
    
//    // working - pair top and pair bottom
//    cout << "\n SEE FROM HERE";
//    vector<vray> l1;
//    l1.push_back(vrays.at(3));
//    l1.push_back(vrays.at(2));
//    vector<vray> l2;
//    l2.push_back(vrays.at(1));
//    l2.push_back(vrays.at(0));
//    vector<vray> l = mergeVrays(l1, l2);
    
    
//    // working - top horizontal and pair top line
//    cout << "\n SEE FROM HERE";
//    vector<vray> l1;
//    l1.push_back(vrays.at(3));
//    l1.push_back(vrays.at(2));
//    vector<vray> l2;
//    l2.push_back(vrays.at(1));
//    l2.push_back(vrays.at(0));
//    vector<vray> l = mergeVrays(l1, l2);

    
//    //  working
//    cout << "\n SEE FROM HERE";
//    vector<vray> l1;
//    l1.push_back(vray(33.69, ofVec2f(0.83205,0.5547), 1500.0, 180.278));
//    l1.push_back(vray(63.435, ofVec2f(0.447214,0.894427), 447.214, 1500.0));
//    vector<vray> l2;
//    l2.push_back(vray(49.3987, ofVec2f(0.650791,0.759257), 1500.0, 1500.0));
//    vector<vray> l = mergeVrays(l1, l2);
    
    
//    vector<vray> ll;
//    vray start(0.0, ofVec2f(1500.0, 0.0), 1500.0, 1500.0);
//    vray end(360.0, ofVec2f(1500.0, 0.0), 1500.0, 1500.0);
//    ll.push_back(start);
//    for(int i=2; i<6; i=i+1){
//        ll.push_back(vrays.at(i));
//    }
//    ll.push_back(end);
//    cout << "\n ll size " << ll.size();
//    printAllVrays(ll);
//    cout << "\n dividing: \n";
//    divideVrays(ll); // TODO do not pass size 0 vector
//    cout << "\n result: \n";
//    printAllVrays(ll);
    
    // TODO ignore the point 0.0f,0.0f , might make some cases bad
    

    cout << "\n ";
    
    // TODO UNDO CHANGE, UNCOMMENT
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
