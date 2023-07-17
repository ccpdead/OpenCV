/**
 * 直方图均衡化
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    Mat src, dst;
    src = imread("/home/jhr/catkin_ws/image/cup.jpg");
    if (src.empty()) {
        printf("could not load image\n");
        return -1;
    }
    namedWindow("input", WINDOW_AUTOSIZE);
    namedWindow("output", WINDOW_AUTOSIZE);
    imshow("input", src);

    // 直方图计算
    //  cvtColor(src,dst,COLOR_BGR2GRAY);
    //  imshow("gray",dst);
    //  equalizeHist(dst,dst);
    //  imshow("output",dst);

    // 分通道显示
    vector<Mat> bgr_planes;
    split(src, bgr_planes);
    // imshow("output",bgr_planes[0]);

    // 计算直方图
    int histSize = 256;
    float range[] = {0, 256};
    const float* histRanges = {range};
    Mat b_hist, g_hist, r_hist;
    calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRanges, true, false);
    calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRanges, true, false);
    calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRanges, true, false);

    int hist_h = 480;
    int hist_w = 640;
    int bin_w = hist_w / histSize;
    // 创建一个图像；宽，高，图层类型，颜色
    Mat histImage(hist_w, hist_h, CV_8UC3, Scalar(255, 255, 255));

    // 将直方图数值限定在 0~hist 范围内
    normalize(b_hist, b_hist, 0, hist_h, NORM_MINMAX, -1, Mat());
    normalize(g_hist, g_hist, 0, hist_h, NORM_MINMAX, -1, Mat());
    normalize(r_hist, r_hist, 0, hist_h, NORM_MINMAX, -1, Mat());

    // 绘制直方图
    for (int i = 1; i < histSize; i++) {
        // line画直线，两个点，颜色，线宽，直线类型
        line(histImage, Point((i - 1) * bin_w, hist_h - cvRound(b_hist.at<float>(i - 1))), Point((i)*bin_w, hist_h - cvRound(b_hist.at<float>(i))),
             Scalar(255, 0, 0), 2, LINE_AA);
        line(histImage, Point((i - 1) * bin_w, hist_h - cvRound(g_hist.at<float>(i - 1))), Point((i)*bin_w, hist_h - cvRound(g_hist.at<float>(i))),
             Scalar(0, 255, 0), 2, LINE_AA);
        line(histImage, Point((i - 1) * bin_w, hist_h - cvRound(r_hist.at<float>(i - 1))), Point((i)*bin_w, hist_h - cvRound(r_hist.at<float>(i))),
             Scalar(0, 0, 255), 2, LINE_AA);
    }

    imshow("output", histImage);

    cv::waitKey(0);
    return 0;
}