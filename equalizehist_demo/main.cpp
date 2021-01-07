/**
 * @file main.cpp
 * @author ssm
 * @brief code for equalizeHist function
 * @version 0.1
 * @date 2021-01-07
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    Mat src = imread(argv[1], IMREAD_COLOR);
    if (src.empty())
    {
        cerr << "Error openning image" << endl;
        return -1;
    }

    Mat src_gray;
    cvtColor(src, src_gray, COLOR_BGR2GRAY);

    Mat dst;
    equalizeHist(src_gray, dst);

    imshow("src", src);
    imshow("src_gray", src_gray);
    imshow("equalize", dst);

    waitKey(0);
    return 0;
}