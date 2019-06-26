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
	//*********��������***********//
	
	Mat image;
	string ImgName;
	string savefile;
	ifstream fin("D:/������/�ƾ���ʶ�𣨿����飩/ROI��ȡ/ͼ��/CASIA/�����ȡ����2/test2.txt");

	while (getline(fin, ImgName)) //���ж�ȡ�ļ��б�  
	{
		//DWORD star_time = GetTickCount();
		string ImgName1 = ImgName;
		string savefile = "D:\\������\\�ƾ���ʶ�𣨿����飩\\ROI��ȡ\\ͼ��\\CASIA\\ROI���1\\" + ImgName1;
		ImgName = "D:/������/�ƾ���ʶ�𣨿����飩/ROI��ȡ/ͼ��/CASIA/CASIA_l/" + ImgName;


		image = imread(ImgName, 1);


		//Mat image = imread("D:/������/�ƾ���ʶ�𣨿����飩/ROI��ȡ/ͼ��/CASIA/CASIA_l/0195_f_l_08.jpg", 1);//��ȡͼƬ  
		//flip(image, image, 0);//����Ϊ��������з�ת

		cvtColor(image, image, CV_BGR2GRAY);
		//threshold(image, image, 150, 255, THRESH_BINARY_INV);

		//����������ȡ�ĻҶ�ͼ����Ϊ������ROI��ͼ��
		Mat imageCopy1 = image.clone();
		//Mat imageCopy1 = CutWristImage1(imageClone);
		Mat imageCopy2 = imageCopy1.clone();
		//Mat imageCopy2 = SetImage(imageCopy1);//���ػҶ�ͼ��
		//�����п�Ժ��ͼ�������Ԥ������ȡ�����󲿷�




		//    TRIANGLE�㷨������ֵ
		//double triThe = 0;

		//triThe = threshold(image, image, triThe, 255, THRESH_TRIANGLE + THRESH_BINARY);//���ж�ֵ��
		Mat imagetemp;//��ʼ��ֵ��
		//��ֵ��
		double OtsuThe = 0;

		OtsuThe = threshold(imageCopy1, imagetemp, OtsuThe, 255, THRESH_OTSU + THRESH_BINARY);//���ж�ֵ��

		threshold(imageCopy1, imageCopy1, OtsuThe*0.65, 255, THRESH_BINARY);//�õ����յĶ�ֵ����ֵ
		//��������
		//Mat element = getStructuringElement(MORPH_RECT, Size(15, 15)); //��һ������MORPH_RECT��ʾ���εľ���ˣ���Ȼ������ѡ����Բ�εġ������͵�

		//morphologyEx(imageCopy1, imageCopy1, MORPH_OPEN, element);
		//morphologyEx(imageCopy1, imageCopy1, MORPH_CLOSE, element);
		//threshold(imageCopy1, imageCopy1, 60, 255, THRESH_BINARY);//��ֵΪ100���ж�ֵ��
		//namedWindow("test");

		//imshow("test", dst);

		//cvWaitKey(0);


		//Mat dst = SetRectImage(image);//����������ȡ�Ķ�ֵͼ��
		//�����п�Ժ��ͼ�������Ԥ������ȡ�����󲿷�
		//	Mat dstcut1 = CutWrist(dst);
		//	Mat dstcut = SetRectImage(dstcut1);
		Mat dstCopy = imageCopy1.clone();
		Mat dstCopy1 = imageCopy1.clone();//ͼ�񱸷�
		//int *t;

		//t = FindPeak(dst);////�ҳ��ĸ���ֵ�㣨��Ϊ�ĸ�ָ�⣩
		//t = FindPeak1(dst);


		int *m;
		m = FindM(dstCopy);
		//m = FindM1(dstCopy, t);
		//delete t;//�ͷ�FindPeak��new�Ķ�̬�ڴ�
		int m1_col = m[0];//�õ��ȵ�M1������
		int m1_row = m[1];//�õ��ȵ�M1������
		int m2_col = m[2];//�õ��ȵ�M2������
		int m2_row = m[3];//�õ��ȵ�M2������

		//**���ں�����б��**//
		int m3_col = m[2];//�õ��ȵ�M3������
		int m3_row = m[3];//�õ��ȵ�M3������
		delete m;//�ͷ�FindM1��new�Ķ�̬�ڴ�
		int N1 = FindLine(dstCopy1, m1_col, m1_row, m3_col, m3_row);//�ҳ�Ĵָ��Ե�ߣ�����ֵΪб�ʵ�ʮ��
		double k = N1 / 10.0;
		double Theta = atan(k);//���ƫ�ƽǶ�(rad)
		double Rad_to_deg = 45.0 / atan(1.0);//����rad��deg�Ƕ�ת��
		int Theta1 = Rad_to_deg*Theta;//���ƫ�ƽǶ�(deg)�����︺��Ϊ��ʱ�룬����Ϊ˳ʱ�롣
		//****ͼ��У��****//
		double x1;
		double y1;
		int x2;
		double y2;
		bool flag = 0;//�жϱ�־λ
		//ȡ��������Ǹ��ȵ�Ϊ��׼��
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
		Point2f vertices[4];      //����4���������
		//����У�����M2����
		if (k != 0)
		{
			x2 = (y1 - y2)*k + x1;//M1,M2���ֱ��Ӧ��kб������ֱ�ߴ�ֱ
			o_x = (x1 + x2) / 2;
			o_y = (y1 + y2) / 2;
			o1_x = (cos(Theta) * 6 * sqrt((y2 - y1)*(y2 - y1) + (x2 - x1)*(x2 - x1)) / 8) + o_x;
			o1_y = k*(o1_x - o_x) + o_y;
			//��o1Ϊ���ĵ������Σ�Ϊ�˱��⻭�ľ����߶��������ȡ�ĸ��ţ�Size�������һ�㣩
			rRect = RotatedRect(Point2f(o1_x, o1_y), Size2f(135, 135), Theta1);

			rRect.points(vertices);   //���ĸ���洢��vertices������
			for (int i = 0; i < 4; i++)
			{
				//������
				line(imageCopy2, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0));
			}
			//��ȡROI

			rot_mat = getRotationMatrix2D(Point2f(o1_x, o1_y), Theta1, 1.0);//����ת����
			warpAffine(imageCopy2, rot_image, rot_mat, imageCopy2.size());//ԭͼ����ת
			result = rot_image(Rect(o1_x - 64, o1_y - 64, 128, 128));//��ȡROI
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
			result = imageCopy2(Rect(o1_x - 64, o1_y - 64, 128, 128));//��ȡROI
			
		}

		//string savefile = "D:\\������\\�ƾ���ʶ�𣨿����飩\\ROI��ȡ\\ͼ��\\CASIA\\ROI���\\0195_f_l_08.jpg";
		//DWORD end_time = GetTickCount();

		//cout << "�����������ʱ��Ϊ��" << (end_time - star_time) << "ms." << endl;

		//system("pause");
		imwrite(savefile, result);
	}

	
//*********SetRectImage��������********//



	return 0;



}