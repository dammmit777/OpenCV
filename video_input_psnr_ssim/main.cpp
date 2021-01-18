/**
 * @file main.cpp
 * @author ssm
 * @brief 如何读取视频文件；使用 PSNR 测试两个输入视频的相似性；
 * 使用 MSSIM 测试低于 PSNR 触发值的帧的相似性
 * @version 0.1
 * @date 2021-01-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <opencv2/core.hpp>    //cv::Mat  Scalar
#include <opencv2/imgproc.hpp> //高斯滤波
#include <opencv2/video.hpp>
#include <opencv2/highgui.hpp> //OpenCV window I/O
#include <iostream>            //数据流输入/输出
#include <string>              //字符串类
#include <iomanip>             //参数化输入/输出
#include <sstream>             //基于字符串的流

using namespace std;
using namespace cv;

//Function header
double getPSNR(const Mat &I1, const Mat &I2);
Scalar getMSSIM(const Mat &I1, const Mat &I2);

int main(int argc, char **argv)
{
    //命令行输入提示
    cout << "Usage:\n"
         << "./video_input_psnr_ssim referenceVideo comparewithVideo PSNR_Trigger_Value Wait_Between_Frames" << endl;

    //判断命令参数是否足够
    if (argc != 5)
    {
        cout << "Not enough parameters" << endl;
        return -1;
    }

    //定义视频文件索引
    const string referenceVideo = argv[1], comparewithVideo = argv[2];

    //定义 PSNR_Trigger_Value and delay
    int psnrTriggerValue, delay;
    stringstream conv; //用于字符串到数字的转换
    conv << argv[3] << endl
         << argv[4];                   //放入字符串
    conv >> psnrTriggerValue >> delay; //输出数字

    char c;
    int frameNum = -1; //帧计数

    //读取视频文件
    VideoCapture sourceReference(referenceVideo), sourceComparewith(comparewithVideo);
    if (!sourceReference.isOpened())
    {
        cout << "Could not open referenceVideo" << endl;
        return -1;
    }
    if (!sourceComparewith.isOpened())
    {
        cout << "Could not open comparewithVideo" << endl;
        return -1;
    }

    //通过 cv::VideoCapture::get 函数获得两个视频的尺寸
    Size refS = Size((int)sourceReference.get(CAP_PROP_FRAME_WIDTH),
                     (int)sourceReference.get(CAP_PROP_FRAME_HEIGHT));
    Size comS = Size((int)sourceComparewith.get(CAP_PROP_FRAME_WIDTH),
                     (int)sourceComparewith.get(CAP_PROP_FRAME_HEIGHT));
    if (refS != comS)
    {
        cout << "两个视频的帧的大小（宽高）不同！" << endl;
        return -1;
    }

    //windows
    const char *window_ref = "Reference";
    const char *window_com = "Comparewith";
    namedWindow(window_ref, WINDOW_AUTOSIZE);
    namedWindow(window_com, WINDOW_AUTOSIZE);
    moveWindow(window_ref, 400, 0);
    moveWindow(window_com, refS.width, 0);

    //输出参考帧的解析度：宽、高和当前是第几帧
    cout << "Reference frame resolution: Width = " << refS.width
         << "  Height = " << refS.height
         << "  of nr#: " << sourceReference.get(CAP_PROP_FRAME_COUNT) << endl;

    //输出 psnrTriggerValue 的值，以带小数点的形式表示浮点数，且精度为小数点后 3 位数字
    cout << "PSNR trigger value " << setiosflags(ios::fixed) << setprecision(3)
         << psnrTriggerValue << endl;

    Mat frameReference, frameComparewith;
    double psnrValue;
    Scalar mssimValue;

    //在窗口中“播放视频”——一帧一帧播放
    while (true)
    {
        sourceReference >> frameReference;
        sourceComparewith >> frameComparewith;
        if (frameReference.empty() || frameComparewith.empty())
        {
            cout << "播放结束" << endl;
            break;
        }

        ++frameNum;
        cout << "Frame: " << frameNum << "# ";

        //对比两帧的相似性，求出峰值信噪比并打印
        psnrValue = getPSNR(frameReference, frameComparewith);
        cout << setiosflags(ios::fixed) << setprecision(3) << psnrValue << "dB";

        //对于小于 PSNR 触发值的飞零峰值信噪比，应用结构性相似 MSSIM
        if (psnrValue < psnrTriggerValue && psnrValue)
        {
            mssimValue = getMSSIM(frameReference, frameComparewith);
            cout << "MSSIM: "
                 << " R " << setiosflags(ios::fixed) << setprecision(2) << mssimValue.val[2] * 100 << "%"
                 << " G " << setiosflags(ios::fixed) << setprecision(2) << mssimValue.val[1] * 100 << "%"
                 << " B " << setiosflags(ios::fixed) << setprecision(2) << mssimValue.val[0] * 100 << "%";
        }
        cout << endl;

        imshow(window_ref, frameReference);
        imshow(window_com, frameComparewith);
        c = (char)waitKey(delay);
        if (c == 27)
            break;
    }
    return 0;
}

/**
 * @brief 求峰值信噪比
 * 
 * @param I1 
 * @param I2 
 * @return double 
 */
