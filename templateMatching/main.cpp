#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//Global Variables
Mat image, templ, result;
const char *image_window = "source image";
const char *result_window = "result window";
int match_method;

//function header
void matchingMethod(int, void *);

//main
int main(int argc, char **argv)
{
    //Load images
    image = imread(argv[1], IMREAD_COLOR);
    templ = imread(argv[2], IMREAD_COLOR);
    if (image.empty() || templ.empty())
    {
        cerr << "Error openning the images" << endl;
        return -1;
    }

    //create windows
    namedWindow(image_window, WINDOW_AUTOSIZE);
    namedWindow(result_window, WINDOW_AUTOSIZE);

    //create Trackbar 提供可选滑块，可以选用不同的算法，直观地看到匹配结果
    createTrackbar("Method:\n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED",
                   image_window, &match_method, 5, matchingMethod);

    //Template Matching
    matchingMethod(0, 0);

    waitKey();
    return 0;
}

void matchingMethod(int, void *)
{
    Mat img_display;
    image.copyTo(img_display);

    //创建 result 矩阵存放每个匹配结果
    int result_cols = image.cols - templ.cols + 1;
    int result_rows = image.rows - templ.rows + 1;
    result.create(result_rows, result_cols, CV_32FC1);

    //执行模板匹配
    matchTemplate(image, templ, result, match_method);

    //归一化操作
    normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

    //在 result 矩阵中，对最小值和最大值所处的坐标进行定位
    double minVal, maxVal;
    Point minLoc, maxLoc, matchLoc;
    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

    //对于 TM_SQDIFF 和 TM_SQDIFF_NORMED 算法来说，结果越小表明匹配程度越高；其他算法是结果越大匹配程度越高
    if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED)
        matchLoc = minLoc;
    else
        matchLoc = maxLoc;

    //用矩阵框选出匹配程度最高的区域
    rectangle(img_display,
              matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows),
              Scalar(0, 0, 255), 2, 8, 0);
    rectangle(result,
              matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows),
              Scalar(0, 255, 0), 2, 8, 0);

    //显示图像匹配结果
    imshow(image_window, img_display);
    imshow(result_window, result);
}
