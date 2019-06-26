#include<opencv2/opencv.hpp>
#include <iostream>
#include<vector>
#include "CmpPic.h"
using namespace std;
using namespace cv;
//��ͼ�����Ԥ������ȡ����Ĵָ����
Mat CutImage(Mat image)
{
	int image_row = image.rows;
	int image_col = image.cols;
	Mat imagecopy = image.clone();//����image
	//��ȡͼ�񲿷�
	Mat  imageROI = image(Rect(0, 0, image_col-50, image_row));
	Mat imageCut;
	
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
	//���������󼫴�ֵ
	vector<int> contours_min_x;
	vector<int> contours_min_y;

	
	for (int i = 10; i < maxsize - 10; i++)
	{
		if (contours[max][i].x >contours[max][i + 10].x && contours[max][i].x >contours[max][i - 10].x)
		{
			contours_min_x.push_back(contours[max][i].x);
			contours_min_y.push_back(contours[max][i].y);
			i = i + 20;//�ų���ֵ�����ڵĸ���
			
		}
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
	
	//�ҳ����ļ���ֵ
	int N = contours_min_x.size();
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
	if (contours_min_y[N - 2]> image_row / 2)//N-2Ϊ��Ĵָ�ȵ�
	{
		if (contours_min_y[N - 3]<image_row / 2)//N-3ΪСĴָ�ȵ�
		{
			if (contours_min_x[N - 2] - 10 > contours_min_x[N - 3])
			{
				imageCut = imagecopy(Rect(0, 0, contours_min_x[N - 2] - 1, image_row));//�ص���Ĵָ������
			}
			else
			{
				imageCut = imagecopy(Rect(0, 0, image_col, contours_min_y[N - 2]));
			}
			
		}
		else
		{
			//N-3Ϊ����
			imageCut = imagecopy(Rect(0, 0, contours_min_x[N - 2] - 1, image_row));//�ص���Ĵָ������
		}
		
	}
	else//N-2ΪСĴָ�ȵ�
	{
		imageCut = imagecopy(Rect(0, 0, image_col, contours_min_y[N - 3]));
	}

	return imageCut;

}