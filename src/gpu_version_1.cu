
// #include "versions.hpp"
#include <cuda_runtime.h>
#include <vector>
#include <stdio.h>
#include "cuda.h"
#include <iostream>
// #include "ofMain.h"
#include <ctime>
#include <cmath>

#define PI 3.14

class ofVec2f {
public:
    float x;
    float y;
    
    ofVec2f(float _x, float _y) {
        x = _x;
        y = _y;
    }

    __device__ ofVec2f(float _x, float _y, int c) {
        x = _x;
        y = _y;
    }

    __device__ ofVec2f(ofVec2f of, int c) {
        x = of.x;
        y = of.y;
    }

    __device__ void set(float _x, float _y) {
        x = _x;
        y = _y;
    }

    float angle(ofVec2f v1) {
        float dotProduct = 0.0;
        float normA = 0.0;
        float normB = 0.0;

        dotProduct = x*v1.x + y*v1.y; // += a[i] * b[i];
        normA = x*x + y*y; // += a[i] * a[i];
        normB = v1.x*v1.x + v1.y*v1.y; // += b[i] * b[i];


        normA = sqrt(normA);
        normB = sqrt(normB);

        float cosTheta = dotProduct / (normA * normB);
        float theta = acos(cosTheta);

        float crossProduct = x*v1.y + y*v1.x; //a[0]*b[1] - a[1]*b[0]; 
        if (crossProduct < 0) {
            theta = -theta; // adjust theta if cross product is negative
        }
        theta = theta * 180 / M_PI;
        return theta;
    }
    
    float length() {
        return sqrt(x*x + y*y);
    }

    __device__ float lengthGpu() {
        return sqrt(x*x + y*y);
    }

    ofVec2f getNormalized() {
        ofVec2f new_vec(x, y);
        return new_vec;
    }

    __device__ ofVec2f operator*(float scale) {
        ofVec2f vec(1,1);
        vec.x = this->x * scale;
        vec.y = this->y * scale;
        return vec;
    }

};

// __device__ ofVec2f* create_ofvec_obj(int x, int y) {
//     ofVec2f* vec = new ofVec2f(x, y, 0);
//     return vec;
// }

using namespace std;
#define BLOCK_SIZE 512 //@@ You can change this

class vray {
public:
    vray(float _theta, ofVec2f _unitVec, float _r, float _l) :
        theta(_theta), unitVec(_unitVec.x, _unitVec.y), r(_r), l(_l) {}
    __device__ vray(float _theta, ofVec2f _unitVec, float _r, float _l, int n) :
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

    __device__ segment(ofVec2f e0, ofVec2f e1, int c) :
    p0(e0.x, e0.y, 0), p1(e1.x, e1.y, 0) {}

    ofVec2f p0;
    ofVec2f p1;
    bool isValid = false;
    
     int signum(float f);
    __device__ void translateToQ(ofVec2f pointQ){
        // printf("inside translateToQ\n");
        p0.set(p0.x - pointQ.x, -(p0.y - pointQ.y)); // because display needs oroginal coordinates but geomterically the y direction is opposite
        p1.set(p1.x - pointQ.x, -(p1.y - pointQ.y));
    }
    __device__ bool possibleIntersectionTestXAxis();
    __device__ ofVec2f splitSegmentInto2();
    __device__ ofVec2f intersectionWithGivenSegment(segment other);
     __device__ bool collinearWithQ(); // q is always 0 after translation
    vector<vray> generateVray(segment seg);
};



// void segment::translateToQ(ofVec2f pointQ){
//     p0.set(p0.x - pointQ.x, -(p0.y - pointQ.y)); // because display needs oroginal coordinates but geomterically the y direction is opposite
//     p1.set(p1.x - pointQ.x, -(p1.y - pointQ.y));
// }


int segment::signum(float f) {
   if (f > 0) return 1;
   if (f < 0) return -1;
   return 0;
}


__device__ bool segment::possibleIntersectionTestXAxis(){
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


  __device__ ofVec2f segment::splitSegmentInto2(){
    // TODO: Implement A Fast Method To Find The Edge Intersection Point.
    // Should return the intersection point or null, if no intersection exists.
    //  Care should be taken to make the implementation CORRECT, but SPEED MATTERS.
     
    segment other(ofVec2f(0.0f, 0.0f, 0), ofVec2f(1500.0f, 0.0f, 0), 0); // x axis as segment
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
             return ofVec2f(pointX, pointY, 0);
        }
    }
    return ofVec2f(-1.0f, 0.0f, 0); // if no intersection THEN returned point has x<0
}


