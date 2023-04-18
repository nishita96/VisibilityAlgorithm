
#include "versions.hpp"

float infinity = 9999.0f;
class CPU: public VisibilityBase {


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

    vector<segment> preprocess(vector<segment>& segments, ofVec2f& q) {
        std::cout<<"Preprocess: step 1"<<std::endl;
        for(int i=0; i< segments.size(); i++){ 
            segments[i].translateToQ(q);
        }
        std::cout<<"Preprocess: step 2"<<std::endl;
        vector<segment>::iterator it = segments.begin();
        while(it != segments.end()) {
            if(it->collinearWithQ() == 0.0f){
                segments.erase(it); // automatically iterates to next item after erasing
            } else {
                ++it;
            }
        }
        std::cout<<"Preprocess: step 3"<<std::endl;
        vector<segment> fresh_segments = check_intersections(segments);   
        return fresh_segments;
    }

    public:
    vector<vray> process_segments(vector<segment> segments, ofVec2f& q) {
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

