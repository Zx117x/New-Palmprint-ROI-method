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
	//遍历图像二三象限
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
	//找出数组中最小值（一般应为T3）
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
	//找出数组中第二小的数T2
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
	//在最小值点与最后一行间遍历图像找出唯一的谷点M2
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
	//找出数组中的最大值即为M1
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
	//得到M2的坐标
	int dst_M2_row = dst_y_max;
	int dst_M2_col = dst_m_max;
	int M2[2];
	M2[0] = dst_M2_row;
	M2[1] = dst_M2_col;

	return M2;

}