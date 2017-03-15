#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp> 
#include <iostream>
#include <stdlib.h>

using namespace cv;
using namespace std;

int help();
int Exp01Main(char *imagePath);

int ShowImg();
int Binarization();
int LogTrans();
int GamaTrans();
int ComplementaryColorTrans();

int CapThreshold();