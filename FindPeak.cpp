#include<opencv2/opencv.hpp>
#include <iostream>
#include<vector>
#include "CmpPic.h"
using namespace std;
using namespace cv;
//�ҳ��ĸ���ֵ�㣨��Ϊ�ĸ�ָ�⣩
int *FindPeak(Mat image)
{   //��ȡͼ��ĵڶ�����
	int image_row = image.rows;
	int image_col = image.cols;
	int image_axis_x = image_row / 2;
	int image_axis_y = image_col / 2;
	Mat imagecopy = image.clone();//image����
	Mat imageROI = image(Rect(0, 0, image_axis_y, image_axis_x));


	//�Զ�ֵͼ������
	vector<vector<Point> > contours;

	vector<Vec4i> hierarchy;

	

	findContours(imageROI, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);//�ҳ���ֵͼ�������
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


	drawContours(imageROI, contours, max, cv::Scalar::all(255));//��image�л�������

	//����������ֵ
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
		/*����ֵ���
		*С��0����-1
		*����0����1
		*����0����0
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
	//�ٶ�sign����λ����  
	//���漫��ֵ�ͼ�Сֵ��λ��  
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
	//�ҳ����м�ֵ������С���ĸ�ֵ��T1,T2,T3,T4��

	//�ҳ���Сֵ
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
	//�ҳ��ڶ�С��ֵ

	int contours_min_x2[200];
	int contours_min_y2[200];
	int mn = 0;
	
	for (int n = 0; n < indMin.size(); n++)
	{
		
		if (contours_min_x[n]>T1_min_x + 15 && abs(contours_min_y[n] - T1_min_y)> 20)//��������ų������ֵ��ĸ���(������׼���д�����)
		{
			contours_min_x2[mn] = contours_min_x[n];
			contours_min_y2[mn] = contours_min_y[n];
			mn++;
		}
		
	}
	//�ҳ��ڶ�С��ֵ
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


	//��ȡͼ��ĵ�������

	Mat imageROI3 = image(Rect(0, image_axis_x, image_axis_y, image_axis_x));
	//�Զ�ֵͼ������
	vector<vector<Point> > contours3;

	vector<Vec4i> hierarchy3;
	findContours(imageROI3, contours3, hierarchy3, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);//�ҳ���ֵͼ�������
	//�ҳ�contours����������size
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

	drawContours(imageROI3, contours3, max1, cv::Scalar::all(255));//��image�л�������

	//����������ֵ
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
		/*����ֵ���
		*С��0����-1
		*����0����1
		*����0����0
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
	//�ٶ�sign����λ����  
	//���漫��ֵ�ͼ�Сֵ��λ��  
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
	//�ҳ����м�ֵ������С���ĸ�ֵ��T1,T2,T3,T4��

	//�ҳ���Сֵ
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
	//�ҳ��ڶ�С��ֵ
	int T4_min_x = contours_min_x3[0];
	int T4_min_y = contours_min_y3[0];
	int contours_min_x4[200];
	int contours_min_y4[200];
	int mn2 = 0;
	
	for (int n = 0; n < indMin3.size(); n++)
	{

		if (contours_min_x3[n]>T3_min_x + 15 && abs(contours_min_y3[n] - T3_min_y)> 20)//��������ų������ֵ��ĸ���
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

	//ȷ�����ĸ�T������
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