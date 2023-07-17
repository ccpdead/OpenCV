/**
 * Sobel算子
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    Mat src, dst;
    Mat gray_dst;
    Mat xgrad, ygrad;

    src = imread("/home/jhr/catkin_ws/image/cup3.jpg");
    if (src.empty()) {
        printf("could not load image\n");
        return -1;
    }
    namedWindow("input image", WINDOW_AUTOSIZE);
    imshow("input image", src);

    // GaussianBlur(src, dst, Size(3, 3), 0, 0);
    bilateralFilter(src, dst, 7, 100, 5);

    cvtColor(dst, gray_dst, COLOR_BGR2GRAY);
    imshow("gray_dst", gray_dst);

    // Scharr计算梯度
    // Scharr(gray_dst, xgrad, CV_16S, 1, 0);
    // Scharr(gray_dst, ygrad, CV_16S, 0, 1);
    // Sobel方式计算梯度
    Sobel(gray_dst, xgrad, CV_16S, 1, 0, 3);
    Sobel(gray_dst, ygrad, CV_16S, 0, 1, 3);

    convertScaleAbs(xgrad, xgrad);  // 图像绝对值
    convertScaleAbs(ygrad, ygrad);
    imshow("xgrad", xgrad);
    imshow("ygrad", ygrad);

    Mat xygrad1;
    addWeighted(xgrad, 0.5, ygrad, 0.5, 0, xygrad1);
    imshow("xygrad1", xygrad1);

    Mat xygrad = Mat(xgrad.size(), xgrad.type());
    printf("type:%d\n", xgrad.type());

    // 遍历图片所有像素点
    int width = xgrad.cols;
    int height = ygrad.rows;
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int xg = xgrad.at<uchar>(row, col);
            int yg = ygrad.at<uchar>(row, col);
            int xy = xg + yg;  // 结合每个像素点的x与y像素值
            xygrad.at<uchar>(row, col) = saturate_cast<uchar>(xy);
        }
    }

    threshold(xygrad, xygrad, 0, 255, THRESH_OTSU | THRESH_BINARY);
    imshow("xygrad", xygrad);
    waitKey(0);
}