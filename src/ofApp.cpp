#include "ofApp.h"
#include "cpu_version.hpp"

// float infinity = 9999.0f;
//--------------------------------------------------------------
void ofApp::setup(){
    // Set the background color to white
    ofBackground(255, 255, 255);
    doEverything();
    
    
//
//    // PREPROCESS
//    // ---- translate all segments to q (q becomes origin)
//    for(int i=0; i< setOfSegments.size(); i++){
//        setOfSegments.at(i).translateToQ(q);
//    }
////    cout << "\n size after transalation to Q " << listSegments.size() ;
//
//    // ---- removing points collinear (checking - if area of triangle Q p0 p1 = 0 then they are collinear)
//    for (vector<segment>::iterator it = setOfSegments.begin(); it != setOfSegments.end();){
//        if(it->collinearWithQ() == 0.0f){
//            setOfSegments.erase(it); // automatically iterates to next item after erasing
//        }
//        else{
//            ++it;
//        }
//    }
////    cout << "\n size after removing collinear segments " << listSegments.size() ;
//
//    // ---- divde segment into 2 is cuts xaxis properly (slope != 0)
////    vector<segment>::iterator it = listSegments.begin(); // TODO has some issue, removes the wrong segment
////    vector<segment> listSegmentsCopy; // TODO find way to not have to make this copy
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
//
//
//
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
//
//
//    // POINTS TO DRAW THE POLYGON
//    // ---- points to draw polygon
//    vector<ofVec2f> pointsPolygon;
//    pointsPolygon = calculatePointsForTriangles(vrayForMerge, q);
//    pointsForTriangle = pointsPolygon;
//
//
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofVec2f pp1;
    pp1.set(50.0f, 50.0f);
    
//    cout << " here" << seg1.p0.x << " " << seg1.p0.y;
    

    // --- drawing the triangles showing visible area ( POLYGON )
    // Define the points of the concave polygon - pointsToDraw
    ofSetColor(204, 229, 255); // Set the color to blue
    
    for(int i=0; i<pointsForTriangle.size(); i=i+3){
        ofBeginShape();
        ofVertex(pointsForTriangle.at(i));
        ofVertex(pointsForTriangle.at(i+1));
        ofVertex(pointsForTriangle.at(i+2));
        ofEndShape(true);
    }
    
//    ofBeginShape(); // Draw the polygon and fill it with the specified color
//    for (auto seg : pointsToDraw) {
//        ofVertex(seg);
//    }
//    ofEndShape(true);
//    cout << " x" << ofGetWindowSize().x << " y" << ofGetWindowSize().y;
    
    
    // ---- the center point ( CIRCLE )
    ofSetColor(204, 0, 0);
    float radiusP = 4.0f;
    ofDrawCircle(q, radiusP);
    
    // ---- line segments ( LINE SEGMENT )
    ofSetColor(0, 102, 51); // Set the line color to Red color
    ofSetLineWidth(3); // Set line width to 3 pixels
    for (auto s : setOfSegmentsToDraw) {
        ofDrawLine(s.p0.x, s.p0.y, s.p1.x, s.p1.y); // Draw a line segment between two points
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
//    cout << "test mouse drag " << x << " , " << y << endl;
    q.set(x, y);
    doEverything();
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}




void ofApp::doEverything(){

    CPU* cpu = new CPU();
    setOfSegments = setOfSegmentsOriginal;
    vrayForMerge = cpu->process_segments(setOfSegments, q);

    
    
//     // PREPROCESS
//     // ---- translate all segments to q (q becomes origin)
//     for(int i=0; i< setOfSegments.size(); i++){
//         setOfSegments.at(i).translateToQ(q);
//     }
// //    cout << "\n size after transalation to Q " << listSegments.size() ;
    
//     // ---- removing points collinear (checking - if area of triangle Q p0 p1 = 0 then they are collinear)
//     for (vector<segment>::iterator it = setOfSegments.begin(); it != setOfSegments.end();){
//         if(it->collinearWithQ() == 0.0f){
//             setOfSegments.erase(it); // automatically iterates to next item after erasing
//         }
//         else{
//             ++it;
//         }
//     }
// //    cout << "\n size after removing collinear segments " << listSegments.size() ;
    
//     // ---- divde segment into 2 is cuts xaxis properly (slope != 0)
// //    vector<segment>::iterator it = listSegments.begin(); // TODO has some issue, removes the wrong segment
// //    vector<segment> listSegmentsCopy; // TODO find way to not have to make this copy
//     listSegmentsCopy.clear();
//     bool splitsAtX = false;
//     for (auto seg: setOfSegments){
//         if(seg.possibleIntersectionTestXAxis()){
//             ofVec2f splitPoint = seg.splitSegmentInto2();
//             if(splitPoint.x != -1.0f){ // there is intersection, HENCE split it in 2 segments
//                 // TODO check if you need to check which has smaller angle
//                 splitsAtX = true;
//                 listSegmentsCopy.push_back(segment(seg.p0, splitPoint));
//                 listSegmentsCopy.push_back(segment(seg.p1, splitPoint));
//             }
//             else{
//                 listSegmentsCopy.push_back(segment(seg.p0, seg.p1));
//             }
//         }
//         else{
//             listSegmentsCopy.push_back(segment(seg.p0, seg.p1));
//         }
//     }
// //    cout << "\n size after spliting xaxis segments(copy) " << listSegmentsCopy.size() << "\n" ;
    
    
    
//     // MAKE THE V RAYS // TODO like the pseudocode, we only need minValueR here so creating vrays is not needed
//     // to handle the edge case where more than 1 line cuts at xaxis
//     int minValueR = 9999.0f;
    
//     mergeSequantially(minValueR);
    
    drawPolygon();

    
}


void ofApp::mergeSequantially(int minValueR){
    // MERGE, TODO currently sequential, creating vrays on the fly 
//    vector<vray> vrayForMerge;
    vrayForMerge.clear();
    vrayForMerge.push_back(listSegmentsCopy.at(0).generateVray(listSegmentsCopy.at(0)).at(0));
    vrayForMerge.push_back(listSegmentsCopy.at(0).generateVray(listSegmentsCopy.at(0)).at(1));
    if(vrayForMerge.at(0).theta == 360.0f && vrayForMerge.at(0).r < minValueR){
        minValueR = vrayForMerge.at(0).r;
    }
    if(vrayForMerge.at(1).theta == 360.0f && vrayForMerge.at(1).r < minValueR){
        minValueR = vrayForMerge.at(1).r;
    }
    vector<vray> vrayNewPair;
    for (int i=1; i<listSegmentsCopy.size(); i++){
        segment seg = listSegmentsCopy.at(i);
        vrayNewPair.clear();
        vrayNewPair.push_back(seg.generateVray(seg).at(0));
        vrayNewPair.push_back(seg.generateVray(seg).at(1));
        if(vrayNewPair.at(0).theta == 360.0f && vrayNewPair.at(0).r < minValueR){
            minValueR = vrayNewPair.at(0).r;
        }
        if(vrayNewPair.at(1).theta == 360.0f && vrayNewPair.at(1).r < minValueR){
            minValueR = vrayNewPair.at(1).r;
        }
        vrayForMerge = mergeVrays(vrayForMerge, vrayNewPair);
    }
    
    vrayForMerge.push_back(vray(360.0, ofVec2f(1.0f, 0.0f), minValueR, infinity));
    
}


void ofApp::drawPolygon(){
    // POINTS TO DRAW THE POLYGON
    // ---- points to draw polygon
    vector<ofVec2f> pointsPolygon;
    pointsPolygon = calculatePointsForTriangles(vrayForMerge, q);
    pointsForTriangle = pointsPolygon;
}


vector<ofVec2f> ofApp::calculatePointsForTriangles(vector<vray> vrayForMerge, ofVec2f pointQ){
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



vector<vray> ofApp::mergeVrays(vector<vray> l1, vector<vray> l2){
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
