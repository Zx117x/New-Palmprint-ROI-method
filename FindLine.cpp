#include<opencv2/opencv.hpp>
#include <iostream>
#include<vector>
#include "CmpPic.h"
#include <sstream>
#include <iomanip>
#include <math.h>
using namespace std;
using namespace cv;
//找出拇指后侧边缘的拟合直线
int FindLine(Mat image,int x1,int y1,int x2,int y2)
{
	int image_row = image.rows;
	int image_col = image.cols;
	Mat imagecopy = image.clone();//备份image
	//*****轮廓分析*****//
	vector<vector<Point> > contours1;

	vector<Vec4i> hierarchy1;

	findContours(image, contours1, hierarchy1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);//找出二值图像的轮廓
	//找出contours中数量最多的size
	int maxsize1 = contours1[0].size();
	int max1 = 0;
	for (int i = 0; i < contours1.size(); i++)
	{
		if (contours1[i].size()>maxsize1)
		{
			maxsize1 = contours1[i].size();
			max1 = i;
		}
	}
	drawContours(image, contours1, max1, cv::Scalar::all(255));//在image中画出轮廓
	//找出最右端的点(二值图像)
	int y4 = 0;//这里可以设置一个debug提示
	int x4 = image_col - 2;
	for (int i = 0; i < image_row; i++)
	{
		if (imagecopy.at<uchar>(i, image_col-2) == 255)
		{
			y4 = i;
			break;
		}
	}
	//算出P1
	//**1.算出斜率值和截距
	double k = double(y1 - y2) / double(x1 - x2);
	int k1 = Round(k * 10, 0);//四舍五入，保留一位小数×10倍为整型
	double b = y1 - 0.1*k1*x1;
	int b1 = Round(b * 10, 0);//四舍五入，保留一位小数×10倍为整型
	//**2.在M1,M2所在直线上取y=0的点(x3,y3)
	double x3_temp = -b1 / k1;
	int x3 = int(x3_temp);//取整
	int y3 = 0;
	//**3.在点(x1,y1)和点(x3,y3)间找轮廓交点
	Point2f pt1 = Point2f(x1, y1);
	Point2f pt2 = Point2f(x3, y3);
	//遍历两个交点之间的线段，得出和轮廓的交点
	LineIterator it(image, pt1, pt2, 8);
	vector<int> x_temp;
	vector<int> y_temp;
	for (int i = 0; i < it.count; i++, ++it)
	{
		Point pt(it.pos());//获得线段上的点
		if (abs(pointPolygonTest(contours1[max1], pt, true)) < 1)
		{
			//circle(image, pt, 5, Scalar(0, 0, 255), -1);
			x_temp.push_back(pt.x);//储存符合条件的交点
			y_temp.push_back(pt.y);//储存符合条件的交点
		}
	}
	//当出现多个交点的情况时，选取y值小的点作为有效点
	int x0, y0;
	y0 = y_temp[0];
	x0 = x_temp[0];
	for (int i = 0; i < y_temp.size(); i++)
	{
		if (y_temp[i] < y0)
		{
			y0 = y_temp[i];
			x0 = x_temp[i];
		}
	}
	

	double val_1 = double(y4 - y0) / double(x4 - x0);
	int val = Round(val_1 * 10, 0);//四舍五入，保留一位小数×10倍为整型
	return val;

}
