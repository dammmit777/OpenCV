#include<opencv2/core.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<iostream>

using namespace std;
using namespace cv;

Mat sample, Mark, img, tmp;
Mat Match;
Mat sample_Affine;
Mat frameDifference, absFrameDifference;
Mat segmentation;

static Point sample_pt = Point(-1, -1);
static Point match_pt = Point(-1, -1);
int thresh = 205, maxval = 255;

//function header
void onMouse(int event, int x, int y, int flags, void* ustc);
void matchingMethod(int, void*);
void Affine();
void findDifference(Mat& image1, Mat& image2);

int main(int argc, char** argv)
{
	sample = imread(argv[1], IMREAD_COLOR);
	sample.copyTo(img);
	sample.copyTo(tmp);
	namedWindow("sample", WINDOW_FREERATIO);
	setMouseCallback("sample", onMouse, 0);
	imshow("sample",sample);
	waitKey(0);

	Match = imread(argv[2], IMREAD_COLOR);
	namedWindow("templateMatching", WINDOW_FREERATIO);
	matchingMethod(0, 0);
	waitKey(0);

	Affine();
	waitKey(0);

	namedWindow("findDifference", WINDOW_FREERATIO);
	findDifference(sample_Affine, Match);
	waitKey(0);

	return 0;
}


void onMouse(int event, int x, int y, int flags, void* ustc)
{
	static Point pre_pt = Point(-1, -1);//初始坐标
	static Point cur_pt = Point(-1, -1);//当前坐标
	char temp[16];

	//左键按下，读取初始坐标，并在图像上该点处画圆
	if (event == EVENT_LBUTTONDOWN)
	{
		sample.copyTo(img);//将原始图像复制到img中
		sprintf(temp, "(%d,%d)", x, y);
		pre_pt = Point(x, y);
		sample_pt = Point(x, y);//鼠标标定点
		printf("(%d,%d)", sample_pt.x, sample_pt.y);

		//在窗口显示坐标
		putText(img, temp, pre_pt, FONT_HERSHEY_SIMPLEX,
			3, Scalar(0, 0, 255), 3, 8);

		//画圆
		circle(img, pre_pt, 2, Scalar(0, 0, 255),
			CV_FILLED, CV_AA, 0);

		imshow("sample", img);
	}

	//左键未被按下时鼠标移动的处理
	else if (event == EVENT_MOUSEMOVE && !(flags & EVENT_FLAG_LBUTTON))
	{
		img.copyTo(tmp);
		sprintf(temp, "(%d,%d)", x, y);
		cur_pt = Point(x, y);

		//实时显示鼠标移动的坐标
		putText(tmp, temp, cur_pt, FONT_HERSHEY_SIMPLEX,
			3, Scalar(0, 0, 255), 3, 8);

		imshow("sample", tmp);
	}

	//左键按下时，鼠标移动，在图像上画矩形
	else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON))
	{
		img.copyTo(tmp);
		sprintf(temp, "(%d,%d)", x, y);
		cur_pt = Point(x, y);

		//实时显示鼠标拖动时形成的矩形
		/*putText(tmp, temp, cur_pt, FONT_HERSHEY_SIMPLEX,
			3, Scalar(0, 0, 255), 3, 8);*/

		rectangle(tmp, pre_pt, cur_pt, Scalar(0, 0, 255), 3, 8, 0);

		imshow("sample", tmp);
	}

	//左键松开，在图像上画矩形
	else if (event == EVENT_LBUTTONUP)
	{
		sample.copyTo(img);
		sprintf(temp, "(%d,%d)", x, y);
		cur_pt = Point(x, y);

		//显示坐标
		putText(img, temp, cur_pt, FONT_HERSHEY_SIMPLEX,
			3, Scalar(0, 0, 255), 3, 8);

		circle(img, pre_pt, 2, Scalar(0, 0, 255), 1, 8, 0);
		rectangle(img, pre_pt, cur_pt, Scalar(0, 0, 255), 3, 8, 0);

		imshow("sample", img);

		//截取矩形包围的图像，保存到Mark中
		int width = abs(pre_pt.x - cur_pt.x);
		int height = abs(pre_pt.y - cur_pt.y);
		if (width == 0 || height == 0)
		{
			return;
		}

		Mark = sample(
			Rect(
				min(cur_pt.x, pre_pt.x),
				min(cur_pt.y, pre_pt.y),
				width,
				height)
		);

		imwrite("Mark.jpg", Mark);
		namedWindow("Mark", WINDOW_FREERATIO);
		imshow("Mark", Mark);
	}
}

