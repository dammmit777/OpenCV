/**
 * @file main.cpp
 * @author ssm
 * @brief shows the function of copyMakeBorder
 * @version 0.1
 * @date 2020-12-31
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//Global Variable
Mat src, dst;
int TOP, BOTTOM;
int LEFT, RIGHT;
int borderType = BORDER_CONSTANT;
const char *windowname = "copyMakeBorder Demo";
RNG rng(12345);

int main(int argc, char **argv)
{
    //Load image
    src = imread(argv[1], IMREAD_COLOR);
    if (src.empty())
    {
        cerr << "Error openning image" << endl;
        return EXIT_FAILURE;
    }

    cout << "\n \t copyMakeBorder Demo:" << endl;
    cout << "--------------------------" << endl;
    cout << "Press 'c' to set the border to a random constant value" << endl;
    cout << "Press 'r' to set the border to be replicated" << endl;
    cout << "Press 'ESC' to exit the program" << endl;

    namedWindow(windowname, WINDOW_AUTOSIZE);

    //![init_arguments]
    TOP = (int)(0.05 * src.rows);
    BOTTOM = TOP;
    LEFT = (int)(0.05 * src.cols);
    RIGHT = LEFT;
    //![init_arguments]

    while (true)
    {
        //Update value
        Scalar value(rng.uniform(0, 255),
                     rng.uniform(0, 255),
                     rng.uniform(0, 255));

        copyMakeBorder(src, dst,
                       TOP, BOTTOM, LEFT, RIGHT,
                       borderType,
                       value);

        imshow(windowname, dst);

        char c = (char)waitKey(1000);
        if (c == 27)
            break;
        else if (c == 'c')
            borderType = BORDER_CONSTANT;
        else if (c == 'r')
            borderType = BORDER_REPLICATE;
    }

    return EXIT_SUCCESS;
}