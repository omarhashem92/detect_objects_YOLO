# detect_objects_YOLO

<img src="images/hotdog.png" width="600" height="400" />

The idea of the project is to classify the picture and identify whether it is hot dog or not.

## Dependencies for Running Locally
* cmake >= 2.8
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* Git LFS
  * Weight files are handled using [LFS](https://git-lfs.github.com/)
* OpenCV >= 4.1
  * This must be compiled from source using the `-D OPENCV_ENABLE_NONFREE=ON` cmake flag for testing the SIFT and SURF detectors.
  * The OpenCV 4.1.0 source code can be found [here](https://github.com/opencv/opencv/tree/4.1.0)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level project directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./detect_objects`.


## Expected behavior
The program started by asking the user to enter a number for an image to choose between image 1 and image 2).
after choosing the picture, it will be loaded and classified as a hot dog or not. 
then you have to press 0 to exit.


## ***Rubric points Coverage***

### `The project reads data from a file and process the data, or the program writes data to a file.` 
- This is implemented in detect_objects_2.cpp in the constructor in lines 38, 39 and 40. 
___


### `The project accepts user input and processes the input.` 
- This is done in the main function in line 15.

---

### `The project uses Object Oriented Programming techniques.` 
- This is implemented by creating a class classification which will provide public interface to detect and classify the object.

---

### `Classes use appropriate access specifiers for class members.` 
- This is implemented in detect_object_2.h by classifying the functions to be public or private based on the need of the usage of the function and same is applicable for variables.


---
### `Class constructors utilize member initialization lists.` 
- This is implemented in the definition of classification class in its constructor in detect_object_2.cpp file in line 34 and 43.

please note that not all members are used in the initializar list for the sake of readability.

---

### `The project makes use of references in function declarations.` 
- This is implemented in detect_objects_2.cpp when calling scanBoundingBoxes(), nonMaximaSuppression() and show_result() in lines 156, 162 and 166 respectively.

---








