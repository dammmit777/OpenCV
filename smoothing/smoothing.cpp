#include <iostream>
#include <vector>
#include <cstring>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

using namespace std;
using namespace cv;

//![Global Variables]
int DELAY_CAPTION = 5000;
int DELAY_BLUR = 500;
int MAX_KERNEL_LENGTH = 31;

Mat src;
Mat dst;
char window_name[] = "Smoothing Demo";
//![Global Variables]

//![function headers]
int display_caption(const char *caption);
int display_dst(int delay);
//![function headers]

/**
 * @brief Main
 * 
 */
int main(int argc,char **argv)
{
    namedWindow(window_name, WINDOW_AUTOSIZE);

    //Load the source image
    src = imread(argv[1], IMREAD_COLOR);
    if (display_caption("Original Image") != 0)
        return 0;

    dst = src.clone();
    if (display_dst(DELAY_CAPTION) != 0)
        return 0;

    //![Normalized Filter]
    if (display_caption("Normalized Blur") != 0)
        return 0;

    for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
    {
        blur(src, dst, Size(i, i), Point(-1, -1));
        if (display_dst(DELAY_BLUR) != 0)
            return 0;
    }
    //![Normalized Filter]

    //![Gaussian Blur]
    if (display_caption("Gaussian Blur") != 0)
        return 0;

    for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
    {
        GaussianBlur(src, dst, Size(i, i), 0, 0);
        if (display_dst(DELAY_BLUR) != 0)
            return 0;
    }
    //![Gaussian Blur]

    //![Median Blur]
    if (display_caption("Median Blur") != 0)
        return 0;

    for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
    {
        medianBlur(src, dst, i);
        if (display_dst(DELAY_BLUR) != 0)
            return 0;
    }
    //![Median Blur]

    //![Bilateral Filter]
    if (display_caption("Bilateral Blur") != 0)
        return 0;

    for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
    {
        bilateralFilter(src, dst, i, i * 2, i / 2);
        if (display_dst(DELAY_BLUR) != 0)
            return 0;
    }
    //![Bilateral Filter]

    display_caption("End: Press a key");
    waitKey(0);
    return 0;
}

/**
 * @brief display_caption
 * 
 * @param caption 
 * @return int 
 */
int display_caption(const char *caption)
{
    dst = Mat::zeros(src.size(), src.type());
    putText(dst, caption,
            Point(src.cols / 4, src.rows / 2),
            FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255));

    imshow(window_name, dst);
    int c = waitKey(DELAY_CAPTION);
    if (c >= 0)
        return -1;
    return 0;
}
int display_dst(int delay)
{
    imshow(window_name, dst);
    int c = waitKey(delay);
    if (c >= 0)
        return -1;
    return 0;
}
