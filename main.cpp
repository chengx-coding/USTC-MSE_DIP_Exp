#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp> 
#include <iostream>

#include "Exp01.h"
#include "Exp02.h"

using namespace cv;
using namespace std;

Mat image;
Mat gray;

int main(int argc, char** argv)
{
    cout << "*** Using opencv-3.2.0-vc14 in Microsoft Visio Studio 2015 ***\n" << endl;
    char *imagePath = "test.jpg";
    //Exp01Main(imagePath);
    Exp02Main(imagePath);
    return 0;
}
