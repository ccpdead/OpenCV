/**
 * 05 调整图像亮度与对比度
 *
 */

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    Mat src, dst;
    src = imread("/home/jhr/catkin_ws/image/jasoo-2.jpg");
    if (!src.data) {
        printf("could not load image ....\n");
        return -1;
    }
    namedWindow("input", WINDOW_NORMAL);
    resizeWindow("input", 640, 480);
    imshow("input", src);

    int height = src.rows;
    int width = src.cols;
    float alpha = 1.3;
    float beta = 30;
    dst = Mat::zeros(src.size(), src.type());

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            if (src.channels() == 3) {
                float b = src.at<Vec3b>(row, col)[0];
                float g = src.at<Vec3b>(row, col)[1];
                float r = src.at<Vec3b>(row, col)[2];

                dst.at<Vec3b>(row, col)[0] = saturate_cast<uchar>(b * alpha + beta);
                dst.at<Vec3b>(row, col)[1] = saturate_cast<uchar>(g * alpha + beta);
                dst.at<Vec3b>(row, col)[2] = saturate_cast<uchar>(r * alpha + beta);
            } else if (src.channels() == 1) {
                float v = src.at<uchar>(row, col);
                dst.at<uchar>(row, col) = saturate_cast<uchar>(v * alpha + beta);
            }
        }
    }

    char output_title[] = "output image";
    namedWindow(output_title, WINDOW_NORMAL);
    resizeWindow(output_title, 640, 480);
    imshow(output_title, dst);

    waitKey(0);
}