#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    Mat in_frame, out_frame;
    const char win1[] = "Grabbing...", win2[] = "Recording...";
    double fps = 30; //每秒的帧数
    char file_out[] = "recorded.avi";

    VideoCapture inVid(0); //打开默认摄像机
    if (!inVid.isOpened())
    {
        cout << "Error! Camera not ready..." << endl;
        return 1;
    }

    //获取输入视频的宽度和高度
    int width = (int)inVid.get(CAP_PROP_FRAME_WIDTH);
    int height = (int)inVid.get(CAP_PROP_FRAME_HEIGHT);
    VideoWriter recVid(file_out, VideoWriter::fourcc('M', 'P', 'E', 'G'), fps, Size(width, height));

    if (!recVid.isOpened())
    {
        cout << "Error! Video file not opened..." << endl;
        return 1;
    }

    //为原始视频和最终视频创建两个窗口
    namedWindow(win1);
    namedWindow(win2);

    while (true)
    {
        //从摄像机读取帧（抓取并解码）
        inVid >> in_frame;

        //将帧转换为灰度
        cvtColor(in_frame, out_frame, COLOR_BGR2GRAY);

        //将帧写入视频文件（编码并保存）
        recVid << out_frame;

        imshow(win1, in_frame);
        imshow(win2, out_frame);

        if (waitKey(1000 / fps) >= 0)
            break;
    }

    inVid.release(); //关闭摄像机
    return 0;
}