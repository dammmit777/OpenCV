/**
 * @file main.cpp
 * @author ssm
 * @brief Demo for Hough Transform
 * @version 0.1
 * @date 2021-01-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat src, src_gray, edges;
Mat standard_hough, probabilistic_hough;
int min_threshold = 50;
int max_trackbar = 300;

const char *standard_name = "Standard Hough Lines Demo";
const char *probabilistic_name = "Probabilistic Hough Lines Demo";

int s_trackbar = max_trackbar;
int p_trackbar = max_trackbar;

void help();
void Standard_Hough(int, void *);
void Probabilistic_Hough(int, void *);

int main(int argc, char **argv)
{
    //load image
    src = imread(argv[1], IMREAD_COLOR);
    if (src.empty())
    {
        help();
        return -1;
    }

    //convert to gray
    cvtColor(src, src_gray, COLOR_BGR2GRAY);

    //apply canny edge detector
    Canny(src_gray, edges, 50, 200, 3);

    //create trackbar for thresholds
    char thresh_label[50];
    sprintf(thresh_label, "Threshold: %d + input", min_threshold);

    namedWindow(standard_name, WINDOW_AUTOSIZE);
    createTrackbar(thresh_label, standard_name,
                   &s_trackbar, max_trackbar, Standard_Hough);

    namedWindow(probabilistic_name, WINDOW_AUTOSIZE);
    createTrackbar(thresh_label, probabilistic_name,
                   &p_trackbar, max_trackbar, Probabilistic_Hough);

    //Init
    Standard_Hough(0, 0);
    Probabilistic_Hough(0, 0);
    waitKey(0);

    return 0;
}

/**
 * @brief Indication of how to run this program and why is it for 
 * 
 */
void help()
{
    cout << "\t Hough Transform to detect lines" << endl;
    cout << "\t--------------------------------" << endl;
    cout << "Usage: ./houghlines_demo <image_name>" << endl;
}

/**
 * @brief Standard_Hough
 * 
 */
void Standard_Hough(int, void *)
{
    vector<Vec2f> s_lines;
    cvtColor(edges, standard_hough, COLOR_GRAY2BGR);

    HoughLines(edges, s_lines, 1, CV_PI / 180,
               min_threshold + s_trackbar, 0, 0);

    for (size_t i = 0; i < s_lines.size(); i++)
    {
        float r = s_lines[i][0], t = s_lines[i][1];
        double cos_t = cos(t), sin_t = sin(t);
        double x0 = r * cos_t, y0 = r * sin_t;
        double alpha = 1000;

        //cvRound:Rounds floating-point number to the nearest integer
        Point pt1(cvRound(x0 + alpha * (-sin_t)), cvRound(y0 + alpha * cos_t));
        Point pt2(cvRound(x0 - alpha * (-sin_t)), cvRound(y0 - alpha * cos_t));
        line(standard_hough, pt1, pt2, Scalar(255, 0, 0), 2, LINE_AA);
    }
    imshow(standard_name, standard_hough);
}

/**
 * @brief Probabilistic Hough
 * 
 */
void Probabilistic_Hough(int, void *)
{
    vector<Vec4i> p_lines;
    cvtColor(edges, probabilistic_hough, COLOR_GRAY2BGR);

    HoughLinesP(edges, p_lines, 1, CV_PI / 180,
                min_threshold + p_trackbar, 30, 10);

    for (size_t i = 0; i < p_lines.size(); i++)
    {
        Vec4i l = p_lines[i];
        line(probabilistic_hough,
             Point(l[0], l[1]), Point(l[2], l[3]),
             Scalar(255, 0, 0), 3, LINE_AA);
    }
    imshow(probabilistic_name, probabilistic_hough);
}
