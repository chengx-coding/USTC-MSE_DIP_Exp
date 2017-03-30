# 实验3 空域滤波

## 实验要求

掌握利用模板对图像进行空域滤波操作，熟练掌握常用空域模板的使用。

1. 利用均值模板平滑灰度图像。 
    具体内容：利用OpenCV对图像像素进行操作，分别利用3 * 3、5 * 5和9 * 9尺寸的均值模板平滑灰度图像 
2. 利用高斯模板平滑灰度图像。 
    具体内容：利用OpenCV对图像像素进行操作，分别利用3 * 3、5 * 5和9 * 9尺寸的高斯模板平滑灰度图像 
3. 利用Laplacian、Robert、Sobel模板锐化灰度图像。 
    具体内容：利用OpenCV对图像像素进行操作，分别利用Laplacian、Robert、Sobel模板锐化灰度图像 
4. 利用高提升滤波算法增强灰度图像。 
    具体内容：利用OpenCV对图像像素进行操作，设计高提升滤波算法增强图像 
5. 利用均值模板平滑彩色图像。 
    具体内容：利用OpenCV分别对图像像素的RGB三个通道进行操作，利用3 * 3、5 * 5和9 * 9尺寸的均值模板平滑彩色图像 
6. 利用高斯模板平滑彩色图像。 
    具体内容：利用OpenCV分别对图像像素的RGB三个通道进行操作，分别利用3 * 3、5 * 5和9 * 9尺寸的高斯模板平滑彩色图像 
7. 利用Laplacian、Robert、Sobel模板锐化灰度图像。 
    具体内容：利用OpenCV分别对图像像素的RGB三个通道进行操作，分别利用Laplacian、Robert、Sobel模板锐化彩色图像


## 实验完成情况

_包括完成的实验内容及每个实验的完成程度_

完成了7项内容：

1. 利用均值模板平滑灰度图像
	需要特别说明的是 int FilterProcessing(Mat src, Mat dst, Mat filter, double ProcessingMethod(Mat filterArea, Mat filter)) 和 double LinearFilterCalc(Mat filterArea, Mat linearFilter) 这两个函数。FilterProcessing是用滤波器对图像进行运算的函数，其中的double ProcessingMethod(Mat filterArea, Mat filter)是回调函数，表示运算方式。本实验中的滤波方法都是线性运算LinearFilterCalc，即利用滤波器对图像进行卷积。但中值滤波等非线性滤波器就要用其他的运算方式，到时只要替换回掉函数即可。把FilterProcessing函数写成这样也是为了提高函数的通用性，方便以后的实验。
	均值滤波分别用3 * 3、5 * 5和9 * 9尺寸的均值模板，均值滤波器很容易直接得到，直接将滤波器代入上面提到的两个函数即可。

    ```C++
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
	                dst.at<uchar>(y - padding, x - padding) = int(ProcessingMethod(filterArea, filter) + 0.5);
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
    
    ```

2. 利用高斯模板平滑灰度图像   
	值得一提的是int GaussianFilterGenerator(Mat gaussianFilter, double variance)函数，这是我写的专门用来生成任意大小的高斯滤波器的函数。除模板矩阵外，一开始写的函数有幅值和方差两个参数，如果幅值不够大，对9 * 9的模板来说边缘会出现很多零。于是后来改成生成的高斯滤波器中最小值为1，如此一来就不要幅值参数，只需要一个方差参数就够了。
	用GaussianFilterGenerator分别生成3 * 3、5 * 5和9 * 9尺寸的均值滤波器后，再代入上面提到的滤波处理函数得到滤波结果。
    ```C++
	int GaussianFilterGenerator(Mat gaussianFilter, double variance)
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
	    int centerValue;
	    centerValue = int((1. / exp(-0.5*(double(pow((0 - center), 2) + pow((0 - center), 2))) / variance)) + 0.5);
	    for (int y = 0; y < gaussianFilter.rows; y++)
	    {
	        for (int x = 0; x < gaussianFilter.cols; x++)
	        {
	            gaussianFilter.at<double>(y, x) = int((double(centerValue)*exp(-0.5*(double(pow((y - center), 2) + pow((x - center), 2))) / variance)) + 0.5);
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

	    GaussianFilterGenerator(gaussianFilter_3x3, 0.72);//0.720 is square of 0.849
	    GaussianFilterGenerator(gaussianFilter_5x5, 1.92);
	    GaussianFilterGenerator(gaussianFilter_9x9, 2.8854);

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
    
    ```

