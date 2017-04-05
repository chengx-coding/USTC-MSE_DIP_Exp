#include "Exp05.h"

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
                DFTProcessing();
                break;
            case '2':
                break;
            case '3':
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

int ShowDFT(Mat complexI)
{
    Mat planes[] = { Mat::zeros(complexI.size(), CV_32F), Mat::zeros(complexI.size(), CV_32F) };

    split(complexI, planes); // 将多通道数组complexI分离成几个单通道数组，planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude  
    Mat magnitudeImage = planes[0];

    //进行对数尺度(logarithmic scale)缩放，将复数转换为幅值，即=> log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    magnitudeImage += Scalar::all(1);
    log(magnitudeImage, magnitudeImage);//求自然对数

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

    //归一化，用0到1之间的浮点值将矩阵变换为可视的图像格式
    //normalize(magnitudeImage, magnitudeImage, 0, 1, CV_MINMAX); //OpenCV2版代码
    normalize(magnitudeImage, magnitudeImage, 0, 1, NORM_MINMAX); 

    //显示效果图
    imshow("频谱幅值", magnitudeImage);
    waitKey(0);
    destroyAllWindows();
    return 0;
}

int DFTProcessing()
{
    Mat src = gray;
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

    //ShowDFT(complexI);

    idft(complexI, complexI, DFT_SCALE | DFT_REAL_OUTPUT);//加DFT_SCALE参数是为了缩一下结果的值

    split(complexI, planes);
    complexI = planes[0];

    Mat dst = Mat::zeros(src.size(), src.type());
    complexI.convertTo(dst, CV_8U);
    namedWindow("2");
    imshow("2", dst);


    cout << src(Rect(0, 0, 5, 5)) << endl;
    cout << dst(Rect(0, 0, 5, 5)) << endl;

    waitKey(0);
    destroyAllWindows();

    return 0;
}

