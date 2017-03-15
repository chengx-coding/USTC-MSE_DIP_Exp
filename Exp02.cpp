#include "Exp01.h"

using namespace cv;
using namespace std;

int help()
{
    cout << "Exp01 --- SA16225037 程欣" << endl;
    cout<< "1 : 显示归一化直方图\n"
        << "2 : 灰度图像直方图均衡处理\n"
        << "3 : 彩色图像直方图均衡处理\n" << endl;
    return 0;
}


Mat image;
Mat gray;
int Exp02Main(char *imagePath)
{
    char chioce;
    image = imread(imagePath);
    //gray = imread(imagePath, IMREAD_GRAYSCALE);
    cvtColor(image, gray, CV_BGR2GRAY);
    //pimage = cvLoadImage("test.jpg");
    if (image.empty())
    {
        cout << "Could not open or find the image" << std::endl;
        return -1;
    }

    help();
    while (1)
    {
        cout << "清选择要运行的程序，按h帮助，按q退出：";
        cin >> chioce;
        if (chioce == 'q')
        {
            exit(0);
        }
        else
        {
            switch (chioce)
            {
            case '1':
                ShowImg();
                break;
            case '2':
                
                break;
            case '3':
                
                break;
            case 'h':
                help();
                break;
            default:
                cout << "无效的输入" << endl;
                break;
            }
        }
    }
    return 0;
}

int CalcNormalizedHistogram()
{

    return 0;
}

int GrayHistogramEqualization()
{

    return 0;
}

int RGBHistogramEqulization()
{

    return 0;
}