#include "Exp04.h"
#include "Exp03.h"
#include "Exp02.h"
#include "procs.h"

#define M_SALT 1
#define M_PEPPER 2
#define M_SALTPEPPER 3
#define M_GAUSSIAN 4
#define M_PI 3.14159265358979323846

int Exp04Help()
{
    cout << "\nExp04 --- SA16225037 程欣\n" << endl;
    cout <<
        "1 : 添加噪声并进行4类均值滤波、中值滤波、自适应均值和中值滤波\n" <<
        "2 : 彩色图像滤波\n" << endl;
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
    Mat result_color = Mat::ones(image.size(), image.type());

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
                NoiseFiltering(gray, result);
                break;
            case '2':
                NoiseFiltering(image, result_color);
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
    switch (type)
    {
    case M_SALT:
        cout << "添加盐噪声，数量：" << n << endl;
        break;
    case M_PEPPER:
        cout << "添加胡椒噪声，数量：" << n << endl;
        break;
    case M_SALTPEPPER:
        cout << "添加椒盐噪声，数量：" << n << endl;
        break;
    case M_GAUSSIAN:
        cout << "添加高斯噪声，数量：" << n << "，均值：" << pParam[0] << "，半径：" << pParam[1] << endl;
        break;
    default:
        break;
    }
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
            c = -255;
            break;
        case M_SALTPEPPER:
            c = ((rand() % 2) * 2 - 1) * 255;
            break;
        case M_GAUSSIAN://通过Box-Muller变换可以产生Gaussian噪声
            //c = -1;
            //while (c < 0 || c>255)
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
            break;
        default:
            break;
        }
        if (type)
        {
            if (img.type() == CV_8UC1)
            {
                img.at<uchar>(i, j) = saturate_cast<uchar>(img.at<uchar>(i, j) + c);
            }
            else if (img.type() == CV_8UC3)
            {
                img.at<Vec3b>(i, j)[0] = saturate_cast<uchar>(img.at<uchar>(i, j) + c);
                img.at<Vec3b>(i, j)[1] = saturate_cast<uchar>(img.at<uchar>(i, j) + c);
                img.at<Vec3b>(i, j)[2] = saturate_cast<uchar>(img.at<uchar>(i, j) + c);
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

//template <typename _Tp>
//int mem_cmp(const void *a, const void *b)
//{
//    //当_Tp为浮点型，可能由于精度，会影响排序  
//    return (*((_Tp *)a) - *((_Tp *)b));
//}
//
////求Mat元素中值  
//template <typename _Tp>
//_Tp medianElem(Mat img)
//{
//    _Tp *buf;
//    size_t total = img.total();
//
//    buf = new _Tp[total];
//
//    for (int i = 0; i < img.rows; i++)
//    {
//        _Tp *p = img.ptr<_Tp>(i);
//        for (int j = 0; j < img.cols; j++)
//        {
//            //buf[i*img.cols + j] = img.ptr<_Tp>(i)[j];
//            buf[i*img.cols + j] = p[j];
//        }
//    }
//
//    qsort(buf, total, sizeof(_Tp), mem_cmp<_Tp>);
//
//    return buf[total / 2];
//}

int mem_cmp(const void *a, const void *b)
{
    //当_Tp为浮点型，可能由于精度，会影响排序  
    return (*((int *)a) - *((int *)b));
}
//求Mat元素中值  
int medianElem(Mat img)
{
    if (img.type() != CV_8U)
    {
        cout << "median type error" << endl;
    }
    int *buf;
    int result;
    int total = img.total();
    buf = new int[total];

    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            buf[i*img.cols + j] = img.at<uchar>(i, j);
        }
    }
    qsort(buf, total, sizeof(int), mem_cmp);
    result = buf[total / 2];
    free(buf);//必须要释放，否则会造成内存泄漏

    return result;
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
            //result *= (double(filterArea.at<uchar>(y, x)));
            result *= (double(filterArea.at<uchar>(y, x) == 0 ? 1 : filterArea.at<uchar>(y, x)));
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
    result = medianElem(filterArea);
    return result;
}

double AdaptiveMeanFilterCalc(Mat filterArea, Mat Filter)
{
    //cv::meanStdDev(img, mean, stddev);
    Scalar mean_local, stddev_local;
    double stddev_global = Filter.at<double>(0, 0);
    meanStdDev(filterArea, mean_local, stddev_local);

    double g = filterArea.at<uchar>(filterArea.rows / 2, filterArea.cols / 2);
    double c = pow(stddev_global, 2) / (pow(stddev_local.val[0], 2) + 0.0000001);

    c = c > 1 ? 1 : c;

    return (g - c*(g - mean_local.val[0]));
}

double StandardDeviationCalc(Mat filterArea, Mat Filter)
{
    Scalar mean_local, stddev_local;
    meanStdDev(filterArea, mean_local, stddev_local);
    return double(stddev_local.val[0]);
}

