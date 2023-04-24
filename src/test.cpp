#include<iostream>

#include <chrono>
#include "cpu_version.hpp"
// #include "gpu_version_1.cu"

using namespace std;
int main() {

    ofVec2f pointQ(500,400); //= ofGetWindowSize() / 2;
    // ofAppNew.q = pointQ;
    
    // ---- made a set of segments covering cases
    vector<segment> listSegments = {
        segment(ofVec2f(600.0f, 550.0f), ofVec2f(650.0f, 400.0f)),  // right small, line to split at 0 degree
        segment(ofVec2f(100.0f, 500.0f), ofVec2f(350.0f, 300.0f)),  // connected pair above
        segment(ofVec2f(550.0f, 700.0f), ofVec2f(100.0f, 500.0f)),  // connected pair bottom
        segment(ofVec2f(600.0f, 600.0f), ofVec2f(800.0f, 150.0f)),  // right side long
        segment(ofVec2f(200.0f, 300.0f), ofVec2f(700.0f, 100.0f)),  // top horizontal

//        segment(ofVec2f(800.0f, 500.0f), ofVec2f(800.0f, 500.01f)),   // the line for ending
        segment(ofVec2f(450.0f, 450.0f), ofVec2f(400.0f, 400.0f))   // the collinear line
    };
    auto begin = std::chrono::high_resolution_clock::now();

    CPU* c1 = new CPU();
    c1->process_segments(segments, q);


    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cout<<elapsed<<endl;
}