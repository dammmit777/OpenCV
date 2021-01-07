/**
 * @file main.cpp
 * @author ssm
 * @brief code to use the function calcHist
 * @version 0.1
 * @date 2021-01-07
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

int main(int argc, char **argv)
{
    Mat src = imread(argv[1], IMREAD_COLOR);
    if (src.empty())
        return -1;

    //separate the image in 3 places(B G and R)
    vector<Mat> bgr_planes;
    split(src, bgr_planes);

    //establish the number of bins
    int histSize = 256;

    //set the range for B G and R
    float range[] = {0, 256};
    const float *histRange = {range};

    //compute the histograms
    Mat b_hist, g_hist, r_hist;
    calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1,
             &histSize, &histRange, true, false);

    calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1,
             &histSize, &histRange, true, false);

    calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1,
             &histSize, &histRange, true, false);

    //draw the histograms for B G and R
    int hist_w = 1024, hist_h = 960;
    int bin_w = cvRound((double)hist_w / histSize);
    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

    //normalize the result
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

    //draw for each channel
    for (int i = 1; i < histSize; i++)
    {
        line(histImage,
             Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
             Point(bin_w * i, hist_h - cvRound(b_hist.at<float>(i))),
             Scalar(255, 0, 0), 2, 8, 0);

        line(histImage,
             Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
             Point(bin_w * i, hist_h - cvRound(g_hist.at<float>(i))),
             Scalar(0, 255, 0), 2, 8, 0);

        line(histImage,
             Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
             Point(bin_w * i, hist_h - cvRound(r_hist.at<float>(i))),
             Scalar(0, 0, 255), 2, 8, 0);
    }

    imshow("src", src);
    imshow("calcHist", histImage);
    waitKey(0);
    return 0;
}