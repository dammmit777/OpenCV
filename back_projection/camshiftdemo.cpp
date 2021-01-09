#include <opencv2/core/utility.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <ctype.h>

using namespace std;
using namespace cv;

//Global Variables
Mat image;
bool backprojMode = false; //背投模式
bool selectObject = false; //选择对象
int trackObject = 0;       //跟踪对象
bool showHist = true;      //显示直方图
Point origin;              //起始点
Rect selection;            //选择框
int vmin = 10, vmax = 256, smin = 30;

string hot_keys = //热键提示
    "\n\nHot keys: \n"
    "\tESC - quit the program\n"
    "\tc - stop the tracking\n"
    "\tb - switch to/from backprojection view\n"
    "\th - show/hide object histogram\n"
    "\tp - pause video\n"
    "To initialize tracking, select the object with mouse\n";

const char *keys =
    {"{help h | | show help message}{@camera_number| 0 | camera number}"};

/**
 * @brief some helpful message for the Demo
 * @param argv 
 */
static void help(char **argv)
{
    cout << "\nThis is a demo that shows mean-shift based tracking\n"
            "You select a color objects to track.\n"
            "This reads from video camera 0 by default.\n"
            "Usage:\n\t";
    cout << argv[0] << "[camera number]\n";
    cout << hot_keys;
}

//header functions
static void onMouse(int event, int x, int y, int, void *);

