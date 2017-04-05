#include "Exp05.h"

#define IDEAL 0
#define BUTTERWORTH 1

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
                DFTandIDFT();
                break;
            case '2':
                FrequencyDomainFiltering(IdealFilteringCalc, IDEAL);
                break;
            case '3':
                FrequencyDomainFiltering(ButterworthFilterCalc, BUTTERWORTH);
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

int ShowSpectrum(Mat complexI, int centered)
{
    Mat planes[] = { Mat::zeros(complexI.size(), CV_32F), Mat::zeros(complexI.size(), CV_32F) };

    split(complexI, planes); // ����ͨ������complexI����ɼ�����ͨ�����飬planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude  
    Mat magnitudeImage = planes[0];

    //���ж����߶�(logarithmic scale)���ţ�������ת��Ϊ��ֵ����=> log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    magnitudeImage += Scalar::all(1);
    log(magnitudeImage, magnitudeImage);//����Ȼ����

    if (centered)//�����Ҫ���Ļ�
    {
        //���к��طֲ�����ͼ���ޡ����������л������У�����Ƶ�ײü�
        magnitudeImage = magnitudeImage(Rect(0, 0, magnitudeImage.cols & -2, magnitudeImage.rows & -2));
        //�������и���Ҷͼ���е����ޣ�ʹ��ԭ��λ��ͼ������  
        int cx = magnitudeImage.cols / 2;
        int cy = magnitudeImage.rows / 2;
        Mat q0(magnitudeImage, Rect(0, 0, cx, cy));// ROI���������
        Mat q1(magnitudeImage, Rect(cx, 0, cx, cy));// ROI���������
        Mat q2(magnitudeImage, Rect(0, cy, cx, cy));// ROI���������
        Mat q3(magnitudeImage, Rect(cx, cy, cx, cy));// ROI���������
        //�������ޣ����������½��н�����
        Mat tmp;
        q0.copyTo(tmp);
        q3.copyTo(q0);
        tmp.copyTo(q3);
        //�������ޣ����������½��н�����
        q1.copyTo(tmp);
        q2.copyTo(q1);
        tmp.copyTo(q2);
    }
    //��һ������0��1֮��ĸ���ֵ������任Ϊ���ӵ�ͼ���ʽ
    magnitudeImage = magnitudeImage(Rect(0, 0, gray.cols, gray.rows));
    //normalize(magnitudeImage, magnitudeImage, 0, 1, CV_MINMAX); //OpenCV2�����
    normalize(magnitudeImage, magnitudeImage, 0, 1, NORM_MINMAX); 

    //��ʾЧ��ͼ
    imshow("Ƶ�׷�ֵ���ѽ��ж����任��", magnitudeImage);
    //waitKey(0);
    //destroyAllWindows();
    return 0;
}

int DFTandIDFT()
{
    Mat src = gray;
    imshow("ԭͼ��", src);

    //src.convertTo(src, CV_32F);
    ////�� -1^(x+y)��ʹƵ����ͼ�����Ļ����������ɺ���ת����
    //for (int i = 0; i<src.rows; i++)
    //{
    //    float *p = src.ptr<float>(i);
    //    for (int j = 0; j<src.cols; j++)
    //    {
    //        p[j] = p[j] * pow(-1, i + j);
    //    }
    //}

    //������ͼ����������ѵĳߴ磬�߽���0����
    int m = getOptimalDFTSize(src.rows);
    int n = getOptimalDFTSize(src.cols);
    //����ӵ����س�ʼ��Ϊ0.
    Mat padded;
    copyMakeBorder(src, padded, 0, m - src.rows, 0, n - src.cols, BORDER_CONSTANT, Scalar::all(0));

    Mat complexI = Mat::zeros(padded.size(), CV_32F);

    //Ϊ����Ҷ�任�Ľ��(ʵ�����鲿)����洢�ռ䡣
    Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };

    ////������DFT_COMPLEX_OUTPUT�������ʡ����һ��
    ////��planes������Ϻϲ���һ����ͨ��������complexI
    //merge(planes, 2, complexI);
    //dft(complexI, complexI);//���о͵���ɢ����Ҷ�任

    padded.convertTo(padded, CV_32F);
    //����DFT_COMPLEX_OUTPUT��ǰ�治����ΪcomplexI��չһ��ͨ�������Զ���չ
    dft(padded, complexI, DFT_COMPLEX_OUTPUT);

    ShowSpectrum(complexI, 1);

    idft(complexI, complexI, DFT_SCALE | DFT_REAL_OUTPUT);//��DFT_SCALE������Ϊ����һ�½����ֵ

    split(complexI, planes);
    complexI = planes[0];

    Mat dst;
    complexI.convertTo(dst, CV_8U);
    namedWindow("Fourier���任��");
    imshow("Fourier���任��", dst);

    //cout << src(Rect(0, 0, 5, 5)) << endl;
    //cout << dst(Rect(0, 0, 5, 5)) << endl;

    waitKey(0);
    destroyAllWindows();

    return 0;
}

