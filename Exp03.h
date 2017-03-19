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
int FilterProcessing(
    Mat src, Mat dst, Mat filter, 
    double ProcessingMethod(Mat filterArea, Mat filter));
int BGRFilterProcessing(
    Mat src, Mat BGRFilterImg, Mat filter,
    Mat BGR_bChannel, Mat BGR_gChannel, Mat BGR_rChannel,
    double ProcessingMethod(Mat filterArea, Mat filter));

int ShowBGRChannels(
    Mat BGR_bChannel, Mat BGR_gChannel, Mat BGR_rChannel,
    string title);

int MeanFilterProcessing();
int BGRMeanFilterProcessing();

int GaussianFilterGenerator(Mat gaussianFilter, int centerValue, double variance);
int GaussianFilterProcessing();
int BGRGaussianFilterProcessing();

int SharpenFilterProcessing();

int LaplacianFilterProcessing(Mat src, Mat dst, Mat laplacianFilter, Mat laplacianFilterImg, double c);
int LaplacianSharpen(Mat src, Mat dst, string title, double c, int filterNum);
int RobertSharpen(Mat src, Mat dst, string title, double c);
int SobelSharpen(Mat src, Mat dst, string title, double c);

int HighboostFilterProcessing();
int BGRSharpenFilterProcessing();