__device__ ofVec2f segment::intersectionWithGivenSegment(segment other){
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
             return ofVec2f(pointX, pointY, 0);
        }
    }
    return ofVec2f(0.0f, 0.0f,0); // if no intersection THEN returned point has x<0, TODO how to pass null
}


__device__ bool segment::collinearWithQ(){
    return abs(p0.x * p1.y - p0.y * p1.x)/2;
}


vector<vray> segment::generateVray(segment seg){
    vector<vray> bothVray;
    ofVec2f xAxisVec(1.0,0.0);
    float infinity = 9999.0f;
    // printf("\n inside s.p0.x: %f, s.p0.y: %f, s.p1.x: %f, s.p1.y: %f", seg.p0.x, seg.p0.y, seg.p1.x, seg.p1.y);
    
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
    // cout << "\n thetas" << theta0 << " " << theta1 ;
    bothVray.push_back(vray(theta0, seg.p0.getNormalized(), infinity, seg.p0.length()));
    bothVray.push_back(vray(theta1, seg.p1.getNormalized(), seg.p1.length(), infinity));
    
    return bothVray;
}

__device__ void mergeVraysGpu(vray *l1, vray *l2, vray *retVray, int mEach){
    float infinity = 99999.0;
    // vector<vray> l;
    // vray *l;
    // cudaMalloc(&l, 10 * sizeof(vray));

    // ofVec2f ofV(0.0f, 0.0f, 0);
    // vray *l = {vray(0.0, ofV, 0.0, 0.0, 0)};
    // printf("\n l1[0].theta:%f, l2[0].theta:%f, m:%d", l1[0].theta, l2[0].theta, m);
    
    // int n = l1.size() + l2.size();
    // int n1 = l1.size();
    // int n2 = l2.size();
    int n = mEach * 2;
    int n1 = mEach;
    int n2 = mEach; //TODO check results
    
    // int i = 0; // diff from paper
    // int i1 = 0;
    // int i2 = 0;
    // int k = 0;
    // int t = 0;
    int i = 0; // diff from paper
    int i1 = 0;
    int i2 = 0;
    int k = 0;
    int t = 0;
    
    // vector<vray> lk;
    // vector<vray> lt;
    // int ik=0;
    // int it=0;
    vray *lk;
    vray *lt;
    int ik=0;
    int it=0;

    int pushCnt = 0;
    

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
        else if(l1[i1].theta <= l2[i2].theta){
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

//         l.push_back(lk.at(ik));
        retVray[i] = lk[ik];
        pushCnt++;

        if(0 < it && it < mEach && lt[it].r < infinity ){
            float ox = lt[it-1].unitVec.x * lt[it-1].l;
            float oy = lt[it-1].unitVec.y * lt[it-1].l;
            ofVec2f o1(ox, oy, 0);
            ox = lt[it].unitVec.x * lt[it].r;
            oy = lt[it].unitVec.y * lt[it].r;
            ofVec2f o2(ox, oy, 0);
            segment s(o1, o2, 0);

            float thetaRad = retVray[i].theta * PI / 180.0;



            segment other(ofVec2f(0.0f, 0.0f, 0), ofVec2f(infinity * cos(thetaRad), infinity * sin(thetaRad), 0), 0); // x axis as segment
            ofVec2f p = s.intersectionWithGivenSegment(other);
           if(p.x == 0.0 && p.y == 0.0){
            //    cout << "\n point" << p.x << " " << p.y ;
               printf("\n it is null - did not find an intersection ");
           }
           else{
            //    cout << "\n it is null - did not find an intersection ";
                printf("\n point %f, %f", p.x, p.y);
           }
            

            retVray[i].l = min(retVray[i].l, p.lengthGpu());
            retVray[i].r = min(retVray[i].r, p.lengthGpu());
        }

        if(k == 1){
            i1 = i1 + 1; // can assign k = i1 or i2 initially based on condition so that we  have to chec value of k again n again
        }
        else{
            i2 = i2 + 1;
        }
        i = i + 1;
    }
    
    // l1 = retVray

    for(int i=0; i<mEach; i++){
        l1[i] = retVray[i];
    }

    // return l;
    return;
}

