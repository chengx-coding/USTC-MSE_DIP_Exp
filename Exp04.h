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

int MeanFiltering();


int MedianFiltering();
