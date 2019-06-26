#include<opencv2/opencv.hpp>
#include <iostream>
#include<vector>
#include "CmpPic.h"
using namespace std;
using namespace cv;
int *FindM1(Mat dst, int *t)
{
	int dst_row = dst.rows;
	int dst_col = dst.cols;
	int dst_axis_x = dst_row / 2;
	int dst_axis_y = dst_col / 2;
	int n1, n2, n3, n4;
	Mat dstcopy = dst.clone();
	//int dst_xmax = CutImage(dstcopy);
	//Mat dstROI = dst(Rect(0, 0, dst_xmax-1, dst_row));
	//Mat dstROI=CutImage(dst);
	int t1_x = t[0];
	int t1_y = t[1];
	int t2_x = t[2];
	int t2_y = t[3];
	int t3_x = t[4];
	int t3_y = t[5];
	int t4_x = t[6];
	int t4_y = t[7];
	


	//画轮廓
	vector<vector<Point> > contours;

	vector<Vec4i> hierarchy;



	findContours(dst, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);//找出二值图像的轮廓
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

	drawContours(dst, contours, max, cv::Scalar::all(255));//在image中画出轮廓

	//找出位于T1,T2间,T3,T4间的所有轮廓点
	for (int i = 0; i <maxsize; i++)
	{

		if (contours[max][i].x == t1_x &&contours[max][i].y == t1_y)
		{
			n1 = i;

		}
		if (contours[max][i].x == t2_x &&contours[max][i].y == t2_y)
		{
			n2 = i;

		}
		if (contours[max][i].x == t3_x &&contours[max][i].y == t3_y)
		{
			n3 = i;

		}
		if (contours[max][i].x == t4_x &&contours[max][i].y == t4_y)
		{
			n4 = i;

		}
	}
	//对n1,n2,n3,n4排序
	int NM[4] = { n1, n2, n3, n4 };
	for (int i = 0; i < 4; i++){
		//比较两个相邻的元素   
		for (int j = 0; j < 4 - i - 1; j++){
			if (NM[j] > NM[j + 1]){
				int t = NM[j];
				NM[j] = NM[j + 1];
				NM[j + 1] = t;
			}
		}
	}
	n1 = NM[0];
	n2 = NM[1];
	n3 = NM[2];
	n4 = NM[3];

	//找出范围内所有的点
	vector<int> dst_x;
	vector<int> dst_y;
	vector<int> dst_x1;
	vector<int> dst_y1;
	for (int i = n1; i < n2; i++)
	{
		dst_x.push_back(contours[max][i].x);
		dst_y.push_back(contours[max][i].y);
		
	}
	for (int i = n3; i < n4; i++)
	{

		dst_x1.push_back(contours[max][i].x);
		dst_y1.push_back(contours[max][i].y);
		
	}
	int N = n2 - n1;
	int N1 = n4 - n3;

	//找出轮廓点中的最大值
	//  
	int dst_x_max = dst_x[0];
	int dst_y_max = dst_y[0];
	int dst_y_sum[100];
	//初始化为0;
	for (int i = 0; i < 100; i++)
	{
		dst_y_sum[i] = 0;
	}
	//找出最大值
	int x = 0;
	for (int i = 0; i <N; i++)
	{
		if (dst_x[i]>dst_x_max)
		{
			dst_x_max = dst_x[i];
			dst_y_max = dst_y[i];
		}

	}
	//找出所有最大值的和
	for (int i = 0; i < N; i++)
	{

		if (dst_x[i] == dst_x_max)
		{
			dst_y_sum[x] = dst_y[i];
			++x;
		}
	}
	//求所有最值的平均值
	int sum = 0;
	//
	int length;
	for (int i = 0; i < 100; i++)
	{
		if (dst_y_sum[i] == 0)
		{
			length = i;
			break;
		}
	}
	for (int i = 0; i < length; i++)
	{

		sum += dst_y_sum[i];
		dst_y_max = sum / length;

	}

	//同理找出T3,T4间的所有轮廓点中的最大值
	int dst_x1_max = dst_x1[0];
	int dst_y1_max = dst_y1[0];
	for (int i = 0; i <N1; i++)
	{
		if (dst_x1[i]>dst_x1_max)
		{
			dst_x1_max = dst_x1[i];
			dst_y1_max = dst_y1[i];
		}
	}
	//
	int dst_y1_sum[100];
	int x1 = 0;
	//初始化为0;
	for (int i = 0; i < 100; i++)
	{
		dst_y1_sum[i] = 0;
	}
	for (int i = 0; i < N1; i++)
	{

		if (dst_x1[i] == dst_x1_max)
		{
			dst_y1_sum[x1] = dst_y1[i];
			++x1;
		}
	}
	//求所有最小值的平均值
	int sum1 = 0;
	//
	int length1;
	for (int i = 0; i < 100; i++)
	{
		if (dst_y1_sum[i] == 0)
		{
			length1 = i;
			break;
		}
	}
	for (int i = 0; i < length1; i++)
	{

		sum1 += dst_y1_sum[i];
		dst_y1_max = sum1 / length1;

	}


	int *M = new int[4];
	M[0] = dst_x_max;
	M[1] = dst_y_max;
	M[2] = dst_x1_max;
	M[3] = dst_y1_max;

	return M;
	

}