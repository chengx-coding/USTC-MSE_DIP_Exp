#include "Exp01.h"

using namespace cv;
using namespace std;

int help()
{
    cout << "Exp01 --- SA16225037 ����" << endl;
    cout<< "1 : ��ʾ��һ��ֱ��ͼ\n"
        << "2 : �Ҷ�ͼ��ֱ��ͼ���⴦��\n"
        << "3 : ��ɫͼ��ֱ��ͼ���⴦��\n" << endl;
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
        cout << "��ѡ��Ҫ���еĳ��򣬰�h��������q�˳���";
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
                cout << "��Ч������" << endl;
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