/**
 * @file imageSegmentation.cpp
 * @author ssm
 * @brief 基于距离变换和分水岭算法的图像分割
 * @version 0.1
 * @date 2021-01-14
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
Mat src;
int blue_track = 0;
int green_track = 0;
int red_track = 0;

void trackbar_callback(int, void *);

int main(int argc, char **argv)
{
    //load image
    src = imread(argv[1], IMREAD_COLOR);
    if (src.empty())
        return -1;
    imshow("Source Image ", src); //显示src

    //![创建颜色调整模块]
    namedWindow("Color Trackbar", WINDOW_AUTOSIZE);
    createTrackbar("Blue", "Color Trackbar", &blue_track, 255, trackbar_callback);
    createTrackbar("Green", "Color Trackbar", &green_track, 255, trackbar_callback);
    createTrackbar("Red", "Color Trackbar", &red_track, 255, trackbar_callback);
    trackbar_callback(0, 0);
    Mat color_track = Mat::zeros(100, 500, CV_8U);
    //imshow("Color Trackbar", color_track);
    //![创建颜色调整模块]

    //锐化
    Mat kernel = (Mat_<float>(3, 3) << 1, 1, 1,
                  1, -8, 1,
                  1, 1, 1);

    Mat imgLaplacian;
    filter2D(src, imgLaplacian, CV_32F, kernel);
    Mat sharp;
    src.convertTo(sharp, CV_32F);
    Mat imgResult = sharp - imgLaplacian;
    imgResult.convertTo(imgResult, CV_8UC3);
    imgLaplacian.convertTo(imgLaplacian, CV_8UC3);
    // imshow("Laplace filter", imgLaplacian);
    // imshow("New sharp", imgResult);

    //bin
    Mat bw;
    cvtColor(imgResult, bw, COLOR_BGR2GRAY);
    threshold(bw, bw, 40, 255, THRESH_BINARY | THRESH_OTSU);
    //imshow("binary", bw);

    //dist
    Mat dist;
    distanceTransform(bw, dist, DIST_L2, 3);
    normalize(dist, dist, 0, 1.0, NORM_MINMAX);
    imshow("Distance", dist);

    //peaks
    threshold(dist, dist, 0.4, 1.0, THRESH_BINARY);

    //dilate a bit the dist image
    Mat kernel1 = Mat::ones(3, 3, CV_8U);
    dilate(dist, dist, kernel1);
    imshow("peaks", dist);

    //seed
    Mat dist_8u;
    dist.convertTo(dist_8u, CV_8U);
    //Find total marks
    vector<vector<Point> > contours;
    findContours(dist_8u, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    //create the marker image for the watershed algorithm
    Mat markers = Mat::zeros(dist.size(), CV_32S);

    //draw the foreground markers
    for (size_t i = 0; i < contours.size(); i++)
    {
        drawContours(markers, contours, static_cast<int>(i), Scalar(static_cast<int>(i) + 1), -1);
    }
    circle(markers, Point(5, 5), 3, Scalar(255), -1);
    imshow("Markers", markers * 10000);

    //watershed
    watershed(imgResult, markers);

    Mat mark;
    markers.convertTo(mark, CV_8U);
    bitwise_not(mark, mark);

    vector<Vec3b> colors;
    for (size_t i = 0; i < contours.size(); i++)
    {
        int b = theRNG().uniform(0, 256);
        int g = theRNG().uniform(0, 256);
        int r = theRNG().uniform(0, 256);

        colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
    }

    //create the result image
    Mat dst = Mat::zeros(markers.size(), CV_8UC3);

    //fill objects with random colors
    for (int i = 0; i < markers.rows; i++)
    {
        for (int j = 0; j < markers.cols; j++)
        {
            int index = markers.at<int>(i, j);
            if (index > 0 && index <= static_cast<int>(contours.size()))
            {
                dst.at<Vec3b>(i, j) = colors[index - 1];
            }
        }
    }
    imshow("final", dst);

    waitKey();
    return 0;
}

void trackbar_callback(int, void *)
{
    // Mat color_display = Mat::zeros(100, 100, CV_8UC3);
    // color_display = Scalar(blue_track, green_track, red_track);
    // imshow("Color display", color_display);

    //将图片背景转换成黑色
    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            if (src.at<Vec3b>(i, j) == Vec3b(255, 255, 255))
            {
                src.at<Vec3b>(i, j)[0] = 0;
                src.at<Vec3b>(i, j)[1] = 0;
                src.at<Vec3b>(i, j)[2] = 0;
            }
        }
    }
    imshow("Black background image", src); //显示背景转换完成后的src
}
