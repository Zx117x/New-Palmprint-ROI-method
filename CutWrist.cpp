#include<opencv2/opencv.hpp>
#include <iostream>
#include<vector>
#include "CmpPic.h"
using namespace std;
using namespace cv;
Mat CutWrist(Mat image)
{
	int image_row = image.rows;
	int image_col = image.cols;
	Mat imagecopy = image.clone();//备份image
	//截取图像下半部分
	Mat  imageROI = image(Rect(image_col*3/4, 0, image_col/4, image_row / 2));

	//对二值图像画轮廓
	vector<vector<Point> > contours;

	vector<Vec4i> hierarchy;



	findContours(imageROI, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);//找出二值图像的轮廓
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


	drawContours(imageROI, contours, max, cv::Scalar::all(255));//在image中画出轮廓

	//对轮廓点求极大值
	vector<int> contours_min_x;
	vector<int> contours_min_y;


	for (int i = 10; i < maxsize - 10; i++)
	{
		if (contours[max][i].y >contours[max][i + 10].y && contours[max][i].y >contours[max][i - 10].y)
		{
			contours_min_x.push_back(contours[max][i].x);
			contours_min_y.push_back(contours[max][i].y);

		}
	}
	//找出第一个出现的极大值

	
	int dst_x_max = contours_min_x[0];
	int dst_y_max = contours_min_y[0];
	Mat imagecut = imagecopy(Rect(0, 0, image_col*3 / 4 + dst_x_max, image_row));

	return imagecut;
}