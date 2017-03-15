#include "Exp01.h"

int help()
{
    cout << "Exp01 --- SA16225037 ����" << endl;
    cout<< "1 : ��ʾԭʼͼ��ͻҶ�ͼ��\n"
        << "2 : �Ҷ�ͼ���ֵ��\n"
        << "3 : �Ҷ�ͼ������任\n"
        << "4 : �Ҷ�ͼ��٤��任\n"
        << "5 : ��ɫͼ��ɫ�任\n"
        << "6 : ����ͷ��Ƶͼ���ֵ��\n" << endl;
    return 0;
}


Mat image;
//IplImage *pimage;
Mat gray;
//IplImage *pgray;
int Exp01Main(char *imagePath)
{
    char chioce;
    image = imread(imagePath);
    gray = imread(imagePath, IMREAD_GRAYSCALE);
    //pimage = cvLoadImage("test.jpg");
    if (image.empty())
    {
        cout << "Could not open or find the image" << std::endl;
        return -1;
    }

    help();
    while (1)
    {
        cout << "��ѡ��Ҫ���еĳ��򣬰�h��������q�˳���";
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
                ShowImg();
                break;
            case '2':
                Binarization();
                break;
            case '3':
                LogTrans();
                break;
            case '4':
                GamaTrans();
                break;
            case '5':
                ComplementaryColorTrans();
                break;
            case '6':
                CapThreshold();
                break;
            case 'h':
                help();
                break;
            default:
                cout << "��Ч������" << endl;
                break;
            }
        }
    }
    return 0;
}

int ShowImg()
{
    namedWindow("Original - ԭʼͼ��", WINDOW_AUTOSIZE);
    imshow("Original - ԭʼͼ��", image);
    namedWindow("Gray - �Ҷ�ͼ��", WINDOW_AUTOSIZE);
    imshow("Gray - �Ҷ�ͼ��", gray);

    waitKey(0);
    destroyAllWindows();
    return 0;
}

void threshold_trackbar(int threshold_val, void *)
{
    setTrackbarPos("Threshold", "Binarization", threshold_val);
}

int Binarization()
{
    Mat bina_image = Mat::zeros(gray.size(), gray.type());
    //uchar threshold_val;
    int threshold_val;
    threshold_val = 128;

    namedWindow("Gray - �Ҷ�ͼ��", WINDOW_AUTOSIZE);
    imshow("Gray - �Ҷ�ͼ��", gray);
    namedWindow("Binarization", WINDOW_AUTOSIZE);

    createTrackbar("Threshold", "Binarization", &threshold_val, 255);
    for (;;)
    {
        for (int y = 0; y < gray.rows; y++)
        {
            for (int x = 0; x < gray.cols; x++)
            {
                //saturate_cast<uchar>();
                bina_image.at<uchar>(y, x) = gray.at<uchar>(y, x) >= threshold_val ? 255 : 0;
            }
        }

        imshow("Binarization", bina_image);
        if (waitKey(10)!=255)
        {
            destroyAllWindows();
            break;
        }
    }
    return 0;
}

int LogTrans()
{
    double c = 255 / (log(1 + 255));//��eΪ��ʱ��ϵ��45.99,��Ӧ0-255��ӳ��
    Mat log_image = Mat::zeros(gray.size(), gray.type());
 
    for (int y = 0; y < gray.rows; y++)
    {
        for (int x = 0; x < gray.cols; x++)
        {
            log_image.at<uchar>(y, x) = saturate_cast<uchar>(c*log(1 + gray.at<uchar>(y, x)));
        }
    }
    namedWindow("Gray - �Ҷ�ͼ��", WINDOW_AUTOSIZE);
    imshow("Gray - �Ҷ�ͼ��", gray);
    namedWindow("LogTrans", WINDOW_AUTOSIZE);
    imshow("LogTrans", log_image);
    waitKey(0);
    destroyAllWindows();

    return 0;
}

int GamaTrans()
{
    Mat gama_image = Mat::zeros(gray.size(), gray.type());
    //uchar threshold_val;
    double c;
    double gama, gama_pre;
    int gama_linear=100;
    int max_gama_linear = 200;
    gama = 1;

    namedWindow("Gray - �Ҷ�ͼ��", WINDOW_AUTOSIZE);
    imshow("Gray - �Ҷ�ͼ��", gray);
    namedWindow("GamaTrans", WINDOW_AUTOSIZE);

    createTrackbar("ָ������", "GamaTrans", &gama_linear, max_gama_linear);
    for (;;)
    {
        gama_pre = gama;
        gama = (double)powf(10, (double)((double)(gama_linear - max_gama_linear / 2) / (double)(max_gama_linear / 2)));
        c = (double)255 / (powf(255, (double)gama));
        if (fabs(gama - gama_pre) > 0.000001)
        {
            cout << "��ʱָ��Ϊ��" << gama << endl;
        }
        
        for (int y = 0; y < gray.rows; y++)
        {
            for (int x = 0; x < gray.cols; x++)
            {
                gama_image.at<uchar>(y, x) = saturate_cast<uchar>(c*powf(gray.at<uchar>(y, x), (double)gama));
            }
        }
        imshow("GamaTrans", gama_image);
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
    Mat comcolor_image = Mat::zeros(image.size(), image.type());
    for (int y = 0; y < image.rows; y++) 
    {
        for (int x = 0; x < image.cols; x++)
        {
            comcolor_image.at<Vec3b>(y, x)[0] = 255 - image.at<Vec3b>(y, x)[0];
            comcolor_image.at<Vec3b>(y, x)[1] = 255 - image.at<Vec3b>(y, x)[1];
            comcolor_image.at<Vec3b>(y, x)[2] = 255 - image.at<Vec3b>(y, x)[2];
        }
    }
    namedWindow("Original - ԭʼͼ��", WINDOW_AUTOSIZE);
    imshow("Original - ԭʼͼ��", image);
    namedWindow("Complementary Color Trans", WINDOW_AUTOSIZE);
    imshow("Complementary Color Trans", comcolor_image);
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