3. 利用Laplacian、Robert、Sobel模板锐化灰度图像   
	这三个锐化模板与之前的平滑模板不同，会出现超过[0,255]范围的结果。对于结算得到的边缘图像，需要标定一下。对于锐化结果，直接用saturate_cast函数封闭两边边界。只要数据类型没问题，计算方面跟上面的过程没有太大区别。
    ```C++
	int LaplacianFilterProcessing(Mat src, Mat dst, Mat laplacianFilter, Mat laplacianFilterImg, double c)//其实没必要多拆出来这一个函数
	{

	    FilterProcessing(gray, laplacianFilterImg, laplacianFilter, LinearFilterCalc);    
	        
	    //计算并标定锐化结果，直接saturate_cast<uchar>
	    for (int y = 0; y < gray.rows; y++)
	    {
	        for (int x = 0; x < gray.cols; x++)
	        {
	            dst.at<uchar>(y, x) = saturate_cast<uchar>(src.at<uchar>(y, x) + int(c*int(laplacianFilterImg.at<double>(y, x)) + 0.5));//int(double + 0.5)用来四舍五入
	        }
	    }

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
	            dst.at <uchar>(y, x) = saturate_cast<uchar>(src.at<uchar>(y, x) + int(c*robertFilterImg.at<double>(y, x) + 0.5));//int(double + 0.5)用来四舍五入
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
	            dst.at <uchar>(y, x) = saturate_cast<uchar>(src.at<uchar>(y, x) + int(c*sobelFilterImg.at<double>(y, x) + 0.5));//int(double + 0.5)用来四舍五入
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

    
    ```

4. 利用高提升滤波算法增强灰度图像  
	用原图像减去平滑后的图像得到边缘，然后再用得到的边缘增强原图像。平滑方法提供了上面的均值和高斯两种方法共六种模板来选择。

    ```C++
int HighboostFilterProcessing()
{
    double k = 1.5;//k>1时为高提升滤波

    Mat meanFilter_3x3 = ((double)1 / 9)*Mat::ones(3, 3, CV_64F);//CV_64F对应double，若CV_32F对饮double会报错
    Mat meanFilter_5x5 = ((double)1 / 25)*Mat::ones(5, 5, CV_64F);
    Mat meanFilter_9x9 = ((double)1 / 81)*Mat::ones(9, 9, CV_64F);
    Mat gaussianFilter_3x3 = Mat::zeros(3, 3, CV_64F);//CV_64F对应double，若CV_32F对应double会报错
    Mat gaussianFilter_5x5 = Mat::zeros(5, 5, CV_64F);
    Mat gaussianFilter_9x9 = Mat::zeros(9, 9, CV_64F);
    GaussianFilterGenerator(gaussianFilter_3x3, 0.72);
    GaussianFilterGenerator(gaussianFilter_5x5, 1.92);
    GaussianFilterGenerator(gaussianFilter_9x9, 2.8854);
    Mat filter;

    char choice;
    while (1)
    {
        cout <<
            "清选择计算 unsharp mask 时所用的平滑滤波器\n" <<
            "1 3x3 均值\n" <<
            "2 5x5 均值\n" <<
            "3 9x9 均值\n" <<
            "4 3x3 高斯\n" <<
            "5 5x5 高斯\n" <<
            "6 9x9 高斯\n" <<
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
                filter = meanFilter_3x3;
                break;
            case '2':
                filter = meanFilter_5x5;
                break;
            case '3':
                filter = meanFilter_9x9;
                break;
            case '4':
                filter = gaussianFilter_3x3;
                break;
            case '5':
                filter = gaussianFilter_5x5;
                break;
            case '6':
                filter = gaussianFilter_9x9;
                break;
            case 'w':
                return 0;
            default:
                cout << "无效的输入" << endl;
                continue;
                //break;
            }
        }

        Mat highboostFilterImg = Mat::zeros(gray.size(), gray.type());
        Mat unsharpMask= Mat::zeros(gray.size(), CV_64F);//非锐化掩蔽
        Mat blurImg = Mat::zeros(gray.size(), gray.type());

        FilterProcessing(gray, blurImg, filter, LinearFilterCalc);

        gray.convertTo(gray, CV_64F);
        blurImg.convertTo(blurImg, CV_64F);
        unsharpMask = gray - blurImg;
        gray.convertTo(gray, CV_8U);

        for (int y = 0; y < highboostFilterImg.rows; y++)
        {
            for (int x = 0; x < highboostFilterImg.cols; x++)
            {
                highboostFilterImg.at <uchar>(y, x) = saturate_cast<uchar>(gray.at<uchar>(y, x) + k*unsharpMask.at<double>(y, x));
            }
        }

        //标定 scaling unsharp mask
        double maxMast, minMast;
        minMaxLoc(unsharpMask, &minMast, &maxMast, 0, 0);
        unsharpMask.convertTo(unsharpMask, CV_8U, 255. / (maxMast - minMast), (-255.*minMast) / (maxMast - minMast));

        namedWindow("Gray - 灰度图像", WINDOW_AUTOSIZE);
        imshow("Gray - 灰度图像", gray);
        namedWindow("Unsharp Mask with scaling", WINDOW_AUTOSIZE);
        imshow("Unsharp Mask with scaling", unsharpMask);
        namedWindow("Highboost Filter Img", WINDOW_AUTOSIZE);
        imshow("Highboost Filter Img", highboostFilterImg);

        waitKey(0);
        destroyAllWindows();

    }

    return 0;
}

    
    ```

