#include "Exp02.h"
#include "Exp01.h"

using namespace cv;
using namespace std;

int Exp02Help()
{
    cout << "\nExp02 --- SA16225037 程欣\n" << endl;
    cout <<
        "1 : 显示归一化直方图\n" <<
        "2 : 灰度图像直方图均衡处理\n" <<
        "3 : BGR彩色图像三通道直方图均衡处理\n" << 
        "4 : HSV彩色图像V通道直方图均衡处理\n" << endl;
    return 0;
}

extern Mat image;
extern Mat gray;
extern Mat hsv;
int Exp02Main(string imagePath)
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

    Exp02Help();
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
                ShowNormalizedHistogram();
                break;
            case '2':
                GrayHistogramEqualization();
                break;
            case '3':
                BGRHistogramEqulization();
                break;
            case '4':
                HSVHistogramEqulization();
                break;
            case 'h':
                Exp02Help();
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

int CalcNormalizedHistogram(Mat img, Mat histImg, int histHeight, int *pmax, int *hist, Scalar color)
{
    //Mat lutTable = Mat::zeros(1, 256, CV_8U);
    //LUT(histImg, lutTable, histImg);
    memset(hist, 0, 256 * sizeof(int));
    histImg = 0;
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
        binValue = hist[i] * histHeight / max;
        rectangle(histImg, Point(i, histHeight), Point(i + 1, histHeight - binValue), color);
    }
    
    return *hist;
}

int ShowNormalizedHistogram()
{
    int hist[256];
    memset(hist, 0, 256 * sizeof(int));
    int histHeight = 256;
    int max;
    int *pmax = &max;
    Mat histImg = Mat::zeros(histHeight, 256, CV_8U);
    //Mat histImg(histHeight, 256, CV_8U, Scalar(0));

    CalcNormalizedHistogram(gray, histImg, histHeight, pmax, hist, Scalar(255));

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
    int histHeight = 256;
    int max;
    int *pmax = &max;
    Mat histImg = Mat::zeros(histHeight, 256, CV_8U);

    CalcNormalizedHistogram(gray, histImg, histHeight, pmax, hist, Scalar(255));

    Mat histEquImg = Mat::zeros(gray.size(), gray.type());
    double table[256];
    table[0] = 255. * double(hist[0]) / (double)(gray.rows*gray.cols);
    for (int i = 1; i < 256; i++)
    {
        table[i] = (255. * double(hist[i]) / double(gray.rows*gray.cols)) + table[i - 1];
    }
    //normalize the table
    for (int i = 0; i < 256; i++)
    {
        table[i] = (255. * table[i] - table[0]) / (table[255] - table[0]);
    }

    for (int y = 0; y < gray.rows; y++)
    {
        for (int x = 0; x < gray.cols; x++)
        {
            histEquImg.at<uchar>(y, x) = saturate_cast<uchar>(cvRound(table[gray.at<uchar>(y, x)]));
        }
    }

    //normalize the histEquImg
    //double maxOrigin, minOrigin;
    //minMaxLoc(gray, &minOrigin, &maxOrigin, 0, 0);
    //LinearTransProcessing(histEquImg, histEquImg, (int)minOrigin, (int)maxOrigin);
    //minMaxLoc(histEquImg, &minOrigin, &maxOrigin, 0, 0);
    //histEquImg.convertTo(histEquImg, CV_8U, 255. / (maxOrigin - minOrigin), (-255.*minOrigin) / (maxOrigin - minOrigin));

    namedWindow("Gray - 灰度图像", WINDOW_AUTOSIZE);
    imshow("Gray - 灰度图像", gray);
    namedWindow("Histogram - 直方图", WINDOW_AUTOSIZE);
    imshow("Histogram - 直方图", histImg);
    namedWindow("GrayHistogramEqualization - 直方图均衡后灰度图像", WINDOW_AUTOSIZE);
    imshow("GrayHistogramEqualization - 直方图均衡后灰度图像", histEquImg);

    CalcNormalizedHistogram(histEquImg, histImg, histHeight, pmax, hist, Scalar(255));

    namedWindow("直方图均衡后灰度图像直方图", WINDOW_AUTOSIZE);
    imshow("直方图均衡后灰度图像直方图", histImg);

    Mat cv_HistEquImg = Mat::zeros(gray.size(), gray.type());
    equalizeHist(gray, cv_HistEquImg);
    namedWindow("调用OpenCV直方图均衡化API得到的结果", WINDOW_AUTOSIZE);
    imshow("调用OpenCV直方图均衡化API得到的结果", cv_HistEquImg);
    Mat cv_HistImg = Mat::zeros(histHeight, 256, CV_8U);
    CalcNormalizedHistogram(cv_HistEquImg, cv_HistImg, histHeight, pmax, hist, Scalar(255));
    namedWindow("调用OpenCV直方图均衡化API得到的结果的直方图", WINDOW_AUTOSIZE);
    imshow("调用OpenCV直方图均衡化API得到的结果的直方图", cv_HistImg);

    waitKey(0);
    destroyAllWindows();
    return 0;
}