void matchingMethod(int, void*)
{
	Mat Match_display;
	Mat result;

	Match.copyTo(Match_display);

	//创建result矩阵存放匹配结果
	int result_cols = Match.cols - Mark.cols + 1;
	int result_rows = Match.rows - Mark.rows + 1;
	result.create(result_rows, result_cols, CV_32FC1);

	//执行模板匹配
	matchTemplate(Match, Mark, result, TM_SQDIFF);

	//归一化
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	//在 result 矩阵中，对最小值和最大值所处的坐标进行定位
	double minVal, maxVal;
	Point minLoc, maxLoc, matchLoc;
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	matchLoc = minLoc;
	match_pt = matchLoc;//模板匹配点
	printf("(%d,%d)", match_pt.x, match_pt.y);

	rectangle(Match_display,
		matchLoc, Point(matchLoc.x + Mark.cols, matchLoc.y + Mark.rows),
		Scalar(0, 0, 255), 3, 8, 0);
	rectangle(result,
		matchLoc, Point(matchLoc.x + Mark.cols, matchLoc.y + Mark.rows),
		Scalar(0, 0, 255), 3, 8, 0);

	imshow("templateMatching", Match_display);

}

void Affine()
{
	//定义平移向量
	int dx = abs(sample_pt.x - match_pt.x);
	int dy = abs(sample_pt.y - match_pt.y);

	//定义平移矩阵
	Mat M = Mat::zeros(2, 3, CV_32FC1);
	M.at<float>(0, 0) = 1;
	M.at<float>(1, 1) = 1;

	if (match_pt.x < sample_pt.x)
	{
		M.at<float>(0, 2) = -dx;

		if (match_pt.y < sample_pt.y)
			M.at<float>(1, 2) = -dy;
		else
			M.at<float>(1, 2) = dy;

	}
	else
	{
		M.at<float>(0, 2) = dx;

		if (match_pt.y < sample_pt.y)
			M.at<float>(1, 2) = -dy;
		else
			M.at<float>(1, 2) = dy;
	}

	warpAffine(sample, sample_Affine, M, sample.size());
	namedWindow("Affine", WINDOW_FREERATIO);
	imshow("Affine", sample_Affine);
	imwrite("Affine.jpg", sample_Affine);
}

void findDifference(Mat& image1, Mat& image2)
{

	if ((image1.rows != image2.rows) || (image1.cols != image2.cols))
	{
		if (image1.rows > image2.rows)
		{
			resize(image1, image1, image2.size(), 0, 0, INTER_LINEAR);
		}
		else if (image1.rows < image2.rows)
		{
			resize(image2, image2, image1.size(), 0, 0, INTER_LINEAR);
		}
	}

	Mat image1_gray, image2_gray;
	if (image1.channels() != 1)
		cvtColor(image1, image1_gray, COLOR_BGR2GRAY);
	if (image2.channels() != 1)
		cvtColor(image2, image2_gray, COLOR_BGR2GRAY);

	Mat previousGrayFrame = image2_gray.clone();

	//图一减图二
	subtract(image1_gray, image2_gray, frameDifference, Mat(), CV_32FC1);

	//取绝对值
	absFrameDifference = abs(frameDifference);

	//改变位深
	absFrameDifference.convertTo(absFrameDifference, CV_8UC1, 1, 0);
	//namedWindow("absFrameDifference", WINDOW_FREERATIO);
	//imshow("absFrameDifference", absFrameDifference);

	//阈值处理
	threshold(absFrameDifference, segmentation, thresh, maxval, THRESH_BINARY);

	//中值滤波
	medianBlur(segmentation, segmentation, 3);

	//形态学处理
	Mat morphologyKernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	morphologyEx(segmentation, segmentation, MORPH_CLOSE, morphologyKernel, Point(-1, -1), 2, BORDER_REPLICATE);

	//显示二值化
	//namedWindow("segmentation", WINDOW_FREERATIO);
	//imshow("segmentation", segmentation);

	//找边界
	vector<vector<Point> > contours;
	vector<Vec4i>hierarchy;
	findContours(segmentation, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<vector<Point> >contours_poly(contours.size());
	vector<Rect> boundRect;
	boundRect.clear();

	for (int index = 0; index < contours.size(); index++)
	{
		approxPolyDP(Mat(contours[index]), contours_poly[index], 3, true);
		Rect rect = boundingRect(Mat(contours_poly[index]));
		rectangle(image2, rect, Scalar(0, 0, 255), 3, 8, 0);
	}
	imshow("findDifference", image2);
	imwrite("findDifference.jpg", image2);
}