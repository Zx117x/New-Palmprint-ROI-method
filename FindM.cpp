#include<opencv2/opencv.hpp>
#include <iostream>
#include<vector>
#include "CmpPic.h"
using namespace std;
using namespace cv;
int *FindM(Mat dst)
{
	
	int dst_row = dst.rows;
	int dst_col = dst.cols;
	int dst_axis_x = dst_row / 2;
	int dst_axis_y = dst_col / 2;
	int n1,n2,n3,n4;
	Mat dstROI = dst(Rect(0, 0, dst_col-50, dst_row));
	//Mat dstROI3 = dst(Rect(0, dst_axis_x, dst_axis_y, dst_axis_x));
	int dst_x[200];
	int dst_y[200];
	int dst_x1[200];
	int dst_y1[200];
	
	//������
	vector<vector<Point> > contours;

	vector<Vec4i> hierarchy;



	findContours(dstROI, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);//�ҳ���ֵͼ�������
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

	drawContours(dstROI, contours, max, cv::Scalar::all(255));//��image�л�������
	
	//�ҳ����м���ֵ�㣨�ȵ㣩
	//����������ֵ
	vector<int> contours_max_x;
	vector<int> contours_max_y;
	for (int i = 15; i < maxsize - 15; i++)
	{
		if (contours[max][i].x > contours[max][i + 15].x && contours[max][i].x > contours[max][i - 15].x)
		{
			contours_max_x.push_back(contours[max][i].x);
			contours_max_y.push_back(contours[max][i].y);
			i = i + 30;//�ų���ֵ�����ڵĸ���
		}
	}
	//�ų����߽�����
	vector<int>::iterator it;
	vector<int>::iterator it_y;
	int temp;//��ʱ���contours_max_x�ı߽�����
	int flag = 0;
	for (int i = 0; i<contours_max_x.size(); i++)
	{
		if (contours_max_x[i] + 30>dst_col - 50)
		{
			temp =i;
			flag = 1;
		}
	}
	if (flag)
	{
		//ɾ����contours_max_x�е�Ԫ��
		contours_max_x.erase(contours_max_x.begin() + temp); //ɾ��Ԫ�� 
		//ͬʱҲҪ��contours_max_y�е�Ԫ��ɾ��
		contours_max_y.erase(contours_max_y.begin() + temp); //ɾ��Ԫ��
	}
	  


	//�ҳ����м�ֵ���е�һ���͵��������ֵĹȵ㣨M1,M2��
	//����ֵ��������갴��С�����������
	for (int i = 0; i < contours_max_y.size(); i++){
		//�Ƚ��������ڵ�Ԫ��   
		for (int j = 0; j < contours_max_y.size() - i - 1; j++){
			if (contours_max_y[j] > contours_max_y[j + 1])
			{
				//��contours_max_y��������
				int t = contours_max_y[j];
				contours_max_y[j] = contours_max_y[j + 1];
				contours_max_y[j + 1] = t;
				//contours_max_x�����Ž�������
				int t1 = contours_max_x[j];
				contours_max_x[j] = contours_max_x[j + 1];
				contours_max_x[j + 1] = t1;
			}
		}
	}
	int *M = new int[6];
		M[0] = contours_max_x[0];
		M[1] = contours_max_y[0];
		M[2] = contours_max_x[2];
		M[3] = contours_max_y[2];
		M[4] = contours_max_x[1];
		M[5] = contours_max_y[1];

	return M;

}