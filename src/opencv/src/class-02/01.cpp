/**
 * Harris角点检测
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat src, gray_src;
int thresh = 130;
int max_count = 255;
void Harris_Demo(int, void*);

int main(int argc, char** argv) {
    // src = imread("/home/jhr/catkin_ws/image/cup3.jpg");
    // if (src.empty()) {
    //     printf("could not load image\n");
    //     return -1;
    // }

    VideoCapture cap(0);
    if (!cap.isOpened()) {
        printf("could not open img\n");
        return -1;
    }

    while (1) {
        cap >> src;
        if (src.empty()) {
            break;
        }
        resize(src, src, Size(640, 480), 0, 0, 1);

        namedWindow("input", WINDOW_AUTOSIZE);
        namedWindow("output", WINDOW_AUTOSIZE);
        imshow("input", src);

        cvtColor(src, gray_src, COLOR_BGR2GRAY);
        createTrackbar("Threshold", "output", &thresh, max_count, Harris_Demo);
        Harris_Demo(0, 0);

        // waitKey(0);

        char c = (char)waitKey(30);
        if (c == 27) {
            break;
        }
    }
    return 0;
}

void Harris_Demo(int, void*) {
    Mat dst, norm_dst, normScaleDst;
    dst = Mat::zeros(gray_src.size(), CV_32FC1);

    int blockSize = 2;//窗口大小
    int ksize = 3;//矩阵大小
    double k = 0.04;//k参数
    cornerHarris(gray_src, dst, blockSize, ksize, k, BORDER_DEFAULT);
    normalize(dst, norm_dst, 0, 255, NORM_MINMAX, CV_32FC1, Mat());//归一化
    convertScaleAbs(norm_dst, normScaleDst);//绝对值
    // imshow("con",normScaleDst);

    Mat resultImg = src.clone();
    for (int row = 0; row < resultImg.rows; row++) {
        uchar* currentRow = normScaleDst.ptr(row);
        for (int col = 0; col < resultImg.cols; col++) {
            int value = (int)*currentRow;
            if (value > thresh) {
                circle(resultImg, Point(col, row), 1, Scalar(0, 0, 255), 2, 8, 0);
            }
            currentRow++;
        }
    }

    imshow("output", resultImg);
}