#include "Exp05.h"

#define IDEAL 0
#define BUTTERWORTH 1

int Exp05Help()
{
    cout << "\nExp05 --- SA16225037 程欣\n" << endl;
    cout <<
        "1 : 灰度图像的DFT和IDFT\n" <<
        "2 : 利用理想高通和理想低通滤波器对灰度图像进行频率域滤波\n" <<
        "3 : 利用布特沃斯高通和低通滤波器对灰度图像进行频率域滤波\n" << endl;
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
                cout << "无效的输入" << endl;
                break;
            }
        }
    }
    return 0;
}

int ShowSpectrum(Mat complexI, int centered)
{
    Mat planes[] = { Mat::zeros(complexI.size(), CV_32F), Mat::zeros(complexI.size(), CV_32F) };

    split(complexI, planes); // 将多通道数组complexI分离成几个单通道数组，planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude  
    Mat magnitudeImage = planes[0];

    //进行对数尺度(logarithmic scale)缩放，将复数转换为幅值，即=> log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    magnitudeImage += Scalar::all(1);
    log(magnitudeImage, magnitudeImage);//求自然对数

    if (centered)//如果需要中心化
    {
        //剪切和重分布幅度图象限。若有奇数行或奇数列，进行频谱裁剪
        magnitudeImage = magnitudeImage(Rect(0, 0, magnitudeImage.cols & -2, magnitudeImage.rows & -2));
        //重新排列傅立叶图像中的象限，使得原点位于图像中心  
        int cx = magnitudeImage.cols / 2;
        int cy = magnitudeImage.rows / 2;
        Mat q0(magnitudeImage, Rect(0, 0, cx, cy));// ROI区域的左上
        Mat q1(magnitudeImage, Rect(cx, 0, cx, cy));// ROI区域的右上
        Mat q2(magnitudeImage, Rect(0, cy, cx, cy));// ROI区域的左下
        Mat q3(magnitudeImage, Rect(cx, cy, cx, cy));// ROI区域的右下
        //交换象限（左上与右下进行交换）
        Mat tmp;
        q0.copyTo(tmp);
        q3.copyTo(q0);
        tmp.copyTo(q3);
        //交换象限（右上与左下进行交换）
        q1.copyTo(tmp);
        q2.copyTo(q1);
        tmp.copyTo(q2);
    }
    //归一化，用0到1之间的浮点值将矩阵变换为可视的图像格式
    magnitudeImage = magnitudeImage(Rect(0, 0, gray.cols, gray.rows));
    //normalize(magnitudeImage, magnitudeImage, 0, 1, CV_MINMAX); //OpenCV2版代码
    normalize(magnitudeImage, magnitudeImage, 0, 1, NORM_MINMAX); 

    //显示效果图
    imshow("频谱幅值（已进行对数变换）", magnitudeImage);
    //waitKey(0);
    //destroyAllWindows();
    return 0;
}

int DFTandIDFT()
{
    Mat src = gray;
    imshow("原图像", src);

    //src.convertTo(src, CV_32F);
    ////乘 -1^(x+y)，使频率域图像中心化。最后处理完成后再转回来
    //for (int i = 0; i<src.rows; i++)
    //{
    //    float *p = src.ptr<float>(i);
    //    for (int j = 0; j<src.cols; j++)
    //    {
    //        p[j] = p[j] * pow(-1, i + j);
    //    }
    //}

    //将输入图像延扩到最佳的尺寸，边界用0补充
    int m = getOptimalDFTSize(src.rows);
    int n = getOptimalDFTSize(src.cols);
    //将添加的像素初始化为0.
    Mat padded;
    copyMakeBorder(src, padded, 0, m - src.rows, 0, n - src.cols, BORDER_CONSTANT, Scalar::all(0));

    Mat complexI = Mat::zeros(padded.size(), CV_32F);

    //为傅立叶变换的结果(实部和虚部)分配存储空间。
    Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };

    ////若增加DFT_COMPLEX_OUTPUT参数则可省略这一步
    ////将planes数组组合合并成一个多通道的数组complexI
    //merge(planes, 2, complexI);
    //dft(complexI, complexI);//进行就地离散傅里叶变换

    padded.convertTo(padded, CV_32F);
    //增加DFT_COMPLEX_OUTPUT则前面不用再为complexI扩展一个通道，会自动扩展
    dft(padded, complexI, DFT_COMPLEX_OUTPUT);

    ShowSpectrum(complexI, 1);

    idft(complexI, complexI, DFT_SCALE | DFT_REAL_OUTPUT);//加DFT_SCALE参数是为了缩一下结果的值

    split(complexI, planes);
    complexI = planes[0];

    Mat dst;
    complexI.convertTo(dst, CV_8U);
    namedWindow("Fourier反变换后");
    imshow("Fourier反变换后", dst);

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
    imshow("原图像", src);
    src.convertTo(src, CV_32F);
    //乘 -1^(x+y)，使频率域图像中心化。最后处理完成后再转回来
    for (int i = 0; i < src.rows; i++)
    {
        float *p = src.ptr<float>(i);
        for (int j = 0; j < src.cols; j++)
        {
            p[j] = p[j] * pow(-1, i + j);
        }
    }

    //将输入图像延扩到最佳的尺寸，边界用0补充
    int m = getOptimalDFTSize(src.rows);
    int n = getOptimalDFTSize(src.cols);
    //将添加的像素初始化为0.
    Mat padded;
    copyMakeBorder(src, padded, 0, m - src.rows, 0, n - src.cols, BORDER_CONSTANT, Scalar::all(0));

    Mat complexI = Mat::zeros(padded.size(), CV_32F);
    //为傅立叶变换的结果(实部和虚部)分配存储空间。
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

    namedWindow("调节窗口", WINDOW_NORMAL);
    createTrackbar("截止频率x10", "调节窗口", &d0control, int(sqrt(powf(padded.cols, 2) + powf(padded.rows, 2)) * 5));
    createTrackbar("n", "调节窗口", &nButterworth, 10);
    createTrackbar("低/高通", "调节窗口", &c, 1);
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
        imshow("滤波后图像", dst);


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
