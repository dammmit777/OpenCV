/**
 * @file main.cpp
 * @author ssm
 * @brief code uses Sobel or Scharr functions for edge detection
 * @version 0.1
 * @date 2021-01-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    cv::CommandLineParser parser(argc, argv,
                                 "{@input    |lena.jpg|input image}"
                                 "{ksize     k|1|ksize (hit 'K' to increase its value at run time)}"
                                 "{scale     s|1|scale (hit 'S' to increase its value at run time)}"
                                 "{delta     d|0|delta (hit 'D' to increase its value at run time)}"
                                 "{help      h|false|show help message}");

    cout << "The sample uses Sobel or Scharr OpenCV functions for edge detection" << endl;
    parser.printMessage();
    cout << "\nPress 'ESC' to exit program.\nPress 'R' to reset values(ksize will be -1 equal to Scharr function )";

    Mat image, src, src_gray;
    Mat grad;
    const String windowname = "Sobel Demo - Edge Detector";
    int ksize = parser.get<int>("ksize");
    int scale = parser.get<int>("scale");
    int delta = parser.get<int>("delta");
    int ddepth = CV_16S;

    String imageName = parser.get<String>("@input");
    image = imread(imageName, IMREAD_COLOR);
    if (image.empty())
    {
        cout << "Error openning image " << imageName << endl;
        return EXIT_FAILURE;
    }

    while (true)
    {
        //remove noise by blurring with a Gaussian filter
        GaussianBlur(image, src, Size(3, 3), 0, 0, BORDER_DEFAULT);

        //convert to gray
        cvtColor(src, src_gray, COLOR_BGR2GRAY);

        //Sobel
        Mat grad_x, grad_y;
        Mat abs_grad_x, abs_grad_y;

        //Gradient X
        Sobel(src_gray, grad_x, ddepth,
              1, 0, ksize, scale, delta, BORDER_DEFAULT);

        //Gradient Y
        Sobel(src_gray, grad_y, ddepth,
              0, 1, ksize, scale, delta, BORDER_DEFAULT);

        //convert back to CV_8U
        convertScaleAbs(grad_x, abs_grad_x);
        convertScaleAbs(grad_y, abs_grad_y);

        //total gradient
        addWeighted(abs_grad_x, 0.5,
                    abs_grad_y, 0.5,
                    0, grad);

        imshow(windowname, grad);

        char key = (char)waitKey(0);

        if (key == 27)
            return EXIT_SUCCESS;

        if (key == 'k' || key == 'K')
            ksize = ksize < 30 ? ksize + 2 : -1;

        if (key == 's' || key == 'S')
            scale++;

        if (key == 'd' || key == 'D')
            delta++;

        if (key == 'r' || key == 'R')
        {
            scale = 1;
            ksize = -1;
            delta = 0;
        }
    }

    return 0;
}