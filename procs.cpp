#include "procs.h"

#define M_PI 3.14159265358979323846

int ProcsHelp()
{
    cout << "\nProcs --- SA16225037 程欣\n" << endl;
    cout <<
        "1 : BGR to HSI & HSI to BGR\n" << 
        "2 : show difference\n" << endl;
    return 0;
}

extern Mat image;
//IplImage *pimage;
extern Mat gray;
//IplImage *pgray;
extern Mat hsv;
int ProcsMain(string imagePath)
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

    ProcsHelp();
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
                test01();
                break;
            case '2':
                ShowDifference();
                break;
            case '3':
                break;
            case 'h':
                ProcsHelp();
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

double FindMax(double *p, int n)
{
    double max = p[0];
    while (n)
    {
        max = p[n - 1] > max ? p[n - 1] : max;
        --n;
    }
    return max;
}

double FindMin(double *p, int n)
{
    double min = p[0];
    while (n)
    {
        min = p[n - 1] < min ? p[n - 1] : min;
        --n;
    }
    return min;
}

int ConvertBGR2HSI(Mat src, Mat dst)
{
    if (src.type() != CV_8UC3)
    {
        printf("src type error\n");
        return 1;
    }
    if (dst.type() != CV_8UC3 && dst.type() != CV_64FC3)
    {
        printf("dst type error\n");
        return 2;
    }
    Mat dst_temp;
    dst.convertTo(dst_temp, CV_64FC3);

    double b, g, r;
    double bgr[3];
    double theta;

    for (int y = 0; y < src.rows; y++)
    {
        for (int x = 0; x < src.cols; x++)
        {
            b = double(src.at<Vec3b>(y, x)[0]) / 255.;
            g = double(src.at<Vec3b>(y, x)[1]) / 255.;
            r = double(src.at<Vec3b>(y, x)[2]) / 255.;
            bgr[0] = b;
            bgr[1] = g;
            bgr[2] = r;
            theta = pow(r - g, 2) + (r - b)*(g - b) == 0 ?
                (acos(0.5*((r - g) + (r - b)) / sqrt(0.0000001))) * 180. / M_PI : 
                (acos(0.5*((r - g) + (r - b)) / sqrt(pow(r - g, 2) + (r - b)*(g - b)))) * 180. / M_PI;
            dst_temp.at<Vec3d>(y, x)[0] = b > g ? (360. - theta) : theta;
            dst_temp.at<Vec3d>(y, x)[1] = 1. - (3. / (r + g + b))*FindMin(bgr, sizeof(bgr) / sizeof(bgr[0]));
            dst_temp.at<Vec3d>(y, x)[2] = (1. / 3.) * (r + g + b);

            dst_temp.at<Vec3d>(y, x)[0] *= 0.5;//[0-360]->[0,180]，保证能被uchar表示
            dst_temp.at<Vec3d>(y, x)[1] *= 255.;
            dst_temp.at<Vec3d>(y, x)[2] *= 255.;

            if (dst.type() == CV_8UC3)
            {
                dst.at<Vec3b>(y, x)[0] = cvRound(dst_temp.at<Vec3d>(y, x)[0]);
                dst.at<Vec3b>(y, x)[1] = cvRound(dst_temp.at<Vec3d>(y, x)[1]);
                dst.at<Vec3b>(y, x)[2] = cvRound(dst_temp.at<Vec3d>(y, x)[2]);
            }
            else
            {
                dst.at<Vec3d>(y, x)[0] = dst_temp.at<Vec3d>(y, x)[0];
                dst.at<Vec3d>(y, x)[1] = dst_temp.at<Vec3d>(y, x)[1];
                dst.at<Vec3d>(y, x)[2] = dst_temp.at<Vec3d>(y, x)[2];
            }
        }
    }

    return 0;
}

