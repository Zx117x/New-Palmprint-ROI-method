#include<opencv2/opencv.hpp>
#include <iostream>
#include<vector>
#include "CmpPic.h"
#include<fstream>
using namespace std;
using namespace cv;
Mat CutWristImage1(Mat image)//输入灰度，输出灰度
{
	//1.根据轮廓截取矩形图像
	int image_row = image.rows;
	int image_col = image.cols;
	Mat imagecopy = image.clone();//备份image
	Mat imagetemp;//初始二值化
	//高斯滤波
	GaussianBlur(image, image, Size(3,3),0,0);
	//二值化
	double OtsuThe = 0;

	OtsuThe = threshold(image, imagetemp, OtsuThe, 255, THRESH_OTSU + THRESH_BINARY);//进行二值化
	
	threshold(image, image, OtsuThe*0.65, 255, THRESH_BINARY);//得到最终的二值化阈值
	//开闭运算
	//Mat element = getStructuringElement(MORPH_RECT, Size(15, 15)); //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的

	//morphologyEx(image, image, MORPH_OPEN, element);
	//morphologyEx(image, image, MORPH_CLOSE, element);

	
	Mat imagecopy1 = image.clone();//二值图像
	
	vector<vector<Point> > contours;

	vector<Vec4i> hierarchy;

	findContours(image, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);//找出二值图像的轮廓
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

	drawContours(image, contours, max, cv::Scalar::all(255));//在image中画出轮廓

	Rect rect1 = boundingRect(contours[max]);//建立轮廓外包矩形

	///
	Mat dst,dstg;

	dst = imagecopy1(rect1);//获得封闭矩形二值图像
	dstg = imagecopy(rect1);//获得封闭矩形灰度图像

    //2.截取掉手腕
	//从图像最右端开始找轮廓
	int dst_row = dst.rows;
	int dst_col = dst.cols;
	Mat dstCopy = dst.clone();
	Mat dstCopy2 = dstg.clone();

	//Mat  dstROI = dst(Rect(dst_col * 3 / 4, 0, dst_col / 4, dst_row / 2));

	//对二值图像画轮廓
	vector<vector<Point> > contours1;

	vector<Vec4i> hierarchy1;



	findContours(dst, contours1, hierarchy1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);//找出二值图像的轮廓
	//找出contours中数量最多的size
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


	drawContours(dst, contours1, max1, cv::Scalar::all(255));//在image中画出轮廓

	//****2.1遍历所有轮廓点，找出最右端开始的轮廓点**
	vector<int> dst_x;
	vector<int> dst_y;
	for (int i = 0; i < maxsize1; i++)
	{
		if (contours1[max1][i].x<dst_col&&contours1[max1][i].x>dst_col*3 / 4)
		{
			dst_x.push_back(contours1[max1][i].x);
			dst_y.push_back(contours1[max1][i].y);
			
		}
	}
	//****2.2找出x值相同的轮廓点（即垂直方向）
	vector<int> dst_x_distance;
	vector<int> dst_y_distance;
	vector<int> dst_y_val;//存储对应的两个y值
	for (int i = 0; i < dst_x.size(); i++)
	{
		for (int j = i + 1; j < dst_x.size(); j++)
		{
			if (dst_x[i] == dst_x[j])
			{
				dst_y_distance.push_back(abs(dst_y[i] - dst_y[j]));
				dst_y_val.push_back(dst_y[i]);
				dst_y_val.push_back(dst_y[j]);
				dst_x_distance.push_back(dst_x[i]);
				break;
			}
		}
		
	}
	//排除相邻点的干扰
	vector<int> dst_x_dist;
	vector<int> dst_y_dist;
	vector<int> dst_y_va1;
	vector<int> dst_y_va2;//存储对应的两个y值
	for (int i = 0; i < dst_y_distance.size(); i++)
	{
		if (dst_y_distance[i]>10)
		{
			dst_x_dist.push_back(dst_x_distance[i]);
			dst_y_dist.push_back(dst_y_distance[i]);
			dst_y_va1.push_back(dst_y_val[2*i]);
			dst_y_va2.push_back(dst_y_val[2*i+1]);
		}

	}

	//****2.3对得到的dst_x_distance按从大到小（从右到左）排序
	for (int i = 0; i < dst_x_dist.size(); i++){
		//比较两个相邻的元素   
		for (int j = 0; j < dst_x_dist.size() - i - 1; j++){
			if (dst_x_dist[j] < dst_x_dist[j + 1])
			{
				//对dst_x_distance进行排序
				int t = dst_x_dist[j];
				dst_x_dist[j] = dst_x_dist[j + 1];
				dst_x_dist[j + 1] = t;
				//dst_y_distance跟随着进行排序
				int t1 = dst_y_dist[j];
				dst_y_dist[j] = dst_y_dist[j + 1];
				dst_y_dist[j + 1] = t1;
				//dst_y_va跟随着进行排序
				int t2 = dst_y_va1[j];
				dst_y_va1[j] = dst_y_va1[j + 1];
				dst_y_va1[j + 1] = t2;
				int t3 = dst_y_va2[j];
				dst_y_va2[j] = dst_y_va2[j + 1];
				dst_y_va2[j + 1] = t3;
			}
		}
	}
	//*相邻项做差，找出第一个出现差值比前项都大的项
	vector<int> Sub;
	vector<int> Sub_val;
	int n=0;
	bool flag=0;
	int count=0;
	if (dst_y_dist.size() < 20)
	{
		count = 0;
	}
	else if (dst_y_dist[4] - dst_y_dist[0]>10)
	{
		count = 0;
	}
	else
	{
		for (int i = 1; i < dst_x_dist.size(); i++)
		{
			Sub.push_back(abs(dst_y_dist[i] - dst_y_dist[i - 1]));
			Sub_val.push_back(dst_y_dist[i] - dst_y_dist[i - 1]);
		}
		//判断是否有手腕
		for (int i = 0; i<Sub_val.size()/2; i++)
		{
			if (Sub_val[i]<=0)
			{
				flag = 1;
				n = i;
				break;
			}
		}
		if (flag==0)
		{
			count = 0;
		}
		else
		{
			for (int i = n; i < Sub.size(); i++)
			{
				for (int j = 0; j < i; j++)
				{
					if (Sub[i]>Sub[j])
					{
						count++;
					}
				}
				//判断是否为突变点，而且后几项均递增
				if (count == i)
				{			
						break;
				}
				else
				{
					count = 0;
				}
			}
		}
	}
	
	
	//**存储距离数值
	//ofstream outfile("D:\\课题组\\掌静脉识别（课题组）\\ROI提取\\源码\\PalmRoi(CASIA)\\distance.txt", ios::in | ios::out | ios::binary);
	//if (!outfile.is_open())
	//{ 
	//	cout << " the file open fail" << endl; exit(1);
	//}
	//for (int i = 0; i < dst_y_dist.size(); i++)
	//{
		//outfile << dst_y_dist[i] << " ";	
		//outfile << "\r\n"; 
	//}
	//outfile.close();

	
	//****2.4找出dst_y_distance突变的值
	int w1_x, w1_y, w2_x, w2_y;
	bool wrist=0;//判断标志位
	int w1_y_total[2];
	//判断是否有手腕
	if (count==0)
	{
		wrist = 0;
	}
	else
	{
				w1_x = dst_x_dist[count];
				w1_y_total[0] = dst_y_va1[count];
				w1_y_total[1] = dst_y_va2[count];
				wrist = 1;	
	}
		//****2.5求出斜率与y值
		Mat  dstROI;
		int val;
		if (wrist)
		{

			//对w1_y_total排序

			if (w1_y_total[0]>w1_y_total[1])
			{
				int t = w1_y_total[0];
				w1_y_total[0] = w1_y_total[1];
				w1_y_total[1] = t;
			}

			dstROI = dstCopy(Rect(w1_x, 0, dst_col - w1_x, dst_row / 2));
			//画轮廓
			vector<vector<Point> > contours11;
			vector<Vec4i> hierarchy11;
			findContours(dstROI, contours11, hierarchy11, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);//找出二值图像的轮廓
			//找出contours中数量最多的size
			int maxsize11 = contours11[0].size();
			int max11 = 0;
			for (int i = 0; i < contours11.size(); i++)
			{
				if (contours11[i].size()>maxsize11)
				{
					maxsize11 = contours11[i].size();
					max11 = i;
				}
			}


			drawContours(dstROI, contours11, max11, cv::Scalar::all(255));//在image中画出轮廓
			//求斜率
			int N = maxsize11;//所有轮廓点的数量
			double contours_x[200];
			double contours_y[200];
			int j = 0;
			int n = 0;
			//以5为步长截取N/5个点
			for (int i = 1; i < N / 2; i++)
			{
				contours_x[j] = contours11[max11][i * 2].x;
				contours_y[j] = contours11[max11][i * 2].y;
				n = j;
				j++;
			}
			//计算相邻点间的斜率并记录
			double contours_k[200];
			for (int i = 1; i < n; i++)
			{
				if (contours_x[i] != contours_x[i - 1])
				{
					contours_k[i - 1] = (contours_y[i] - contours_y[i - 1]) / (contours_x[i] - contours_x[i - 1]);
				}
				if (contours_x[i] == contours_x[i - 1])
				{
					contours_k[i - 1] = 10;
				}

			}
			//对斜率数据进行处理
			int contours_k1[200];
			for (int i = 0; i < n - 1; i++)
			{

				contours_k1[i] = Round(contours_k[i] * 10, 0);//四舍五入，保留一位小数×10倍为整型
			}


			//找出出现的斜率最多的元素


			map<int, int> m;
			int w1_y_hat;
			for (int i = 0; i<n - 1; i++)
			{
				m[contours_k1[i]]++;
				if (m[contours_k1[i]] >= m[val])
					val = contours_k1[i];//斜率值就应为val/10
			}
			//求y值
			if (val < 0)
			{
				w1_y = w1_y_total[1];
				w1_y_hat = w1_y_total[0];
			}
			else
			{
				w1_y = w1_y_total[0];
				w1_y_hat = w1_y_total[1];
			}
			//****2.6根据斜率求出w2的值
			if (val != 0)
			{
				w2_x =Round(double(w1_x) + double(10 * val*(w1_y - w1_y_hat)) / double(val*val + 100),0);
				//w2_y = val / 10 * (w2_x - w1_x) + w1_y_hat;
			}
			else
			{
				w2_y = w1_y;
				w2_x = w1_x;
			}
		}
		else
		{
			w2_y = dst_row;
			w2_x = dst_col;
		}
	
	//****2.7切割掉手腕

	Mat dstcut = dstCopy2(Rect(0, 0, w2_x, dst_row));
	

	//3.根据轮廓获得矩形图像

	//int dstcut_row = dstcut.rows;
	//int dstcut_col = dstcut.cols;
//	Mat dstcutcopy = dstcut.clone();//备份image
	//二值化
//	vector<vector<Point> > contours2;

	//vector<Vec4i> hierarchy2;

//	findContours(dstcut, contours2, hierarchy2, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);//找出二值图像的轮廓
	//找出contours中数量最多的size
//	int maxsize2 = contours2[0].size();
//	int max2 = 0;
//	for (int i = 0; i < contours2.size(); i++)
//	{
//		if (contours2[i].size()>maxsize2)
//		{
//			maxsize2 = contours2[i].size();
//			max2 = i;
//		}
//	}

//	drawContours(dstcut, contours2, max2, cv::Scalar::all(255));//在image中画出轮廓

//	Rect rect2 = boundingRect(contours2[max2]);//建立轮廓外包矩形

	///
//	Mat dstout, dstoutg;

//	dstout = dstcutcopy(rect2);//获得封闭矩形二值图像
	//dstoutg = dstcutg(rect2);

	return dstcut;
}