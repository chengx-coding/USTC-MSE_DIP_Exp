## 实验名称

实验2 直方图均衡

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
    一开始按照课本上的算法进行直方图均衡，后来发现对于有较大黑暗区域的图像，比如实验用的moon.jpg，原始图像0灰度级经过均衡化计算后会映射到120左右，使得图片看起来偏灰，因此后来又对图片进行了一下标定，将均衡后的图像灰度级再拉伸到原始图像的灰度级范围内。
    ```C++

    ```

3. BGR彩色图像三通道直方图均衡处理   
    三个通道BGR分别进行直方图均衡，最后合起来，但BGR的变换程度不一，会导致一个问题，就是原始图像的色度会发生改变。
    ```C++


    ```

4. HSV彩色图像V通道直方图均衡处理  
    为了不改变图像色度，将BGR图像转换为HSV图像，只对亮度进行直方图均衡，在不改变图像色度的情况下实现对图像的增强。
    ```C++

    ```
 

## 实验中的问题

_包括在实验中遇到的问题，以及解决问题的方法_

1. 主要的问题就是直方图均衡的算法，要在课本上的算法之后再对图像进行一下拉伸。我是按照原始图像的灰度范围进行的拉伸，调用了OpenCV的资方图均衡API发现OpenCV中的equalizeHist是将图像灰度级拉伸到[0,255]区间内。

2. 算法当中一个小问题，包括所有double型计算中，整型（包括int，uchar等）与double掺杂运算需要对整型数据进行类型转换，并且double转到整型时需要进行四舍五入，不然只会取整数位在很多情况下会造成1灰度级的误差。我的代码中直接用int(double + 0.5)进行的四舍五入。

3. 程序=算法+数据结构，这几次实验的算法都不算复杂，只需要注意几个小坑，不然算法出了类似四舍五入一类的小问题很不容易发现，我现在的直方图均衡代码，虽然大的方面从直方图图像以及增强效果来看已经没问题了，也依旧不能完全肯定里面是否仍有些小问题。

4. 另外会比较容易出错以及浪费时间的是数据结构，包括数据类型的用法，函数接口的使用，个别时候还可能因为C/C++的语法不熟出问题。这都需要高效的信息检索能力，需要找到相关资料确定用法以及可能的错误。不过这都是开始时的问题，慢慢写的多了熟了这类基础问题不会再发生。


## 实验结果

[实验2源码地址](https://github.com/chengx-coding/USTC-MSE_DIP_Exp/blob/master/Exp02.cpp)


---
##### [返回实验主页](https://chengx-coding.github.io/USTC-MSE_DIP_Exp/)