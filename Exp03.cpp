#include "Exp03.h"

#define M_PI 3.14159265358979323846

using namespace cv;
using namespace std;

int Exp03Help()
{
    cout << "\nExp03 --- SA16225037 程欣\n" << endl;
    cout <<
        "1 : 利用均值模板平滑灰度图像\n" <<
        "2 : 利用高斯模板平滑灰度图像\n" <<
        "3 : 利用Laplacian、Robert、Sobel模板锐化灰度图像\n" <<
        "4 : 利用高提升滤波算法增强灰度图像\n" <<
        "5 : 利用均值模板平滑彩色图像RBG通道\n" <<
        "6 : 利用高斯模板平滑彩色图像RGB通道\n" <<
        "7 : 利用Laplacian、Robert、Sobel模板锐化彩色图像RGB通道\n" << endl;
    return 0;
}


extern Mat image;
extern Mat gray;
extern Mat hsv;
int Exp03Main(char *imagePath)
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

    Exp03Help();
    char choice;
    while (1)
    {
        cout << "清选择要运行的程序，按h帮助，按w返回上一级，按q退出：";
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
                MeanFilterProcessing();
                break;
            case '2':
                GaussianFilterProcessing();
                break;
            case '3':
                break;
            case '4':
                break;
            case '5':
                RGBMeanFilterProcessing();
                break;
            case '6':
                RGBGaussianFilterProcessing();
                break;
            case '7':
                break;
            case 'h':
                Exp03Help();
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
 
double LinearFilterCalc(Mat filterArea, Mat linearFilter)
{
    double result = 0;
    for (int y = 0; y < filterArea.rows; y++)
    {
        for (int x = 0; x < filterArea.cols; x++)
        {
            result += ((double)filterArea.at<uchar>(y, x))*(linearFilter.at<double>(y, x));
        }
    }
    return result;
}

/* padding and scan */
int FilterProcessing(Mat src, Mat dst, Mat filter, double ProcessingMethod(Mat filterArea, Mat filter))
{
    Mat src_padding=src.clone();
    Mat filterArea;
    int padding = (filter.rows - 1) / 2;
    //padding the border
    copyMakeBorder(src, src_padding, padding, padding, padding, padding, BORDER_REPLICATE);

    for (int y = padding; y < src_padding.rows - padding; y++)
    {
        for (int x = padding; x < src_padding.cols - padding; x++)
        {
            filterArea = src_padding(Range(y - padding, y + padding + 1), Range(x - padding, x + padding + 1));
            dst.at<uchar>(y - padding, x - padding) = ProcessingMethod(filterArea, filter);
        }
    }

    return 0;
}

int RGBFilterProcessing(Mat src, Mat BGRFilterImg, Mat filter, string title, double ProcessingMethod(Mat filterArea, Mat filter))
{
    vector<Mat> BGRchannels;
    split(image, BGRchannels);
    Mat bChannel, gChannel, rChannel;
    bChannel = BGRchannels.at(0);
    gChannel = BGRchannels.at(1);
    rChannel = BGRchannels.at(2);

    if (bChannel.size() != gray.size())
    {
        cout << "size error !" << endl;
        return 0;
    }

    Mat BGR_bChannel = Mat::zeros(bChannel.size(), bChannel.type());
    Mat BGR_gChannel = Mat::zeros(gChannel.size(), gChannel.type());
    Mat BGR_rChannel = Mat::zeros(rChannel.size(), rChannel.type());

    FilterProcessing(bChannel, BGR_bChannel, filter, ProcessingMethod);
    FilterProcessing(gChannel, BGR_gChannel, filter, ProcessingMethod);
    FilterProcessing(rChannel, BGR_rChannel, filter, ProcessingMethod);

    string bChannelTitle = "B channel:" + title;
    string gChannelTitle = "G channel:" + title;
    string rChannelTitle = "R channel:" + title;

    namedWindow("B channel", WINDOW_AUTOSIZE);
    imshow("B channel", bChannel);
    namedWindow(bChannelTitle, WINDOW_AUTOSIZE);
    imshow(bChannelTitle, BGR_bChannel);

    namedWindow("G channel", WINDOW_AUTOSIZE);
    imshow("G channel", gChannel);
    namedWindow(gChannelTitle, WINDOW_AUTOSIZE);
    imshow(gChannelTitle, BGR_gChannel);

    namedWindow("R channel", WINDOW_AUTOSIZE);
    imshow("R channel", rChannel);
    namedWindow(rChannelTitle, WINDOW_AUTOSIZE);
    imshow(rChannelTitle, BGR_rChannel);

    waitKey(0);
    destroyAllWindows();

    vector<Mat> BGRchannels_merge(3);
    BGRchannels_merge.at(0) = BGR_bChannel;
    BGRchannels_merge.at(1) = BGR_gChannel;
    BGRchannels_merge.at(2) = BGR_rChannel;

    //Mat BGRFilterImg;
    merge(BGRchannels_merge, BGRFilterImg);

    namedWindow("原始图像", WINDOW_AUTOSIZE);
    namedWindow(title, WINDOW_AUTOSIZE);
    imshow("原始图像", src);
    imshow(title, BGRFilterImg);

    waitKey(0);
    destroyAllWindows();

    return 0;
}

int MeanFilterProcessing()
{
    //Mat countFilter_3x3 = (Mat_<double>(3, 3) <<
    //    1, 2, 3,
    //    4, 5, 6,
    //    7, 8, 9);
    Mat meanFilter_3x3 = ((double)1 / 9)*Mat::ones(3, 3, CV_64F);//CV_64F对应double，若CV_32F对饮double会报错
    Mat meanFilter_5x5 = ((double)1 / 25)*Mat::ones(5, 5, CV_64F);
    Mat meanFilter_9x9 = ((double)1 / 81)*Mat::ones(9, 9, CV_64F);

    Mat meanImg_3x3 = Mat::zeros(gray.size(), gray.type());
    Mat meanImg_5x5 = Mat::zeros(gray.size(), gray.type());
    Mat meanImg_9x9 = Mat::zeros(gray.size(), gray.type());

    FilterProcessing(gray, meanImg_3x3, meanFilter_3x3, LinearFilterCalc);
    FilterProcessing(gray, meanImg_5x5, meanFilter_5x5, LinearFilterCalc);
    FilterProcessing(gray, meanImg_9x9, meanFilter_9x9, LinearFilterCalc);

    namedWindow("Gray - 灰度图像", WINDOW_AUTOSIZE);
    namedWindow("meanFilter_3x3", WINDOW_AUTOSIZE);
    namedWindow("meanFilter_5x5", WINDOW_AUTOSIZE);
    namedWindow("meanFilter_5x5", WINDOW_AUTOSIZE);

    imshow("Gray - 灰度图像", gray);
    imshow("meanFilter_3x3", meanImg_3x3);
    imshow("meanFilter_5x5", meanImg_5x5);
    imshow("meanFilter_9x9", meanImg_9x9);

    waitKey(0);
    destroyAllWindows();

    return 0;
}


int RGBMeanFilterProcessing()
{
    Mat meanFilter_3x3 = ((double)1 / 9)*Mat::ones(3, 3, CV_64F);//CV_64F对应double，若CV_32F对饮double会报错
    Mat meanFilter_5x5 = ((double)1 / 25)*Mat::ones(5, 5, CV_64F);
    Mat meanFilter_9x9 = ((double)1 / 81)*Mat::ones(9, 9, CV_64F);

    Mat meanFilterImg_3x3, meanFilterImg_5x5, meanFilterImg_9x9;

    RGBFilterProcessing(image, meanFilterImg_3x3, meanFilter_3x3, "meanFilter 3x3", LinearFilterCalc);
    RGBFilterProcessing(image, meanFilterImg_5x5, meanFilter_5x5, "meanFilter 5x5", LinearFilterCalc);
    RGBFilterProcessing(image, meanFilterImg_9x9, meanFilter_9x9, "meanFilter 9x9", LinearFilterCalc);

    //vector<Mat> BGRchannels;
    //split(image, BGRchannels);
    //Mat bChannel, gChannel, rChannel;
    //bChannel = BGRchannels.at(0);
    //gChannel = BGRchannels.at(1);
    //rChannel = BGRchannels.at(2);

    //Mat mean_bChannel_3x3 = Mat::zeros(gray.size(), gray.type());
    //Mat mean_bChannel_5x5 = Mat::zeros(gray.size(), gray.type());
    //Mat mean_bChannel_9x9 = Mat::zeros(gray.size(), gray.type());

    //Mat mean_gChannel_3x3 = Mat::zeros(gray.size(), gray.type());
    //Mat mean_gChannel_5x5 = Mat::zeros(gray.size(), gray.type());
    //Mat mean_gChannel_9x9 = Mat::zeros(gray.size(), gray.type());

    //Mat mean_rChannel_3x3 = Mat::zeros(gray.size(), gray.type());
    //Mat mean_rChannel_5x5 = Mat::zeros(gray.size(), gray.type());
    //Mat mean_rChannel_9x9 = Mat::zeros(gray.size(), gray.type());

    //FilterProcessing(bChannel, mean_bChannel_3x3, meanFilter_3x3, LinearFilterCalc);
    //FilterProcessing(bChannel, mean_bChannel_5x5, meanFilter_5x5, LinearFilterCalc);
    //FilterProcessing(bChannel, mean_bChannel_9x9, meanFilter_9x9, LinearFilterCalc);
    //FilterProcessing(gChannel, mean_gChannel_3x3, meanFilter_3x3, LinearFilterCalc);
    //FilterProcessing(gChannel, mean_gChannel_5x5, meanFilter_5x5, LinearFilterCalc);
    //FilterProcessing(gChannel, mean_gChannel_9x9, meanFilter_9x9, LinearFilterCalc);
    //FilterProcessing(rChannel, mean_rChannel_3x3, meanFilter_3x3, LinearFilterCalc);
    //FilterProcessing(rChannel, mean_rChannel_5x5, meanFilter_5x5, LinearFilterCalc);
    //FilterProcessing(rChannel, mean_rChannel_9x9, meanFilter_9x9, LinearFilterCalc);

    //namedWindow("B channel", WINDOW_AUTOSIZE);
    //namedWindow("meanFilter_b_3x3", WINDOW_AUTOSIZE);
    //namedWindow("meanFilter_b_5x5", WINDOW_AUTOSIZE);
    //namedWindow("meanFilter_b_9x9", WINDOW_AUTOSIZE);

    //imshow("B channel", bChannel);
    //imshow("meanFilter_b_3x3", mean_bChannel_3x3);
    //imshow("meanFilter_b_5x5", mean_bChannel_5x5);
    //imshow("meanFilter_b_9x9", mean_bChannel_9x9);

    //waitKey(0);
    //destroyAllWindows();

    //namedWindow("G channel", WINDOW_AUTOSIZE);
    //namedWindow("meanFilter_g_3x3", WINDOW_AUTOSIZE);
    //namedWindow("meanFilter_g_5x5", WINDOW_AUTOSIZE);
    //namedWindow("meanFilter_g_9x9", WINDOW_AUTOSIZE);

    //imshow("G channel", bChannel);
    //imshow("meanFilter_g_3x3", mean_gChannel_3x3);
    //imshow("meanFilter_g_5x5", mean_gChannel_5x5);
    //imshow("meanFilter_g_9x9", mean_gChannel_9x9);

    //waitKey(0);
    //destroyAllWindows();

    //namedWindow("R channel", WINDOW_AUTOSIZE);
    //namedWindow("meanFilter_r_3x3", WINDOW_AUTOSIZE);
    //namedWindow("meanFilter_r_5x5", WINDOW_AUTOSIZE);
    //namedWindow("meanFilter_r_5x5", WINDOW_AUTOSIZE);

    //imshow("R channel", rChannel);
    //imshow("meanFilter_r_3x3", mean_rChannel_3x3);
    //imshow("meanFilter_r_5x5", mean_rChannel_5x5);
    //imshow("meanFilter_r_9x9", mean_rChannel_9x9);

    //waitKey(0);
    //destroyAllWindows();

    //vector<Mat> BGRchannels_3x3(3), BGRchannels_5x5(3), BGRchannels_9x9(3);

    //BGRchannels_3x3.at(0) = mean_bChannel_3x3;
    //BGRchannels_3x3.at(1) = mean_gChannel_3x3;
    //BGRchannels_3x3.at(2) = mean_rChannel_3x3;

    //BGRchannels_5x5.at(0) = mean_bChannel_5x5;
    //BGRchannels_5x5.at(1) = mean_gChannel_5x5;
    //BGRchannels_5x5.at(2) = mean_rChannel_5x5;

    //BGRchannels_9x9.at(0) = mean_bChannel_9x9;
    //BGRchannels_9x9.at(1) = mean_gChannel_9x9;
    //BGRchannels_9x9.at(2) = mean_rChannel_9x9;

    //Mat meanFilterImg_3x3, meanFilterImg_5x5, meanFilterImg_9x9;

    //merge(BGRchannels_3x3, meanFilterImg_3x3);
    //merge(BGRchannels_5x5, meanFilterImg_5x5);
    //merge(BGRchannels_9x9, meanFilterImg_9x9);

    //namedWindow("原始图像", WINDOW_AUTOSIZE);
    //namedWindow("meanFilter_3x3", WINDOW_AUTOSIZE);
    //namedWindow("meanFilter_5x5", WINDOW_AUTOSIZE);
    //namedWindow("meanFilter_9x9", WINDOW_AUTOSIZE);

    //imshow("原始图像", image);
    //imshow("meanFilter_3x3", meanFilterImg_3x3);
    //imshow("meanFilter_5x5", meanFilterImg_5x5);
    //imshow("meanFilter_9x9", meanFilterImg_9x9);

    //waitKey(0);
    //destroyAllWindows();

    return 0;
}

int GaussianFilterGenerator(Mat gaussianFilter, int centerValue, double variance)
{
    int filterSize = gaussianFilter.rows;
    if (filterSize % 2 != 1)
    {
        cout << "Bad Gaussian filter size" << endl;
        return -1;
    }
    //vector<int> center(2);
    int center = filterSize / 2;
    int sum = 0;
    for (int y = 0; y < gaussianFilter.rows; y++)
    {
        for (int x = 0; x < gaussianFilter.cols; x++)
        {
            gaussianFilter.at<double>(y, x) = (int)(double(centerValue)*exp(-0.5*(double(pow((y - center), 2) + pow((x - center), 2))) / variance));
            sum += (int)gaussianFilter.at<double>(y, x);
        }
    }
    cout << filterSize << "x" << filterSize << " Gaussian Filter is : " << endl;
    cout << "1/" << sum << " * " << endl << gaussianFilter << endl;
    gaussianFilter = (1 / (double(sum)))*gaussianFilter;

    return 0;
}

int GaussianFilterProcessing()
{
    Mat gaussianFilter_3x3 = Mat::zeros(3, 3, CV_64F);//CV_64F对应double，若CV_32F对应double会报错
    Mat gaussianFilter_5x5 = Mat::zeros(5, 5, CV_64F);
    Mat gaussianFilter_9x9 = Mat::zeros(9, 9, CV_64F);

    GaussianFilterGenerator(gaussianFilter_3x3, 4, 1);
    GaussianFilterGenerator(gaussianFilter_5x5, 16, 1);
    GaussianFilterGenerator(gaussianFilter_9x9, 8192, 1);

    Mat gaussianImg_3x3 = Mat::zeros(gray.size(), gray.type());
    Mat gaussianImg_5x5 = Mat::zeros(gray.size(), gray.type());
    Mat gaussianImg_9x9 = Mat::zeros(gray.size(), gray.type());

    FilterProcessing(gray, gaussianImg_3x3, gaussianFilter_3x3, LinearFilterCalc);
    FilterProcessing(gray, gaussianImg_5x5, gaussianFilter_5x5, LinearFilterCalc);
    FilterProcessing(gray, gaussianImg_9x9, gaussianFilter_9x9, LinearFilterCalc);

    namedWindow("Gray - 灰度图像", WINDOW_AUTOSIZE);
    namedWindow("gaussianFilter_3x3", WINDOW_AUTOSIZE);
    namedWindow("gaussianFilter_5x5", WINDOW_AUTOSIZE);
    namedWindow("gaussianFilter_5x5", WINDOW_AUTOSIZE);

    imshow("Gray - 灰度图像", gray);
    imshow("gaussianFilter_3x3", gaussianImg_3x3);
    imshow("gaussianFilter_5x5", gaussianImg_5x5);
    imshow("gaussianFilter_9x9", gaussianImg_9x9);

    waitKey(0);
    destroyAllWindows();

    return 0;
}

int RGBGaussianFilterProcessing()
{
    Mat gaussianFilter_3x3 = Mat::zeros(3, 3, CV_64F);//CV_64F对应double，若CV_32F对应double会报错
    Mat gaussianFilter_5x5 = Mat::zeros(5, 5, CV_64F);
    Mat gaussianFilter_9x9 = Mat::zeros(9, 9, CV_64F);

    GaussianFilterGenerator(gaussianFilter_3x3, 4, 1);
    GaussianFilterGenerator(gaussianFilter_5x5, 16, 1);
    GaussianFilterGenerator(gaussianFilter_9x9, 8192, 1);

    vector<Mat> BGRchannels;
    split(image, BGRchannels);
    Mat bChannel, gChannel, rChannel;
    bChannel = BGRchannels.at(0);
    gChannel = BGRchannels.at(1);
    rChannel = BGRchannels.at(2);

    Mat gaussian_bChannel_3x3 = Mat::zeros(gray.size(), gray.type());
    Mat gaussian_bChannel_5x5 = Mat::zeros(gray.size(), gray.type());
    Mat gaussian_bChannel_9x9 = Mat::zeros(gray.size(), gray.type());

    Mat gaussian_gChannel_3x3 = Mat::zeros(gray.size(), gray.type());
    Mat gaussian_gChannel_5x5 = Mat::zeros(gray.size(), gray.type());
    Mat gaussian_gChannel_9x9 = Mat::zeros(gray.size(), gray.type());

    Mat gaussian_rChannel_3x3 = Mat::zeros(gray.size(), gray.type());
    Mat gaussian_rChannel_5x5 = Mat::zeros(gray.size(), gray.type());
    Mat gaussian_rChannel_9x9 = Mat::zeros(gray.size(), gray.type());

    FilterProcessing(bChannel, gaussian_bChannel_3x3, gaussianFilter_3x3, LinearFilterCalc);
    FilterProcessing(bChannel, gaussian_bChannel_5x5, gaussianFilter_5x5, LinearFilterCalc);
    FilterProcessing(bChannel, gaussian_bChannel_9x9, gaussianFilter_9x9, LinearFilterCalc);
    FilterProcessing(gChannel, gaussian_gChannel_3x3, gaussianFilter_3x3, LinearFilterCalc);
    FilterProcessing(gChannel, gaussian_gChannel_5x5, gaussianFilter_5x5, LinearFilterCalc);
    FilterProcessing(gChannel, gaussian_gChannel_9x9, gaussianFilter_9x9, LinearFilterCalc);
    FilterProcessing(rChannel, gaussian_rChannel_3x3, gaussianFilter_3x3, LinearFilterCalc);
    FilterProcessing(rChannel, gaussian_rChannel_5x5, gaussianFilter_5x5, LinearFilterCalc);
    FilterProcessing(rChannel, gaussian_rChannel_9x9, gaussianFilter_9x9, LinearFilterCalc);

    namedWindow("B channel", WINDOW_AUTOSIZE);
    namedWindow("gaussianFilter_b_3x3", WINDOW_AUTOSIZE);
    namedWindow("gaussianFilter_b_5x5", WINDOW_AUTOSIZE);
    namedWindow("gaussianFilter_b_9x9", WINDOW_AUTOSIZE);

    imshow("B channel", bChannel);
    imshow("gaussianFilter_b_3x3", gaussian_bChannel_3x3);
    imshow("gaussianFilter_b_5x5", gaussian_bChannel_5x5);
    imshow("gaussianFilter_b_9x9", gaussian_bChannel_9x9);

    waitKey(0);
    destroyAllWindows();

    namedWindow("G channel", WINDOW_AUTOSIZE);
    namedWindow("gaussianFilter_g_3x3", WINDOW_AUTOSIZE);
    namedWindow("gaussianFilter_g_5x5", WINDOW_AUTOSIZE);
    namedWindow("gaussianFilter_g_9x9", WINDOW_AUTOSIZE);

    imshow("G channel", bChannel);
    imshow("gaussianFilter_g_3x3", gaussian_gChannel_3x3);
    imshow("gaussianFilter_g_5x5", gaussian_gChannel_5x5);
    imshow("gaussianFilter_g_9x9", gaussian_gChannel_9x9);

    waitKey(0);
    destroyAllWindows();

    namedWindow("R channel", WINDOW_AUTOSIZE);
    namedWindow("gaussianFilter_r_3x3", WINDOW_AUTOSIZE);
    namedWindow("gaussianFilter_r_5x5", WINDOW_AUTOSIZE);
    namedWindow("gaussianFilter_r_5x5", WINDOW_AUTOSIZE);

    imshow("R channel", rChannel);
    imshow("gaussianFilter_r_3x3", gaussian_rChannel_3x3);
    imshow("gaussianFilter_r_5x5", gaussian_rChannel_5x5);
    imshow("gaussianFilter_r_9x9", gaussian_rChannel_9x9);

    waitKey(0);
    destroyAllWindows();

    vector<Mat> BGRchannels_3x3(3), BGRchannels_5x5(3), BGRchannels_9x9(3);

    BGRchannels_3x3.at(0) = gaussian_bChannel_3x3;
    BGRchannels_3x3.at(1) = gaussian_gChannel_3x3;
    BGRchannels_3x3.at(2) = gaussian_rChannel_3x3;

    BGRchannels_5x5.at(0) = gaussian_bChannel_5x5;
    BGRchannels_5x5.at(1) = gaussian_gChannel_5x5;
    BGRchannels_5x5.at(2) = gaussian_rChannel_5x5;

    BGRchannels_9x9.at(0) = gaussian_bChannel_9x9;
    BGRchannels_9x9.at(1) = gaussian_gChannel_9x9;
    BGRchannels_9x9.at(2) = gaussian_rChannel_9x9;

    Mat gaussianFilterImg_3x3, gaussianFilterImg_5x5, gaussianFilterImg_9x9;

    merge(BGRchannels_3x3, gaussianFilterImg_3x3);
    merge(BGRchannels_5x5, gaussianFilterImg_5x5);
    merge(BGRchannels_9x9, gaussianFilterImg_9x9);

    namedWindow("原始图像", WINDOW_AUTOSIZE);
    namedWindow("gaussianFilter_3x3", WINDOW_AUTOSIZE);
    namedWindow("gaussianFilter_5x5", WINDOW_AUTOSIZE);
    namedWindow("gaussianFilter_9x9", WINDOW_AUTOSIZE);

    imshow("原始图像", image);
    imshow("gaussianFilter_3x3", gaussianFilterImg_3x3);
    imshow("gaussianFilter_5x5", gaussianFilterImg_5x5);
    imshow("gaussianFilter_9x9", gaussianFilterImg_9x9);

    waitKey(0);
    destroyAllWindows();

    return 0;
}