int ConvertHSI2BGR(Mat src, Mat dst)
{
    if (dst.type() != CV_8UC3)
    {
        printf("dst type error");
        return 1;
    }
    if (src.type() != CV_8UC3 && src.type() != CV_64FC3)
    {
        printf("src type error");
        return 2;
    }
    Mat src_temp;
    src.convertTo(src_temp, CV_64FC3);

    double h, s, i;
    double b, g, r;
    for (int y = 0; y < src.rows; y++)
    {
        for (int x = 0; x < src.cols; x++)
        {
            h = (src_temp.at<Vec3d>(y, x)[0] * 2. / 180.) * M_PI;
            s = src_temp.at<Vec3d>(y, x)[1] / 255.;
            i = src_temp.at<Vec3d>(y, x)[2] / 255.;

            if (i > (2. / 3.))
            {
                s = s > (2. / i - 2) ? 2. / i - 2 : s;
            }

            if (h >= 0 && h < 2. / 3. * M_PI)
            {
                b = i * (1 - s);
                r = i * (1 + (s * cos(h) / cos(1. / 3. * M_PI - h)));
                g = 3 * i - (r + b);
            }
            else if (h >= 2. / 3. * M_PI&&h < 4. / 3. * M_PI)
            {
                h = h - 2. / 3. * M_PI;
                r = i * (1 - s);
                g = i * (1 + (s * cos(h) / cos(1. / 3. * M_PI - h)));
                b = 3 * i - (r + g);
            }
            else if (h >= 4. / 3. * M_PI&&h <= 2. * M_PI)
            {
                h = h - 4. / 3. * M_PI;
                g = i * (1 - s);
                b = i * (1 + (s * cos(h) / cos(1. / 3. * M_PI - h)));
                r = 3 * i - (g + b);
            }
            else
            {
                cout << "h error : " << h << endl;
                //return -1;
            }

            dst.at<Vec3b>(y, x)[0] = saturate_cast<uchar>(cvRound(b * 255.));
            dst.at<Vec3b>(y, x)[1] = saturate_cast<uchar>(cvRound(g * 255.));
            dst.at<Vec3b>(y, x)[2] = saturate_cast<uchar>(cvRound(r * 255.));
        }
    }
    return 0;
}

int test01()
{
    Mat imhsi = Mat::ones(image.size(), image.type());
    //Mat imhsi = Mat::ones(image.size(), CV_64FC3);
    Mat imbgr = Mat::ones(image.size(), image.type());
    imbgr = image.clone();

    for (int i = 0; i < 10; i++)
    {
        ConvertBGR2HSI(imbgr, imhsi);
        ConvertHSI2BGR(imhsi, imbgr);
        //cout << i << endl;
    }

    namedWindow("test");
    imshow("test", imbgr);

    namedWindow("original");
    imshow("original", image);

    vector<Mat> HSIchannels;
    split(imhsi, HSIchannels);
    Mat hChannel, sChannel, iChannel;
    hChannel = HSIchannels.at(0);
    sChannel = HSIchannels.at(1);
    iChannel = HSIchannels.at(2);

    imshow("I channel", iChannel);

    vector<Mat> HSVchannels;
    split(hsv, HSIchannels);
    imshow("V channel", HSIchannels.at(2));

    waitKey(0);
    destroyAllWindows();
    return 0;
}

