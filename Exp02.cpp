#include "Exp02.h"

using namespace cv;
using namespace std;

int Exp02Help()
{
    cout << "Exp01 --- SA16225037 程欣" << endl;
    cout<< "1 : 显示归一化直方图\n"
        << "2 : 灰度图像直方图均衡处理\n"
        << "3 : 彩色图像直方图均衡处理\n" << endl;
    return 0;
}


extern Mat image;
extern Mat gray;
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

    Exp02Help();
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
                ShowNormalizedHistogram();
                break;
            case '2':
                GrayHistogramEqualization();
                break;
            case '3':
                RGBHistogramEqulization();
                break;
            case 'h':
                Exp02Help();
                break;
            default:
                cout << "无效的输入" << endl;
                break;
            }
        }
    }
    return 0;
}

int CalcNormalizedHistogram(Mat img, Mat histImg, int histHight, int *pmax, int *hist, Scalar color)
{
    Mat lutTable = Mat::zeros(1, 256, CV_8U);
    LUT(histImg, lutTable, histImg);
    int max = 0;
    for (int y = 0; y < img.rows; y++)
    {
        for (int x = 0; x < img.cols; x++)
        {
            hist[(int)img.at<uchar>(y, x)]++;
            max = hist[(int)img.at<uchar>(y, x)] > max ? hist[(int)img.at<uchar>(y, x)] : max;
        }
    }
    *pmax = max;
    //minMaxLoc(img, &min, &max, 0, 0);
    int binValue;
    for (int i=0; i < 256; ++i)
    {
        binValue = hist[i] * histHight / max;
        rectangle(histImg, Point(i, histHight), Point(i + 1, histHight - binValue), color);
    }
    
    return *hist;
}

int ShowNormalizedHistogram()
{
    int hist[256];
    memset(hist, 0, 256 * sizeof(int));
    int histHight = 256;
    int max;
    int *pmax = &max;
    Mat histImg = Mat::zeros(histHight, 256, CV_8U);
    //Mat histImg(histHight, 256, CV_8U, Scalar(0));

    CalcNormalizedHistogram(gray, histImg, histHight, pmax, hist, Scalar(255));

    namedWindow("Gray - 灰度图像", WINDOW_AUTOSIZE);
    imshow("Gray - 灰度图像", gray);
    namedWindow("Histogram - 直方图", WINDOW_AUTOSIZE);
    imshow("Histogram - 直方图", histImg);
    waitKey(0);
    destroyAllWindows();
    return 0;
}

int GrayHistogramEqualization()
{
    int hist[256];
    memset(hist, 0, 256 * sizeof(int));
    int histHight = 256;
    int max;
    int *pmax = &max;
    Mat histImg = Mat::zeros(histHight, 256, CV_8U);

    CalcNormalizedHistogram(gray, histImg, histHight, pmax, hist, Scalar(255));

    Mat histequ_image = Mat::zeros(gray.size(), gray.type());
    double table[256];
    table[0] = (double)255 * hist[0] / (double)(gray.rows*gray.cols);
    for (int i = 1; i < 256; i++)
    {
        table[i] = ((double)255 * hist[i] / (double)(gray.rows*gray.cols)) + table[i - 1];
    }

    for (int y = 0; y < gray.rows; y++)
    {
        for (int x = 0; x < gray.cols; x++)
        {
            histequ_image.at<uchar>(y, x) = saturate_cast<uchar>(table[gray.at<uchar>(y, x)]);
        }
    }

    namedWindow("Gray - 灰度图像", WINDOW_AUTOSIZE);
    imshow("Gray - 灰度图像", gray);
    namedWindow("Histogram - 直方图", WINDOW_AUTOSIZE);
    imshow("Histogram - 直方图", histImg);
    namedWindow("GrayHistogramEqualization - 直方图均衡后灰度图像", WINDOW_AUTOSIZE);
    imshow("GrayHistogramEqualization - 直方图均衡后灰度图像", histequ_image);

    CalcNormalizedHistogram(histequ_image, histImg, histHight, pmax, hist, Scalar(255));

    namedWindow("直方图均衡后灰度图像直方图", WINDOW_AUTOSIZE);
    imshow("直方图均衡后灰度图像直方图", histImg);

    Mat cv_histequ_image = Mat::zeros(gray.size(), gray.type());
    equalizeHist(gray, cv_histequ_image);
    namedWindow("调用OpenCV直方图均衡化API得到的结果", WINDOW_AUTOSIZE);
    imshow("调用OpenCV直方图均衡化API得到的结果", cv_histequ_image);

    waitKey(0);
    destroyAllWindows();
    return 0;
}

