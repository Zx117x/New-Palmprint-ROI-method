#include<opencv2/opencv.hpp>
#include <iostream>
#include "CmpPic.h"
using namespace std;
using namespace cv;


double CmpPic(IplImage *Image1, IplImage *Image2)
{
	//创建直方图 
	int HistogramBins = 256;
	float HistogramRange1[2] = { 0, 255 };
	float *HistogramRange[1] = { &HistogramRange1[0] };
	CvHistogram *Histogram1 = cvCreateHist(1, &HistogramBins, CV_HIST_SPARSE, HistogramRange);
	CvHistogram *Histogram2 = cvCreateHist(1, &HistogramBins, CV_HIST_SPARSE, HistogramRange);
	//计算直方图 
	cvCalcHist(&Image1, Histogram1);
	cvCalcHist(&Image1, Histogram2);
	//归一化直方图 
	cvNormalizeHist(Histogram1, 1);
	cvNormalizeHist(Histogram2, 1);
	return cvCompareHist(Histogram1, Histogram2, CV_COMP_CORREL);
}