int BGRHistogramEqulization()
{
    vector<Mat> BGRchannels;
    split(image, BGRchannels);
    Mat bChannel, gChannel, rChannel;
    bChannel = BGRchannels.at(0);
    gChannel = BGRchannels.at(1);
    rChannel = BGRchannels.at(2);

    int b_Hist[256], g_Hist[256], r_Hist[256];
    memset(b_Hist, 0, 256 * sizeof(int));
    memset(g_Hist, 0, 256 * sizeof(int));
    memset(r_Hist, 0, 256 * sizeof(int));
    int histHeight = 256;
    int bMax, gMax, rMax;
    int *b_pmax = &bMax, *g_pmax = &gMax, *r_pmax = &rMax;
    Mat b_HistImg = Mat::zeros(histHeight, 256, CV_8UC3);
    Mat g_HistImg = Mat::zeros(histHeight, 256, CV_8UC3);
    Mat r_HistImg = Mat::zeros(histHeight, 256, CV_8UC3);

    CalcNormalizedHistogram(bChannel, b_HistImg, histHeight, b_pmax, b_Hist, Scalar(255, 0, 0));
    CalcNormalizedHistogram(gChannel, g_HistImg, histHeight, g_pmax, g_Hist, Scalar(0, 255, 0));
    CalcNormalizedHistogram(rChannel, r_HistImg, histHeight, r_pmax, r_Hist, Scalar(0, 0, 255));

    namedWindow("B channel", WINDOW_AUTOSIZE);
    namedWindow("G channel", WINDOW_AUTOSIZE);
    namedWindow("R channel", WINDOW_AUTOSIZE);
    imshow("G channel", gChannel);
    imshow("B channel", bChannel);
    imshow("R channel", rChannel);

    namedWindow("B channel histogram", WINDOW_AUTOSIZE);
    namedWindow("G channel histogram", WINDOW_AUTOSIZE);
    namedWindow("R channel histogram", WINDOW_AUTOSIZE);
    imshow("B channel histogram", b_HistImg);
    imshow("G channel histogram", g_HistImg);
    imshow("R channel histogram", r_HistImg);

    waitKey(0);

    Mat b_HistEquImg = Mat::zeros(gray.size(), gray.type());
    Mat g_HistEquImg = Mat::zeros(gray.size(), gray.type());
    Mat r_HistEquImg = Mat::zeros(gray.size(), gray.type());
    double b_table[256], g_table[256], r_table[256];
    b_table[0] = 255. * double(b_Hist[0]) / (double)(image.rows*image.cols);
    g_table[0] = 255. * double(g_Hist[0]) / (double)(image.rows*image.cols);
    r_table[0] = 255. * double(r_Hist[0]) / (double)(image.rows*image.cols);
    for (int i = 1; i < 256; i++)
    {
        b_table[i] = (255. * double(b_Hist[i]) / (double)(image.rows*image.cols)) + b_table[i - 1];
        g_table[i] = (255. * double(g_Hist[i]) / (double)(image.rows*image.cols)) + g_table[i - 1];
        r_table[i] = (255. * double(r_Hist[i]) / (double)(image.rows*image.cols)) + r_table[i - 1];
    }

    for (int y = 0; y < image.rows; y++)
    {
        for (int x = 0; x < image.cols; x++)
        {
            b_HistEquImg.at<uchar>(y, x) = saturate_cast<uchar>(cvRound(b_table[bChannel.at<uchar>(y, x)]));
            g_HistEquImg.at<uchar>(y, x) = saturate_cast<uchar>(cvRound(g_table[gChannel.at<uchar>(y, x)]));
            r_HistEquImg.at<uchar>(y, x) = saturate_cast<uchar>(cvRound(r_table[rChannel.at<uchar>(y, x)]));
        }
    }

    //normalize the histEquImg
    double maxOrigin, minOrigin;
    minMaxLoc(bChannel, &minOrigin, &maxOrigin, 0, 0);
    LinearTransProcessing(b_HistEquImg, b_HistEquImg, (int)minOrigin, (int)maxOrigin);
    minMaxLoc(gChannel, &minOrigin, &maxOrigin, 0, 0);
    LinearTransProcessing(g_HistEquImg, g_HistEquImg, (int)minOrigin, (int)maxOrigin);
    minMaxLoc(rChannel, &minOrigin, &maxOrigin, 0, 0);
    LinearTransProcessing(r_HistEquImg, r_HistEquImg, (int)minOrigin, (int)maxOrigin);

    BGRchannels.at(0) = b_HistEquImg;
    BGRchannels.at(1) = g_HistEquImg;
    BGRchannels.at(2) = r_HistEquImg;
    Mat BGRHistEquImg;
    merge(BGRchannels, BGRHistEquImg);

    CalcNormalizedHistogram(b_HistEquImg, b_HistImg, histHeight, b_pmax, b_Hist, Scalar(255, 0, 0));
    CalcNormalizedHistogram(g_HistEquImg, g_HistImg, histHeight, g_pmax, g_Hist, Scalar(0, 255, 0));
    CalcNormalizedHistogram(r_HistEquImg, r_HistImg, histHeight, r_pmax, r_Hist, Scalar(0, 0, 255));

    imshow("B channel", b_HistEquImg);
    imshow("G channel", g_HistEquImg);
    imshow("R channel", r_HistEquImg);

    imshow("B channel histogram", b_HistImg);
    imshow("G channel histogram", g_HistImg);
    imshow("R channel histogram", r_HistImg);

    waitKey(0);
    destroyAllWindows();

    namedWindow("Original - 原始图像", WINDOW_AUTOSIZE);
    imshow("Original - 原始图像", image);
    namedWindow("BGRHistogramEqulization", WINDOW_AUTOSIZE);
    imshow("BGRHistogramEqulization", BGRHistEquImg);

    waitKey(0);
    destroyAllWindows();

    return 0;
}

