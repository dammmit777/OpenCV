/**
 * @file main.cpp
 * @author ssm
 * @brief showing how to detect edges using the laplace operator
 * @version 0.1
 * @date 2021-01-05
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
    Mat src, src_gray, dst;
    int kernel_size = 3;
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;
    const char *windowname = "Laplace Demo";

    src = imread(argv[1], IMREAD_COLOR);
    if (src.empty())
    {
        cerr << "Error openning image" << endl;
        return EXIT_FAILURE;
    }

    //reduce noise
    GaussianBlur(src, src, Size(3, 3),
                 0, 0, BORDER_DEFAULT);

    //convert to gray
    cvtColor(src, src_gray, COLOR_BGR2GRAY);

    //apply laplace function
    Mat abs_dst;
    Laplacian(src_gray, dst, ddepth,
              kernel_size, scale, delta, BORDER_DEFAULT);

    //convert back to CV_8U
    convertScaleAbs(dst, abs_dst);

    imshow(windowname, abs_dst);
    waitKey(0);

    return 0;
}