int trackbarChange = 0;
int noiseChange = 0;
int filterChange = 0;
static void onTrackbar(int, void*)
{
    trackbarChange = 1;
}
static void onTrackbarNoise(int, void*)
{
    noiseChange = 1;
    trackbarChange = 1;
}
static void onTrackbarFilter(int, void*)
{
    filterChange = 1;
    trackbarChange = 1;
}

int NoiseFiltering(Mat src, Mat dst)
{
    Mat MeanFilteringImg = Mat::zeros(src.size(), gray.type());
    //Mat MeanFilteringImgB, MeanFilteringImgG, MeanFilteringImgR;
    Mat MeanFilteringColorImg, MeanFilteringColorImg_dst;

    Mat filter = Mat::zeros(3, 3, CV_64F);
    vector<Mat> HSIchannels(3), HSIchannels_dst(3);
    Mat hChannel, sChannel, iChannel;

    if (src.type() == CV_8UC1)
    {
        src.copyTo(MeanFilteringImg);
    }
    else if (src.type() == CV_8UC3)
    {
        MeanFilteringColorImg = Mat::zeros(src.size(), src.type());
        MeanFilteringColorImg_dst = Mat::zeros(src.size(), src.type());
        ConvertBGR2HSI(src, MeanFilteringColorImg);
        //cvtColor(src, MeanFilteringColorImg, CV_BGR2HSV);
        split(MeanFilteringColorImg, HSIchannels);
        hChannel = HSIchannels.at(0);
        sChannel = HSIchannels.at(1);
        iChannel = HSIchannels.at(2);
        MeanFilteringImg = HSIchannels.at(2);
        split(dst, HSIchannels_dst);
        dst = HSIchannels_dst.at(2);
        HSIchannels_dst.at(0) = hChannel;
        HSIchannels_dst.at(1) = sChannel;
    }
    else
    {
        cout << "src type error!" << endl;
        return -1;
    }

    double gaussianParam[2];
    gaussianParam[0] = 0;
    gaussianParam[1] = 32;
    int noiseNum = 0;
    int noiseType = 0;
    int filterType = 0;
    int q = 4;//Q=-1(q=4)时，为谐波滤波；Q=0(q=5)时，为算数均值滤波
    int filterSize = 0;
    int averageNoise = gaussianParam[0] + 255;
    int sigmaNoise = gaussianParam[1];
    Scalar mean, stddev;
    meanStdDev(MeanFilteringImg, mean, stddev);

    int hist[256];
    memset(hist, 0, 256 * sizeof(int));
    int histHeight = 256;
    int max_i;
    int *pmax_i = &max_i;
    Mat histImg = Mat::zeros(histHeight, 256, CV_8U);

    namedWindow("调节窗口", WINDOW_NORMAL);
    createTrackbar("滤波器大小", "调节窗口", &filterSize, 3, onTrackbarFilter);
    createTrackbar("噪声数量", "调节窗口", &noiseNum, MeanFilteringImg.total(), onTrackbarNoise);
    createTrackbar("噪声类型", "调节窗口", &noiseType, 4, onTrackbarNoise);
    createTrackbar("滤波方式", "调节窗口", &filterType, 7, onTrackbar);
    createTrackbar("逆谐波Q", "调节窗口", &q, 10, onTrackbar);
    createTrackbar("高斯噪声均值", "调节窗口", &averageNoise, 510, onTrackbarNoise);
    createTrackbar("高斯噪声标准差", "调节窗口", &sigmaNoise, 128, onTrackbarNoise);
    namedWindow("Noise Image", WINDOW_AUTOSIZE);
    imshow("Noise Image", src);
    namedWindow("Filtered Image", WINDOW_AUTOSIZE);
    imshow("Filtered Image", dst);

    for (;;)
    {
        if (filterChange)
        {
            cout << "滤波器大小调整为 : " << 2 * filterSize + 3 << "x" << 2 * filterSize + 3 << endl;
            filter = Mat::zeros(2 * filterSize + 3, 2 * filterSize + 3, CV_64F);
            filterChange = 0;
        }
        if (noiseChange)
        {
            namedWindow("Noise Image", WINDOW_AUTOSIZE);
            if (src.type() == CV_8UC1)
            {
                src.copyTo(MeanFilteringImg);
                gaussianParam[0] = averageNoise - 255;
                gaussianParam[1] = sigmaNoise;
                AddNoise(MeanFilteringImg, noiseNum, noiseType, gaussianParam);
                imshow("Noise Image", MeanFilteringImg);
            }
            else if (src.type() == CV_8UC3)
            {
                //HSIchannels.at(2) = MeanFilteringImg;
                MeanFilteringImg = iChannel.clone();
                gaussianParam[0] = averageNoise - 255;
                gaussianParam[1] = sigmaNoise;
                AddNoise(MeanFilteringImg, noiseNum, noiseType, gaussianParam);
                HSIchannels.at(2) = MeanFilteringImg;
                merge(HSIchannels, MeanFilteringColorImg);
                //cvtColor(MeanFilteringColorImg, MeanFilteringColorImg, CV_HSV2BGR);
                ConvertHSI2BGR(MeanFilteringColorImg, MeanFilteringColorImg);
                imshow("Noise Image", MeanFilteringColorImg);
            }
            noiseChange = 0;

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
                cout << "自适应均值滤波" << endl;
                FilterProcessing(MeanFilteringImg, dst, filter, StandardDeviationCalc);
                imshow("Standard Deviation Img", dst);
                CalcNormalizedHistogram(dst, histImg, histHeight, pmax_i, hist, Scalar(255));
                imshow("StdImg Hist", histImg);
                cout << "std peak position：" << max_i << endl;

                //sigmaNoise = max_i;
                filter.at<double>(0, 0) = max_i;
                FilterProcessing(MeanFilteringImg, dst, filter, AdaptiveMeanFilterCalc);

                break;
            case 5:
                cout << "几何均值滤波" << endl;
                FilterProcessing(MeanFilteringImg, dst, filter, GeometricMeanFilterCalc);
                break;
            case 6:
                cout << "中值滤波" << endl;
                FilterProcessing(MeanFilteringImg, dst, filter, MedianFilterCalc);
                break;
            case 7:
                cout << "自适应中值滤波" << endl;
                AdaptiveMedianFiltering(MeanFilteringImg, dst, filter);
                break;
            default:
                break;
            }
            namedWindow("Filtered Image", WINDOW_AUTOSIZE);

            if (src.type() == CV_8UC1)
            {
                imshow("Filtered Image", dst);
            }
            else if (src.type() == CV_8UC3)
            {
                //HSIchannels_dst.at(2) = dst;
                merge(HSIchannels_dst, MeanFilteringColorImg_dst);
                //cvtColor(MeanFilteringColorImg_dst, MeanFilteringColorImg_dst, CV_HSV2BGR);
                ConvertHSI2BGR(MeanFilteringColorImg_dst, MeanFilteringColorImg_dst);
                imshow("Filtered Image", MeanFilteringColorImg_dst);
            }
            trackbarChange = 0;
        }

        if (filterType != 4)
        {
            destroyWindow("Standard Deviation Img");
            destroyWindow("StdImg Hist");
        }

        if (waitKey(100) != 255)
        {
            destroyAllWindows();
            break;
        }
    }

    if (src.type() == CV_8UC3)
    {
        dst = MeanFilteringColorImg_dst;
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

int AdaptiveMedianFilterCalc(Mat filterArea, int s_max)
{
    int result = 0;
    double z_min, z_max;
    int z_med, z_xy;
    int a1, a2, b1, b2;

    z_xy = filterArea.at<uchar>(filterArea.rows / 2, filterArea.cols / 2);
    z_med = medianElem(filterArea);
    minMaxLoc(filterArea, &z_min, &z_max, 0, 0);

    a1 = z_med - int(z_min);
    a2 = z_med - int(z_max);
    if (a1 > 0 && a2 < 0)
    {
        b1 = z_xy - z_min;
        b2 = z_xy - z_max;
        if (b1 > 0 && b2 < 0)
        {
            return z_xy;
        }
        else
        {
            return z_med;
        }
    }
    else if (filterArea.rows + 2 <= s_max)
    {
        return -1;
    }
    else
    {
        return z_med;
    }

    return result;
}

int AdaptiveMedianFiltering(Mat src, Mat dst, Mat filter)
{
    Mat src_padding = src.clone();
    Mat filterArea;
    int padding = (filter.rows - 1) / 2;
    //padding the border
    copyMakeBorder(src, src_padding, padding, padding, padding, padding, BORDER_REPLICATE);

    int i = 0;
    int result = 0;
    int s_max = 9;
    for (int y = padding; y < src_padding.rows - padding; y++)
    {
        for (int x = padding; x < src_padding.cols - padding; x++)
        {
            i = 1;
            filterArea = src_padding(Range(y - padding, y + padding + 1), Range(x - padding, x + padding + 1));
            result = -1;
            while (result < 0)
            {
                result = AdaptiveMedianFilterCalc(filterArea, s_max);
                if (result < 0)
                {
                    //if (x > (s_max / 2 + 2) && x < (src_padding.cols - s_max / 2 - 2) && y >(s_max / 2 + 2) && y < (src_padding.cols - s_max / 2 - 2))
                    if (y - padding - i >= 0 && y + padding + 1 + i <= src_padding.rows && x - padding - i >= 0 && x + padding + 1 + i <= src_padding.cols)
                    {
                        filterArea = src_padding(Range(y - padding - i, y + padding + 1 + i), Range(x - padding - i, x + padding + 1 + i));
                        i++;
                    }
                    else
                    {
                        //result = src_padding.at<uchar>(y, x);
                        result = medianElem(filterArea);
                    }
                }
            }
            dst.at<uchar>(y - padding, x - padding) = result;
        }
    }

    return 0;
}
