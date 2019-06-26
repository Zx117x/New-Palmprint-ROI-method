#include<opencv2/opencv.hpp>
#include <iostream>
#include "CmpPic.h"
#include<vector>
#include<cmath>
#include <fstream> 
#include <windows.h>
using namespace std;
using namespace cv;
int roidetect();


int main(){


	roidetect();

}


int roidetect(){
	//*********函数测试***********//
	
	Mat image;
	string ImgName;
	string savefile;
	ifstream fin("D:/课题组/掌静脉识别（课题组）/ROI提取/图库/CASIA/手腕截取测试2/test2.txt");

	while (getline(fin, ImgName)) //逐行读取文件列表  
	{
		//DWORD star_time = GetTickCount();
		string ImgName1 = ImgName;
		string savefile = "D:\\课题组\\掌静脉识别（课题组）\\ROI提取\\图库\\CASIA\\ROI结果1\\" + ImgName1;
		ImgName = "D:/课题组/掌静脉识别（课题组）/ROI提取/图库/CASIA/CASIA_l/" + ImgName;


		image = imread(ImgName, 1);


		//Mat image = imread("D:/课题组/掌静脉识别（课题组）/ROI提取/图库/CASIA/CASIA_l/0195_f_l_08.jpg", 1);//读取图片  
		//flip(image, image, 0);//输入为右手需进行翻转

		cvtColor(image, image, CV_BGR2GRAY);
		//threshold(image, image, 150, 255, THRESH_BINARY_INV);

		//根据轮廓截取的灰度图像作为最后进行ROI的图像
		Mat imageCopy1 = image.clone();
		//Mat imageCopy1 = CutWristImage1(imageClone);
		Mat imageCopy2 = imageCopy1.clone();
		//Mat imageCopy2 = SetImage(imageCopy1);//返回灰度图像
		//对与中科院的图库需进行预处理，截取掉手腕部分




		//    TRIANGLE算法计算阈值
		//double triThe = 0;

		//triThe = threshold(image, image, triThe, 255, THRESH_TRIANGLE + THRESH_BINARY);//进行二值化
		Mat imagetemp;//初始二值化
		//二值化
		double OtsuThe = 0;

		OtsuThe = threshold(imageCopy1, imagetemp, OtsuThe, 255, THRESH_OTSU + THRESH_BINARY);//进行二值化

		threshold(imageCopy1, imageCopy1, OtsuThe*0.65, 255, THRESH_BINARY);//得到最终的二值化阈值
		//开闭运算
		//Mat element = getStructuringElement(MORPH_RECT, Size(15, 15)); //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的

		//morphologyEx(imageCopy1, imageCopy1, MORPH_OPEN, element);
		//morphologyEx(imageCopy1, imageCopy1, MORPH_CLOSE, element);
		//threshold(imageCopy1, imageCopy1, 60, 255, THRESH_BINARY);//阈值为100进行二值化
		//namedWindow("test");

		//imshow("test", dst);

		//cvWaitKey(0);


		//Mat dst = SetRectImage(image);//根据轮廓截取的二值图像
		//对与中科院的图库需进行预处理，截取掉手腕部分
		//	Mat dstcut1 = CutWrist(dst);
		//	Mat dstcut = SetRectImage(dstcut1);
		Mat dstCopy = imageCopy1.clone();
		Mat dstCopy1 = imageCopy1.clone();//图像备份
		//int *t;

		//t = FindPeak(dst);////找出四个极值点（即为四个指尖）
		//t = FindPeak1(dst);


		int *m;
		m = FindM(dstCopy);
		//m = FindM1(dstCopy, t);
		//delete t;//释放FindPeak内new的动态内存
		int m1_col = m[0];//得到谷点M1的列数
		int m1_row = m[1];//得到谷点M1的行数
		int m2_col = m[2];//得到谷点M2的列数
		int m2_row = m[3];//得到谷点M2的行数

		//**用于后面找斜率**//
		int m3_col = m[2];//得到谷点M3的列数
		int m3_row = m[3];//得到谷点M3的行数
		delete m;//释放FindM1内new的动态内存
		int N1 = FindLine(dstCopy1, m1_col, m1_row, m3_col, m3_row);//找出拇指边缘线，返回值为斜率的十倍
		double k = N1 / 10.0;
		double Theta = atan(k);//求出偏移角度(rad)
		double Rad_to_deg = 45.0 / atan(1.0);//用于rad至deg角度转换
		int Theta1 = Rad_to_deg*Theta;//求出偏移角度(deg)。这里负数为逆时针，正数为顺时针。
		//****图像校正****//
		double x1;
		double y1;
		int x2;
		double y2;
		bool flag = 0;//判断标志位
		//取列数大的那个谷点为基准点
		if (m1_col >= m2_col)
		{
			x1 = m1_col;
			y1 = m1_row;
			y2 = m2_row;
		}
		else
		{
			x1 = m2_col;
			y1 = m2_row;
			y2 = m1_row;
			flag = 1;
		}

		int o_x, o_y, o1_x, o1_y, M1M2;
		Mat rot_image;
		Mat rot_mat;
		Mat result;
		RotatedRect rRect;
		Point2f vertices[4];      //定义4个点的数组
		//计算校正后的M2坐标
		if (k != 0)
		{
			x2 = (y1 - y2)*k + x1;//M1,M2间的直线应与k斜率所在直线垂直
			o_x = (x1 + x2) / 2;
			o_y = (y1 + y2) / 2;
			o1_x = (cos(Theta) * 6 * sqrt((y2 - y1)*(y2 - y1) + (x2 - x1)*(x2 - x1)) / 8) + o_x;
			o1_y = k*(o1_x - o_x) + o_y;
			//以o1为中心点做矩形（为了避免画的矩形线对于最后提取的干扰，Size可以设大一点）
			rRect = RotatedRect(Point2f(o1_x, o1_y), Size2f(135, 135), Theta1);

			rRect.points(vertices);   //将四个点存储到vertices数组中
			for (int i = 0; i < 4; i++)
			{
				//画矩形
				line(imageCopy2, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0));
			}
			//截取ROI

			rot_mat = getRotationMatrix2D(Point2f(o1_x, o1_y), Theta1, 1.0);//求旋转矩阵
			warpAffine(imageCopy2, rot_image, rot_mat, imageCopy2.size());//原图像旋转
			result = rot_image(Rect(o1_x - 64, o1_y - 64, 128, 128));//提取ROI
		}
		else
		{
			if (flag)
			{
				x2 = m1_col;
			}
			else
			{
				x2 = m2_col;
			}

			o1_x = x1 + 5 * abs(y2 - y1) / 8;
			o1_y = (y1 + y2) / 2;
			result = imageCopy2(Rect(o1_x - 64, o1_y - 64, 128, 128));//提取ROI
			
		}

		//string savefile = "D:\\课题组\\掌静脉识别（课题组）\\ROI提取\\图库\\CASIA\\ROI结果\\0195_f_l_08.jpg";
		//DWORD end_time = GetTickCount();

		//cout << "这个程序运行时间为：" << (end_time - star_time) << "ms." << endl;

		//system("pause");
		imwrite(savefile, result);
	}

	
//*********SetRectImage函数测试********//



	return 0;



}