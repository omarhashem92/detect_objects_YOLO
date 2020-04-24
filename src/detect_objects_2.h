#ifndef DETECT_OBJECT_H
#define DETECT_OBJECT_H

#include <iostream>
#include <numeric>
#include <fstream>


#include "dataStructures.h"

using namespace std;

class classification{

    public:
    void detectObjects2();
    classification();
    //~classification();
    classification(int selector);

    private:
    //class methods
    void load_image();
    bool is_hotdog(std::string class_name);
    void scanBoundingBoxes(vector<cv::Mat> &netOutput);
    void nonMaximaSuppression(std::vector<BoundingBox> &Boxes);
    void show_result(std::vector<BoundingBox> &Boxes);
    
    //class members
    int _img_selector;
    string _yoloBasePath;
    string _yoloClassesFile;
    string _yoloModelConfiguration;
    string _yoloModelWeights;
    cv::Mat _img;
    float confThreshold;
    vector<int> classIds;
    vector<float> confidences;
    vector<cv::Rect> boxes;
    //std::vector<BoundingBox> bBoxes;
    cv::Mat visImg;
    vector<string> classes;
};
//void detectObjects2();


#endif