float IdealFilteringCalc(float d, float d0, int n)
{
    return d >= d0 ? 0 : 1;
}

float ButterworthFilterCalc(float d, float d0, int n)
{
    if (d0 > 0.0000001)
    {
        return (1. / (1. + pow(d / float(d0), 2 * n)));
        //return sqrt(1. / (1. + powf(d / d0, 2 * n)));
    }
    else
    {
        return 0;
    }
}

int FrequencyDomainFiltering(float FilteringCalc(float d, float d0, int n), int filterType)
{
    Mat src = gray;
    imshow("ԭͼ��", src);
    src.convertTo(src, CV_32F);
    //�� -1^(x+y)��ʹƵ����ͼ�����Ļ����������ɺ���ת����
    for (int i = 0; i < src.rows; i++)
    {
        float *p = src.ptr<float>(i);
        for (int j = 0; j < src.cols; j++)
        {
            p[j] = p[j] * pow(-1, i + j);
        }
    }

    //������ͼ����������ѵĳߴ磬�߽���0����
    int m = getOptimalDFTSize(src.rows);
    int n = getOptimalDFTSize(src.cols);
    //����ӵ����س�ʼ��Ϊ0.
    Mat padded;
    copyMakeBorder(src, padded, 0, m - src.rows, 0, n - src.cols, BORDER_CONSTANT, Scalar::all(0));

    Mat complexI = Mat::zeros(padded.size(), CV_32F);
    //Ϊ����Ҷ�任�Ľ��(ʵ�����鲿)����洢�ռ䡣
    Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
    padded.convertTo(padded, CV_32F);

    dft(padded, complexI, DFT_COMPLEX_OUTPUT);

    Point center;
    center.x = padded.cols / 2;
    center.y = padded.rows / 2;
    //Mat idealFilter[] = { Mat::zeros(padded.size(), CV_32F) ,Mat::zeros(padded.size(), CV_32F) };
    Mat idealFilter = Mat::zeros(padded.size(), CV_32FC2);
    float d0 = 1;//cutoff frequency
    int d0control = 0;
    int c = 0;
    int nButterworth = 1;
    Mat dst = Mat::zeros(padded.size(), CV_32FC2);

    namedWindow("���ڴ���", WINDOW_NORMAL);
    createTrackbar("��ֹƵ��x10", "���ڴ���", &d0control, int(sqrt(powf(padded.cols, 2) + powf(padded.rows, 2)) * 5));
    createTrackbar("n", "���ڴ���", &nButterworth, 10);
    createTrackbar("��/��ͨ", "���ڴ���", &c, 1);
    for (;;)
    {
        d0 = float(d0control) / 10;
        d0 = (2. / (sqrt(powf(padded.cols, 2) + powf(padded.rows, 2)))) * powf(d0, 2);

        for (int y = 0; y < src.rows; y++)
        {
            for (int x = 0; x < src.cols; x++)
            {
                //idealFilter.at<Vec2f>(y, x)[0] =
                //    sqrt(powf((y - center.y), 2) + powf((x - center.x), 2)) > d0 ? 0 : 1;
                idealFilter.at<Vec2f>(y, x)[0] =
                    FilteringCalc(sqrt(powf((y - center.y), 2) + powf((x - center.x), 2)), d0, nButterworth);
                if (c == 1)
                {
                    idealFilter.at<Vec2f>(y, x)[0] = 1 - idealFilter.at<Vec2f>(y, x)[0];
                }
                idealFilter.at<Vec2f>(y, x)[1] = idealFilter.at<Vec2f>(y, x)[0];

            }
        }

        dst = complexI.mul(idealFilter);
        ShowSpectrum(dst, 0);

        idft(dst, dst, DFT_SCALE | DFT_REAL_OUTPUT);
        split(dst, planes);
        dst = planes[0](Rect(0, 0, src.cols, src.rows));

        for (int i = 0; i < src.rows; i++)
        {
            float *p = dst.ptr<float>(i);
            for (int j = 0; j < src.cols; j++)
            {
                p[j] = p[j] * pow(-1, i + j);
            }
        }

        dst.convertTo(dst, CV_8U);
        imshow("�˲���ͼ��", dst);


        if (waitKey(10) != 255)
        {
            destroyAllWindows();
            break;
        }
    }


    //waitKey(0);
    //destroyAllWindows();
    return 0;
}
