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

string inputPath()
{
    string imagePath;
    while (imagePath == "")
    {
        cout <<
            "1 - 默认图片lena.tif\n" <<
            "2 - 默认图片moon.jpg\n" <<
            "3 - 自定义图片路径\n" << endl;
        cout << "请选择：";
        char pathNum;
        cin >> pathNum;
        cin.ignore(CHAR_MAX, '\n');
        switch (pathNum)
        {
        case '1':
            imagePath = "lena.tif";
            break;
        case '2':
            imagePath = "moon.jpg";
            break;
        case '3':
            cout << "请输入图片路径：";
            cin >> imagePath;
            break;
        case 'q':
            exit(0);
        default:
            cout << "无效的输入" << endl;
            break;
        }
        if (imread(imagePath).empty())
        {
            cout << "该路径找不到图片！" << endl;
            imagePath = "";
        }
    }
    return imagePath;
}

Mat image;
Mat gray;
Mat hsv;

int main(int argc, char** argv)
{
    cout << "*** Using opencv-3.2.0-vc14 in Microsoft Visio Studio 2015 ***\n" << endl;
    
    string imagePath = "";
    imagePath = inputPath();

    char choice;
    while (1)
    {
        Mainhelp();

        cout << "清选择要运行的程序，按q退出，按r重选图片：";
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
            case 'r':
                imagePath = inputPath();
                break;
            default:
                cout << "无效的输入" << endl;
                break;
            }
        }
    }
    return 0;
}