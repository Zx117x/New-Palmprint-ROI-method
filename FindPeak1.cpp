#include<opencv2/opencv.hpp>
#include <iostream>
#include<vector>
#include "CmpPic.h"
using namespace std;
using namespace cv;
//�ҳ��ĸ���ֵ�㣨��Ϊ�ĸ�ָ�⣩
int *FindPeak1(Mat image)
{   //��ȡͼ�����벿��
	int image_row = image.rows;
	int image_col = image.cols;
	int image_axis_x = image_row / 2;
	int image_axis_y = image_col / 2;
	Mat imagecopy = image.clone();//image����
	Mat imagecopy1 = image.clone();//image����
	//int image_x_max = CutImage(imagecopy1);
	//Mat imageROI = image(Rect(0, 0, image_x_max-1, image_row));//�ص���Ĵָ������
	//Mat imageROI = CutImage(imagecopy1);


	//�Զ�ֵͼ������
	vector<vector<Point> > contours;

	vector<Vec4i> hierarchy;



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

	//����������ֵ
	vector<int> contours_min_x;
	vector<int> contours_min_y;
	for (int i = 10; i < maxsize-10; i++)
	{
		if (contours[max][i].x < contours[max][i + 10].x && contours[max][i].x < contours[max][i - 10].x)
		{	
			contours_min_x.push_back(contours[max][i].x);
			contours_min_y.push_back(contours[max][i].y);
			i = i + 30;//�ų���ֵ�����ڵĸ���
		}
	}
	//�����Ϸ���ֵ�Ĳ����ж�
	if (contours_min_x.size() == 3)
	{
		contours_min_x.push_back(contours[max][0].x);
		contours_min_y.push_back(contours[max][0].y);
	}
	//int j = 0;
	//for (size_t i = 1; i < contours[contours.size() - 1].size(); ++i)
	//{
	//if (contours[contours.size() - 1][i].x < contours[contours.size() - 1][i + 1].x )
	//{
	//contours_min_x[j] = contours[contours.size() - 1][i].x;
	//contours_min_y[j] = contours[contours.size() - 1][i].y;
	//++j;
	//}
	//}
	
	//�ҳ����м�ֵ������С���ĸ�ֵ��T1,T2,T3,T4��

//
	for (int i = 0; i < contours_min_x.size(); i++){
		//�Ƚ��������ڵ�Ԫ��   
		for (int j = 0; j < contours_min_x.size() - i - 1; j++){
			if (contours_min_x[j] > contours_min_x[j + 1])
			{
				//��contours_min_x��������
				int t = contours_min_x[j];
				contours_min_x[j] = contours_min_x[j + 1];
				contours_min_x[j + 1] = t;
				//contours_min_y�����Ž�������
				int t1 = contours_min_y[j];
				contours_min_y[j] = contours_min_y[j + 1];
				contours_min_y[j + 1] = t1;
			}
		}
	}


	int T1_min_x = contours_min_x[0];
	int T1_min_y = contours_min_y[0];
	int T2_min_x = contours_min_x[1];
	int T2_min_y = contours_min_y[1];
	int T3_min_x = contours_min_x[2];
	int T3_min_y = contours_min_y[2];
	int T4_min_x = contours_min_x[3]; 
	int T4_min_y = contours_min_y[3];


	//ȷ�����ĸ�T������
	int *T = new int[8];
	T[0] = T1_min_x;
	T[1] = T1_min_y;
	T[2] = T2_min_x;
	T[3] = T2_min_y;
	T[4] = T3_min_x;
	T[5] = T3_min_y;
	T[6] = T4_min_x;
	T[7] = T4_min_y;

	return T;
}