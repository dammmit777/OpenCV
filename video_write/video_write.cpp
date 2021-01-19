/**
 * @file video_write.cpp
 * @author ssm
 * @brief 对视频流进行写操作；提取输入视频的 R、G、B 通道
 * @version 0.1
 * @date 2021-01-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    cout << "./video_write inputVideoName [R | G | B] [Y | N]" << endl;
    if (argc != 4)
    {
        cout << "No enough parameters" << endl;
        return -1;
    }

    const string source = argv[1];                //源文件名称
    const bool askOutputType = argv[3][0] == 'Y'; //如果为 false，则使用输入视频的编解码类型

    VideoCapture inputVideo(source);
    if (!inputVideo.isOpened())
    {
        cout << "Could not open the input video: " << source << endl;
        return -1;
    }

    string::size_type pAt = source.find_last_of('.');                //查找扩展点，如.avi的点
    const string NAME = source.substr(0, pAt) + argv[2][0] + ".avi"; //使用容器形成新名称
    int ex = static_cast<int>(inputVideo.get(CAP_PROP_FOURCC));      //获取编解码器类型-整数形式

    //将 int 类型转换为 char 类型
    union
    {
        int v;
        char c[5];
    } uEx;
    uEx.v = ex;
    uEx.c[4] = '\0';

    //获取输入视频的尺寸
    Size S = Size((int)inputVideo.get(CAP_PROP_FRAME_WIDTH), (int)inputVideo.get(CAP_PROP_FRAME_HEIGHT));

    //打开输出
    VideoWriter outputVideo;
    if (askOutputType)
        outputVideo.open(NAME, ex = -1, inputVideo.get(CAP_PROP_FPS), S, true);
    else
        outputVideo.open(NAME, ex, inputVideo.get(CAP_PROP_FPS), S, true);
    if (!outputVideo.isOpened())
    {
        cout << "Could not open the output video for write:" << source << endl;
        return -1;
    }

    cout << "Input frame resolution: Width = " << S.width << " Height = " << S.height
         << " of nr#: " << inputVideo.get(CAP_PROP_FRAME_COUNT) << endl;
    cout << "Input codec type: " << uEx.c << endl;

    int channel = 2; //选择要保存的通道
    switch (argv[2][0])
    {
    case 'R':
        channel = 2;
        break;
    case 'G':
        channel = 1;
        break;
    case 'B':
        channel = 0;
        break;
    }

    Mat src, res;
    vector<Mat> spl;
    while (true)
    {
        inputVideo >> src; //read
        if (src.empty())
            break;

        split(src, spl); //仅提取正确的通道
        for (int i = 0; i < 3; i++)
        {
            if (i != channel)
                spl[i] = Mat::zeros(S, spl[0].type());
        }
        merge(spl, res);
        outputVideo << res;
    }
    cout << "finish writing" << endl;
    return 0;
}