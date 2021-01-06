#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <string>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    string ImageName("/home/ssm/OpenCV_C++/IO_image/1.jpeg");
    if (argc > 1)
    {
        ImageName = argv[1];
    }

    Mat img;
    img = imread(ImageName.c_str(), IMREAD_COLOR);
    if (img.empty())
    {
        cout << "could not open or find the image" << endl;
        return -1;
    }

    namedWindow("Display Window", WINDOW_AUTOSIZE);
    imshow("Display Window", img);
    waitKey(0);
    return 0;
}