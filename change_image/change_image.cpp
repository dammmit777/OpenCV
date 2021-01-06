#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    char *ImageName = argv[1];

    Mat img = imread(ImageName, IMREAD_COLOR);
    if (argc != 2 || !img.data)
    {
        cout << "not image data" << endl;
        return -1;
    }

    Mat gray_img;
    cvtColor(img, gray_img, COLOR_BGR2GRAY);
    imwrite("/home/ssm/OpenCV_C++/change_image/gray_image.jpg", gray_img);

    namedWindow(ImageName, WINDOW_AUTOSIZE);
    namedWindow("gray_image", WINDOW_AUTOSIZE);

    imshow(ImageName, img);
    imshow("gray_image", gray_img);
    waitKey(0);
    return 0;
}