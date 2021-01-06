/**
 * @file main.cpp
 * @author ssm
 * @brief showing how to detect edges using the Canny Detector
 * @version 0.1
 * @date 2021-01-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat src, src_gray;
Mat dst, detected_edges;

int lowThreshold = 0;
const int max_lowThreshold = 100;
const int ratio = 3;
const int kernel_size = 3;
const char *windowname = "Edge Map";

/**
 * @function CannyThreshold
 * @brief Trackbar caliback -canny thresholds input with a ratio 1:3
 * 
 */
static void CannyThreshold(int, void *)
{
    //reduce noise
    blur(src_gray, detected_edges, Size(3, 3));

    Canny(detected_edges, detected_edges,
          lowThreshold, lowThreshold * ratio, kernel_size);

    //using Canny's output as a mask
    //fill
    dst = Scalar::all(0);
    src.copyTo(dst, detected_edges);
    imshow(windowname, dst);
}

int main(int argc, char **argv)
{
    src = imread(argv[1], IMREAD_COLOR);
    if (src.empty())
    {
        cout << "Could not open or find the image" << endl;
        return -1;
    }
    imshow("src", src);
    //create a matrix of the same type and size as src(for dst)
    dst.create(src.size(), src.type());

    //convert to gray
    cvtColor(src, src_gray, COLOR_BGR2GRAY);

    //create window
    namedWindow(windowname, WINDOW_AUTOSIZE);

    //create trackbar
    createTrackbar("Min Threshold:", windowname,
                   &lowThreshold, max_lowThreshold,
                   CannyThreshold);

    CannyThreshold(0, 0);
    waitKey(0);

    return 0;
}