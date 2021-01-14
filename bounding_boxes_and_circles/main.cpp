#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//Global Variables
Mat src, src_gray;
int thresh = 100;
const char *source_window = "Source";
const char *contours_window = "Contours";

//function header
void thresh_callback(int, void *);

//main
int main(int argc, char **argv)
{
    //load image
    src = imread(argv[1], IMREAD_COLOR);
    if (src.empty())
    {
        cout << "Could not open or find the image " << endl;
        return -1;
    }

    //convert the image to gray scale and blur it
    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    blur(src_gray, src_gray, Size(3, 3), Point(-1, -1), BORDER_REFLECT101);

    namedWindow(source_window, WINDOW_AUTOSIZE);
    //create trackbar
    createTrackbar("Thresh", source_window, &thresh, 255, thresh_callback);
    thresh_callback(0, 0);

    imshow(source_window, src);
    waitKey();

    return 0;
}

void thresh_callback(int, void *)
{
    //阈值处理
    Mat thresh_output;
    threshold(src_gray, thresh_output, thresh, 255, THRESH_BINARY);

    //寻找图像中的轮廓
    std::vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(thresh_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

    vector<vector<Point> > contours_poly(contours.size()); //轮廓的近似轮廓
    vector<Rect> boundRect(contours.size());              //轮廓的最小矩形边框
    vector<Point2f> center(contours.size());              //轮廓的最小圆形边框的圆心
    vector<float> radius(contours.size());                //圆形边框的半径

    for (size_t i = 0; i < contours.size(); i++)
    {
        /**
         * @brief 以指定精度逼近多边形曲线
         * @param curve, input vector of a 2D point stored in std::vector or Mat
         * @param approxCurve, Result of the approximation. The type should match the type of the input cuve.
         * @param epsilon, This is the maximum distance between the original curve and its approximation.
         * @param closed, If true,the approximated curve is closed(its first and last vertices are connected).Otherwise,it is not closed.
         * 
         */
        approxPolyDP(contours[i], contours_poly[i], 3, true);
        //绘制矩形边框
        boundRect[i] = boundingRect(contours_poly[i]);
        //绘制圆形边框
        minEnclosingCircle(contours_poly[i], center[i], radius[i]);
    }

    //画出结果
    Mat drawing = Mat::zeros(thresh_output.size(), CV_8UC3);
    for (size_t i = 0; i < contours.size(); i++)
    {
        //绘制轮廓
        drawContours(drawing, contours_poly, (int)i, Scalar(255, 255, 255), 1, 8, vector<Vec4i>(), 0, Point(0, 0));
        //绘制矩形边框
        rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 0, 255), 1, 8, 0);
        //绘制圆形边框
        circle(drawing, center[i], (int)radius[i], Scalar(0, 255, 0), 1, 8, 0);
    }

    imshow(contours_window, drawing);
}