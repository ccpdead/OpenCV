/**
 * 提取垂直线与竖线
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
int main(int argc, char** argv) {
    Mat src, gray_src, dst,out_dst;
    src = imread("/home/jhr/catkin_ws/image/opencv.png");
    if (src.empty()) {
        printf("could not read image\n");
        return -1;
    }

    char title1[] = "input";
    char title2[] = "output";

    namedWindow(title1, WINDOW_NORMAL);
    resizeWindow(title1, 640, 480);

    cvtColor(src, gray_src, COLOR_BGR2GRAY);  // 灰度转化
    adaptiveThreshold(gray_src, dst, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 11, 0);//二值化
    imshow(title1, dst);

    //水平元素
    // Mat hline = getStructuringElement(MORPH_RECT, Size(src.cols/16, 1), Point(-1, -1));
    //垂直元素
    Mat vline = getStructuringElement(MORPH_RECT, Size(1, src.rows / 16), Point(-1. - 1));

    Mat temp;
    erode(dst, temp, vline);
    dilate(temp,out_dst,vline);
    bitwise_not(out_dst,out_dst);
    namedWindow(title2, WINDOW_NORMAL);
    resizeWindow(title2, 640, 480);
    imshow(title2, out_dst);

    waitKey(0);
    return 0;
}