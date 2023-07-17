/**
 * 直方图比较
 *      cvtColor
 *      calcHist
 *      normalize
 *      compareHist
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

//将浮点型数据转化为string类型
string convertToString(double d);

int main(int argc, char** argv) {
    Mat base, test1, test2;
    Mat hsvbase, hsvtest1, hsvtest2;

    base = imread("/home/jhr/catkin_ws/image/cup.jpg");
    test1 = imread("/home/jhr/catkin_ws/image/cup2.jpg");
    test2 = imread("/home/jhr/catkin_ws/image/cup3.jpg");
    if (base.empty() && test1.empty() && test2.empty()) {
        printf("could not load image\n");
        return -1;
    }

    // bgr->hsv
    cvtColor(base, hsvbase, COLOR_BGR2HSV);
    cvtColor(test1, hsvtest1, COLOR_BGR2HSV);
    cvtColor(test2, hsvtest2, COLOR_BGR2HSV);

    int h_bins = 50;
    int s_bins = 60;
    int histSize[] = {h_bins, s_bins};

    float h_ranges[] = {0, 180};
    float s_ranges[] = {0, 256};
    const float* ranges[] = {h_ranges, s_ranges};

    // 使用两个通道
    int channels[] = {0, 1};
    MatND hist_base;
    MatND hist_test1;
    MatND hist_test2;

    // 计算直方图
    calcHist(&hsvbase, 1, channels, Mat(), hist_base, 2, histSize, ranges, true, false);
    // 归一化处理
    normalize(hist_base, hist_base, 0, 1, NORM_MINMAX, -1, Mat());

    calcHist(&hsvtest1, 1, channels, Mat(), hist_test1, 2, histSize, ranges, true, false);
    normalize(hist_test1, hist_test1, 0, 1, NORM_MINMAX, -1, Mat());

    calcHist(&hsvtest2, 1, channels, Mat(), hist_test2, 2, histSize, ranges, true, false);
    normalize(hist_test2, hist_test2, 0, 1, NORM_MINMAX, -1, Mat());

    // 比较直方图
    double basebase = compareHist(hist_base, hist_base, HISTCMP_CORREL );
    double basetest1 = compareHist(hist_base, hist_test1, HISTCMP_CORREL );
    double basetest2 = compareHist(hist_base, hist_test2, HISTCMP_CORREL );
    double tes1test2 = compareHist(hist_test1, hist_test2, HISTCMP_CORREL );
    printf("basebase :%f\n", basebase);
    printf("basetest1 :%f\n", basetest1);
    printf("basetest2 :%f\n", basetest2);
    printf("tes1test2 :%f\n", tes1test2);

    Mat test12;
    test2.copyTo(test12);

    putText(base,convertToString(basebase),Point(50,50),FONT_HERSHEY_COMPLEX,1,Scalar(0,255,0),2,LINE_AA);
    putText(test1,convertToString(basetest1),Point(50,50),FONT_HERSHEY_COMPLEX,1,Scalar(0,255,0),2,LINE_AA);
    putText(test2,convertToString(basetest2),Point(50,50),FONT_HERSHEY_COMPLEX,1,Scalar(0,255,0),2,LINE_AA);
    putText(test12,convertToString(tes1test2),Point(50,50),FONT_HERSHEY_COMPLEX,1,Scalar(0,255,0),2,LINE_AA);


    namedWindow("base", WINDOW_AUTOSIZE);
    namedWindow("test1", WINDOW_AUTOSIZE);
    namedWindow("test2", WINDOW_AUTOSIZE);
    imshow("base", base);
    imshow("test1", test1);
    imshow("test2", test2);
    imshow("test12", test12);

    waitKey(0);
    return 0;
}

string convertToString(double d){
  ostringstream os;
  if(os<<d)
    return os.str();
  return"invalid conversion";
}