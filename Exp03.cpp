#include "Exp03.h"
#include "Exp01.h"

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
        "5 : 利用均值模板平滑彩色图像BGR通道\n" <<
        "6 : 利用高斯模板平滑彩色图像BGR通道\n" <<
        "7 : 利用Laplacian、Robert、Sobel模板锐化彩色图像BGR通道\n" << endl;
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
                SharpenFilterProcessing();
                break;
            case '4':
                HighboostFilterProcessing();
                break;
            case '5':
                BGRMeanFilterProcessing();
                break;
            case '6':
                BGRGaussianFilterProcessing();
                break;
            case '7':
                BGRSharpenFilterProcessing();
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
            result += (double(filterArea.at<uchar>(y, x)))*(linearFilter.at<double>(y, x));
        }
    }
    return result;
}

/* padding and scan */
int FilterProcessing(
    Mat src, Mat dst, Mat filter, 
    double ProcessingMethod(Mat filterArea, Mat filter))
{
    Mat src_padding=src.clone();
    Mat filterArea;
    int padding = (filter.rows - 1) / 2;
    //padding the border
    copyMakeBorder(src, src_padding, padding, padding, padding, padding, BORDER_REPLICATE);

    if (dst.type() == CV_8U)
    {
        for (int y = padding; y < src_padding.rows - padding; y++)
        {
            for (int x = padding; x < src_padding.cols - padding; x++)
            {
                filterArea = src_padding(Range(y - padding, y + padding + 1), Range(x - padding, x + padding + 1));
                dst.at<uchar>(y - padding, x - padding) = ProcessingMethod(filterArea, filter);
            }
        }
    }
    else if (dst.type() == CV_64F)
    {
        for (int y = padding; y < src_padding.rows - padding; y++)
        {
            for (int x = padding; x < src_padding.cols - padding; x++)
            {
                filterArea = src_padding(Range(y - padding, y + padding + 1), Range(x - padding, x + padding + 1));
                dst.at<double>(y - padding, x - padding) = ProcessingMethod(filterArea, filter);
            }
        }
    }
    else
    {
        cout << "type error" << endl;
    }

    return 0;
}

int BGRFilterProcessing(
    Mat src, Mat dst, Mat filter, 
    Mat BGR_bChannel, Mat BGR_gChannel, Mat BGR_rChannel,
    double ProcessingMethod(Mat filterArea, Mat filter))
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

    FilterProcessing(bChannel, BGR_bChannel, filter, ProcessingMethod);
    FilterProcessing(gChannel, BGR_gChannel, filter, ProcessingMethod);
    FilterProcessing(rChannel, BGR_rChannel, filter, ProcessingMethod);

    vector<Mat> BGRchannels_merge(3);
    BGRchannels_merge.at(0) = BGR_bChannel;
    BGRchannels_merge.at(1) = BGR_gChannel;
    BGRchannels_merge.at(2) = BGR_rChannel;

    merge(BGRchannels_merge, dst);

    return 0;
}

