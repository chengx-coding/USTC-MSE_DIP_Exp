#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp> 
#include <iostream>
#include <stdlib.h>

using namespace cv;
using namespace std;

int Exp02Help();
int Exp02Main(char *imagePath);

int CalcNormalizedHistogram(Mat img, Mat histImg, int histHight, int *pmax, int *hist, Scalar color);
int ShowNormalizedHistogram();
int GrayHistogramEqualization();
int RGBHistogramEqulization();