__global__ void preprocess_in_parallel(segment* input, int size, int x, int y, segment* output) {
 
    int tid = threadIdx.x;
    if(tid < size) {
        // ofVec2f* q = create_ofvec_obj(x, y);
        ofVec2f q(x, y, 0);
        // printf("Id: %d, po.x: %f p1.x: %f\n", tid, input[tid].p0.x, input[tid].p1.x);
        input[tid].translateToQ(q);
        // printf("After translation to q Id: %d, po.x: %f p1.x: %f\n", tid, input[tid].p0.x, input[tid].p1.x);
        if(input[tid].collinearWithQ() != 0.0f){
            if(input[tid].possibleIntersectionTestXAxis()) {
                ofVec2f splitPoint = input[tid].splitSegmentInto2();
                if(splitPoint.x != -1.0f){ 
                    // printf("splitting to q Id: %d, po.x: %f p1.x: %f\n", tid, input[tid].p0.x, input[tid].p1.x);
                    output[2 * tid] = segment(input[tid].p0, splitPoint, 0);
                    output[2 * tid + 1] = segment(input[tid].p1, splitPoint, 0);
                    output[2 * tid].isValid = true;
                    output[2 * tid + 1].isValid = true;
                } else {
                    // printf("No splitting to q Id: %d, po.x: %f p1.x: %f\n", tid, input[tid].p0.x, input[tid].p1.x);
                    output[2 * tid] = segment(input[tid].p0, input[tid].p1, 0);
                    output[2 * tid].isValid = true;
                }
            } else {
                output[2 * tid] = segment(input[tid].p0, input[tid].p1, 0);
                output[2 * tid].isValid = true;
                // printf("No splitting 2 to q Id: %d, po.x: %f p1.x: %f\n", tid, input[tid].p0.x, input[tid].p1.x);
                // printf("No splitting 2 in output to q Id: %d, po.x: %f p1.x: %f\n", tid, output[2*tid].p0.x, output[2*tid].p1.x);
            }
        }
    }
}

__global__ void merge_in_parallel(vray *input_vrays, int size, vray *output_vrays){
    // shared memory 
    // extern __shared__ vray vrayData[];
  
    int tid = threadIdx.x;

    int n = 4;

    // if (tid < size){
    //     for(int m=4; m <= blockDim.x ; m = m * 2){
    //         printf("\n m:%d", m);
    //         if(tid%m == 0){ // merge 4 vrays here 
    //             printf("\n m=%d, tid:%d, it is vray0", m, tid);
    //             printf("\n \t %d, %d", tid, tid + (m/2));
    //             vray *v1 = &input_vrays[tid];
    //             vray *v2 = &input_vrays[tid+(m/2)];
    //             vray *res = &output_vrays[tid];
    //             // mergeVraysGpu(v1, v2, res, m);

    //         }
    //         // else{
    //             // printf("\n m=%d, tid:%d, it is other vray", m, tid);
    //             // do nothing 
    //         // }

    //         __syncthreads();
    //         //TODO kill the thread not required?
    //     }
    // }

     if (tid < size){
        for(int m=4; m < 2*size ; m = m * 2){ 
            printf("\n m:%d", m);
            if(tid%m == 0){ // merge 4 vrays here 
                printf("\n m=%d, tid:%d, it is vray0", m, tid);
                printf("\n \t %d, %d", tid, tid + (m/2));
                vray *v1 = &input_vrays[tid];
                vray *v2 = &input_vrays[tid+(m/2)];
                vray *res = &output_vrays[tid];

                mergeVraysGpu(v1, v2, res, m);

            }

            __syncthreads();
            //TODO kill the thread not required?
        }
    }

}

__global__ void generate_vrays_in_parallel(segment* input, int size, vray* output) {
 
    int tid = threadIdx.x;
    if(tid < size) {
        printf("Id: %d, po.x: %f p1.x: %f\n", tid, input[tid].p0.x, input[tid].p1.x);
        // vector<vray> segVrays = input[tid].generateVray(input[tid]);
        // output[2 * tid] = segVrays.at(0);
        // output[2 * tid + 1] = segVrays.at(1);
    }
}

class GPU_V1 {
    float infinity = 9999.0f;

    public:

