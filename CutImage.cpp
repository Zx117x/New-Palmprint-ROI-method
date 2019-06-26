#include<opencv2/opencv.hpp>
#include <iostream>
#include<vector>
#include "CmpPic.h"
using namespace std;
using namespace cv;
//对图像进行预处理，截取掉大拇指部分
Mat CutImage(Mat image)
{
	int image_row = image.rows;
	int image_col = image.cols;
	Mat imagecopy = image.clone();//备份image
	//截取图像部分
	Mat  imageROI = image(Rect(0, 0, image_col-50, image_row));
	Mat imageCut;
	
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
	//对轮廓点求极大值
	vector<int> contours_min_x;
	vector<int> contours_min_y;

	
	for (int i = 10; i < maxsize - 10; i++)
	{
		if (contours[max][i].x >contours[max][i + 10].x && contours[max][i].x >contours[max][i - 10].x)
		{
			contours_min_x.push_back(contours[max][i].x);
			contours_min_y.push_back(contours[max][i].y);
			i = i + 20;//排除极值点相邻的干扰
			
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
	
	//找出最大的极大值
	int N = contours_min_x.size();
	for (int i = 0; i < contours_min_x.size(); i++){
		//比较两个相邻的元素   
		for (int j = 0; j < contours_min_x.size() - i - 1; j++){
			if (contours_min_x[j] > contours_min_x[j + 1])
			{
				//对contours_min_x进行排序
				int t = contours_min_x[j];
				contours_min_x[j] = contours_min_x[j + 1];
				contours_min_x[j + 1] = t;
				//contours_min_y跟随着进行排序
				int t1 = contours_min_y[j];
				contours_min_y[j] = contours_min_y[j + 1];
				contours_min_y[j + 1] = t1;
			}
		}
	}
	if (contours_min_y[N - 2]> image_row / 2)//N-2为大拇指谷点
	{
		if (contours_min_y[N - 3]<image_row / 2)//N-3为小拇指谷点
		{
			if (contours_min_x[N - 2] - 10 > contours_min_x[N - 3])
			{
				imageCut = imagecopy(Rect(0, 0, contours_min_x[N - 2] - 1, image_row));//截掉大拇指的轮廓
			}
			else
			{
				imageCut = imagecopy(Rect(0, 0, image_col, contours_min_y[N - 2]));
			}
			
		}
		else
		{
			//N-3为干扰
			imageCut = imagecopy(Rect(0, 0, contours_min_x[N - 2] - 1, image_row));//截掉大拇指的轮廓
		}
		
	}
	else//N-2为小拇指谷点
	{
		imageCut = imagecopy(Rect(0, 0, image_col, contours_min_y[N - 3]));
	}

	return imageCut;

}