#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

Mat img, new_img;
int alpha = 0; //simple contrast control
int beta = 0;  //simple brightness control

void on_track(int, void *)
{
    // for (int i = 0; i < img.rows; i++)
    // {
    //     for (int j = 0; j < img.cols; j++)
    //     {
    //         for (int c = 0; c < 3; c++)
    //         {
    //             new_img.at<Vec3b>(i, j)[c] = saturate_cast<uchar>((double)alpha * (img.at<Vec3b>(i, j)[c]) + (double)beta);
    //         }
    //     }
    // }
    img.convertTo(new_img, -1, (double)alpha, (double)beta);
    imshow("New Image", new_img);
}

int main(int argc, char **argv)
{
    img = imread(argv[1]);
    new_img = Mat::zeros(img.size(), img.type());

    std::cout << "Basic Linear Transforms" << std::endl;
    std::cout << "-----------------------" << std::endl;
    // std::cout << "Enter the alpha value [1.0-3.0]:";
    // std::cin >> alpha;
    // std::cout << "Enter the beta value [0-100]:";
    // std::cin >> beta;

    namedWindow("Original Image", WINDOW_AUTOSIZE);
    namedWindow("New Image", WINDOW_AUTOSIZE);
    createTrackbar("alpha", "Original Image", &alpha, 3, on_track);
    createTrackbar("beta", "Original Image", &beta, 100, on_track);
    on_track(0, 0);

    imshow("Original Image", img);
    

    waitKey(0);

    return 0;
}