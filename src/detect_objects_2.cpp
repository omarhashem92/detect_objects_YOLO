#include <iostream>
#include <numeric>
#include <fstream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/dnn.hpp>
#include "detect_objects_2.h"
#include "dataStructures.h"

using namespace std;


void classification::load_image(){

    if(_img_selector == 1){

        _img = cv::imread("../images/hotdog.png");

    }
    else if(_img_selector == 2){

        _img = cv::imread("../images/imresizer_com2.jpg");

    }
    else{

        std::cout<<"incorrect image"<<std::endl;
    }


}

classification::classification():_yoloBasePath("../dat/yolo/"),confThreshold(0.21){

    // load class names from file
    //_yoloBasePath = "../dat/yolo/";
    _yoloClassesFile = _yoloBasePath + "coco.names";
    _yoloModelConfiguration = _yoloBasePath + "yolov3.cfg";
    _yoloModelWeights = _yoloBasePath + "yolov3.weights"; 
}

classification::classification(int selector):_img_selector(selector), _yoloBasePath("../dat/yolo/"),confThreshold(0.21){

    // load class names from file
    //_yoloBasePath = "../dat/yolo/";
    _yoloClassesFile = _yoloBasePath + "coco.names";
    _yoloModelConfiguration = _yoloBasePath + "yolov3.cfg";
    _yoloModelWeights = _yoloBasePath + "yolov3.weights"; 
}

bool classification::is_hotdog(std::string class_name){
    if(class_name == "hot dog"){
        return true;
    }
    else{
        return false;
    }
}

void classification::scanBoundingBoxes(vector<cv::Mat> &netOutput){
    for (size_t i = 0; i < netOutput.size(); ++i)
    {
        float* data = (float*)netOutput[i].data;
        for (int j = 0; j < netOutput[i].rows; ++j, data += netOutput[i].cols)
        {
            cv::Mat scores = netOutput[i].row(j).colRange(5, netOutput[i].cols);
            cv::Point classId;
            double confidence;
            
            // Get the value and location of the maximum score
            cv::minMaxLoc(scores, 0, &confidence, 0, &classId);
            if (confidence > confThreshold)
            {
                cv::Rect box; int cx, cy;
                cx = (int)(data[0] * _img.cols);
                cy = (int)(data[1] * _img.rows);
                box.width = (int)(data[2] * _img.cols);
                box.height = (int)(data[3] * _img.rows);
                box.x = cx - box.width/2; // left
                box.y = cy - box.height/2; // top
                
                boxes.push_back(box);
                classIds.push_back(classId.x);
                confidences.push_back((float)confidence);
            }
        }
    }

}


void classification::nonMaximaSuppression(std::vector<BoundingBox> &Boxes){

    float nmsThreshold = 0.5;  // Non-maximum suppression threshold
    vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);
    
    for (auto it = indices.begin(); it != indices.end(); ++it)
    {
        BoundingBox bBox;
        bBox.roi = boxes[*it];
        bBox.classID = classIds[*it];
        bBox.confidence = confidences[*it];
        bBox.boxID = (int)Boxes.size(); // zero-based unique identifier for this bounding box
        
        Boxes.push_back(bBox);
    }
}

void classification::detectObjects2()
{
    // load image from file
    load_image(); 
    

    ifstream ifs(_yoloClassesFile.c_str());
    string line;
    while (getline(ifs, line)) classes.push_back(line);
    
    // load neural network
    cv::dnn::Net net = cv::dnn::readNetFromDarknet(_yoloModelConfiguration, _yoloModelWeights);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

    // generate 4D blob from input image
    cv::Mat blob;
    double scalefactor = 1/255.0;
    auto startTime = std::chrono::steady_clock::now();
    cv::Size size = cv::Size(416, 416);
    

    cv::Scalar mean = cv::Scalar(0,0,0);
    bool swapRB = false;
    bool crop = false;
    cv::dnn::blobFromImage(_img, blob, scalefactor, size, mean, swapRB, crop);

    
    // Get names of output layers
    vector<cv::String> names;
    vector<int> outLayers = net.getUnconnectedOutLayers(); // get indices of output layers, i.e. layers with unconnected outputs
    vector<cv::String> layersNames = net.getLayerNames(); // get names of all layers in the network
    
    names.resize(outLayers.size());
    for (size_t i = 0; i < outLayers.size(); ++i) // Get the names of the output layers in names
    {
        names[i] = layersNames[outLayers[i] - 1];
    }

    // invoke forward propagation through network
    vector<cv::Mat> netOutput;
    net.setInput(blob);
    net.forward(netOutput, names);

    // Scan through all bounding boxes and keep only the ones with high confidence
    scanBoundingBoxes(netOutput);
    std::cout<<"Number of Boxes :"<< boxes.size()<<std::endl;


    // perform non-maxima suppression
    std::vector<BoundingBox> bBoxes;
    nonMaximaSuppression(bBoxes);
    
    
    // show results
    show_result(bBoxes);

    auto endTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout<<"Elapsed time is "<< elapsedTime.count() <<std::endl;

    string windowName = "Object classification";
    cv::namedWindow( windowName, 1 );
    cv::imshow( windowName, visImg );
    cv::waitKey(0); // wait for key to be pressed
}


void classification::show_result(std::vector<BoundingBox> &Boxes){

    visImg = _img.clone();
    for (auto it = Boxes.begin(); it != Boxes.end(); ++it)
    {
        // Draw rectangle displaying the bounding box
        int top, left, width, height;
        top = (*it).roi.y;
        left = (*it).roi.x;
        width = (*it).roi.width;
        height = (*it).roi.height;
        cv::rectangle(visImg, cv::Point(left, top), cv::Point(left + width, top + height), cv::Scalar(0, 255, 0), 2);

        string label = cv::format("%.2f", (*it).confidence);


        
        if (is_hotdog(classes[((*it).classID)])){

            label = classes[((*it).classID)] + ":" + label;
        }
        else
        {
            label = "Not a hot dog";
        }
        

        // Display label at the top of the bounding box
        int baseLine;
        cv::Size labelSize = getTextSize(label, cv::FONT_ITALIC, 0.5, 1, &baseLine);
        top = max(top, labelSize.height);
        rectangle(visImg, cv::Point(left, top - round(1.5 * labelSize.height)), cv::Point(left + round(1.5 * labelSize.width), top + baseLine), cv::Scalar(255, 255, 255), cv::FILLED);
        cv::putText(visImg, label, cv::Point(left, top), cv::FONT_ITALIC, 0.75, cv::Scalar(0, 0, 0), 1);
    }

    std::cout<<"End of Code"<<std::endl;
}
/*

classification::~classification(){

    std::cout << "Object has been deleted" <<std::endl;
}
 */