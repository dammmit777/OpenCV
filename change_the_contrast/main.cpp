#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

double alpha; //simple contrast control
double beta;  //simple brightness control

int main(int argc, char **argv)
{
    Mat img = imread(argv[1]);
    Mat new_img = Mat::zeros(img.size(), img.type());

    std::cout << "Basic Linear Transforms" << std::endl;
    std::cout << "-----------------------" << std::endl;
    std::cout << "Enter the alpha value [1.0-3.0]:";
    std::cin >> alpha;
    std::cout << "Enter the beta value [0-100]:";
    std::cin >> beta;

    // for (int y = 0; y < img.rows; y++)
    // {
    //     for (int x = 0; x < img.cols; x++)
    //     {
    //         for (int c = 0; c < 3; c++)
    //         {
    //             new_img.at<Vec3b>(y, x)[c] =
    //                 saturate_cast<uchar>(alpha * (img.at<Vec3b>(y, x)[c]) + beta);
    //         }
    //     }
    // }
    img.convertTo(new_img, -1, alpha, beta);
    namedWindow("Original Image", WINDOW_AUTOSIZE);
    namedWindow("New Image", WINDOW_AUTOSIZE);

    imshow("Origenal Image", img);
    imshow("New Image", new_img);

    waitKey(0);

    return 0;
}