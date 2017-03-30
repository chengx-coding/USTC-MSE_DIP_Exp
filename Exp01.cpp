#include "Exp01.h"
#define M_SAVEFLAG 0

int Exp01Help()
{
    cout << "\nExp01 --- SA16225037 程欣\n" << endl;
    cout <<
        "1 : 显示原始图像和灰度图像\n" <<
        "2 : 灰度图像二值化\n" <<
        "3 : 灰度图像线性变换\n" <<
        "4 : 灰度图像对数变换\n" <<
        "5 : 灰度图像伽马变换\n" <<
        "6 : 彩色图像补色变换\n" <<
        "7 : 彩色图像色调取反变换\n" <<
        "8 : 摄像头视频图像二值化\n" << endl;
    return 0;
}

extern Mat image;
//IplImage *pimage;
extern Mat gray;
//IplImage *pgray;
extern Mat hsv;
int Exp01Main(string imagePath)
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

    Exp01Help();
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
                ShowImg();
                break;
            case '2':
                Binarization();
                break;
            case '3':
                LinearTrans();
                break;
            case '4':
                LogTrans();
                break;
            case '5':
                GamaTrans();
                break;
            case '6':
                ComplementaryColorTrans();
                break;
            case '7':
                HueInvertColorTrans();
                break;
            case '8':
                CapThreshold();
                break;
            case 'h':
                Exp01Help();
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

int ShowImg()
{
    namedWindow("Original - 原始图像", WINDOW_AUTOSIZE);
    imshow("Original - 原始图像", image);
    namedWindow("Gray - 灰度图像", WINDOW_AUTOSIZE);
    imshow("Gray - 灰度图像", gray);
    if(M_SAVEFLAG)
        imwrite("010101.jpg", gray);

    waitKey(0);
    destroyAllWindows();
    return 0;
}

int Binarization()
{
    Mat binaImg = Mat::zeros(gray.size(), gray.type());
    //uchar threshold_val;
    int threshold_val;
    threshold_val = 128;

    namedWindow("Gray - 灰度图像", WINDOW_AUTOSIZE);
    imshow("Gray - 灰度图像", gray);
    namedWindow("Binarization", WINDOW_AUTOSIZE);

    createTrackbar("Threshold", "Binarization", &threshold_val, 255);
    for (;;)
    {
        for (int y = 0; y < gray.rows; y++)
        {
            for (int x = 0; x < gray.cols; x++)
            {
                //saturate_cast<uchar>();
                binaImg.at<uchar>(y, x) = gray.at<uchar>(y, x) >= threshold_val ? 255 : 0;
            }
        }

        imshow("Binarization", binaImg);
        if (waitKey(10)!=255)
        {
            destroyAllWindows();
            break;
        }
    }
    if (M_SAVEFLAG)
        imwrite("010201.jpg", binaImg);
    return 0;
}

int LinearTransProcessing(Mat src, Mat dst, int minValue, int maxValue)
{
    double srcMin, srcMax;
    minMaxLoc(src, &srcMin, &srcMax, 0, 0);

    /*参数检查*/
    if (dst.type() != CV_8U)
    {
        cout << "type error at LinearTrans" << endl;
        return -1;
    }
    if (dst.size() != src.size())
    {
        cout << "size error at LinearTrans: dst.size() != src.size()" << endl;
        return -2;
    }
    if (srcMin == srcMax)
    {
        dst = src.clone();
        return 0;
    }

    if (src.type() == CV_8U)
    {
        for (int y = 0; y < src.rows; y++)
        {
            for (int x = 0; x < src.cols; x++)
            {
                dst.at<uchar>(y, x) =
                    saturate_cast<uchar>(minValue + int((double(src.at<uchar>(y, x)) - srcMin) / (srcMax - srcMin)*double(maxValue - minValue) + 0.5));//int(double + 0.5)用来四舍五入
            }
        }
    }
    else if (src.type() == CV_64F)
    {
        for (int y = 0; y < src.rows; y++)
        {
            for (int x = 0; x < src.cols; x++)
            {
                dst.at<uchar>(y, x) =
                    saturate_cast<uchar>(minValue + int((src.at<double>(y, x) - srcMin) / (srcMax - srcMin)*double(maxValue - minValue) + 0.5));
            }
        }
    }
    else
    {
        cout << "type error at LinearTrans" << endl;
        return -1;
    }
    return 0;
}

