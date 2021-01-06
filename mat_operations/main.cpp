#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    std::string filename = "1.jpeg";

    //![input/output]
    {
        //![load an image from a file]
        Mat img = imread(filename);
        //![load an image from a file]
        CV_UNUSED(img);
    }

    {
        //![load an image from a file in grayscale]
        Mat img = imread(filename, IMREAD_GRAYSCALE);
        //![load an image from a file in grayscale]
        CV_UNUSED(img);
    }

    {
        Mat img(4, 4, CV_8U);
        //![save image]
        imwrite(filename, img);
        //![save image]
    }

    //Accessing pixel intensity values
    {
        Mat img(4, 4, CV_8U);
        int y = 0, x = 0;
        {
            //![pixel access 1]
            Scalar intensity = img.at<uchar>(y, x);
            //![pixel access 1]
            CV_UNUSED(intensity);
        }

        {
            //![pixel access 2]
            Scalar intensity = img.at<uchar>(Point(x, y));
            //![pixel access 2]
            CV_UNUSED(intensity);
        }

        {
            //![pixel access 3]
            Vec3b intensity = img.at<Vec3b>(y, x);
            uchar blue = intensity.val[0];
            uchar green = intensity.val[1];
            uchar red = intensity.val[2];
            //![pixel access 3]
            CV_UNUSED(blue);
            CV_UNUSED(green);
            CV_UNUSED(red);
        }

        {
            //![pixel access 5]
            img.at<uchar>(y, x) = 128;
            //![pixel access 5]
        }

        {
            int i = 0;

            //![Mat from points vector]
            vector<Point2f> points;
            //...fill the array
            Mat pointsMat = Mat(points);
            //![Mat from points vector]

            //![points access]
            Point2f point = pointsMat.at<Point2f>(i, 0);
            //![points access]
            CV_UNUSED(point);
        }
    }

    //memory management and reference counting
    {
        //![reference counting 1]
        std::vector<Point3f> points;
        //...fill the array
        Mat pointsMat = Mat(points).reshape(1);
        //![reference counting 1]
        CV_UNUSED(pointsMat);
    }

    {
        //![reference counting 2]
        Mat img = imread("1.jpeg");
        Mat img1 = img.clone();
        //![reference counting 2]
        CV_UNUSED(img1);
    }

    {
        //![reference counting 3]
        Mat img = imread("1.jpeg");
        Mat sobelx;
        Sobel(img, sobelx, CV_32F, 1, 0);
        //![reference counting 3]
    }

    //primitive operations
    {
        Mat img;
        {
            //![set image to black]
            img = Scalar(0);
            //![set image to black]
        }

        {
            //![select ROI]
            Rect r(10, 10, 100, 100);
            Mat smallImg = img(r);
            //![select ROI]
            CV_UNUSED(smallImg);
        }
    }

    {
        //![BGR to gray]
        Mat img = imread("1.jpeg");
        Mat gray;
        cvtColor(img, gray, COLOR_BGR2GRAY);
        //![BGR to gray]
    }

    {
        Mat dst, src;
        //![convert to CV_32F]
        src.convertTo(dst, CV_32F);
        //![convert to CV_32F]
    }

    //visualizing image
    {
        //![imshow 1]
        Mat img = imread("1.jpeg");
        namedWindow("image", WINDOW_AUTOSIZE);
        imshow("image", img);
        waitKey();
        //![imshow 1]
    }

    {
        //![imshow 2]
        Mat img = imread("1.jpeg");
        Mat gray;
        cvtColor(img, gray, COLOR_BGR2GRAY);
        Mat sobelx;
        Sobel(gray, sobelx, CV_32F, 1, 0);
        double minVal, maxVal;
        minMaxLoc(sobelx, &minVal, &maxVal);
        Mat draw;
        sobelx.convertTo(draw, CV_8U, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));
        namedWindow("image", WINDOW_AUTOSIZE);
        imshow("image", draw);
        waitKey();
        //![imshow 2]
    }

    return 0;
}