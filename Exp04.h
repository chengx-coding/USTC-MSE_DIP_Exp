#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp> 
#include <iostream>
#include <stdlib.h>

using namespace cv;
using namespace std;

int Exp04Help();
int Exp04Main(string imagePath);

//int AddNoise(Mat img, int n, int type, double *pParam);
//template <typename _Tp>
//int mem_cmp(const void *a, const void *b);
//template <typename _Tp>
//_Tp medianElem(Mat img);
int mem_cmp(const void *a, const void *b);
int medianElem(Mat img);

double ArithmeticMeanFilterCalc(Mat filterArea, Mat Filter);
double GeometricMeanFilterCalc(Mat filterArea, Mat Filter);
double HarmonicMeanFilterCalc(Mat filterArea, Mat Filter);
double ComtraharmonicMeanFilterCalc(Mat filterArea, Mat Filter);
double MedianFilterCalc(Mat filterArea, Mat Filter);
double AdaptiveMeanFilterCalc(Mat filterArea, Mat Filter);
double StandardDeviationCalc(Mat filterArea, Mat Filter);

static void onTrackbar(int, void*);
static void onTrackbarNoise(int, void*);
static void onTrackbarFilter(int, void*);

int NoiseFiltering(Mat src, Mat dst);

int AdaptiveMedianFilterCalc(Mat filterArea, int s_max);
int AdaptiveMedianFiltering(Mat src, Mat dst, Mat filter);