    vector<segment> preprocess(vector<segment>& segments, ofVec2f& q) {
            
        cudaError_t err = cudaSuccess;

        segment *d_segments;
        cudaMalloc(&d_segments, segments.size() * sizeof(segment));

        err = cudaMemcpy(d_segments, segments.data(), segments.size() * sizeof(segment), cudaMemcpyHostToDevice);
        if (err != cudaSuccess)
        {
            fprintf(stderr, "Failed to allocate d_segments (error code %s)!\n", cudaGetErrorString(err));
            exit(EXIT_FAILURE);
        }

        segment *d_output_segments;
        cudaMalloc(&d_output_segments, 2 * segments.size() * sizeof(segment));

        int threadsPerBlock = 100;
        int blocksPerGrid = 1;
        preprocess_in_parallel <<<blocksPerGrid, threadsPerBlock>>> (d_segments, segments.size(), q.x, q.y, d_output_segments);
        cudaDeviceSynchronize();
        err = cudaGetLastError();
        if (err != cudaSuccess)
        {
            fprintf(stderr, "Failed to launch preprocess_in_parallel kernel (error code %s)!\n", cudaGetErrorString(err));
            exit(EXIT_FAILURE);
        }
        segment* preprocessed_segments = (segment*) malloc(sizeof(segment) * 2 * segments.size());
        err = cudaMemcpy(preprocessed_segments, d_output_segments, 2 * segments.size() * sizeof(segment), cudaMemcpyDeviceToHost);
        if (err != cudaSuccess)
        {
            fprintf(stderr, "Failed to copy d_output_segments kernel (error code %s)!\n", cudaGetErrorString(err));
            exit(EXIT_FAILURE);
        }
        vector<segment> filtered_segments;
        for(int i=0;i<2*segments.size();i++) {
            if(preprocessed_segments[i].isValid) {
                filtered_segments.push_back(preprocessed_segments[i]);
            }
        }
        // cudaFree(d_output_segments);
        // cudaFree(d_segments);
        return filtered_segments;

    }

    vector<vray> generate_vrays_from_segments(vector<segment>& segments) {
            
        cudaError_t err = cudaSuccess;

        segment *d_segments;
        cudaMalloc(&d_segments, segments.size() * sizeof(segment));

        err = cudaMemcpy(d_segments, segments.data(), segments.size() * sizeof(segment), cudaMemcpyHostToDevice);
        if (err != cudaSuccess)
        {
            fprintf(stderr, "Failed to allocate d_segments (error code %s)!\n", cudaGetErrorString(err));
            exit(EXIT_FAILURE);
        }

        vray *d_output_vrays;
        cudaMalloc(&d_output_vrays, 2 * segments.size() * sizeof(vray));

        int threadsPerBlock = 100;
        int blocksPerGrid = 1;
        generate_vrays_in_parallel <<<blocksPerGrid, threadsPerBlock>>> (d_segments, segments.size(), d_output_vrays);
        cudaDeviceSynchronize();
        err = cudaGetLastError();
        if (err != cudaSuccess)
        {
            fprintf(stderr, "Failed to launch generate_vrays_in_parallel kernel (error code %s)!\n", cudaGetErrorString(err));
            exit(EXIT_FAILURE);
        }

        vray* gpu_generated_vrays = (vray*) malloc(sizeof(vray) * 2 * segments.size());
        err = cudaMemcpy(gpu_generated_vrays, d_output_vrays, 2 * segments.size() * sizeof(vray), cudaMemcpyDeviceToHost);
        if (err != cudaSuccess)
        {
            fprintf(stderr, "Failed to copy d_output_vrays kernel (error code %s)!\n", cudaGetErrorString(err));
            exit(EXIT_FAILURE);
        }
        vector<vray> generated_vrays;
        for(int i=0;i<2*segments.size();i++) {
            cout << " " << gpu_generated_vrays[i].theta ;
                generated_vrays.push_back(gpu_generated_vrays[i]);
        }
        cudaFree(d_output_vrays);
        cudaFree(d_segments);
        return generated_vrays;

    }

