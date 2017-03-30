# 实验2 直方图均衡

## 实验要求

1. 计算灰度图像的归一化直方图。   
    具体内容：利用OpenCV对图像像素进行操作，计算归一化直方图，并在 窗口中以图形的方式显示出来 
2. 灰度图像直方图均衡处理  
    具体内容：通过计算归一化直方图,设计算法实现直方图均衡化处理。 
3. 彩色图像直方图均衡处理  
    具体内容： 在灰度图像直方图均衡处理的基础上实现彩色直方图均衡处理。

## 实验完成情况

_包括完成的实验内容及每个实验的完成程度_

完成了4项内容：

1. 显示归一化直方图  
    两个函数，一个CalcNormalizedHistogram()是本实验主要的一个函数，用于计算直方图数组以及直方图的图像。接口中，img是需要统计直方图的灰度图像，histImg是直方图图像，pmax指向直方图中最大值，hist是大小为256的直方图数组，用于统计每个灰度级像素点个数，color是直方图显示的颜色，主要针对BGR图像的直方图的显示。ShowNormalizedHistogram()就是简单显示一下直方图。
    ```C++
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
    ```

2. 灰度图像直方图均衡处理   
    一开始按照课本上的算法进行直方图均衡，后来发现对于有较大黑暗区域的图像，比如实验用的moon.jpg，原始图像0灰度级经过均衡化计算后会映射到120左右，使得图片看起来偏灰，因此后来又对图片进行了一下标定，将均衡后的图像灰度级再拉伸到原始图像的灰度级范围内。一开始是在映射之后对图像进行的标定，与直接调用API的结果略有不同。后来发现应该在映射之前对灰度级进行标定，所得结果就跟调用官方API的一样了。
    ```C++
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
                histEquImg.at<uchar>(y, x) = saturate_cast<uchar>(int(table[gray.at<uchar>(y, x)] + 0.5));//int(double + 0.5)用来四舍五入
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

    ```

3. BGR彩色图像三通道直方图均衡处理   
    三个通道BGR分别进行直方图均衡，最后合起来，但BGR的变换程度不一，会导致一个问题，就是原始图像的色度会发生改变。
    ```C++
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
                b_HistEquImg.at<uchar>(y, x) = saturate_cast<uchar>(int(b_table[bChannel.at<uchar>(y, x)] + 0.5));
                g_HistEquImg.at<uchar>(y, x) = saturate_cast<uchar>(int(g_table[gChannel.at<uchar>(y, x)] + 0.5));
                r_HistEquImg.at<uchar>(y, x) = saturate_cast<uchar>(int(r_table[rChannel.at<uchar>(y, x)] + 0.5));
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

    ```

4. HSV彩色图像V通道直方图均衡处理  
    为了不改变图像色度，将BGR图像转换为HSV图像，只对亮度进行直方图均衡，在不改变图像色度的情况下实现对图像的增强。
    ```C++
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
                v_HistEquImg.at<uchar>(y, x) = saturate_cast<uchar>(int(v_table[vChannel.at<uchar>(y, x)] + 0.5));
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
    ```
 

## 实验中的问题

_包括在实验中遇到的问题，以及解决问题的方法_

1. 主要的问题就是直方图均衡的算法，要在课本上的算法之后再对图像进行一下拉伸。我是按照原始图像的灰度范围进行的拉伸，调用了OpenCV的资方图均衡API发现OpenCV中的equalizeHist是将图像灰度级拉伸到[0,255]区间内。

2. 算法当中一个小问题，包括所有double型计算中，整型（包括int，uchar等）与double掺杂运算需要对整型数据进行类型转换，并且double转到整型时需要进行四舍五入，不然只会取整数位在很多情况下会造成1灰度级的误差。我的代码中直接用int(double + 0.5)进行的四舍五入。

3. 程序=算法+数据结构，这几次实验的算法都不算复杂，只需要注意几个小坑，不然算法出了类似四舍五入一类的小问题很不容易发现，我现在的直方图均衡代码，虽然大的方面从直方图图像以及增强效果来看已经没问题了，也依旧不能完全肯定里面是否仍有些小问题。

4. 另外会比较容易出错以及浪费时间的是数据结构，包括数据类型的用法，函数接口的使用，个别时候还可能因为C/C++的语法不熟出问题。这都需要高效的信息检索能力，需要找到相关资料确定用法以及可能的错误。不过这都是开始时的问题，慢慢写的多了熟了这类基础问题不会再发生。

5. 这次实验第2部分没有写一个接口，导致后面BGR和HSV都有大量重复代码，复制粘贴然后机械重复修改耗时耗力而且没什么意义，是需要在今后实验中改进的地方。

## 实验结果

[实验2源码地址](https://github.com/chengx-coding/USTC-MSE_DIP_Exp/blob/master/Exp02.cpp)


---
##### [返回实验主页](https://chengx-coding.github.io/USTC-MSE_DIP_Exp/)