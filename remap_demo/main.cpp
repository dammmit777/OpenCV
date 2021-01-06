/**
 * @file main.cpp
 * @author ssm
 * @brief Remap
 * @version 0.1
 * @date 2021-01-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;

void update_map(int &ind, Mat &map_x, Mat &map_y);

int main(int argc, char **argv)
{
    //Variables
    Mat src, dst;
    Mat map_x, map_y;
    const char *windowname = "Remap Demo";

    src = imread(argv[1], IMREAD_COLOR);
    if (src.empty())
    {
        std::cout << "Error openning image " << std::endl;
        return -1;
    }
    imshow("src", src);

    dst.create(src.size(), src.type());
    map_x.create(src.size(), CV_32FC1);
    map_y.create(src.size(), CV_32FC1);

    namedWindow(windowname, WINDOW_AUTOSIZE);

    int ind = 0;
    char c = 0;
    while (c != 27)
    {
        if (c == '1')
            ind = 0;
        if (c == '2')
            ind = 1;
        if (c == '3')
            ind = 2;
        if (c == '4')
            ind = 3;
        update_map(ind, map_x, map_y);
        remap(src, dst, map_x, map_y,
              INTER_LINEAR, BORDER_CONSTANT,
              Scalar(0, 0, 0));

        imshow(windowname, dst);

        c = (char)waitKey(1000);
    }
}

void update_map(int &ind, Mat &map_x, Mat &map_y)
{
    for (int i = 0; i < map_x.rows; i++)
    {
        for (int j = 0; j < map_x.cols; j++)
        {
            switch (ind)
            {
            case 0:
                if (j > map_x.cols * 0.25 &&
                    j < map_x.cols * 0.75 &&
                    i > map_x.rows * 0.25 &&
                    i < map_x.rows * 0.75)
                {
                    map_x.at<float>(i, j) =
                        2 * (j - map_x.cols * 0.25f) + 0.5f;
                    map_y.at<float>(i, j) =
                        2 * (i - map_x.rows * 0.25f) + 0.5f;
                }
                else
                {
                    map_x.at<float>(i, j) = 0;
                    map_y.at<float>(i, j) = 0;
                }
                break;

            case 1:
                map_x.at<float>(i, j) = (float)j;
                map_y.at<float>(i, j) = (float)(map_x.rows - i);
                break;

            case 2:
                map_x.at<float>(i, j) = (float)(map_x.cols - j);
                map_y.at<float>(i, j) = (float)i;
                break;

            case 3:
                map_x.at<float>(i, j) = (float)(map_x.cols - j);
                map_y.at<float>(i, j) = (float)(map_x.rows - i);
                break;

            default:
                break;
            }
        }
    }
}