double getPSNR(const Mat &I1, const Mat &I2)
{
    Mat s1;
    absdiff(I1, I2, s1);                         // |I1 - I2|
    s1.convertTo(s1, CV_32F);                    // CV_8U 无法进行平方计算
    s1 = s1.mul(s1);                             // (|I1 - I2|)^2
    Scalar s = sum(s1);                          //每个通道的元素求和
    double sse = s.val[0] + s.val[1] + s.val[2]; //通道求和
    if (sse <= 1e-10)                            //对于非常小的值，返回0
        return 0;
    else
    {
        double mse = sse / (double)(I1.channels() * I1.total());
        double psnr = 10.0 * log10((255 * 255) / mse);
        return psnr;
    }
}

/**
 * @brief 求结构性相似
 * 
 * @param I1 
 * @param I2 
 * @return Scalar 
 */
Scalar getMSSIM(const Mat &i1, const Mat &i2)
{
    const double C1 = 6.5025, C2 = 58.5225;
    Mat I1, I2;
    i1.convertTo(I1, CV_32F);
    i2.convertTo(I2, CV_32F);
    Mat I2_2 = I2.mul(I2);  //(I2)^2
    Mat I1_2 = I1.mul(I1);  //(I1)^2
    Mat I1_I2 = I1.mul(I2); //I1 * I2

    Mat mu1, mu2;
    GaussianBlur(I1, mu1, Size(11, 11), 1.5);
    GaussianBlur(I2, mu2, Size(11, 11), 1.5);

    Mat mu1_2 = mu1.mul(mu1);   //(mu1)^2
    Mat mu2_2 = mu2.mul(mu2);   //(mu2)^2
    Mat mu1_mu2 = mu1.mul(mu2); //mu1 * mu2

    Mat sigma1_2, sigma2_2, sigma12;
    GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
    sigma1_2 -= mu1_2;
    GaussianBlur(I2_2, sigma2_2, Size(11, 11), 1.5);
    sigma2_2 -= mu2_2;
    GaussianBlur(I1_I2, sigma12, Size(11, 11), 1.5);
    sigma12 -= mu1_mu2;

    Mat t1, t2, t3;
    t1 = 2 * mu1_mu2 + C1;
    t2 = 2 * sigma12 + C2;
    t3 = t1.mul(t2); // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))

    t1 = mu1_2 + mu2_2 + C1;
    t2 = sigma1_2 + sigma2_2 + C2;
    t1 = t1.mul(t2); // t1 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))

    Mat ssim_map;
    divide(t3, t1, ssim_map); // ssim_map =  t3./t1;

    Scalar mssim = mean(ssim_map); // mssim = average of ssim map
    return mssim;
}