int main(int argc, char **argv)
{
    VideoCapture cap; //定义一个摄像头捕捉类对象
    Rect trackWindow;
    int hsize = 16;
    float hrange[] = {0, 180};
    const float *phranges = hrange;
    CommandLineParser parser(argc, argv, keys); //命令解析器
    if (parser.has("help"))
    {
        help(argv);
        return 0;
    }

    //打开计算机自带的摄像头
    int camNum = parser.get<int>(0);
    cap.open(camNum);
    if (!cap.isOpened())
    {
        help(argv);
        cout << "***Could not initialize capturing...***" << endl;
        cout << "Current parameter's value: " << endl;
        parser.printMessage();
        return -1;
    }

    cout << hot_keys;

    //create display window
    namedWindow("Histogram", WINDOW_AUTOSIZE);
    namedWindow("CamShift Demo", WINDOW_AUTOSIZE);

    //在 "CamShift Demo" 窗口中设置鼠标动作反馈
    setMouseCallback("CamShift Demo", onMouse, 0);

    //create Trackbar
    createTrackbar("Vmin", "CamShift Demo", &vmin, 256, 0);
    createTrackbar("Vmax", "CamShift Demo", &vmax, 256, 0);
    createTrackbar("Smin", "CamShift Demo", &smin, 256, 0);

    Mat frame, hsv, hue, mask, hist, backproj;
    Mat histImg = Mat::zeros(200, 320, CV_8UC3);
    bool pause = false;

    while (true)
    {
        if (!pause)
        {
            //相机将图像传入frame (一帧)
            cap >> frame;
            if (frame.empty())
                break;
        }
        //将帧复制到image容器中，用于后续处理
        frame.copyTo(image);

        if (!pause)
        {
            //将RGB图转换为色相饱和度图
            cvtColor(image, hsv, COLOR_BGR2HSV);
            
            if (trackObject)
            {
                int _vmin = vmin, _vmax = vmax;
                //图像阈值化，对在高低范围内的像素置255 (白色)
                //其余像素置0 (黑色)
                //得到的 mask 为二值化图像
                inRange(hsv,
                        Scalar(0, smin, MIN(_vmin, _vmax)),
                        Scalar(180, 256, MAX(_vmin, _vmax)),
                        mask);

                int channel[] = {0, 0};
                hue.create(hsv.size(), hsv.depth());
                mixChannels(&hsv, 1, &hue, 1, channel, 1);

                //鼠标左键被抬起后，即已选定好追踪框
                if (trackObject < 0)
                {
                    Mat roi(hue, selection);
                    Mat maskroi(mask, selection);
                    //计算直方图
                    calcHist(&roi, 1, 0, maskroi, hist,
                             1, &hsize, &phranges);
                    //归一化处理
                    normalize(hist, hist, 0, 255, NORM_MINMAX, -1);

                    trackWindow = selection;
                    trackObject = 1;

                    //初始化直方图显示界面（背景黑色）
                    histImg = Scalar::all(0);
                    int binW = histImg.cols / hsize;
                    Mat buff(1, hsize, CV_8UC3);
                    for (int i = 0; i < hsize; i++)
                    {
                        buff.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i * 180. / hsize), 255, 255);
                    }
                    cvtColor(buff, buff, COLOR_HSV2BGR);
                    //绘制直方图
                    for (int i = 0; i < hsize; i++)
                    {
                        int val = saturate_cast<int>(hist.at<float>(i) * histImg.rows / 255);
                        rectangle(histImg, Point(i * binW, histImg.rows),
                                  Point((i + 1) * binW, histImg.rows - val),
                                  Scalar(buff.at<Vec3b>(i)), -1, 8);
                    }
                }

                //执行 CAMShift
                calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
                backproj &= mask;
                //定义旋转矩阵类
                RotatedRect trackBox =
                    CamShift(backproj, trackWindow,
                             TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 10, 1));

                if (trackWindow.area() <= 1)
                {
                    int cols = backproj.cols;
                    int rows = backproj.rows;
                    int r = (MIN(cols, rows) + 5) / 6;
                    trackWindow = Rect(trackWindow.x - r, trackWindow.y - r,
                                       trackWindow.x + r, trackWindow.y + r) &
                                  Rect(0, 0, cols, rows);
                }

                if (backprojMode)
                    cvtColor(backproj, image, COLOR_GRAY2BGR);

                ellipse(image, trackBox, Scalar(0, 0, 255), 3, LINE_AA);
            }
        }

        else if (trackObject < 0)
            pause = false;

        if (selectObject && selection.width > 0 && selection.height > 0)
        {
            Mat roi(image, selection);
            bitwise_not(roi, roi);
        }

        imshow("CamShift Demo", image);
        imshow("Histogram", histImg);

        char c = (char)waitKey(10);
        if (c == 27)
        {
            cap.release();
            break;
        }

        switch (c)
        {
        case 'b':
            backprojMode = !backprojMode;
            break;

        case 'c':
            trackObject = 0;
            histImg = Scalar::all(0);
            break;

        case 'h':
            showHist = !showHist;
            if (!showHist)
                destroyWindow("Histogram");
            else
                namedWindow("Histogram", WINDOW_AUTOSIZE);
            break;

        case 'p':
            pause = !pause;
            break;

        default:;
        }
    }

    return 0;
}

/**
 * @brief 在要追踪的对象周围绘制方框
 *         从而触发CAMshift开始追踪
 * @param event - 鼠标动作事件
 * @param x 
 * @param y 
 */
static void onMouse(int event, int x, int y, int, void *)
{
    //获取选择框的大小和位置
    if (selectObject)
    {
        selection.x = MIN(x, origin.x);
        selection.y = MIN(y, origin.y);
        selection.width = std::abs(x - origin.x);
        selection.height = std::abs(y - origin.y);

        //选择框最大=输入图像的大小
        selection &= Rect(0, 0, image.cols, image.rows);
    }

    //控制鼠标，画选择框
    switch (event)
    {
    //鼠标左键被按下的情况
    case EVENT_LBUTTONDOWN:
        origin = Point(x, y);
        selection = Rect(x, y, 0, 0);
        selectObject = true;
        break;

    //鼠标左键被抬起的情况
    case EVENT_LBUTTONUP:
        selectObject = false;
        if (selection.width > 0 && selection.height > 0)
            trackObject = -1; //在 main 函数中设置 CAMshift 属性
        break;
    }
}
