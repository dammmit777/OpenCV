/**
 * @file hull_demo.cpp
 * @author ssm
 * @brief 在多维空间中有一群散布各处的点，“凸包”是包裹这群点的所有外壳当中，表面积及容积最小的一个而最小的外壳一定是凸的
 * 
 * @version 0.1
 * @date 2021-01-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//Global Variables
Mat src, src_gray;
int thresh = 100;

//function header
void thresh_callback(int, void *);

//Main
int main(int argc, char **argv)
{
    //load image
    src = imread(argv[1], IMREAD_COLOR);
    if (src.empty())
    {
        cerr << "Could not open or find the image " << endl;
        return -1;
    }

    //convert the image to gray scale
    cvtColor(src, src_gray, COLOR_BGR2GRAY);

    //blur the gray image
    blur(src_gray, src_gray, Size(3, 3), Point(-1, -1), BORDER_REFLECT_101);

    //create window
    const char *windowname = "Source";
    namedWindow(windowname, WINDOW_AUTOSIZE);
    imshow(windowname, src);

    //create trackbar
    createTrackbar("Thresh:", windowname, &thresh, 255, thresh_callback);
    thresh_callback(0, 0);

    waitKey();
    return 0;
}

/**
 * @brief 找到图像的轮廓，并保存拐点，根据拐点画出凸包
 * 
 */
void thresh_callback(int, void *)
{
    //detect edges
    Mat edges;
    Canny(src_gray, edges, thresh, thresh * 2);

    //find contours
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    //检测所有轮廓，并建立等级树，但是只保存轮廓的拐点信息
    findContours(edges, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    //find the convex hull object for each contour
    vector<vector<Point> > hull(contours.size());
    for (size_t i = 0; i < contours.size(); i++)
    {
        convexHull(contours[i], hull[i]);
    }

    //draw contours and hull result
    Mat drawing = Mat::zeros(edges.size(), CV_8UC3);
    for (size_t i = 0; i < contours.size(); i++)
    {
        drawContours(drawing, contours, (int)i, Scalar(0, 0, 255));
        drawContours(drawing, hull, (int)i, Scalar(0, 255, 0));
    }

    imshow("result", drawing);
}

/**
 * @brief void convexHull(InputArray points, OutputArray hull, bool clockwise=false, bool returnPoints=true);
 * @param points, 用来求凸包的点集
 *                当参数为 std::vector<int> 时，返回凸包点在原轮廓点集中的索引
 *                当参数为 std::vector<cv::Point> 时，表示凸包点的位置
 * @param hull, 输出的凸包点
 * @param clockwise, 表示求得的凸包是顺时针方向还是逆时针方向，true 是顺时针， false 是 逆时针
 * 
 * 
 */