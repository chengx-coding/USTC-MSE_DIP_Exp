#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp> 
#include <iostream>
#include <stdlib.h>

using namespace cv;
using namespace std;

int Exp05Help();
int Exp05Main(string imagePath);

int ShowSpectrum(Mat complexI, int centered);
int DFTandIDFT();

float IdealFilteringCalc(float d, float d0, int n);
float ButterworthFilterCalc(float d, float d0, int n);
int FrequencyDomainFiltering(float FilteringCalc(float d, float d0, int n), int filterType);



