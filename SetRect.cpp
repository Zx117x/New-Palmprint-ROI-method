#include<opencv2/opencv.hpp>
#include <iostream>
#include<vector>
#include "CmpPic.h"
using namespace std;
using namespace cv;

//��������Ķ�ֵͼ�񣬷���һ�����������ķ�վ���ͼ��
Mat SetRectImage(Mat image)
{
	vector<vector<Point> > contours;

	vector<Vec4i> hierarchy;

	Mat imagecopy = image.clone();

	findContours(image, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);//�ҳ���ֵͼ�������
	//�ҳ�contours����������size
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

	drawContours(image, contours, max, cv::Scalar::all(255));//��image�л�������

	Rect rect1 = boundingRect(contours[max]);//���������������

	///
	Mat dst;

	return dst = imagecopy(rect1);//��÷�վ��ζ�ֵͼ��

	//namedWindow("2");
	//imshow("2", dst);
	//waitKey(0);
}
