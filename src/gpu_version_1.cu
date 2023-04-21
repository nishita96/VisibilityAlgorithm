
// #include "versions.hpp"
#include <cuda_runtime.h>
#include <vector>
#include <stdio.h>
#include <iostream>
// #include "ofMain.h"

#define PI 3.14

class ofVec2f {
public:
    float x;
    float y;
    
    ofVec2f(float _x, float _y) {
        x = _x;
        y = _y;
    }

    void set(float _x, float _y) {
        x = _x;
        y = _y;
    }

    float angle(ofVec2f v1) {
        return 0;
    }
    float length() {
        return 0;
    }

    ofVec2f getNormalized() {
        ofVec2f new_vec(x, y);
        return new_vec;
    }


};


using namespace std;
#define BLOCK_SIZE 512 //@@ You can change this

class vray {
public:
    vray(float _theta, ofVec2f _unitVec, float _r, float _l) :
        theta(_theta), unitVec(_unitVec.x, _unitVec.y), r(_r), l(_l) {}
    // vray(float _theta, float _r, float _l) :
    //     theta(_theta), r(_r), l(_l) {}
    
    float theta;
    ofVec2f unitVec;
    float r;
    float l;

};

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
    ofVec2f xAxisVec(1.0,0.0);
    float infinity = 9999.0f;
    
    if((seg.p0.x * seg.p1.y - seg.p0.y * seg.p1.x) < 0){ // p0 X p1
        // to ensure p0 theta < p1 theta
        swap(seg.p0, seg.p1);
    }
    float theta0 = xAxisVec.angle(seg.p0);
    float theta1;
    if(xAxisVec.angle(seg.p1) == 0.0f){
        theta1 = 360.0;
    }
    else{
        theta1 = xAxisVec.angle(seg.p1);
    }
    theta0 = theta0 < 0? theta0 + 360.0f : theta0;
    theta1 = theta1 < 0? theta1 + 360.0f : theta1;
    bothVray.push_back(vray(theta0, seg.p0.getNormalized(), infinity, seg.p0.length()));
    bothVray.push_back(vray(theta1, seg.p1.getNormalized(), seg.p1.length(), infinity));
    
    return bothVray;
}


class GPU_V1 {
    float infinity = 9999.0f;
    vector<segment> check_intersections(vector<segment>& segments) {
        vector<segment> listSegmentsCopy;
        for (auto seg: segments){
            if(seg.possibleIntersectionTestXAxis()){
                ofVec2f splitPoint = seg.splitSegmentInto2();
                if(splitPoint.x != -1.0f){ // there is intersection, HENCE split it in 2 segments
                    // TODO check if you need to check which has smaller angle
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
        return listSegmentsCopy;
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

    vector<vray> merge(vector<segment>& listSegmentsCopy) {
        
        
        int minValueR = 9999.0f;
        vector<vray> vrayForMerge;
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
        return vrayForMerge;
    }

    __global__ void preprocess_in_parallel(float* input, int num_input, ofVec2f q, float* output, int& num_output) {

        
        std::cout<<"Hello\n";


    }

    vector<segment> preprocess(vector<segment>& segments, ofVec2f& q) {
            
        float *d_segments = NULL;
        cudaMalloc((void **) &d_segments, segments.size() * sizeof(segment));
        ofVec2f *d_q = NULL;
        cudaMalloc((ofVec2f **) &d_q, sizeof(ofVec2f));
        void *d_vrays = NULL;
        cudaMalloc((void **) &d_vrays, 100 * sizeof(vray));
        cudaError_t err = cudaSuccess;
        err = cudaMemcpy(d_segments, segments.data(), segments.size() * sizeof(segment), cudaMemcpyHostToDevice);
        if (err != cudaSuccess)
        {
            fprintf(stderr, "Failed to allocate d_segments (error code %s)!\n", cudaGetErrorString(err));
            exit(EXIT_FAILURE);
        }
        d_q->x = q.x;
        d_q->y = q.y;
        // err = cudaMemcpy(d_q, q, sizeof(ofVec2f), cudaMemcpyHostToDevice);
        // if (err != cudaSuccess)
        // {
        //     fprintf(stderr, "Failed to allocate d_q (error code %s)!\n", cudaGetErrorString(err));
        //     exit(EXIT_FAILURE);
        // }
        // int threadsPerBlock = 100;
        // int blocksPerGrid = 1;
        // float *d_vray_count = NULL;
        // cudaMalloc((void **) &d_vray_count,sizeof(float));

        // preprocess_in_parallel<<blocksPerGrid, threadsPerBlock>> (d_segments, segments.size(), d_q, d_vrays, d_vray_count);
        // cudaDeviceSynchronize();
        // err = cudaGetLastError();

        // if (err != cudaSuccess)
        // {
        //     fprintf(stderr, "Failed to launch preprocess_in_parallel kernel (error code %s)!\n", cudaGetErrorString(err));
        //     exit(EXIT_FAILURE);
        // }
        // float vray_count = 0;
        // err = cudaMemcpy(vray_count, d_vray_count, size(float), cudaMemcpyDeviceToHost);
        // vray preprocessed_vrays[100];
        // err = cudaMemcpy(preprocessed_vrays, d_vrays, size(vray) * vray_count, cudaMemcpyDeviceToHost);
                
        vector<segment> res;
        return res;
    }

    public:
    virtual vector<vray> process_segments(vector<segment> segments, ofVec2f& q) {
        // start = 0;
        std::cout<<"Starting Process"<<std::endl;
        vector<segment> updated_segments = this->preprocess(segments, q);
        std::cout<<"Preprocess complete"<<std::endl;
        vector<vray> merged_vrays = this->merge(updated_segments);
        std::cout<<"Ending Process"<<std::endl;
        // end = 0;
        return merged_vrays;
    }
};