int RGBHistogramEqulization()
{
    vector<Mat> BGRchannels;
    split(image, BGRchannels);
    Mat blue, green, red;
    blue = BGRchannels.at(0);
    green = BGRchannels.at(1);
    red = BGRchannels.at(2);

    int b_hist[256], g_hist[256], r_hist[256];
    memset(b_hist, 0, 256 * sizeof(int));
    memset(g_hist, 0, 256 * sizeof(int));
    memset(r_hist, 0, 256 * sizeof(int));
    int histHight = 256;
    int b_max, g_max, r_max;
    int *b_pmax = &b_max, *g_pmax = &g_max, *r_pmax = &r_max;
    Mat b_histImg = Mat::zeros(histHight, 256, CV_8UC3);
    Mat g_histImg = Mat::zeros(histHight, 256, CV_8UC3);
    Mat r_histImg = Mat::zeros(histHight, 256, CV_8UC3);

    CalcNormalizedHistogram(blue, b_histImg, histHight, b_pmax, b_hist, Scalar(255, 0, 0));
    CalcNormalizedHistogram(green, g_histImg, histHight, g_pmax, g_hist, Scalar(0, 255, 0));
    CalcNormalizedHistogram(red, r_histImg, histHight, r_pmax, r_hist, Scalar(0, 0, 255));

    Mat b_histequ_image = Mat::zeros(gray.size(), gray.type());
    Mat g_histequ_image = Mat::zeros(gray.size(), gray.type());
    Mat r_histequ_image = Mat::zeros(gray.size(), gray.type());
    double b_table[256], g_table[256], r_table[256];
    b_table[0] = (double)255 * b_hist[0] / (double)(image.rows*image.cols);
    g_table[0] = (double)255 * g_hist[0] / (double)(image.rows*image.cols);
    r_table[0] = (double)255 * r_hist[0] / (double)(image.rows*image.cols);
    for (int i = 1; i < 256; i++)
    {
        b_table[i] = ((double)255 * b_hist[i] / (double)(image.rows*image.cols)) + b_table[i - 1];
        g_table[i] = ((double)255 * g_hist[i] / (double)(image.rows*image.cols)) + g_table[i - 1];
        r_table[i] = ((double)255 * r_hist[i] / (double)(image.rows*image.cols)) + r_table[i - 1];
    }

    for (int y = 0; y < image.rows; y++)
    {
        for (int x = 0; x < image.cols; x++)
        {
            b_histequ_image.at<uchar>(y, x) = saturate_cast<uchar>(b_table[blue.at<uchar>(y, x)]);
            g_histequ_image.at<uchar>(y, x) = saturate_cast<uchar>(g_table[green.at<uchar>(y, x)]);
            r_histequ_image.at<uchar>(y, x) = saturate_cast<uchar>(r_table[red.at<uchar>(y, x)]);
        }
    }

    BGRchannels.at(0) = b_histequ_image;
    BGRchannels.at(1) = g_histequ_image;
    BGRchannels.at(2) = r_histequ_image;
    Mat BGRhistequ_image;
    merge(BGRchannels, BGRhistequ_image);

    namedWindow("Blue", WINDOW_AUTOSIZE);
    namedWindow("Green", WINDOW_AUTOSIZE);
    namedWindow("Red", WINDOW_AUTOSIZE);
    imshow("Green", green);
    imshow("Blue", blue);
    imshow("Red", red);

    namedWindow("B channel histogram", WINDOW_AUTOSIZE);
    namedWindow("G channel histogram", WINDOW_AUTOSIZE);
    namedWindow("R channel histogram", WINDOW_AUTOSIZE);
    imshow("B channel histogram", b_histImg);
    imshow("G channel histogram", g_histImg);
    imshow("R channel histogram", r_histImg);

    waitKey(0);

    CalcNormalizedHistogram(b_histequ_image, b_histImg, histHight, b_pmax, b_hist, Scalar(255, 0, 0));
    CalcNormalizedHistogram(g_histequ_image, g_histImg, histHight, g_pmax, g_hist, Scalar(0, 255, 0));
    CalcNormalizedHistogram(r_histequ_image, r_histImg, histHight, r_pmax, r_hist, Scalar(0, 0, 255));

    imshow("Blue", b_histequ_image);
    imshow("Green", g_histequ_image);
    imshow("Red", r_histequ_image);

    imshow("B channel histogram", b_histImg);
    imshow("G channel histogram", g_histImg);
    imshow("R channel histogram", r_histImg);

    waitKey(0);
    destroyAllWindows();

    namedWindow("Original - 原始图像", WINDOW_AUTOSIZE);
    imshow("Original - 原始图像", image);
    namedWindow("RGBHistogramEqulization", WINDOW_AUTOSIZE);
    imshow("RGBHistogramEqulization", BGRhistequ_image);

    waitKey(0);
    destroyAllWindows();

    return 0;
}