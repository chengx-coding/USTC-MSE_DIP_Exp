#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp> 
#include <iostream>

#include "Exp01.h"
#include "Exp02.h"
#include "Exp03.h"

using namespace cv;
using namespace std;

int Mainhelp()
{
    cout << "* Index *\n" << endl;
    cout <<
        "1 - 灰度变换\n" <<
        "2 - 直方图均衡\n" << 
        "3 - 空域滤波\n" << endl;
    return 0;
}

Mat image;
Mat gray;
Mat hsv;

int main(int argc, char** argv)
{
    cout << "*** Using opencv-3.2.0-vc14 in Microsoft Visio Studio 2015 ***\n" << endl;
    char *imagePath = "moon.jpg";
    char choice;
    while (1)
    {
        Mainhelp();

        cout << "清选择要运行的程序，按q退出：";
        cin >> choice;
        if (choice == 'q')
        {
            exit(0);
        }
        else
        {
            switch (choice)
            {
            case '1':
                Exp01Main(imagePath);
                break;
            case '2':
                Exp02Main(imagePath);
                break;
            case '3':
                Exp03Main(imagePath);
                break;
            default:
                cout << "无效的输入" << endl;
                break;
            }
        }
    }
    //Exp01Main(imagePath);
    //Exp02Main(imagePath);
    //Exp03Main(imagePath);
    return 0;
}
