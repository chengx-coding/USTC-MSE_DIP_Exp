#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp> 
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace cv;
using namespace std;

int ProcsHelp();
int ProcsMain(string imagePath);

double FindMax(double *p, int n);
double FindMin(double *p, int n);

int ConvertBGR2HSI(Mat src, Mat dst);
int ConvertHSI2BGR(Mat src, Mat dst);
int test01();