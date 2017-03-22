#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp> 
#include <iostream>
#include <stdlib.h>

using namespace cv;
using namespace std;

int Exp01Help();
int Exp01Main(string imagePath);

int ShowImg();
int LinearTransProcessing(Mat src, Mat dst, int minValue, int maxValue);
int LinearTrans();
int Binarization();
int LogTrans();
int GamaTrans();
int ComplementaryColorTrans();
int HueInvertColorTrans();

int CapThreshold();