int HSVHistogramEqulization()
{
    vector<Mat> HSVchannels;
    split(hsv, HSVchannels);
    Mat hChannel, sChannel, vChannel;
    hChannel = HSVchannels.at(0);
    sChannel = HSVchannels.at(1);
    vChannel = HSVchannels.at(2);

    int v_Hist[256];
    memset(v_Hist, 0, 256 * sizeof(int));
    int histHeight = 256;
    int hMax, sMax, vMax;
    int *h_pmax = &hMax, *s_pmax = &sMax, *v_pmax = &vMax;
    Mat v_HistImg = Mat::zeros(histHeight, 256, CV_8UC3);

    CalcNormalizedHistogram(vChannel, v_HistImg, histHeight, v_pmax, v_Hist, Scalar(255, 255, 255));

    namedWindow("V channel", WINDOW_AUTOSIZE);
    imshow("V channel", vChannel);
    namedWindow("V channel Histogram", WINDOW_AUTOSIZE);
    imshow("V channel Histogram", v_HistImg);

    waitKey(0);

    Mat v_HistEquImg = Mat::zeros(gray.size(), gray.type());
    double v_table[256];
    v_table[0] = 255. * double(v_Hist[0]) / (double)(image.rows*image.cols);
    for (int i = 1; i < 256; i++)
    {
        v_table[i] = (255. * double(v_Hist[i]) / (double)(image.rows*image.cols)) + v_table[i - 1];
    }

    for (int y = 0; y < image.rows; y++)
    {
        for (int x = 0; x < image.cols; x++)
        {
            v_HistEquImg.at<uchar>(y, x) = saturate_cast<uchar>(cvRound(v_table[vChannel.at<uchar>(y, x)]));
        }
    }

    //normalize the histEquImg
    double maxOrigin, minOrigin;
    minMaxLoc(vChannel, &minOrigin, &maxOrigin, 0, 0);
    LinearTransProcessing(v_HistEquImg, v_HistEquImg, (int)minOrigin, (int)maxOrigin);

    HSVchannels.at(2) = v_HistEquImg;
    Mat HSVHistEquImg;
    merge(HSVchannels, HSVHistEquImg);
    cvtColor(HSVHistEquImg, HSVHistEquImg, CV_HSV2BGR);

    CalcNormalizedHistogram(v_HistEquImg, v_HistImg, histHeight, v_pmax, v_Hist, Scalar(255, 255, 255));

    imshow("V channel", v_HistEquImg);
    imshow("V channel Histogram", v_HistImg);

    waitKey(0);
    destroyAllWindows();

    namedWindow("Original - 原始图像", WINDOW_AUTOSIZE);
    imshow("Original - 原始图像", image);
    namedWindow("HSVHistogramEqulization", WINDOW_AUTOSIZE);
    imshow("HSVHistogramEqulization", HSVHistEquImg);

    waitKey(0);
    destroyAllWindows();

    return 0;
}
