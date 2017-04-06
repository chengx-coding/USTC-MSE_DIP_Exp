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

int AddNoise(Mat img, int n, int type, double *pParam);

double ArithmeticMeanFilterCalc(Mat filterArea, Mat Filter);
double GeometricMeanFilterCalc(Mat filterArea, Mat Filter);
double HarmonicMeanFilterCalc(Mat filterArea, Mat Filter);
double ComtraharmonicMeanFilterCalc(Mat filterArea, Mat Filter);
double MedianFilterCalc(Mat filterArea, Mat Filter);

static void onTrackbar(int, void*);
static void onTrackbarNoise(int, void*);

int MeanFiltering(Mat src, Mat dst);
int MedianFiltering(Mat src, Mat dst);
int AdaptiveMeanFiltering(Mat src, Mat dst);
int AdaptiveMedianFiltering(Mat src, Mat dst);
