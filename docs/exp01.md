## 实验名称

实验1 图像的灰度变换

## 实验要求

1. 利用OpenCV读取图像  
	具体内容：用打开OpenCV打开图像，并在窗口中显示。	
2. 灰度图像二值化处理  
	具体内容：设置并调整阈值对图像进行二值化处理。   	
3. 灰度图像的对数变换	  
	具体内容：设置并调整r值对图像进行对数变换。 	
4. 灰度图像的伽马变换	  
	具体内容：设置并调整γ值对图像进行伽马变换。	
5. 彩色图像的补色变换  
	具体内容：对彩色图像进行补色变换。 	


## 实验完成情况

_包括完成的实验内容及每个实验的完成程度_

完成了8项内容：

1. 利用OpenCV读取图像。  
    ```C++
    int ShowImg()
    {
        namedWindow("Original - 原始图像", WINDOW_AUTOSIZE);
        imshow("Original - 原始图像", image);
        namedWindow("Gray - 灰度图像", WINDOW_AUTOSIZE);
        imshow("Gray - 灰度图像", gray);

        waitKey(0);
        destroyAllWindows();
        return 0;
    }
    ```

2. 灰度图像二值化处理   
    逐个图像像素扫描操作，灰度值大于等于threshold置为255，小于threshold置为0。还增加了一个trackbar用于调节threshold。
    ```C++
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
        return 0;
    }
    ```

3. 灰度图像的线性变换   
    逐个图像像素扫描操作，将图像中的灰度级重新映射到minValue到maxValue的范围中。这两个值可以小于0或大于255。被映射到[0,255]区间之外的灰度级会经过saturate_cast<>()函数置为0或255。LinearTransProcessing函数用于进行线性变换，LinearTrans函数用于显示，以及增加一个调节映射区间的trackbar。函数接口仿照OpenCV的API的一般写法写的。
    ```C++
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

    ```

4. 灰度图像的对数变换  
    逐个图像像素扫描操作，按对数变换公式计算。如果通过调节系数c使得对数变换后的灰度级能对应到[0,255]区间内，那么底数的变化不会影响变换结果，对数变换将始终遵循：  
    $$ s = 255log_{256}(r + 1) $$
    ```C++
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
    ```
   
5. 灰度图像的伽马变换  
	逐个图像像素扫描操作，按伽马变换公式$ s = cr^\gamma $计算。指数可以通过trackbar调节，调节范围0.1 - 1 - 10，分别对应较小和较大灰度级的拉伸。
    ```C++
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
    ```

6. 彩色图像的补色变换  
	这种变换是直接用255减去BGR通道的值，得到的图像色调与原来相反，但饱和度和亮度也随之变化。
    ```C++
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
    ```

7. 彩色图像色调取反变换
	这种变换是将BGR图像转换成HSV图像，其中H通道的取值范围是[0,179]，对应360度色度盘。要注意色度取反不是用179去减，而是分小于和大于90两种情况。H通道和V通道不变，最后得到的图像饱和度和亮度都得到了保留，与前一个实验结果形成对比。
	```C++
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
	```

8. 摄像头视频图像二值化  
	这个很有意思，是开启摄像头后对每帧图像的BGR通道进行二值化，每个通道的阈值有两个，一个low一个high，在区间内的灰度级为255，区间外为0。三个通道一共六个阈值由trackbar进行调节。通过这种方法可以对特定颜色的目标进行识别，测试的时候效果还不错，通过调节trackbar可以锁定某些特定颜色的物体。因为自己逐个像素扫描太慢没法完全满足摄像头图像的实时要求，所以取阈值直接用了inRange函数。
	```C++
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
	```

## 实验中的问题

_包括在实验中遇到的问题，以及解决问题的方法_

1. 环境配置问题，对于OpenCV 3.2.0,链接器的附加依赖项只需opencv_world320d.lib即可。64位机器要配置x64属性并在x64下Debug
2. 数据结构问题，opencv有大量自己的数据结构，需要查看官方文档了解其意义和用法。
3. 图像操作问题，实际上也包含了数据结构问题，有许多不同类型相同的功能函数，比如cvLoadImage和imread两个函数都是读取图像，前者返回的是指向图像的指针，后者返回的是图像对象，类似的情况有很多，如cvCvtGray和CvtGray等等，网络上不同的示例有不同的用法，增加了一定困难，百度还是不如直接看官方文档。
4. 有些比较早的参考资料使用了Iplmage等指针对图像进行操作。Iplmage是C语言的数据结构，opencv 2以后引入了新的C++接口，多是对于对象的操作，很多针对OpenCV 2以后版本的资料上都建议用新的数据结构和函数


## 实验结果

[实验1源码地址](https://github.com/chengx-coding/USTC-MSE_DIP_Exp/blob/master/Exp01.cpp)


---
##### [返回实验主页](https://chengx-coding.github.io/USTC-MSE_DIP_Exp/)