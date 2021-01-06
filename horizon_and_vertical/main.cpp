/**
 * @file horizon_and_vertical
 * @author ssm
 * @brief use morphology transformations for extracting horizontal and vertical lines
 * @version 0.1
 * @date 2020-12-28
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    //Load image
    Mat src = imread(argv[1]);

    //Check if image is loaded fine
    if (src.empty())
    {
        cerr << "Problem loading image!" << endl;
        return -1;
    }

    //Show source image
    imshow("src", src);

    //change the src to grayscale
    Mat gray;
    if (src.channels() == 3)
        cvtColor(src, gray, COLOR_BGRA2GRAY);
    else
        gray = src;

    //Show gray image
    //imshow("gray", gray);

    //Change the gray to binaryscale
    //Apply adaptiveTreshold at the bitwise_not of gray
    Mat bw;
    adaptiveThreshold(~gray, bw, 255,
                      CV_ADAPTIVE_THRESH_MEAN_C,
                      THRESH_BINARY, 15, -2);

    //Show binary image
    //imshow("binary", bw);

    //Create the image that will use to extract the horizontal and vertical lines
    Mat horizontal = bw.clone();
    Mat vertical = bw.clone();

    //Specify size on horizontal axis
    int horizontalsize = horizontal.cols / 30;

    //Create structure element for extracting horizontal lines through morphology operatrons
    Mat horizontalStructure =
        getStructuringElement(MORPH_RECT,
                              Size(horizontalsize, 1));

    //Apply morphology operations
    morphologyEx(horizontal, horizontal,
                 MORPH_OPEN,
                 horizontalStructure,
                 Point(-1, -1));

    //Show extracted horizontal lines
    imshow("horizontal", horizontal);

    //Specify size on vertical axis
    int verticalsize = vertical.rows / 30;

    //Create structure element for extracting vertical lines through morphology operations
    Mat verticalStructure =
        getStructuringElement(MORPH_RECT,
                              Size(1, verticalsize));

    //Apply morphology operations
    morphologyEx(vertical, vertical,
                 MORPH_OPEN,
                 verticalStructure,
                 Point(-1, -1));

    //Show extracted vertical lines
    imshow("vertical", vertical);

    //Smooth
    //Inverse vertical image
    bitwise_not(vertical, vertical);
    imshow("vertical_inv", vertical);

    /**
     * @brief Extract edges and smooth image according to the logic
     * 1. extract edges
     * 2. dilate(edges)
     * 3. src.copyTo(smooth)
     * 4. blur smooth img
     * 5. smooth.copyTo(src,edges)
     */

    //1.
    Mat edges;
    adaptiveThreshold(vertical,
                      edges, 255,
                      CV_ADAPTIVE_THRESH_MEAN_C,
                      THRESH_BINARY, 3, -2);
    imshow("edges", edges);

    //2.
    Mat kernel = Mat::ones(2, 2, CV_8UC1);
    dilate(edges, edges, kernel);
    imshow("dilate", edges);

    //3.
    Mat smooth;
    vertical.copyTo(smooth);

    //4.
    blur(smooth, smooth, Size(2, 2));

    //5.
    smooth.copyTo(vertical, edges);

    //Show final result
    imshow("smooth", vertical);

    waitKey(0);
    return 0;
}