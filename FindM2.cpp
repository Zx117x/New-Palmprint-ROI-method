#include<opencv2/opencv.hpp>
#include <iostream>
#include<vector>
#include "CmpPic.h"
using namespace std;
using namespace cv;
int *FindM2(Mat dst)
{
	int dst_row = dst.rows;
	int dst_col = dst.cols;
	int dst_axis_x = dst_row / 2;
	int dst_axis_y = dst_col / 2;
	int N = dst_axis_x;
	int dst_x[500];
	//����ͼ���������
	for (int i = dst_axis_x; i < dst_row; ++i)
	{
		for (int j = 0; j < dst_axis_y; ++j)
		{

			if (255 == dst.at<uchar>(i, j))
			{
				dst_x[i] = j;
				break;
			}
		}
	}
	//�ҳ���������Сֵ��һ��ӦΪT3��
	int dst_x_min = dst_x[dst_axis_x];
	int dst_min;
	int dst_y_min;
	for (int i = dst_axis_x; i<dst_row; i++)
	{
		if (dst_x[i]<dst_x_min)
		{
			dst_x_min = dst_x[i];
			dst_y_min = i;
		}
	}
	//�ҳ������еڶ�С����T2
	//int dst_x_secondmin = dst_x[0];
	//int dst_y_secondmin;

	//for (int j = 1; j <N; j++)
	//{

	//if (dst_x[j] < dst_x_secondmin && dst_x[j] > dst_x_min)
	//{

	//dst_x_secondmin = dst_x[j];
	//dst_y_secondmin = j;

	//}

	//}
	//����Сֵ�������һ�м����ͼ���ҳ�Ψһ�Ĺȵ�M2
	int dst_m[500];

	for (int i = dst_y_min; i < dst_row; ++i)
	{
		for (int j = 0; j < dst_axis_y; ++j)
		{

			if (255 == dst.at<uchar>(i, j))
			{

				dst_m[i] = j;
				break;
			}
		}
	}
	//�ҳ������е����ֵ��ΪM1
	int dst_m_max = dst_m[dst_y_min];
	int dst_y_max;
	for (int i = dst_y_min; i<dst_row; i++)
	{
		if (dst_m[i]>dst_m_max)
		{
			dst_m_max = dst_m[i];
			dst_y_max = i;
		}
	}
	//�õ�M2������
	int dst_M2_row = dst_y_max;
	int dst_M2_col = dst_m_max;
	int M2[2];
	M2[0] = dst_M2_row;
	M2[1] = dst_M2_col;

	return M2;

}