int ShowDifference()
{
    Mat a = imread("0.tif", IMREAD_GRAYSCALE);
    Mat b = imread("5.tif", IMREAD_GRAYSCALE);
    Mat dst = Mat::zeros(b.size(), b.type());

    Mat dst1 = Mat::zeros(b.size(), b.type());
    Mat dst2 = Mat::zeros(b.size(), b.type());
    Mat dst3 = Mat::zeros(b.size(), b.type());

    imshow("a im", a);
    imshow("b im", b);

    cout << a(Rect(0, 0, 1, 1)) << endl;
    cout << a(Rect(64, 64, 1, 1)) << endl;
    cout << a(Rect(128, 128, 1, 1)) << endl;

    Mat atemp, btemp, dsttemp;
    a.convertTo(atemp, CV_32F);
    b.convertTo(btemp, CV_32F);
    dst.convertTo(dsttemp, CV_32F);

    dst1.convertTo(dst1, CV_32F);
    dst2.convertTo(dst2, CV_32F);
    dst3.convertTo(dst3, CV_32F);


    //dsttemp = atemp - btemp;
    for (int y = 0; y < dst.rows; y++)
    {
        for (int x = 0; x < dst.cols; x++)
        {
            //dsttemp.at <float>(y, x) = b.at<uchar>(y, x) - a.at<uchar>(y, x);

            dsttemp.at <float>(y, x) =
                abs(b.at<uchar>(y, x) - a.at<uchar>(y, x)) < 1 ? 0 : b.at<uchar>(y, x) - a.at<uchar>(y, x);
            
            //dsttemp.at <float>(y, x) =
            //    abs(b.at<uchar>(y, x) - a.at<uchar>(y, x)) < 1 ? 0 : b.at<uchar>(y, x);
            if (a.at<uchar>(y, x) == 0)
            {
                dst1.at <float>(y, x) = b.at<uchar>(y, x) - a.at<uchar>(y, x);
            }
            if (a.at<uchar>(y, x) == 127)
            {
                dst2.at <float>(y, x) = b.at<uchar>(y, x) - a.at<uchar>(y, x);
            }
            if (a.at<uchar>(y, x) == 229)
            {
                dst3.at <float>(y, x) = b.at<uchar>(y, x) - a.at<uchar>(y, x);
            }
        }
    }
    //dsttemp(Rect(0, 0, 128, 128)) = 0;
    double max, min;
    minMaxLoc(dsttemp, &min, &max, 0, 0);
    dsttemp.convertTo(dst, CV_8U, 255. / (max - min), (-255.*min) / (max - min));
    //dsttemp.convertTo(dst, CV_8U, 1, -min);
    cout << " max " << max << " min " << min << endl;
    imshow("dif im", dst);


    minMaxLoc(dst1, &min, &max, 0, 0);
    dst1.convertTo(dst1, CV_8U, 1, -min);
    cout << " max1 " << max << " min1 " << min << endl;
    minMaxLoc(dst2, &min, &max, 0, 0);
    dst2.convertTo(dst2, CV_8U, 1, -min);
    cout << " max2 " << max << " min2 " << min << endl;
    minMaxLoc(dst3, &min, &max, 0, 0);
    dst3.convertTo(dst3, CV_8U, 1, -min);
    cout << " max3 " << max << " min3 " << min << endl;


    int hist[256];
    memset(hist, 0, 256 * sizeof(int));
    int histHeight = 256;
    int maxh;
    int *pmax = &maxh;
    Mat histImg1 = Mat::zeros(histHeight, 256, CV_8U);
    Mat histImg2 = Mat::zeros(histHeight, 256, CV_8U);
    Mat histImg3 = Mat::zeros(histHeight, 256, CV_8U);

    //Mat histImg(histHeight, 256, CV_8U, Scalar(0));

    CalcNormalizedHistogram(dst, histImg1, histHeight, pmax, hist, Scalar(255));
    imshow("dif hist", histImg1);
    CalcNormalizedHistogram(a, histImg2, histHeight, pmax, hist, Scalar(255));
    imshow("a", histImg2);
    CalcNormalizedHistogram(b, histImg3, histHeight, pmax, hist, Scalar(255));
    imshow("b", histImg3);

    CalcNormalizedHistogram(dst1, histImg1, histHeight, pmax, hist, Scalar(255));
    imshow("dst1", histImg1);
    CalcNormalizedHistogram(dst2, histImg2, histHeight, pmax, hist, Scalar(255));
    imshow("dst2", histImg2);
    CalcNormalizedHistogram(dst3, histImg3, histHeight, pmax, hist, Scalar(255));
    imshow("dst3", histImg3);



    waitKey(0);
    destroyAllWindows();
    return 0;
}