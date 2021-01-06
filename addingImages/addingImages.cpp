/**
 * @file addingImages.cpp
 * @author ssm
 * @brief  Simple linear blender (dst=alpha*src1+beta*src2)
 * @version 0.1
 * @date 2020-12-17
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

/**
 * @function main
 * @brief Main function
 */
int main(int argc, char **argv)
{
    double alpha = 0.5;
    double beta;
    double input;

    Mat src1, src2, dst;

    //Ask the user enter alpha
    std::cout << "Simple linear Blender" << std::endl;
    std::cout << "---------------------" << std::endl;
    std::cout << "Enter alpha [0.0-1.0]:";
    std::cin >> input;

    //use the alpha provided by the user if it is between 0 and 1
    if (input >= 0 && input <= 1)
        alpha = input;

    //![load]
    //Read images(both have to be of the same size and type)
    src1 = imread(argv[1], IMREAD_UNCHANGED);
    src2 = imread(argv[2], IMREAD_UNCHANGED);
    //![load]

    if (src1.empty())
    {
        std::cout << "Error loading src1" << std::endl;
        return EXIT_FAILURE;
    }
    if (src2.empty())
    {
        std::cout << "Error loading src2" << std::endl;
        return EXIT_FAILURE;
    }

    if ((src1.rows != src2.rows) || (src1.cols != src2.cols))
    {
        if (src1.rows > src2.rows)
            resize(src1, src1, src2.size(), 0, 0, INTER_LINEAR);
        else if (src1.rows < src2.rows)
            resize(src2, src2, src1.size(), 0, 0, INTER_LINEAR);
    }

    //![blender images]
    beta = 1.0 - alpha;
    addWeighted(src1, alpha, src2, beta, 0.0, dst);
    //![blender images]

    //![display]
    imshow("Linear blend", dst);
    waitKey(0);
    //![display]

    return 0;
}