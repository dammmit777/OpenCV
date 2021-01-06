/**
 * @file main.cpp
 * @author ssm
 * @brief how to use your own linear filters
 * @version 0.1
 * @date 2020-12-31
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
using namespace cv;

int main(int argc, char **argv)
{
    Mat src, dst;
    Mat kernel;
    Point anchor;
    double delta;
    int ddepth;
    int kernel_size;
    const char *windowname = "filter2D Demo";

    src = imread(argv[1], IMREAD_COLOR);
    if (src.empty())
    {
        std::cerr << "Error openning image" << std::endl;
        return EXIT_FAILURE;
    }

    //![init_arguements]
    anchor = Point(-1, -1);
    delta = 0;
    ddepth = -1;
    //![init_arguements]

    int ind = 0;
    while (true)
    {
        //update kernel size
        kernel_size = 3 + 2 * (ind % 5);
        kernel = Mat::ones(kernel_size, kernel_size, CV_32F) / (float)(kernel_size * kernel_size);

        filter2D(src, dst, ddepth, kernel, anchor, delta, BORDER_DEFAULT);

        imshow(windowname, dst);

        char c = (char)waitKey(1000);
        if (c == 27)
            break;

        ind++;
    }
    return EXIT_SUCCESS;
}