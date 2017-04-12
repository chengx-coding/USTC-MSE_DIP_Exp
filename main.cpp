#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp> 
#include <iostream>

#include "Exp01.h"
#include "Exp02.h"
#include "Exp03.h"
#include "Exp04.h"
#include "Exp05.h"
#include "procs.h"

using namespace cv;
using namespace std;

int Mainhelp()
{
    cout << "* Index *\n" << endl;
    cout <<
        "1 - �Ҷȱ任\n" <<
        "2 - ֱ��ͼ����\n" <<
        "3 - �����˲�\n" <<
        "4 - ͼ��ȥ��\n" <<
        "5 - Ƶ���˲�\n" <<
        "6 - ��������\n" << endl;

    return 0;
}

string inputPath()
{
    string imagePath;
    while (imagePath == "")
    {
        cout <<
            "0 - �Զ���ͼƬ·��\n" << 
            "1 - Ĭ��ͼƬlena.tif\n" <<
            "2 - Ĭ��ͼƬmoon.tif\n" << 
            "3 - Ĭ��ͼƬcharacters.tif\n" << 
            "4 - Ĭ��ͼƬckt.tif\n" << endl;
        cout << "��ѡ��";
        char pathNum;
        cin >> pathNum;
        cin.ignore(CHAR_MAX, '\n');
        switch (pathNum)
        {
        case '1':
            imagePath = "lena.tif";
            break;
        case '2':
            imagePath = "moon.tif";
            break;
        case '3':
            imagePath = "characters.tif";
            break;
        case '4':
            imagePath = "ckt.tif";
            break;
        case '0':
            cout << "������ͼƬ·����";
            cin >> imagePath;
            break;
        case 'q':
            exit(0);
        default:
            cout << "��Ч������" << endl;
            break;
        }
        if (imread(imagePath).empty())
        {
            cout << "��·���Ҳ���ͼƬ��" << endl;
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

        cout << "��ѡ��Ҫ���еĳ��򣬰�q�˳�����r��ѡͼƬ��";
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
            case '4':
                Exp04Main(imagePath);
                break;
            case '5':
                Exp05Main(imagePath);
                break;
            case '6':
                ProcsMain(imagePath);
                break;
            case 'r':
                imagePath = inputPath();
                break;
            default:
                cout << "��Ч������" << endl;
                break;
            }
        }
    }
    return 0;
}