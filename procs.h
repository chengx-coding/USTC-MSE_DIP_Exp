#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp> 
#include <iostream>
#include <stdlib.h>
#include <math.h>

#include "Exp01.h"
#include "Exp02.h"
#include "Exp03.h"
#include "Exp04.h"
#include "Exp05.h"

using namespace cv;
using namespace std;

int ProcsHelp();
int ProcsMain(string imagePath);

double FindMax(double *p, int n);
double FindMin(double *p, int n);

int ConvertBGR2HSI(Mat src, Mat dst);
int ConvertHSI2BGR(Mat src, Mat dst);
int test01();
int ShowDifference();
