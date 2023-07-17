/**
 * 直方图反向投影
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat src, hsv;
Mat hue;
int bins = 12;
void Hist_And_Backprojection(int, void*);
int main(int argc, char** argv) {
    src = imread("/home/jhr/catkin_ws/image/hand2.jpg");
    if (src.empty()) {
        printf("could not load image\n");
        return -1;
    }
    namedWindow("input", WINDOW_AUTOSIZE);

    cvtColor(src, hsv, COLOR_BGR2HSV);
    hue.create(hsv.size(), hsv.depth());  // 创建mat
    int nchannels[] = {0, 0};             // 通道
    // 将输入数组中指定通道复制到输出数组的指定通道
    mixChannels(&hsv, 1, &hue, 1, nchannels, 1);

    createTrackbar("Histogram Bins", "input", &bins, 180, Hist_And_Backprojection);
    Hist_And_Backprojection(0, 0);

    imshow("input", src);
    waitKey(0);
    return 0;
}

void Hist_And_Backprojection(int, void*) {
    float range[] = {0, 180};
    const float* histRanges = {range};
    Mat h_hist;
    // 计算直方图
    calcHist(&hue, 1, 0, Mat(), h_hist, 1, &bins, &histRanges, true, false);
    normalize(h_hist, h_hist, 0, 255, NORM_MINMAX, -1, Mat());  // 归一化处理

    Mat backprjImage;
    //直方图反投影
    calcBackProject(&hue, 1, 0, h_hist, backprjImage, &histRanges, 1, true);
    imshow("BackProj", backprjImage);

    // 绘制直方图
    int hist_h = 400;
    int hist_w = 400;
    Mat histImage(hist_w, hist_h, CV_8UC3, Scalar(0, 0, 0));

    int bin_w = (hist_w / bins);//定义每个bins宽度
    for (int i = 1; i < bins; i++) {
        // 绘制矩形
        rectangle(histImage, 
        Point((i - 1) * bin_w, (hist_h - cvRound(h_hist.at<float>(i - 1) * (400 / 255)))), 
        Point(i * bin_w, hist_h),
        Scalar(0, 0, 255), -1);
    }
    imshow("Histogram", histImage);
    return;
}