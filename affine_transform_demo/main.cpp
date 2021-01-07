/**
 * @file main.cpp
 * @author ssm
 * @brief Geometric Transform by using Affine Transform
 * @version 0.1
 * @date 2021-01-07
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

const char *src_window = "src image";
const char *warp_window = "warp";
const char *warp_rotate_window = "warp+rotate";

int main(int argc, char **argv)
{
    Mat src = imread(argv[1], IMREAD_COLOR);
    if (src.empty())
    {
        cerr << "Error openning image" << endl;
        return -1;
    }
    imshow(src_window, src);

    //![set 3 points to calculate the Affine Transform]
    Point2f srcTri[3];
    srcTri[0] = Point2f(0.f, 0.f);
    srcTri[1] = Point2f(src.cols - 1.f, 0.f);
    srcTri[2] = Point2f(0.f, src.rows - 1.f);

    Point2f dstTri[3];
    dstTri[0] = Point2f(0.f, src.rows * 0.25f);
    dstTri[1] = Point2f(src.cols * 0.85f, src.rows * 0.25f);
    dstTri[2] = Point2f(src.cols * 0.15f, src.rows * 0.7f);
    //![set 3 points to calculate the Affine Transform]

    //![get the affine transform]
    Mat M = getAffineTransform(srcTri, dstTri);
    //![get the affine transform]

    //![apply the M just found to the src image]
    Mat warp_dst = Mat::zeros(src.size(), src.type());
    warpAffine(src, warp_dst, M, warp_dst.size());
    imshow(warp_window, warp_dst);
    //![apply the M just found to the src image]

    //![rotating the image after warp]
    //compute a rotation matrix with respect to the center of the image
    Point center = Point(warp_dst.cols / 2, warp_dst.rows / 2);
    double angle = -50;
    double scale = 0.6;

    //get the rotation matrix
    Mat rotation_mat = getRotationMatrix2D(center, angle, scale);

    //rotate the warped image
    Mat warp_rotate_dst;
    warpAffine(warp_dst, warp_rotate_dst, rotation_mat, warp_dst.size());
    imshow(warp_rotate_window, warp_rotate_dst);
    //![rotating the image after warp]

    waitKey(0);
    return 0;
}