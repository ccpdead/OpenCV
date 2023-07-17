/**
 * 05 图象操作
 * 读写图像，读写像素，修改像素值
 */

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    Mat src, gray_src;
    src = imread("/home/jhr/catkin_ws/image/jasoo-2.jpg");
    if (!src.data) {
        printf("could not load image ....\n");
        return -1;
    }
    namedWindow("input", WINDOW_NORMAL);
    resizeWindow("input", 640, 480);
    imshow("input", src);

    // cvtColor(src, gray_src, COLOR_BGR2GRAY);
    int height = gray_src.rows;
    int width = gray_src.cols;
    // 单通道
    // for (int row = 0; row < height; row++) {
    //     for (int col = 0; col < width; col++) {
    //         int gray = gray_src.at<uchar>(row, col);
    //         gray_src.at<uchar>(row, col) = 255 - gray;
    //     }
    // }

    // 多通道
    Mat dst;
    dst.create(src.size(), src.type());
    height = src.rows;
    width = src.cols;
    int nc = src.channels();

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            if (nc == 1) {
                int gray = gray_src.at<uchar>(row, col);
                gray_src.at<uchar>(row, col) = 255 - gray;
            } else if (nc == 3) {
                int b = src.at<Vec3b>(row, col)[0];
                int g = src.at<Vec3b>(row, col)[1];
                int r = src.at<Vec3b>(row, col)[2];

                dst.at<Vec3b>(row, col)[0] = b-100;
                dst.at<Vec3b>(row, col)[1] = g-100;
                dst.at<Vec3b>(row, col)[2] = r-100;

            }
        }
    }
    // bitwise_not(src,dst);
    namedWindow("output", WINDOW_NORMAL);
    resizeWindow("output", 640, 480);
    imshow("output", dst);

    waitKey(0);
    return 0;
}