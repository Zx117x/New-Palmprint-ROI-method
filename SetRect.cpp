#include<opencv2/opencv.hpp>
#include <iostream>
#include<vector>
#include "CmpPic.h"
using namespace std;
using namespace cv;

//根据输入的二值图像，返回一个包含轮廓的封闭矩形图像
Mat SetRectImage(Mat image)
{
	vector<vector<Point> > contours;

	vector<Vec4i> hierarchy;

	Mat imagecopy = image.clone();

	findContours(image, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);//找出二值图像的轮廓
	//找出contours中数量最多的size
	int maxsize = contours[0].size();
	int max = 0;
	for (int i = 0; i < contours.size(); i++)
	{
		if (contours[i].size()>maxsize)
		{
			maxsize = contours[i].size();
			max = i;
		}
	}

	drawContours(image, contours, max, cv::Scalar::all(255));//在image中画出轮廓

	Rect rect1 = boundingRect(contours[max]);//建立轮廓外包矩形

	///
	Mat dst;

	return dst = imagecopy(rect1);//获得封闭矩形二值图像

	//namedWindow("2");
	//imshow("2", dst);
	//waitKey(0);
}
