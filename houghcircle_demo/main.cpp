/**
 * @file main.cpp
 * @author ssm
 * @brief code for Hough Circle Transform
 * @version 0.1
 * @date 2021-01-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

namespace
{
    //window and trackbar name
    const string windowname = "Hough Circle Detection Demo";
    const string cannyThresholdTrackbarnName = "Canny threshold";
    const string accumulatorThresholdTrackbarName = "Accumulator Threshold";

    //Initial and max values of the parameters of interests
    const int cannyThresholdInitialValue = 100;
    const int accumulatorThresholdInitialValue = 50;
    const int maxCannyThreshold = 255;
    const int maxAccumulatorThreshold = 200;

    void HoughDetection(const Mat &src_gray,
                        const Mat &src_display,
                        int cannyThreshold,
                        int accumulatorThreshold)
    {
        vector<Vec3f> circles;

        HoughCircles(src_gray, circles, HOUGH_GRADIENT, 1,
                     src_gray.rows / 8,
                     cannyThreshold,
                     accumulatorThreshold, 0, 0);

        Mat display = src_display.clone();
        for (size_t i = 0; i < circles.size(); i++)
        {
            Point center(cvRound(circles[i][0]),
                         cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);

            //draw circle center
            circle(display, center, 3, Scalar(0, 255, 0),
                   -1, 8, 0);

            //draw circle
            circle(display, center, radius, Scalar(0, 0, 255),
                   3, 8, 0);
        }
        imshow(windowname, display);
    }
} // namespace

int main(int argc, char **argv)
{
    Mat src, src_gray;
    src = imread(argv[1], IMREAD_COLOR);
    if (src.empty())
        return -1;

    //convert to gray
    cvtColor(src, src_gray, COLOR_BGR2GRAY);

    //reduce noise
    GaussianBlur(src_gray, src_gray, Size(5, 5), 2, 2);

    int cannyThreshold = cannyThresholdInitialValue;
    int accumulatorThreshold = accumulatorThresholdInitialValue;

    namedWindow(windowname, WINDOW_AUTOSIZE);
    createTrackbar(cannyThresholdTrackbarnName,
                   windowname,
                   &cannyThreshold,
                   maxCannyThreshold);
    createTrackbar(accumulatorThresholdTrackbarName,
                   windowname,
                   &accumulatorThreshold,
                   maxAccumulatorThreshold);

    char key = 0;
    while (key != 'q' && key != 'Q')
    {
        cannyThreshold = std::max(cannyThreshold, 1);
        accumulatorThreshold = std::max(accumulatorThreshold, 1);

        HoughDetection(src_gray, src,
                       cannyThreshold,
                       accumulatorThreshold);

        key = (char)waitKey(10);
    }

    return 0;
}