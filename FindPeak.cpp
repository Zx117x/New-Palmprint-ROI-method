#include<opencv2/opencv.hpp>
#include <iostream>
#include<vector>
#include "CmpPic.h"
using namespace std;
using namespace cv;
//找出四个极值点（即为四个指尖）
int *FindPeak(Mat image)
{   //截取图像的第二象限
	int image_row = image.rows;
	int image_col = image.cols;
	int image_axis_x = image_row / 2;
	int image_axis_y = image_col / 2;
	Mat imagecopy = image.clone();//image备份
	Mat imageROI = image(Rect(0, 0, image_axis_y, image_axis_x));


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

	//对轮廓点求极值
	int contours_min_x[300];
	int contours_min_y[300];
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
	vector<int> sign;

	for (int i = 1; i<maxsize; i++)
	{
		/*相邻值做差：
		*小于0，赋-1
		*大于0，赋1
		*等于0，赋0
		*/
		int diff = contours[max][i].x - contours[max][i - 1].x;
		if (diff>0)
		{
			sign.push_back(1);
		}
		else if (diff < 0)
		{
			sign.push_back(-1);
		}
		else
		{
			sign.push_back(0);
		}
	}
	//再对sign相邻位做差  
	//保存极大值和极小值的位置  
	vector<int> indMax;
	vector<int> indMin;

	for (int j = 1; j < sign.size(); j++)
	{
		int diff = sign[j] - sign[j - 1];
		if (diff<0)
		{
			indMax.push_back(j);
		}
		else if (diff>0)
		{
			indMin.push_back(j);
		}
	}
	for (int n = 0; n < indMin.size(); n++)
	{
		contours_min_x[n] = contours[max][indMin[n]].x;
		contours_min_y[n] = contours[max][indMin[n]].y;
	}
	//找出所有极值点中最小的四个值（T1,T2,T3,T4）

	//找出最小值
	int T1_min_x = contours_min_x[0];
	int T1_min_y = contours_min_y[0];

	for (int n = 0; n < indMin.size(); n++)
	{
		if (T1_min_x>contours_min_x[n])
		{
			T1_min_x = contours_min_x[n];
			T1_min_y = contours_min_y[n];
		     
		}
	}
	//找出第二小的值

	int contours_min_x2[200];
	int contours_min_y2[200];
	int mn = 0;
	
	for (int n = 0; n < indMin.size(); n++)
	{
		
		if (contours_min_x[n]>T1_min_x + 15 && abs(contours_min_y[n] - T1_min_y)> 20)//这个条件排除相近极值点的干扰(不够精准，有待完善)
		{
			contours_min_x2[mn] = contours_min_x[n];
			contours_min_y2[mn] = contours_min_y[n];
			mn++;
		}
		
	}
	//找出第二小的值
	int T2_min_x = contours_min_x2[0];
	int T2_min_y = contours_min_y2[0];
	for (int n = 0; n < mn; n++)
	{
		if (T2_min_x>contours_min_x2[n])
		{
			T2_min_x = contours_min_x2[n];
			T2_min_y = contours_min_y2[n];

		}
	}


	//截取图像的第三象限

	Mat imageROI3 = image(Rect(0, image_axis_x, image_axis_y, image_axis_x));
	//对二值图像画轮廓
	vector<vector<Point> > contours3;

	vector<Vec4i> hierarchy3;
	findContours(imageROI3, contours3, hierarchy3, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);//找出二值图像的轮廓
	//找出contours中数量最多的size
	int maxsize1 = contours3[0].size();
	int max1 = 0;
	for (int i = 0; i < contours3.size(); i++)
	{
		if (contours3[i].size()>maxsize1)
		{
			maxsize1 = contours3[i].size();
			max1 = i;
		}
	}

	drawContours(imageROI3, contours3, max1, cv::Scalar::all(255));//在image中画出轮廓

	//对轮廓点求极值
	int contours_min_x3[300];
	int contours_min_y3[300];
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
	vector<int> sign3;

	for (int i = 1; i<maxsize1; i++)
	{
		/*相邻值做差：
		*小于0，赋-1
		*大于0，赋1
		*等于0，赋0
		*/
		int diff3 = contours3[max1][i].x - contours3[max1][i - 1].x;
		if (diff3>0)
		{
			sign3.push_back(1);
		}
		else if (diff3 < 0)
		{
			sign3.push_back(-1);
		}
		else
		{
			sign3.push_back(0);
		}
	}
	//再对sign相邻位做差  
	//保存极大值和极小值的位置  
	vector<int> indMax3;
	vector<int> indMin3;

	for (int j = 1; j < sign3.size(); j++)
	{
		int diff3 = sign3[j] - sign3[j - 1];
		if (diff3<0)
		{
			indMax3.push_back(j);
		}
		else if (diff3>0)
		{
			indMin3.push_back(j);
		}
	}
	for (int n = 0; n < indMin3.size(); n++)
	{
		contours_min_x3[n] = contours3[max1][indMin3[n]].x;
		contours_min_y3[n] = contours3[max1][indMin3[n]].y;
	}
	//找出所有极值点中最小的四个值（T1,T2,T3,T4）

	//找出最小值
	int T3_min_x = contours_min_x3[0];
	int T3_min_y = contours_min_y3[0];
	
	for (int n = 0; n < indMin3.size(); n++)
	{
		if (T3_min_x>contours_min_x3[n])
		{
			T3_min_x = contours_min_x3[n];
			T3_min_y = contours_min_y3[n];
			
		}
	}
	//找出第二小的值
	int T4_min_x = contours_min_x3[0];
	int T4_min_y = contours_min_y3[0];
	int contours_min_x4[200];
	int contours_min_y4[200];
	int mn2 = 0;
	
	for (int n = 0; n < indMin3.size(); n++)
	{

		if (contours_min_x3[n]>T3_min_x + 15 && abs(contours_min_y3[n] - T3_min_y)> 20)//这个条件排除相近极值点的干扰
		{
			contours_min_x4[mn2] = contours_min_x3[n];
			contours_min_y4[mn2] = contours_min_y3[n];
			mn2++;
			

		}

	}
	for (int n = 0; n < mn2; n++)
	{
		if (T4_min_x>contours_min_x4[n])
		{
			T4_min_x = contours_min_x4[n];
			T4_min_y = contours_min_y4[n];

		}
	}

	//确定出四个T的坐标
	int T[8];
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