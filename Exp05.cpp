#include "Exp05.h"

int Exp05Help()
{
    cout << "\nExp05 --- SA16225037 ����\n" << endl;
    cout <<
        "1 : �Ҷ�ͼ���DFT��IDFT\n" <<
        "2 : ���������ͨ�������ͨ�˲����ԻҶ�ͼ�����Ƶ�����˲�\n" <<
        "3 : ���ò�����˹��ͨ�͵�ͨ�˲����ԻҶ�ͼ�����Ƶ�����˲�\n" << endl;
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
        cout << "��ѡ��Ҫ���еĳ��򣬰�h��������w������һ������q�˳���";
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
                cout << "��Ч������" << endl;
                break;
            }
        }
    }
    return 0;
}