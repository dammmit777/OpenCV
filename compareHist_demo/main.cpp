/**
 * @file main.cpp
 * @author ssm
 * @brief use the function compareHist
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

int main(int argc, char **argv)
{
    Mat src_base, hsv_base;
    Mat src_test1, hsv_test1;
    Mat src_test2, hsv_test2;
    Mat hsv_half_down;

    if (argc < 4)
    {
        cout << "Error. Usage: ./compareHist_demo <image1><image2><image3>" << endl;
        return -1;
    }

    src_base = imread(argv[1], IMREAD_COLOR);
    src_test1 = imread(argv[2], IMREAD_COLOR);
    src_test2 = imread(argv[3], IMREAD_COLOR);

    imshow("src_base", src_base);
    imshow("src_test1", src_test1);
    imshow("src_test2", src_test2);

    //HSV：色相饱和度
    cvtColor(src_base, hsv_base, COLOR_BGR2HSV);
    cvtColor(src_test1, hsv_test1, COLOR_BGR2HSV);
    cvtColor(src_test2, hsv_test2, COLOR_BGR2HSV);

    hsv_half_down =
        hsv_base(Range(hsv_base.rows / 2, hsv_base.rows - 1),
                 Range(0, hsv_base.cols - 1));

    imshow("hsv_base", hsv_base);
    imshow("hsv_half_down", hsv_half_down);
    imshow("hsv_test1", hsv_test1);
    imshow("hsv_test2", hsv_test2);

    int h_bins = 50, s_bins = 60;
    int histSize[] = {h_bins, s_bins};

    //hue(色相) varies from 0 to 179,
    //staturation(饱和度)from 0 to 255
    float h_ranges[] = {0, 179};
    float s_ranges[] = {0, 255};
    const float *ranges[] = {h_ranges, s_ranges};

    //use the 0-th and 1-st channels
    int channels[] = {0, 1};

    Mat hist_base;
    Mat hist_half_down;
    Mat hist_test1;
    Mat hist_test2;

    calcHist(&hsv_base, 1, channels, Mat(),
             hist_base, 2, histSize, ranges, true, false);
    normalize(hist_base, hist_base, 0, 1, NORM_MINMAX, -1, Mat());

    calcHist(&hsv_half_down, 1, channels, Mat(),
             hist_half_down, 2, histSize, ranges, true, false);
    normalize(hist_half_down, hist_half_down, 0, 1, NORM_MINMAX, -1, Mat());

    calcHist(&hsv_test1, 1, channels, Mat(),
             hist_test1, 2, histSize, ranges, true, false);
    normalize(hist_test1, hist_test1, 0, 1, NORM_MINMAX, -1, Mat());

    calcHist(&hsv_test2, 1, channels, Mat(),
             hist_test2, 2, histSize, ranges, true, false);
    normalize(hist_test2, hist_test2, 0, 1, NORM_MINMAX, -1, Mat());

    for (int i = 0; i < 4; i++)
    {
        int compare_method = i;
        double base_base = compareHist(hist_base, hist_base, compare_method);
        double base_half = compareHist(hist_base, hist_half_down, compare_method);
        double base_test1 = compareHist(hist_base, hist_test1, compare_method);
        double base_test2 = compareHist(hist_base, hist_test2, compare_method);

        printf("Method [%d] Perfect, Base-Half, Base-Test1, Base-Test2 : %f,%f,%f,%f\n",
               i, base_base, base_half, base_test1, base_test2);
    }

    cout << "Done" << endl;
    waitKey();
    return 0;
}