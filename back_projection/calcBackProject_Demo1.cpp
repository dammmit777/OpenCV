/**
 * @file calcBackProject_Demo1.cpp
 * @author ssm
 * @brief code for backproject function usage
 * @version 0.1
 * @date 2021-01-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat hue;
int bins = 25;

void Hist_and_Backprojection(int, void *);

int main(int argc, char **argv)
{
    Mat src = imread(argv[1], IMREAD_COLOR);
    if (src.empty())
        return -1;

    Mat hsv;
    cvtColor(src, hsv, COLOR_BGR2HSV);

    //use only the Hue value
    hue.create(hsv.size(), hsv.depth());
    int channels[] = {0, 0};
    mixChannels(&hsv, 1, &hue, 1, channels, 1);

    //create trackbar to enter the number of bins
    const char *windowname = "source image";
    namedWindow(windowname, WINDOW_AUTOSIZE);
    createTrackbar("* Hue bins: ",
                   windowname, &bins, 180,
                   Hist_and_Backprojection);
    Hist_and_Backprojection(0, 0);

    //show the image
    imshow(windowname, src);

    waitKey();
    return 0;
}

/**
 * @brief Callback to Trackbar
 * 
 */
void Hist_and_Backprojection(int, void *)
{
    //init
    int histSize = MAX(bins, 2);
    float hue_range[] = {0, 180};
    const float *ranges = {hue_range};

    //get the histogram and normalize it
    Mat hist;
    calcHist(&hue, 1, 0, Mat(), hist, 1, &histSize, &ranges, true, false);
    normalize(hist, hist, 0, 255, NORM_MINMAX, -1, Mat());

    //get backprojection
    Mat backprojection;
    calcBackProject(&hue, 1, 0, hist, backprojection, &ranges, 1, true);

    //draw the backprojection
    imshow("backprojection", backprojection);

    //draw the histogram
    int w = 400, h = 400;
    int bin_w = cvRound((double)w / histSize);
    Mat histImg = Mat::zeros(h, w, CV_8UC3);

    for (int i = 0; i < bins; i++)
    {
        rectangle(histImg, Point(i * bin_w, h),
                  Point((i + 1) * bin_w, h - cvRound(hist.at<float>(i) * h / 255.0)),
                  Scalar(0, 0, 255), FILLED);
    }

    imshow("Histogram", histImg);
}
