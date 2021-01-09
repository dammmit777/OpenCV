/**
 * @file calcBackProject_Demo2.cpp
 * @author ssm
 * @brief using H-S histograms and floodFill to define a mask for the area
 * @version 0.1
 * @date 2021-01-09
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

//Global Variables
Mat src, hsv, mask;
int low = 20, up = 20;

void Hist_and_BackProject();
void pickPoint(int event, int x, int y, int, void *);

int main(int argc, char **argv)
{
    src = imread(argv[1], IMREAD_COLOR);

    cvtColor(src, hsv, COLOR_BGR2HSV);

    namedWindow("src", WINDOW_AUTOSIZE);
    imshow("src", src);

    createTrackbar("Low thresh", "src", &low, 255, 0);
    createTrackbar("High thresh", "src", &up, 255, 0);

    //set a mouse callback
    setMouseCallback("src", pickPoint, 0);

    waitKey();
    return 0;
}

/**
 * @brief calculate and histogram the image
 * 
 */
void Hist_and_BackProject()
{
    Mat hist;
    int h_bins = 30, s_bins = 32;
    int histSize[] = {h_bins, s_bins};
    float h_range[] = {0, 180};
    float s_range[] = {0, 256};
    const float *ranges[] = {h_range, s_range};
    int channels[] = {0, 1};

    //get the histogram
    calcHist(&hsv, 1, channels, mask, hist, 2, histSize,
             ranges, true, false);

    //normalize the histogram
    normalize(hist, hist, 0, 255, NORM_MINMAX, -1, Mat());

    //get backProjection
    Mat backProject;
    calcBackProject(&hsv, 1, channels, hist, backProject,
                    ranges, 1, true);

    imshow("BackProjection", backProject);
}

/**
 * @brief 应用FLOODFILL算法
 * 算法遍历所有的节点以寻找和种子节点相连的节点，然后
 * 改变他们的颜色为替换颜色
 */
void pickPoint(int event, int x, int y, int, void *)
{
    if (event != EVENT_LBUTTONDOWN) //事件！=鼠标左键按下
        return;

    //fill and get the mask
    Point seed = Point(x, y);

    int newMaskVal = 255;
    Scalar newVal = Scalar(120, 120, 120);

    int connectivity = 8;
    int flags = connectivity +
                (newMaskVal << 8) +
                FLOODFILL_FIXED_RANGE +
                FLOODFILL_MASK_ONLY;

    Mat mask2 = Mat::zeros(src.rows + 2,
                           src.cols + 2, CV_8U);

    floodFill(src, mask2, seed, newVal,
              0, Scalar(low, low, low),
              Scalar(up, up, up), flags);

    mask = mask2(Range(1, mask2.rows - 1),
                 Range(1, mask2.cols - 1));

    imshow("Mask", mask);

    Hist_and_BackProject();
}
