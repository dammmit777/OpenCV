/**
 * @file main.cpp
 * @author ssm
 * @brief 应用 findContours 函数寻找轮廓，应用 drawContours 函数画出轮廓
 * @version 0.1
 * @date 2021-01-11
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
Mat src, src_gray;
int thresh = 100;
int mode_thresh = 0;
int max_mode_thresh = 3;
int method_thresh = 1;
int max_method_thresh = 4;
int max_threshold = 255;

//Function header
void thresh_callback(int, void *);

//Main
int main(int argc, char **argv)
{
    src = imread(argv[1], IMREAD_COLOR);
    if (src.empty())
    {
        cerr << "Error openning image." << endl;
        return -1;
    }

    //转换成灰度图并滤波
    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    blur(src_gray, src_gray, Size(3, 3), Point(-1, -1), BORDER_REFLECT_101);

    //create display window
    namedWindow("Source", WINDOW_AUTOSIZE);
    imshow("Source", src);

    //create trackbar
    createTrackbar("Canny thresh:", "Source", &thresh, max_threshold, thresh_callback);
    createTrackbar("Mode thresh", "Source", &mode_thresh, max_mode_thresh, thresh_callback);
    createTrackbar("Method thresh", "Source", &method_thresh, max_method_thresh, thresh_callback);
    thresh_callback(0, 0);

    waitKey();
    return 0;
}

void thresh_callback(int, void *)
{
    Mat canny_dst;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    Canny(src_gray, canny_dst, thresh, thresh * 2, 3);

    //找边界*
    findContours(canny_dst, contours, hierarchy, mode_thresh, method_thresh, Point(0, 0));

    Mat drawing = Mat::zeros(canny_dst.size(), CV_8UC3);
    for (size_t i = 0; i < contours.size(); i++)
    {
        drawContours(drawing, contours, (int)i, Scalar(255, 255, 255), 1, 8, hierarchy, 0, Point());
    }

    namedWindow("Contours", WINDOW_AUTOSIZE);
    imshow("Contours", drawing);
}

/**
 * @brief findContours 函数详解
 * @brief findContours( InputOutputArray image, 
 *                      OutputArrayOfArrays contours,
 *                      OutputArray hierarchy, int mode,
 *                      int method, Point offset = Point());
 * @param image，单通道图像矩阵，可以是灰度图，但更常用的是二值图像，
 *              一般是经过 Canny、拉普拉斯等边缘检测算子处理过的二值图像；
 * @param contours, 定义为 “vector<vector<Point>> contours”, 是一个向量，
 *                  并且是一个双向向量，向量内每个元素保存了一组由连续的 Point 
 *                  点构成的点的集合的向量，每一组 Point 点集就是一个轮廓，
 *                  有多少轮廓，向量 contours 就有多少元素。
 * @param hierarchy，定义为 "vector<Vec4i> hierarchy"，Vec4i 定义了一个
 *                  "向量内每一个元素包含了4个 int 型变量"的向量。即 hierarchy 是一个向量，
 *                  hierarchy 内每个元素保存了一个包含 4 个 int 型的数组。
 *                  hierarchy 内的元素和 contours 内的元素是一一对应的，向量的容量相同。
 *                  hierarchy 内每个元素——hierarchy[i][0]~hierarchy[i][3],分别表示
 *                  第 i 个轮廓的 后一个轮廓、 前一个轮廓、 父轮廓、 内嵌轮廓 的索引编号。
 *                  如果当前轮廓没有对应的 后一个、前一个、父、 内嵌轮廓，则
 *                  hierarchy[i][0]~hierarchy[i][3] 的相应位被设置为默认值 -1。
 * @param mode, 定义轮廓的检索模式：
 *              CV_RETR_EXTERNAL = 0 : 只检测最外围轮廓，包含在外围轮廓内的内围轮廓被忽略；
 *              CV_RETR_LIST = 1 : 检测所有的轮廓，包括内围、外围轮廓，但是检测到的轮廓不建立等级关系，
 *                                  彼此之间独立，这就意味着这个检索模式下不存在父轮廓和内嵌轮廓，
 *                                  所以 hierarchy 向量内所有元素的第3、4 个分量都会被置为 -1。
 *              CV_RETR_CCOMP = 2 : 检测所有的轮廓，但所有轮廓只建立两个等级关系，外围为顶层
 *              CV_RETR_TREE = 3 : 检测所有轮廓，所有轮廓建立一个等级树解构
 * @param method, 定义轮廓的近似方法：
 *                CV_CHAIN_APPROX_NONE = 1 : 保存物体边界上所有连续的轮廓点到 contours 内
 *                CV_CHAIN_APPROX_SIMPLE = 2 : 仅保存轮廓的拐点信息，把所有轮廓的拐点处的点保存入 contours , 拐点与拐点之间直线段上的信息点不保留
 *                CV_CHAIN_APPROX_TC89_L1 = 3
 *                CV_CHAIN_APPROX_TC89_KCOS = 4 : 3和4 使用 tec-Chinl chain 近似算法
 * @param Point, 所有的轮廓信息相对于原始图像对应点的偏移量。
 * 
 */