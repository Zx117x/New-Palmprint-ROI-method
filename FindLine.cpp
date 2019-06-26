#include<opencv2/opencv.hpp>
#include <iostream>
#include<vector>
#include "CmpPic.h"
#include <sstream>
#include <iomanip>
#include <math.h>
using namespace std;
using namespace cv;
//�ҳ�Ĵָ����Ե�����ֱ��
int FindLine(Mat image,int x1,int y1,int x2,int y2)
{
	int image_row = image.rows;
	int image_col = image.cols;
	Mat imagecopy = image.clone();//����image
	//*****��������*****//
	vector<vector<Point> > contours1;

	vector<Vec4i> hierarchy1;

	findContours(image, contours1, hierarchy1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);//�ҳ���ֵͼ�������
	//�ҳ�contours����������size
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
	drawContours(image, contours1, max1, cv::Scalar::all(255));//��image�л�������
	//�ҳ����Ҷ˵ĵ�(��ֵͼ��)
	int y4 = 0;//�����������һ��debug��ʾ
	int x4 = image_col - 2;
	for (int i = 0; i < image_row; i++)
	{
		if (imagecopy.at<uchar>(i, image_col-2) == 255)
		{
			y4 = i;
			break;
		}
	}
	//���P1
	//**1.���б��ֵ�ͽؾ�
	double k = double(y1 - y2) / double(x1 - x2);
	int k1 = Round(k * 10, 0);//�������룬����һλС����10��Ϊ����
	double b = y1 - 0.1*k1*x1;
	int b1 = Round(b * 10, 0);//�������룬����һλС����10��Ϊ����
	//**2.��M1,M2����ֱ����ȡy=0�ĵ�(x3,y3)
	double x3_temp = -b1 / k1;
	int x3 = int(x3_temp);//ȡ��
	int y3 = 0;
	//**3.�ڵ�(x1,y1)�͵�(x3,y3)������������
	Point2f pt1 = Point2f(x1, y1);
	Point2f pt2 = Point2f(x3, y3);
	//������������֮����߶Σ��ó��������Ľ���
	LineIterator it(image, pt1, pt2, 8);
	vector<int> x_temp;
	vector<int> y_temp;
	for (int i = 0; i < it.count; i++, ++it)
	{
		Point pt(it.pos());//����߶��ϵĵ�
		if (abs(pointPolygonTest(contours1[max1], pt, true)) < 1)
		{
			//circle(image, pt, 5, Scalar(0, 0, 255), -1);
			x_temp.push_back(pt.x);//������������Ľ���
			y_temp.push_back(pt.y);//������������Ľ���
		}
	}
	//�����ֶ����������ʱ��ѡȡyֵС�ĵ���Ϊ��Ч��
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
	int val = Round(val_1 * 10, 0);//�������룬����һλС����10��Ϊ����
	return val;

}
