
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
        std::cout<<"Starting Process"<<std::endl;
        vector<segment> updated_segments = this->preprocess(segments, q);
        std::cout<<"Preprocess complete"<<std::endl;
        vector<vray> merged_vrays = this->merge(updated_segments);
        std::cout<<"Ending Process"<<std::endl;
        end = 0;
        return merged_vrays;
    }
};


