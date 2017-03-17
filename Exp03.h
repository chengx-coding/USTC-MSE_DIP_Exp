#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp> 
#include <iostream>
#include <stdlib.h>

using namespace cv;
using namespace std;

int Exp03Help();
int Exp03Main(char *imagePath);

double LinearFilterCalc(Mat filterArea, Mat linearFilter);
int FilterProcessing(Mat src, Mat dst, Mat filter, double ProcessingMethod(Mat filterArea, Mat filter));

int MeanFilterProcessing();
int RGBMeanFilterProcessing();

int GaussianFilterGenerator(Mat gaussianFilter, int centerValue, double variance);
int GaussianFilterProcessing();
int RGBGaussianFilterProcessing();
