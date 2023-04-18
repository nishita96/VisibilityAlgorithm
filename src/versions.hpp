
#include "segment.hpp"
#include<iostream>

class VisibilityBase {
    
    int start;
    int end;

    virtual vector<segment> preprocess(vector<segment>& segments, ofVec2f q) {
        //fatal
        vector<segment> re;
        cout<<"Fatal preprocess. Shouldn't reach here";
        return re; 
    }

    virtual vector<vray> merge(vector<segment> listSegmentsCopy) {
        vector<vray> re;
        cout<<"Fatal merge. Shouldn't reach here";
        return re; 

    }

    public:

    int time_taken() {
        return end - start;
    }

    vector<vray> process_segments(vector<segment> segments, ofVec2f& q) {
        start = 0;
        vector<segment> updated_segments = preprocess(segments, q);
        vector<vray> merged_vrays = merge(updated_segments);
        end = 0;
        return merged_vrays;
    }
};