    vray* mergeVraysGpu(vector<vray>& initial_vray_list){
        cout << "\n do";
        vector<vray> ret;

  
        // cudaError_t err = cudaSuccess;

        // segment *d_segments;
        // cudaMalloc(&d_segments, segments.size() * sizeof(segment));

        // err = cudaMemcpy(d_segments, segments.data(), segments.size() * sizeof(segment), cudaMemcpyHostToDevice);
        // if (err != cudaSuccess)
        // {
        //     fprintf(stderr, "Failed to allocate d_segments (error code %s)!\n", cudaGetErrorString(err));
        //     exit(EXIT_FAILURE);
        // }

        cudaError_t err = cudaSuccess;

        vray *d_vrays;
        cudaMalloc(&d_vrays, initial_vray_list.size() * sizeof(vray));

        err = cudaMemcpy(d_vrays, initial_vray_list.data(), initial_vray_list.size() * sizeof(vray), cudaMemcpyHostToDevice);
        if (err != cudaSuccess)
        {
            fprintf(stderr, "Failed to allocate d_vrays (error code %s)!\n", cudaGetErrorString(err));
            exit(EXIT_FAILURE);
        }

        // segment *d_output_segments;
        // cudaMalloc(&d_output_segments, 2 * segments.size() * sizeof(segment));

        vray *d_output_vrays;
        cudaMalloc(&d_output_vrays, initial_vray_list.size() * sizeof(vray));

        // int threadsPerBlock = 100;
        // int blocksPerGrid = 1;
        int threadsPerBlock = 16;
        int blocksPerGrid = 1;
        merge_in_parallel <<<blocksPerGrid, threadsPerBlock>>> (d_vrays, initial_vray_list.size(), d_output_vrays);
        cudaDeviceSynchronize();
        err = cudaGetLastError();
        if (err != cudaSuccess)
        {
            fprintf(stderr, "Failed to launch preprocess_in_parallel kernel (error code %s)!\n", cudaGetErrorString(err));
            exit(EXIT_FAILURE);
        }
        vray* return_vrays = (vray*) malloc(sizeof(vray) * initial_vray_list.size());
        err = cudaMemcpy(return_vrays, d_output_vrays, initial_vray_list.size() * sizeof(vray), cudaMemcpyDeviceToHost);
        if (err != cudaSuccess)
        {
            fprintf(stderr, "Failed to copy d_output_segments kernel (error code %s)!\n", cudaGetErrorString(err));
            exit(EXIT_FAILURE);
        }
        // vector<segment> filtered_segments;
        // for(int i=0;i<2*segments.size();i++) {
        //     if(preprocessed_segments[i].isValid) {
        //         filtered_segments.push_back(preprocessed_segments[i]);
        //     }
        // }
        cudaFree(d_output_vrays);
        cudaFree(d_vrays);
        return return_vrays;

    }

    virtual vector<vray> process_segments(vector<segment> segments, ofVec2f& q) {
        // start = 0;
        std::cout<<"\n Starting Preprocess, size: "<< segments.size() <<std::endl;
        vector<segment> updated_segments = this->preprocess(segments, q);
        std::cout<<"\n Preprocess complete, size: "<< updated_segments.size() <<std::endl;

        // for(int i=0; i<updated_segments.size(); i++){
        //     printf("Id: %d, p0.x: %f p0.y: %f, p1.x: %fp1.y: %f\n", i, updated_segments.at(i).p0.x, updated_segments.at(i).p0.y, updated_segments.at(i).p1.x, updated_segments.at(i).p1.y);
        // }

        std::cout<<"\n Starting generate vrays"<<std::endl;
        vector<vray> initial_vrays;// = this->generate_vrays_from_segments(updated_segments);
        for(int i =0; i<updated_segments.size(); i++){
            segment s = updated_segments.at(i);
            // printf("i: %d, s.p0.x: %f, s.p0.y: %f, s.p1.x: %f, s.p1.y: %f\n", i, s.p0.x, s.p0.y, s.p1.x, s.p1.y);
            initial_vrays.push_back(s.generateVray(s).at(0));
            initial_vrays.push_back(s.generateVray(s).at(1));
            // printf("i: %d, .theta: %f\n", i, s.generateVray(s).at(0).theta);
        }
        std::cout<<"\n End of generate vrays, size: "<< initial_vrays.size() << std::endl;
        // for(int i=0; i<initial_vrays.size(); i++){
        //     printf("i: %d, .theta: %f\n", i, initial_vrays.at(i).theta);
        // }

        vector<vray> merged_vrays;
        std::cout<<"\n Starting merge vrays"<<std::endl;
        vray *merged_vrayssss = this->mergeVraysGpu(initial_vrays);
        std::cout<<"\n Ending merge"<<std::endl;
        // for(int i=0; i<initial_vrays.size(); i++){
        //     printf("i: %d, .theta: %f\n", i, merged_vrayssss[i].theta);
        // }
        // end = 0;
        return merged_vrays;
    }
};

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

