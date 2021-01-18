/**
 * @file pyramid
 * @author ssm
 * @brief image pyramids
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

using namespace cv;

//Global Variables
Mat src, dst, tmp;
const char *windowname = "Pyramids Demo";

int main(int argc, char **argv)
{
    std::cout << "\n Zoom In-Out demo" << std::endl;
    std::cout << "--------------------" << std::endl;
    std::cout << " * [u] -> Zoon in" << std::endl;
    std::cout << " * [d] -> Zoon out" << std::endl;
    std::cout << "* [ESC] -> Close program" << std::endl;

    src = imread(argv[1]);
    if (src.empty())
    {
        std::cout << "no data !" << std::endl;
        return -1;
    }

    tmp = src;
    dst = tmp;

    namedWindow(windowname, WINDOW_AUTOSIZE);
    imshow(windowname, dst);

    while (true)
    {
        char c;
        c = waitKey(10);

        if (c == 27)
            break;
        if (c == 'u')
        {
            pyrUp(tmp, dst, Size(tmp.cols * 2, tmp.rows * 2));
            std::cout << "** Zoom In: Image x 2" << std::endl;
        }
        else if (c == 'd')
        {
            pyrDown(tmp, dst, Size(tmp.cols / 2, tmp.rows / 2));
            std::cout << "** Zoom out: Image /2" << std::endl;
        }

        imshow(windowname, dst);
        tmp = dst;
    }

    return 0;
}