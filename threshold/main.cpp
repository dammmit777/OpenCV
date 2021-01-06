/**
 * @file main.cpp
 * @author ssm
 * @brief how to use the diverse threshold options 
 * @version 0.1
 * @date 2020-12-29
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//Global Variables
int threshold_value = 0;
int threshold_type = 3;
int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;
Mat src, src_gray, dst;
const char *windowname = "Threshold Demo";
const char *trackbar_type =
    "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
const char *trackbar_value = "Value";

//Function headers
void Threshold_Demo(int, void *);

/**
 * @brief Main
 * 
 */
int main(int argc, char **argv)
{
    //Load Image
    src = imread(argv[1], 1);

    //Convert the image to grayscale
    cvtColor(src, src_gray, COLOR_RGB2GRAY);

    namedWindow(windowname, WINDOW_AUTOSIZE);

    //Create Trackbar to choose type of threshold
    createTrackbar(trackbar_type, windowname,
                   &threshold_type, max_type,
                   Threshold_Demo);
    createTrackbar(trackbar_value, windowname,
                   &threshold_value, max_value,
                   Threshold_Demo);

    Threshold_Demo(0, 0);

    //wait
    while (true)
    {
        char c;
        c = waitKey(20);
        if (c == 27)
            break;
    }
}

/**
 * @brief Threshold Demo
 * 
 */

void Threshold_Demo(int, void *)
{
    /**
      * 0. Binary
      * 1. Binary Inverted
      * 2. Truncate
      * 3. To Zero
      * 4. To Zero Inverted
      */
    threshold(src_gray, dst, threshold_value, max_BINARY_value, threshold_type);
    imshow(windowname, dst);
}