void test_preprocessing() {

    vector<int> all_time;
    int iteration = 500;
    double timeDuration = 0.0;

    
    vector<segment> listSegments = generateSegments(iteration);
    ofVec2f pointQ(500,400);

    GPU_V1 *gpu = new GPU_V1();
    int iteration = 500;
    double timeCpu = 0.00;
    vector<segment> updated_segments;
    for(int i=0; i<iteration; i++){
        time_t start = time(NULL);
        updated_segments = gpu->preprocess(listSegments, q);
        time_t end = time(NULL);
        // double duration = double(end-begin) / CLOCKS_PER_SEC;
        double duration = double(end-start);
        timeCpu = timeCpu + duration;
    }
    cout.precision(17);
    cout << fixed << "\n Preprocessing in GPU: " << timeCpu/iteration;

    vector<vray> initial_vrays;
    for(int i =0; i<updated_segments.size(); i++){
        segment s = updated_segments.at(i);
        initial_vrays.push_back(s.generateVray(s).at(0));
        initial_vrays.push_back(s.generateVray(s).at(1));
    }
    
    timeCpu = 0.00;
    for(int i=0; i<iteration; i++){
        time_t start = time(NULL);

        gpu->mergeVraysGpu(initial_vrays);

        time_t end = time(NULL);
        // double duration = double(end-begin) / CLOCKS_PER_SEC;
        double duration = double(end-start);
        timeCpu = timeCpu + duration;
    }
    cout.precision(17);
    cout << fixed << "\n Merge Sequential time in GPU: " << timeCpu/iteration;
    
    // for(int i=0;i<iteration;i++) {
    // //     vector<segment> listSegments = {
    // //         segment(ofVec2f(600.0f, 550.0f), ofVec2f(650.0f, 400.0f)),  // right small, line to split at 0 degree
    // //         segment(ofVec2f(100.0f, 500.0f), ofVec2f(350.0f, 300.0f)),  // connected pair above
    // //         segment(ofVec2f(550.0f, 700.0f), ofVec2f(100.0f, 500.0f)),  // connected pair bottom
    // //         segment(ofVec2f(600.0f, 600.0f), ofVec2f(800.0f, 150.0f)),  // right side long
    // //         segment(ofVec2f(200.0f, 300.0f), ofVec2f(700.0f, 100.0f)),  // top horizontal

    // // //        segment(ofVec2f(800.0f, 500.0f), ofVec2f(800.0f, 500.01f)),   // the line for ending
    // //         segment(ofVec2f(450.0f, 450.0f), ofVec2f(400.0f, 400.0f))   // the collinear line
    // //     };
        
        

        
    //     // auto start = chrono::high_resolution_clock::now();
    //     // std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    //     clock_t begin = clock();

    //     vector<segment> segments = gpu->preprocess(listSegments, pointQ);
    //     // auto stop = chrono::high_resolution_clock::now();
    //     // std::chrono::steady_clock::time_point stop = std::chrono::steady_clock::now();
    //     clock_t end = clock();
    //     double duration = double(end-begin) / CLOCKS_PER_SEC;
    //     // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
    //     // all_time.push_back(duration);
    //     timeDuration = timeDuration + duration;
    // }
    //return timeDuration/iteration;//(float)std::reduce(all_time.begin(), all_time.end())/(float) all_time.size();
    // cout << "\n Prreprocessing in GPU: " << timeDuration/iteration;
}

int main() {

    //TEST Preprocessing
    // std::cout<<"Time taken For Preprocessing in GPU: ";//<< test_preprocessing() <<std::endl;
    // test_preprocessing();

    vector<segment> listSegments = {
        segment(ofVec2f(600.0f, 550.0f), ofVec2f(650.0f, 400.0f)),  // right small, line to split at 0 degree
        segment(ofVec2f(100.0f, 500.0f), ofVec2f(350.0f, 300.0f)),  // connected pair above
        segment(ofVec2f(550.0f, 700.0f), ofVec2f(100.0f, 500.0f)),  // connected pair bottom
        segment(ofVec2f(600.0f, 600.0f), ofVec2f(800.0f, 150.0f)),  // right side long
        segment(ofVec2f(200.0f, 300.0f), ofVec2f(700.0f, 100.0f)),  // top horizontal

//        segment(ofVec2f(800.0f, 500.0f), ofVec2f(800.0f, 500.01f)),   // the line for ending
        segment(ofVec2f(450.0f, 450.0f), ofVec2f(400.0f, 400.0f))   // the collinear line
    };
    
    ofVec2f pointQ(500,400);
    GPU_V1 *v1 = new GPU_V1();
    v1->process_segments(listSegments, pointQ);
    return 0;
}

