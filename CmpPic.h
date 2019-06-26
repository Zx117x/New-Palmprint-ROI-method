#ifndef _CmpPic_h_ 
#define _CmpPic_h_ 
#include<opencv2/opencv.hpp>
#include <iostream>
#include<vector>
#include<windows.h>
using namespace std;
using namespace cv;
double CmpPic(IplImage *Image1, IplImage *Image2);
Mat SetRectImage(Mat image);
int *FindM(Mat dst);
int *FindM2(Mat dst);
int *FindPeak(Mat image);
int FindLine(Mat image, int x1, int y1, int x2, int y2);
float LeastSquares(double x[], double y[]);
double Round(_In_ double dSrc, _In_ int iBit);
Mat SetImage(Mat image);
Mat CutImage(Mat image);
int *FindPeak1(Mat image);
int *FindM1(Mat dst, int *t);
Mat CutWrist(Mat image);
Mat CutWristImage1(Mat image);
#endif 