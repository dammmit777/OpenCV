#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    Mat in_img, out_img;
    in_img = imread(argv[1], IMREAD_UNCHANGED);
    if (in_img.empty())
    {
        cout << "Error! Input image cannot be read..." << endl;
        return 1;
    }

    //创建窗口
    namedWindow(argv[1], WINDOW_AUTOSIZE);
    namedWindow(argv[2], WINDOW_AUTOSIZE);

    imshow(argv[1], in_img);
    cvtColor(in_img, out_img, COLOR_BGR2GRAY);
    imshow(argv[2], out_img);
    waitKey();
    imwrite(argv[2], in_img);
    return 0;
}
