#include <iostream>
#include <numeric>
#include <fstream>
#include "detect_objects_2.h"
#include "dataStructures.h"

using namespace std;


int main()
{
    int img_selector;

    std::cout<<"Please enter 1 to check image1 or enter 2 to check image2"<<std::endl;
    std::cin>>img_selector;
    classification classify1(img_selector);
    classify1.detectObjects2();
}