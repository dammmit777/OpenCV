#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    // char *ImageName = argv[1];

    // Mat A, C;
    // A = imread(ImageName, IMREAD_COLOR);
    // Mat B(A);
    // C = A;

    // Mat D(A, Rect(10, 10, 100, 100));
    // Mat E = A(Range::all(), Range(1, 3));

    // imshow("A", A);
    // imshow("B", B);
    // imshow("C", C);
    // imshow("D",D);
    // imshow("E",E);

    // waitKey(0);

    Mat M(2, 2, CV_8UC3, Scalar(0, 0, 255));
    cout << "M=" << endl
         << M << endl;

    int sz[3] = {2, 2, 2};
    Mat L(3, sz, CV_8UC1, Scalar::all(0));
    // cout << "L=" << endl
    //      << L << endl;

    M.create(4, 4, CV_8UC2);
    cout << "M=" << endl
         << ""
         << M << endl;
    return 0;
}