int LinearTrans()
{
    Mat linearImg = Mat::zeros(gray.size(), gray.type());
    int max = 255, min = 0;
    int baseBias = 255;
    int maxBias = baseBias, minBias = baseBias;

    namedWindow("Gray - 灰度图像", WINDOW_AUTOSIZE);
    imshow("Gray - 灰度图像", gray);
    namedWindow("LinearTrans", WINDOW_AUTOSIZE);

    createTrackbar("最大值", "LinearTrans", &max, 255);
    createTrackbar("最小值", "LinearTrans", &min, 255);
    createTrackbar("maxBias+baseBias", "LinearTrans", &maxBias, 2*baseBias);
    createTrackbar("minBias+baseBias", "LinearTrans", &minBias, 2*baseBias);

    for (;;)
    {
        LinearTransProcessing(gray, linearImg, min + minBias - baseBias, max + maxBias - baseBias);
        imshow("LinearTrans", linearImg);
        if (waitKey(10) != 255)
        {
            destroyAllWindows();
            break;
        }
    }

    return 0;
}

int LogTrans()
{
    double c = 255 / (log(1 + 255));//以e为底时的系数45.99,对应0-255的映射
    Mat logImg = Mat::zeros(gray.size(), gray.type());
 
    for (int y = 0; y < gray.rows; y++)
    {
        for (int x = 0; x < gray.cols; x++)
        {
            logImg.at<uchar>(y, x) = saturate_cast<uchar>(c*log(1 + gray.at<uchar>(y, x)));
        }
    }
    namedWindow("Gray - 灰度图像", WINDOW_AUTOSIZE);
    imshow("Gray - 灰度图像", gray);
    namedWindow("LogTrans", WINDOW_AUTOSIZE);
    imshow("LogTrans", logImg);
    waitKey(0);
    destroyAllWindows();

    return 0;
}

int GamaTrans()
{
    Mat gamaImg = Mat::zeros(gray.size(), gray.type());
    //uchar threshold_val;
    double c;
    double gama, gama_pre;
    int gama_linear=100;
    int max_gama_linear = 200;
    gama = 1;

    namedWindow("Gray - 灰度图像", WINDOW_AUTOSIZE);
    imshow("Gray - 灰度图像", gray);
    namedWindow("GamaTrans", WINDOW_AUTOSIZE);

    createTrackbar("指数调节", "GamaTrans", &gama_linear, max_gama_linear);
    for (;;)
    {
        gama_pre = gama;
        gama = (double)pow(10, (double)((double)(gama_linear - max_gama_linear / 2) / (double)(max_gama_linear / 2)));
        c = (double)255 / (pow(255, (double)gama));
        if (fabs(gama - gama_pre) > 0.000001)
        {
            cout << "此时指数为：" << gama << endl;
        }
        
        for (int y = 0; y < gray.rows; y++)
        {
            for (int x = 0; x < gray.cols; x++)
            {
                gamaImg.at<uchar>(y, x) = saturate_cast<uchar>(c*pow(gray.at<uchar>(y, x), (double)gama));
            }
        }
        imshow("GamaTrans", gamaImg);
        if (waitKey(10) != 255)
        {
            destroyAllWindows();
            break;
        }
    }
    return 0;
}