int ShowBGRChannels(
    Mat BGR_bChannel, Mat BGR_gChannel, Mat BGR_rChannel,
    string title)
{
    vector<Mat> BGRchannels;
    split(image, BGRchannels);
    Mat bChannel, gChannel, rChannel;
    bChannel = BGRchannels.at(0);
    gChannel = BGRchannels.at(1);
    rChannel = BGRchannels.at(2);

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

int BGRMeanFilterProcessing()
{
    Mat meanFilter_3x3 = ((double)1 / 9)*Mat::ones(3, 3, CV_64F);//CV_64F对应double，若CV_32F对饮double会报错
    Mat meanFilter_5x5 = ((double)1 / 25)*Mat::ones(5, 5, CV_64F);
    Mat meanFilter_9x9 = ((double)1 / 81)*Mat::ones(9, 9, CV_64F);

    Mat meanFilterImg_3x3 = Mat::zeros(image.size(), image.type());//这里一定要给三个结果对象分配内存空间，不然调用函数得到的结果无法保留
    Mat meanFilterImg_5x5 = Mat::zeros(image.size(), image.type());
    Mat meanFilterImg_9x9 = Mat::zeros(image.size(), image.type());

    //Mat mean_bChannel_3x3 = Mat::zeros(gray.size(), CV_64F);
    Mat mean_bChannel_3x3 = Mat::zeros(gray.size(), gray.type());
    Mat mean_gChannel_3x3 = Mat::zeros(gray.size(), gray.type());
    Mat mean_rChannel_3x3 = Mat::zeros(gray.size(), gray.type());

    Mat mean_bChannel_5x5 = Mat::zeros(gray.size(), gray.type());
    Mat mean_gChannel_5x5 = Mat::zeros(gray.size(), gray.type());
    Mat mean_rChannel_5x5 = Mat::zeros(gray.size(), gray.type());

    Mat mean_bChannel_9x9 = Mat::zeros(gray.size(), gray.type());
    Mat mean_gChannel_9x9 = Mat::zeros(gray.size(), gray.type());
    Mat mean_rChannel_9x9 = Mat::zeros(gray.size(), gray.type());

    BGRFilterProcessing(
        image, meanFilterImg_3x3, meanFilter_3x3, 
        mean_bChannel_3x3, mean_gChannel_3x3, mean_rChannel_3x3,
        LinearFilterCalc);
    BGRFilterProcessing(
        image, meanFilterImg_5x5, meanFilter_5x5,
        mean_bChannel_5x5, mean_gChannel_5x5, mean_rChannel_5x5,
        LinearFilterCalc);
    BGRFilterProcessing(
        image, meanFilterImg_9x9, meanFilter_9x9,
        mean_bChannel_9x9, mean_gChannel_9x9, mean_rChannel_9x9,
        LinearFilterCalc);

    ShowBGRChannels(mean_bChannel_3x3, mean_gChannel_3x3, mean_rChannel_3x3, "Mean Filter 3x3");
    ShowBGRChannels(mean_bChannel_5x5, mean_gChannel_5x5, mean_rChannel_5x5, "Mean Filter 5x5");
    ShowBGRChannels(mean_bChannel_9x9, mean_gChannel_9x9, mean_rChannel_9x9, "Mean Filter 9x9");

    namedWindow("原始图像", WINDOW_AUTOSIZE);
    imshow("原始图像", image);

    namedWindow("Mean Filter 3x3", WINDOW_AUTOSIZE);
    namedWindow("Mean Filter 5x5", WINDOW_AUTOSIZE);
    namedWindow("Mean Filter 9x9", WINDOW_AUTOSIZE);

    imshow("Mean Filter 3x3", meanFilterImg_3x3);
    imshow("Mean Filter 5x5", meanFilterImg_5x5);
    imshow("Mean Filter 9x9", meanFilterImg_9x9);

    waitKey(0);
    destroyAllWindows();

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
    GaussianFilterGenerator(gaussianFilter_5x5, 25, 2);
    GaussianFilterGenerator(gaussianFilter_9x9, 81, 3);

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

int BGRGaussianFilterProcessing()
{
    Mat gaussianFilter_3x3 = Mat::zeros(3, 3, CV_64F);//CV_64F对应double，若CV_32F对应double会报错
    Mat gaussianFilter_5x5 = Mat::zeros(5, 5, CV_64F);
    Mat gaussianFilter_9x9 = Mat::zeros(9, 9, CV_64F);

    GaussianFilterGenerator(gaussianFilter_3x3, 4, 1);
    GaussianFilterGenerator(gaussianFilter_5x5, 25, 2);
    GaussianFilterGenerator(gaussianFilter_9x9, 81, 3);

    Mat gaussianFilterImg_3x3 = Mat::zeros(image.size(), image.type());//这里一定要给三个结果对象分配内存空间，不然调用函数得到的结果无法保留
    Mat gaussianFilterImg_5x5 = Mat::zeros(image.size(), image.type());
    Mat gaussianFilterImg_9x9 = Mat::zeros(image.size(), image.type());

    Mat gaussian_bChannel_3x3 = Mat::zeros(gray.size(), gray.type());
    Mat gaussian_gChannel_3x3 = Mat::zeros(gray.size(), gray.type());
    Mat gaussian_rChannel_3x3 = Mat::zeros(gray.size(), gray.type());

    Mat gaussian_bChannel_5x5 = Mat::zeros(gray.size(), gray.type());
    Mat gaussian_gChannel_5x5 = Mat::zeros(gray.size(), gray.type());
    Mat gaussian_rChannel_5x5 = Mat::zeros(gray.size(), gray.type());

    Mat gaussian_bChannel_9x9 = Mat::zeros(gray.size(), gray.type());
    Mat gaussian_gChannel_9x9 = Mat::zeros(gray.size(), gray.type());
    Mat gaussian_rChannel_9x9 = Mat::zeros(gray.size(), gray.type());

    BGRFilterProcessing(
        image, gaussianFilterImg_3x3, gaussianFilter_3x3,
        gaussian_bChannel_3x3, gaussian_gChannel_3x3, gaussian_rChannel_3x3,
        LinearFilterCalc);
    BGRFilterProcessing(
        image, gaussianFilterImg_5x5, gaussianFilter_5x5,
        gaussian_bChannel_5x5, gaussian_gChannel_5x5, gaussian_rChannel_5x5,
        LinearFilterCalc);
    BGRFilterProcessing(
        image, gaussianFilterImg_9x9, gaussianFilter_9x9,
        gaussian_bChannel_9x9, gaussian_gChannel_9x9, gaussian_rChannel_9x9,
        LinearFilterCalc);

    ShowBGRChannels(gaussian_bChannel_3x3, gaussian_gChannel_3x3, gaussian_rChannel_3x3, "Gaussian Filter 3x3");
    ShowBGRChannels(gaussian_bChannel_5x5, gaussian_gChannel_5x5, gaussian_rChannel_5x5, "Gaussian Filter 5x5");
    ShowBGRChannels(gaussian_bChannel_9x9, gaussian_gChannel_9x9, gaussian_rChannel_9x9, "Gaussian Filter 9x9");

    namedWindow("原始图像", WINDOW_AUTOSIZE);
    namedWindow("Gaussian Filter 3x3", WINDOW_AUTOSIZE);
    namedWindow("Gaussian Filter 5x5", WINDOW_AUTOSIZE);
    namedWindow("Gaussian Filter 5x5", WINDOW_AUTOSIZE);

    imshow("原始图像", image);
    imshow("Gaussian Filter 3x3", gaussianFilterImg_3x3);
    imshow("Gaussian Filter 5x5", gaussianFilterImg_5x5);
    imshow("Gaussian Filter 9x9", gaussianFilterImg_9x9);

    waitKey(0);
    destroyAllWindows();

    return 0;
}

int LaplacianFilterProcessing(Mat src, Mat dst, Mat laplacianFilter, Mat laplacianFilterImg, double c)//其实没必要多拆出来这一个函数
{

    FilterProcessing(gray, laplacianFilterImg, laplacianFilter, LinearFilterCalc);    
        
    //计算并标定锐化结果，直接saturate_cast<uchar>
    for (int y = 0; y < gray.rows; y++)
    {
        for (int x = 0; x < gray.cols; x++)
        {
            dst.at<uchar>(y, x) = saturate_cast<uchar>(src.at<uchar>(y, x) + c*int(laplacianFilterImg.at<double>(y, x)));
        }
    }
    //方案2，根据最大值最小值再变换一下。经过实验发现这种方案不可行。
    //dst.convertTo(dst, CV_64F);
    //for (int y = 0; y < gray.rows; y++)
    //{
    //    for (int x = 0; x < gray.cols; x++)
    //    {
    //        dst.at<double>(y, x) = src.at<uchar>(y, x) + c*int(laplacianFilterImg.at<double>(y, x));
    //    }
    //}
    //double min, max;
    //minMaxLoc(dst, &min, &max, 0, 0);
    //dst.convertTo(dst, CV_8U, 255. / (max - min), (-255.*min) / (max - min));

    return 0;
}

int LaplacianSharpen(Mat src, Mat dst, string title, double c, int filterNum)//dst是锐化后的结果图像
{
    Mat laplacianFilter_n4 = (Mat_<double>(3, 3) <<
        0, 1, 0,
        1, -4, 1,
        0, 1, 0);
    Mat laplacianFilter_n8 = (Mat_<double>(3, 3) <<
        1, 1, 1,
        1, -8, 1,
        1, 1, 1);
    Mat laplacianFilter;
    string filterTitle;
    switch (filterNum)
    {
    case 1:
        laplacianFilter = laplacianFilter_n4;
        filterTitle = "1 -4 1";
        break;
    case 2:
        laplacianFilter = laplacianFilter_n8;
        filterTitle = "1 -8 1";
        break;
    default:
        break;
    }

    Mat laplacianFilterImg = Mat::zeros(src.size(), CV_64F);//滤波后得到的边缘图像

    LaplacianFilterProcessing(src, dst, laplacianFilter, laplacianFilterImg, c);

    /*标定Laplacian滤波得到的边缘结果*/
    //LinearTransProcessing(laplacianFilterImg, dst, 0, 255);//自己写的函数，不如自带的成员方法convertTo()好用
    double maxLap, minLap;
    minMaxLoc(laplacianFilterImg, &minLap, &maxLap, 0, 0);
    laplacianFilterImg.convertTo(laplacianFilterImg, CV_8U, 255. / (maxLap - minLap), (-255.*minLap) / (maxLap - minLap));

    if (title.compare("") != 0)//标题不空则显示结果
    {
        namedWindow(title + "Laplacian Filter Img" + filterTitle, WINDOW_AUTOSIZE);
        imshow(title + "Laplacian Filter Img" + filterTitle, laplacianFilterImg);

        namedWindow(title + "Laplacian Sharpen Img" + filterTitle, WINDOW_AUTOSIZE);
        imshow(title + "Laplacian Sharpen Img" + filterTitle, dst);

        namedWindow(title, WINDOW_AUTOSIZE);
        imshow(title, src);

        waitKey(0);
        destroyAllWindows();
    }

    return 0;
}

int RobertSharpen(Mat src, Mat dst, string title, double c)
{
    Mat robertFilterImg = Mat::zeros(src.size(), CV_64F);//滤波后得到的边缘图像
    for (int y = 0; y < src.rows - 1; y++)
    {
        for (int x = 0; x < src.cols - 1; x++)
        {
            robertFilterImg.at<double>(y, x) =
                abs(src.at<uchar>(y + 1, x + 1) - src.at<uchar>(y, x)) + abs(src.at<uchar>(y + 1, x) - src.at<uchar>(y, x + 1));
            dst.at <uchar>(y, x) = saturate_cast<uchar>(src.at<uchar>(y, x) + c*robertFilterImg.at<double>(y, x));
        }
    }
    /*标定Rober滤波得到的边缘结果*/
    double maxRob, minRob;
    minMaxLoc(robertFilterImg, &minRob, &maxRob, 0, 0);
    robertFilterImg.convertTo(robertFilterImg, CV_8U, 255. / (maxRob - minRob), (-255.*minRob) / (maxRob - minRob));

    if (title != "")//标题不空则显示结果
    {
        namedWindow(title + "Robert Filter Img", WINDOW_AUTOSIZE);
        imshow(title + "Robert Filter Img", robertFilterImg);

        namedWindow(title + "Robert Sharpen Img", WINDOW_AUTOSIZE);
        imshow(title + "Robert Sharpen Img", dst);

        namedWindow(title, WINDOW_AUTOSIZE);
        imshow(title, src);

        waitKey(0);
        destroyAllWindows();
    }

    return 0;
}

int SobelSharpen(Mat src, Mat dst, string title, double c)
{
    Mat sobelFilterImg = Mat::zeros(src.size(), CV_64F);//滤波后得到的边缘图像
    Mat filterArea;
    Mat sobelFilter_x= (Mat_<double>(3, 3) <<
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1);
    Mat sobelFilter_y = (Mat_<double>(3, 3) <<
        -1, -2, -1,
        0, 0, 0,
        1, 2, 1);
    for (int y = 1; y < src.rows - 1; y++)
    {
        for (int x = 1; x < src.cols - 1; x++)
        {
            filterArea = src(Range(y - 1, y + 1 + 1), Range(x - 1, x + 1 + 1));
            sobelFilterImg.at<double>(y, x) = abs(LinearFilterCalc(filterArea, sobelFilter_x)) + abs(LinearFilterCalc(filterArea, sobelFilter_y));
            dst.at <uchar>(y, x) = saturate_cast<uchar>(src.at<uchar>(y, x) + c*sobelFilterImg.at<double>(y, x));
        }
    }
    /*标定Sobel滤波得到的边缘结果*/
    double maxSob, minSob;
    minMaxLoc(sobelFilterImg, &minSob, &maxSob, 0, 0);
    sobelFilterImg.convertTo(sobelFilterImg, CV_8U, 255. / (maxSob - minSob), (-255.*minSob) / (maxSob - minSob));

    if (title != "")//标题不空则显示结果
    {
        namedWindow(title + "Sobel Filter Img", WINDOW_AUTOSIZE);
        imshow(title + "Sobel Filter Img", sobelFilterImg);

        namedWindow(title + "Sobel Sharpen Img", WINDOW_AUTOSIZE);
        imshow(title + "Sobel Sharpen Img", dst);

        namedWindow(title, WINDOW_AUTOSIZE);
        imshow(title, src);

        waitKey(0);
        destroyAllWindows();
    }

    return 0;
}

int SharpenFilterProcessing()
{
    /*Laplacian*/
    int lablacianFilterNum = 1;//1为中心为-4的laplacian模板，2为中心为-8的
    double cLap = -1;
    Mat laplacianSharpenImg = Mat::zeros(gray.size(), gray.type());

    /*Robert*/
    double cRob = 1;
    Mat robertSharpenImg = Mat::zeros(gray.size(), gray.type());

    /*Sobel*/
    double cSob = 0.5;
    Mat sobelSharpenImg = Mat::zeros(gray.size(), gray.type());

    char choice;
    while (1)
    {
        cout << 
            "清选择锐化模板\n" <<
            "1 Laplacian\n" <<
            "2 Robert\n" <<
            "3 Sobel\n" <<
            "按w返回上一级，按q退出：";
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
                LaplacianSharpen(gray, laplacianSharpenImg, "灰度图像 ", cLap, lablacianFilterNum);
                break;
            case '2':
                RobertSharpen(gray, robertSharpenImg, "灰度图像 ", cRob);//系数取正会加强较亮的部分，系数取负会加强较暗的部分
                break;
            case '3':
                SobelSharpen(gray, sobelSharpenImg, "灰度图像 ", cSob);//系数取正会加强较亮的部分，系数取负会加强较暗的部分
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

int BGRSharpenFilterProcessing()
{
    vector<Mat> BGRchannels;
    split(image, BGRchannels);
    Mat bChannel, gChannel, rChannel;
    bChannel = BGRchannels.at(0);
    gChannel = BGRchannels.at(1);
    rChannel = BGRchannels.at(2);

    vector<Mat> BGRchannels_merge(3);

    /*Laplacian*/
    int lablacianFilterNum = 1;//1为中心为-4的laplacian模板，2为中心为-8的
    double cLap = -1;
    Mat laplacianSharpen_bChannel = Mat::zeros(gray.size(), gray.type());
    Mat laplacianSharpen_gChannel = Mat::zeros(gray.size(), gray.type());
    Mat laplacianSharpen_rChannel = Mat::zeros(gray.size(), gray.type());
    Mat laplacianSharpenImg = Mat::zeros(image.size(), image.type());

    /*Robert*/
    double cRob = 1;
    Mat robertSharpen_bChannel = Mat::zeros(gray.size(), gray.type());
    Mat robertSharpen_gChannel = Mat::zeros(gray.size(), gray.type());
    Mat robertSharpen_rChannel = Mat::zeros(gray.size(), gray.type());
    Mat robertSharpenImg = Mat::zeros(image.size(), image.type());

    /*Sobel*/
    double cSob = 0.5;
    Mat sobelSharpen_bChannel = Mat::zeros(gray.size(), gray.type());
    Mat sobelSharpen_gChannel = Mat::zeros(gray.size(), gray.type());
    Mat sobelSharpen_rChannel = Mat::zeros(gray.size(), gray.type());
    Mat sobelSharpenImg = Mat::zeros(image.size(), image.type());

    char choice;
    while (1)
    {
        cout <<
            "清选择锐化模板\n" <<
            "1 Laplacian\n" <<
            "2 Robert\n" <<
            "3 Sobel\n" <<
            "按w返回上一级，按q退出：";
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
                LaplacianSharpen(bChannel, laplacianSharpen_bChannel, "B channel ", cLap, lablacianFilterNum);
                LaplacianSharpen(gChannel, laplacianSharpen_gChannel, "G channel ", cLap, lablacianFilterNum);
                LaplacianSharpen(rChannel, laplacianSharpen_rChannel, "R channel ", cLap, lablacianFilterNum);

                BGRchannels_merge.at(0) = laplacianSharpen_bChannel;
                BGRchannels_merge.at(1) = laplacianSharpen_gChannel;
                BGRchannels_merge.at(2) = laplacianSharpen_rChannel;
                merge(BGRchannels_merge, laplacianSharpenImg);

                namedWindow("Original - 原始图像", WINDOW_AUTOSIZE);
                imshow("Original - 原始图像", image);
                namedWindow("Laplacian Sharpen Img", WINDOW_AUTOSIZE);
                imshow("Laplacian Sharpen Img", laplacianSharpenImg);
                waitKey(0);
                destroyAllWindows();
                break;
            case '2':
                RobertSharpen(bChannel, laplacianSharpen_bChannel, "B channel ", cRob);
                RobertSharpen(gChannel, laplacianSharpen_gChannel, "G channel ", cRob);
                RobertSharpen(rChannel, laplacianSharpen_rChannel, "R channel ", cRob);

                BGRchannels_merge.at(0) = laplacianSharpen_bChannel;
                BGRchannels_merge.at(1) = laplacianSharpen_gChannel;
                BGRchannels_merge.at(2) = laplacianSharpen_rChannel;
                merge(BGRchannels_merge, laplacianSharpenImg);

                namedWindow("Original - 原始图像", WINDOW_AUTOSIZE);
                imshow("Original - 原始图像", image);
                namedWindow("Robert Sharpen Img", WINDOW_AUTOSIZE);
                imshow("Robert Sharpen Img", laplacianSharpenImg);
                waitKey(0);
                destroyAllWindows();
                break;
            case '3':
                SobelSharpen(bChannel, sobelSharpen_bChannel, "B channel ", cSob);
                SobelSharpen(gChannel, sobelSharpen_gChannel, "G channel ", cSob);
                SobelSharpen(rChannel, sobelSharpen_rChannel, "R channel ", cSob);

                BGRchannels_merge.at(0) = sobelSharpen_bChannel;
                BGRchannels_merge.at(1) = sobelSharpen_gChannel;
                BGRchannels_merge.at(2) = sobelSharpen_rChannel;
                merge(BGRchannels_merge, sobelSharpenImg);

                namedWindow("Original - 原始图像", WINDOW_AUTOSIZE);
                imshow("Original - 原始图像", image);
                namedWindow("Sobel Sharpen Img", WINDOW_AUTOSIZE);
                imshow("Sobel Sharpen Img", sobelSharpenImg);
                waitKey(0);
                destroyAllWindows();
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

int HighboostFilterProcessing()
{
    cout << "施工中..." << endl;
    return 0;
}
