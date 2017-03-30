#include "Exp05.h"

int Exp05Help()
{
    cout << "\nExp05 --- SA16225037 程欣\n" << endl;
    cout <<
        "1 : 灰度图像的DFT和IDFT\n" <<
        "2 : 利用理想高通和理想低通滤波器对灰度图像进行频率域滤波\n" <<
        "3 : 利用布特沃斯高通和低通滤波器对灰度图像进行频率域滤波\n" << endl;
    return 0;
}

extern Mat image;
//IplImage *pimage;
extern Mat gray;
//IplImage *pgray;
extern Mat hsv;
int Exp05Main(string imagePath)
{
    image = imread(imagePath);
    //gray = imread(imagePath, IMREAD_GRAYSCALE);
    cvtColor(image, gray, CV_BGR2GRAY);
    cvtColor(image, hsv, CV_BGR2HSV);
    //pimage = cvLoadImage("test.jpg");
    if (image.empty())
    {
        cout << "Could not open or find the image" << std::endl;
        return -1;
    }

    Exp05Help();
    char choice;
    while (1)
    {
        cout << "清选择要运行的程序，按h帮助，按w返回上一级，按q退出：";
        cin >> choice;
        cin.ignore(CHAR_MAX, '\n');
        if (choice == 'q')
        {
            exit(0);
        }
        else
        {
            switch (choice)
            {
            case '1':
                break;
            case '2':
                break;
            case '3':
                break;
            case 'h':
                Exp05Help();
                break;
            case 'w':
                return 0;
            default:
                cout << "无效的输入" << endl;
                break;
            }
        }
    }
    return 0;
}