int ComplementaryColorTrans()
{
    Mat complementaryColorImg = Mat::zeros(image.size(), image.type());
    for (int y = 0; y < image.rows; y++) 
    {
        for (int x = 0; x < image.cols; x++)
        {
            complementaryColorImg.at<Vec3b>(y, x)[0] = 255 - image.at<Vec3b>(y, x)[0];
            complementaryColorImg.at<Vec3b>(y, x)[1] = 255 - image.at<Vec3b>(y, x)[1];
            complementaryColorImg.at<Vec3b>(y, x)[2] = 255 - image.at<Vec3b>(y, x)[2];
        }
    }
    namedWindow("Original - 原始图像", WINDOW_AUTOSIZE);
    imshow("Original - 原始图像", image);
    namedWindow("Complementary Color Trans", WINDOW_AUTOSIZE);
    imshow("Complementary Color Trans", complementaryColorImg);
    waitKey(0);
    destroyAllWindows();

    return 0;
}

int HueInvertColorTrans()
{
    vector<Mat> HSVchannels;
    split(hsv, HSVchannels);
    Mat hChannel, sChannel, vChannel;
    hChannel = HSVchannels.at(0);
    sChannel = HSVchannels.at(1);
    vChannel = HSVchannels.at(2);

    /* show the full color situation */
    //Mat fullTable = 255 * Mat::ones(1, 256, CV_8U);
    //LUT(sChannel, fullTable, sChannel);
    //LUT(vChannel, fullTable, vChannel);
    //HSVchannels.at(1) = sChannel;
    //HSVchannels.at(2) = vChannel;

    //Mat fullColorImage;
    //merge(HSVchannels, fullColorImage);
    //cvtColor(fullColorImage, fullColorImage, CV_HSV2BGR);
    //namedWindow("Full Color Image", WINDOW_AUTOSIZE);
    //imshow("Full Color Image", fullColorImage);

    /* LUT函数快速扫描赋值 */
    Mat hueInvertColorTable = Mat::zeros(1, 256, CV_8U);//这里必须是256，不然会报中断错误
    uchar *p = hueInvertColorTable.ptr();
    for (int i = 0; i < 90; i++)
    {
        p[i] = i + 90;
    }
    for (int i = 90; i < 180; i++)
    {
        p[i] = i - 90;
    }
    LUT(hChannel, hueInvertColorTable, hChannel);    
    HSVchannels.at(0) = hChannel;

    Mat hueInvertColorImg;
    merge(HSVchannels, hueInvertColorImg);
    cvtColor(hueInvertColorImg, hueInvertColorImg, CV_HSV2BGR);

    namedWindow("Original - 原始图像", WINDOW_AUTOSIZE);
    imshow("Original - 原始图像", image);
    namedWindow("Hue Invert Color Image", WINDOW_AUTOSIZE);
    imshow("Hue Invert Color Image", hueInvertColorImg);

    waitKey(0);
    destroyAllWindows();

    return 0;
}

int CapThreshold()
{
    int low_r = 100, low_g = 100, low_b = 100;
    int high_r = 200, high_g = 200, high_b = 200;
    Mat frame, frame_threshold;
    VideoCapture cap(0);
    namedWindow("Video Capture", WINDOW_NORMAL);
    namedWindow("Binary Image", WINDOW_AUTOSIZE);
    namedWindow("Binary Image Adjust", WINDOW_NORMAL);
    
    for (;;)
    {
        cap >> frame;
        if (frame.empty())
            break;
        createTrackbar("Low R", "Binary Image Adjust", &low_r, 255);
        createTrackbar("High R", "Binary Image Adjust", &high_r, 255);
        createTrackbar("Low G", "Binary Image Adjust", &low_g, 255);
        createTrackbar("High G", "Binary Image Adjust", &high_g, 255);
        createTrackbar("Low B", "Binary Image Adjust", &low_b, 255);
        createTrackbar("High B", "Binary Image Adjust", &high_b, 255);
        inRange(frame, Scalar(low_b, low_g, low_r), Scalar(high_b, high_g, high_r), frame_threshold);
        imshow("Video Capture", frame);
        imshow("Binary Image", frame_threshold);
        if (waitKey(10) != 255)
        {
            destroyAllWindows();
            break;
        }
    }
    destroyAllWindows();
    return 0;
}
