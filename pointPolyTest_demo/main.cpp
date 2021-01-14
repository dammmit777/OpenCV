/**
 * @file main.cpp
 * @author ssm
 * @brief use the pointPolyTest function
 * @version 0.1
 * @date 2021-01-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
    const int r = 100;
    Mat src = Mat::zeros(Size(4 * r, 4 * r), CV_8U);

    //创建点序列
    vector<Point2f> vert(6);
    vert[0] = Point(3 * r / 2, static_cast<int>(1.34 * r));
    vert[1] = Point(1 * r, 2 * r);
    vert[2] = Point(3 * r / 2, static_cast<int>(2.866 * r));
    vert[3] = Point(5 * r / 2, static_cast<int>(2.866 * r));
    vert[4] = Point(3 * r, 2 * r);
    vert[5] = Point(5 * r / 2, static_cast<int>(1.34 * r));

    //draw
    for (int i = 0; i < 6; i++)
    {
        line(src, vert[i], vert[(i + 1) % 6], Scalar(255), 3);
    }

    //get contours
    vector<vector<Point> > contours;
    findContours(src, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

    //计算到轮廓的距离
    Mat dist(src.size(), CV_32F);
    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            dist.at<float>(i, j) = (float)pointPolygonTest(contours[0], Point2f((float)j, (float)i), true);
        }
    }

    double minVal, maxVal;
    minMaxLoc(dist, &minVal, &maxVal, 0, 0, Mat());
    minVal = abs(minVal);
    maxVal = abs(maxVal);

    Mat drawing = Mat::zeros(src.size(), CV_8UC3);
    for (int j = 0; j < src.rows; j++)
    {
        for (int i = 0; i < src.cols; i++)
        {
            if (dist.at<float>(j, i) < 0)
            {
                drawing.at<Vec3b>(j, i)[0] = (uchar)(255 - abs(dist.at<float>(j, i)) * 255 / minVal);
            }
            else if (dist.at<float>(j, i) > 0)
            {
                drawing.at<Vec3b>(j, i)[2] = (uchar)(255 - dist.at<float>(j, i) * 255 / maxVal);
            }
            else
            {
                drawing.at<Vec3b>(j, i)[0] = 255;
                drawing.at<Vec3b>(j, i)[1] = 255;
                drawing.at<Vec3b>(j, i)[2] = 255;
            }
        }
    }
    const char *source_window = "Source";
    namedWindow(source_window, WINDOW_AUTOSIZE);
    imshow(source_window, src);
    namedWindow("Distance", WINDOW_AUTOSIZE);
    imshow("Distance", drawing);
    waitKey(0);
    return (0);
}