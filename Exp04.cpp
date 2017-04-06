#include "Exp04.h"
#include "Exp03.h"
#include "Exp02.h"

#define M_SALT 1
#define M_PEPPER 2
#define M_SALTPEPPER 3
#define M_GAUSSIAN 4
#define M_PI 3.14159265358979323846

int Exp04Help()
{
    cout << "\nExp04 --- SA16225037 程欣\n" << endl;
    cout <<
        "1 : 均值滤波\n" <<
        "2 : 中值滤波\n" <<
        "3 : 自适应均值滤波\n" <<
        "4 : 自适应中值滤波\n" <<
        "5 : 彩色图像均值滤波\n" << endl;
    return 0;
}

extern Mat image;
//IplImage *pimage;
extern Mat gray;
//IplImage *pgray;
extern Mat hsv;
int Exp04Main(string imagePath)
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
    Mat result = Mat::ones(gray.size(), gray.type());

    Exp04Help();
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
                MeanFiltering(gray, result);
                break;
            case '2':
                MedianFiltering(gray, result);
                break;
            case '3':
                break;
            case '4':
                break;
            case '5':
                break;
            case 'h':
                Exp04Help();
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

int AddNoise(Mat img, int n, int type, double *pParam)//pParam[0] is the central gray value, pParam[1] is std_deviation
{
    int i, j;
    int c = 0;
    double rand1, rand2;
    for (int k = 0; k < n; k++)
    {
        i = rand() % img.rows;
        j = rand() % img.cols;
        switch (type)
        {
        case M_SALT:
            c = 255;
            break;
        case M_PEPPER:
            c = 0;
            break;
        case M_SALTPEPPER:
            c = (rand() % 2) * 255;
            break;
        case M_GAUSSIAN://通过Box-Muller变换可以产生Gaussian噪声
            c = -1;
            while (c < 0 || c>255)
            {
                rand1 = rand() / ((double)RAND_MAX);
                if (rand1 < 1e-100)
                {
                    rand1 = 1e-100;
                }
                rand1 = -2 * log(rand1);

                rand2 = rand() / ((double)RAND_MAX);
                if (rand2 < 1e-100)
                {
                    rand2 = 1e-100;
                }
                rand2 = rand2 * 2 * M_PI;
                c = pParam[0] + (sqrt(rand1) * cos(rand2))*pParam[1];
            }
            break;
        default:
            break;
        }
        if (type)
        {
            if (img.type() == CV_8UC1)
            {
                img.at<uchar>(i, j) = c;
            }
            else if (img.type() == CV_8UC3)
            {
                img.at<Vec3b>(i, j)[0] = c;
                img.at<Vec3b>(i, j)[1] = c;
                img.at<Vec3b>(i, j)[2] = c;
            }
            else
            {
                cout << "image type error!" << endl;
                return -1;
            }
        }
    }
    return 0;
}

double ArithmeticMeanFilterCalc(Mat filterArea, Mat Filter)
{
    double result = 0;
    for (int y = 0; y < filterArea.rows; y++)
    {
        for (int x = 0; x < filterArea.cols; x++)
        {
            result += (double(filterArea.at<uchar>(y, x)));
        }
    }

    return result / double(filterArea.total());
}

double GeometricMeanFilterCalc(Mat filterArea, Mat Filter)
{
    double result = 1;
    for (int y = 0; y < filterArea.rows; y++)
    {
        for (int x = 0; x < filterArea.cols; x++)
        {
            result *= (double(filterArea.at<uchar>(y, x)));
        }
    }

    return pow(result, 1. / double(filterArea.total()));
}

double HarmonicMeanFilterCalc(Mat filterArea, Mat Filter)
{
    double result = 0;
    for (int y = 0; y < filterArea.rows; y++)
    {
        for (int x = 0; x < filterArea.cols; x++)
        {
            result += 1. / (double(filterArea.at<uchar>(y, x)));
        }
    }

    return double(filterArea.total()) / result;
}

double ComtraharmonicMeanFilterCalc(Mat filterArea, Mat Filter)
{
    double result[2] = { 0, 0 };
    int q = Filter.at<double>(0, 0);
    for (int y = 0; y < filterArea.rows; y++)
    {
        for (int x = 0; x < filterArea.cols; x++)
        {
            result[0] += pow(double(filterArea.at<uchar>(y, x)), q + 1);
            result[1] += pow(double(filterArea.at<uchar>(y, x)), q);
        }
    }

    return result[0] / result[1];
}


double MedianFilterCalc(Mat filterArea, Mat Filter)
{
    double result = 0;
    for (int y = 0; y < filterArea.rows; y++)
    {
        for (int x = 0; x < filterArea.cols; x++)
        {
            result += (double(filterArea.at<uchar>(y, x)));
        }
    }

    return result;
}

int trackbarChange = 0;
int noiseChange = 0;
static void onTrackbar(int, void*)
{
    trackbarChange = 1;
}
static void onTrackbarNoise(int, void*)
{
    noiseChange = 1;
    trackbarChange = 1;
}