5. 利用均值模板平滑彩色图像BGR通道  
	三个通道分别平滑再合并。BGRFilterProcessing和ShowBGRChannels两个函数是专门为彩色图像处理写的函数。
    ```C++
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
	    
    ```

6. 利用高斯模板平滑彩色图像BGR通道  
	三个通道分别平滑再合并。
    ```C++
	int BGRGaussianFilterProcessing()
	{
	    Mat gaussianFilter_3x3 = Mat::zeros(3, 3, CV_64F);//CV_64F对应double，若CV_32F对应double会报错
	    Mat gaussianFilter_5x5 = Mat::zeros(5, 5, CV_64F);
	    Mat gaussianFilter_9x9 = Mat::zeros(9, 9, CV_64F);

	    GaussianFilterGenerator(gaussianFilter_3x3, 0.72);
	    GaussianFilterGenerator(gaussianFilter_5x5, 1.92);
	    GaussianFilterGenerator(gaussianFilter_9x9, 2.8854);

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
    
    ```

7. 利用Laplacian、Robert、Sobel模板锐化彩色图像BGR通道  
	三个通道分别锐化再合并。
    ```C++
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

    ```
 

## 实验中的问题

_包括在实验中遇到的问题，以及解决问题的方法_

1. 仍然是关于数据类型的问题，凡是涉及计算过程和结果的量我都使用了double型，因此允许了小数和负值的相关计算，提高了函数的通用性，也稍微增加了一些运算代价。

2. FilterProcessing本来只能得到CV_8U的结果，后来做到锐化算子时发现不能适用了，于是函数内增加了数据类型判断和分别处理。

3. 一开始写的时候处理彩色图像用了大段重复代码，看起来繁琐而且不容易编辑修改，于是整合了一些代码写了BGRFilterProcessing和ShowBGRChannels两个函数对彩色图像进行处理。不过现在看来BGRFilterProcessing函数的接口有一点过度设计，还不够好用。

4. 大量的时间已不是花在数据结构和函数用法等初级问题上，而是对函数接口设计、算法的精确实现、性能改进等方面有了更多思考，努力追求结果的精确，为了一点小小的改进往往要花费大量时间以及思考和设计，因此本次实验总共写了八百多行代码。虽仍有不足之处，但经过反复确认不会出现错误。

## 实验结果

[实验3源码地址](https://github.com/chengx-coding/USTC-MSE_DIP_Exp/blob/master/Exp03.cpp)


---
##### [返回实验主页](https://chengx-coding.github.io/USTC-MSE_DIP_Exp/)