int MeanFiltering(Mat src, Mat dst)
{
    Mat MeanFilteringImg = Mat::zeros(src.size(), src.type());
    gray.copyTo(MeanFilteringImg);
    Mat filter = Mat::zeros(5, 5, CV_64F);

    double gaussianParam[2];
    gaussianParam[0] = 128;
    gaussianParam[1] = 32;
    int noiseNum = 0;
    int noiseType = 0;
    int filterType = 0;
    int q = 4;//Q=-1(q=4)时，为谐波滤波；Q=0(q=5)时，为算数均值滤波

    namedWindow("调节窗口", WINDOW_NORMAL);
    createTrackbar("噪声数量", "调节窗口", &noiseNum, src.total(), onTrackbarNoise);
    createTrackbar("噪声类型", "调节窗口", &noiseType, 4, onTrackbarNoise);
    createTrackbar("滤波方式", "调节窗口", &filterType, 4, onTrackbar);
    createTrackbar("逆谐波Q", "调节窗口", &q, 10, onTrackbar);
    namedWindow("Noise Image", WINDOW_AUTOSIZE);
    imshow("Noise Image", MeanFilteringImg);
    namedWindow("Filtered Image", WINDOW_AUTOSIZE);
    imshow("Filtered Image", dst);
    for (;;)
    {
        if (noiseChange)
        {
            gray.copyTo(MeanFilteringImg);
            AddNoise(MeanFilteringImg, noiseNum, noiseType, gaussianParam);
            noiseChange = 0;
            namedWindow("Noise Image", WINDOW_AUTOSIZE);
            imshow("Noise Image", MeanFilteringImg);
        }
        if (trackbarChange)
        {
            switch (filterType)
            {
            case 1:
                cout << "谐波均值滤波" << endl;
                FilterProcessing(MeanFilteringImg, dst, filter, HarmonicMeanFilterCalc);
                break;
            case 2:
                filter.at<double>(0, 0) = q - 5;
                cout << "逆谐波均值滤波" << " Q=" << q - 5;
                if (q - 5 == -1)
                {
                    cout << " 此时等价于谐波均值滤波" << endl;
                }
                else if (q - 5 == 0)
                {
                    cout << " 此时等价于算数均值滤波" << endl;
                }
                else
                {
                    cout << endl;
                }
                FilterProcessing(MeanFilteringImg, dst, filter, ComtraharmonicMeanFilterCalc);
                break;
            case 3:
                cout << "算数均值滤波" << endl;
                FilterProcessing(MeanFilteringImg, dst, filter, ArithmeticMeanFilterCalc);
                break;
            case 4:
                cout << "几何均值滤波" << endl;
                FilterProcessing(MeanFilteringImg, dst, filter, GeometricMeanFilterCalc);
                break;
            default:
                break;
            }
            namedWindow("Filtered Image", WINDOW_AUTOSIZE);
            imshow("Filtered Image", dst);
            trackbarChange = 0;
        }

        if (waitKey(10) != 255)
        {
            destroyAllWindows();
            break;
        }
    }

    //int hist[256];
    //memset(hist, 0, 256 * sizeof(int));
    //int histHeight = 256;
    //int max;
    //int *pmax = &max;
    //Mat histImg = Mat::zeros(histHeight, 256, CV_8U);
    //CalcNormalizedHistogram(MeanFilteringImg, histImg, histHeight, pmax, hist, Scalar(255));
    //namedWindow("MeanFiltering hist", WINDOW_AUTOSIZE);
    //imshow("MeanFiltering hist", histImg);

    //waitKey(0);
    //destroyAllWindows();
    return 0;
}

int MedianFiltering(Mat src, Mat dst)
{
    //cout << cvRound(-0.5) << endl;
    //cout << cvRound(-0.49) << endl;
    //cout << cvRound(-0.51) << endl;

    //cout << cvRound(0.5) << endl;
    //cout << cvRound(0.49) << endl;
    //cout << cvRound(0.51) << endl;

    //cout << cvRound(-1.5) << endl;
    //cout << cvRound(-1.49) << endl;
    //cout << cvRound(-1.51) << endl;

    //cout << cvRound(-2.5) << endl;
    //cout << cvRound(-2.49) << endl;
    //cout << cvRound(-2.51) << endl;

    //cout << cvRound(2.5) << endl;
    //cout << cvRound(2.49) << endl;
    //cout << cvRound(2.51) << endl;

    //cout << cvRound(3.5) << endl;
    //cout << cvRound(3.49) << endl;
    //cout << cvRound(3.51) << endl;
    Mat bgr = image.clone();
    Mat hsv;
    for (int i = 0; i < 10000; i++)
    {
        cvtColor(bgr, hsv, CV_BGR2HSV);
        cvtColor(hsv, bgr, CV_HSV2BGR);
    }

    namedWindow("bgr");
    namedWindow("hsv");
    imshow("bgr", image);
    imshow("hsv", bgr);

    waitKey(0);


    return 0;
}

int AdaptiveMeanFiltering(Mat src, Mat dst)
{
    //cv::meanStdDev(img, mean, stddev);

    return 0;
}

int AdaptiveMedianFiltering(Mat src, Mat dst)
{

    return 0;
}
