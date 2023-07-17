/**
 * 像素重映射
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat src, dst, map_x, map_y;
int index_n = 0;

void update_map(void);

int main(int argc, char** argv) {
        src = imread("/home/jhr/catkin_ws/image/cup.jpg");
    if (src.empty()) {
        printf("could not load image\n");
        return -1;
    }
    namedWindow("input", WINDOW_AUTOSIZE);
    imshow("input", src);

    map_x.create(src.size(), CV_32FC1);
    map_y.create(src.size(), CV_32FC1);

    int c = 0;
    while (true) {
        c = waitKey(500);
        if ((char)c == 27) {
            break;
        }
        index_n = c % 4;
        printf("index:%d\n",index_n);
        update_map();
        remap(src, dst, map_x, map_y, INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 255, 255));
        imshow("outpur", dst);
    }

    waitKey(0);
    return 0;
}

void update_map(void) {
    for (int row = 0; row < src.rows; row++) {
        for (int col = 0; col < src.cols; col++) {
            switch (index_n) {
                case 0:
                //图像取半
                    // 取图片0.25-0.75的区域
                    if (col > (src.cols * 0.25) && col <= (src.cols * 0.75) && row > (src.rows * 0.25) && row <= (src.rows * 0.75)) {
                        map_x.at<float>(row, col) = 2 * (col - (src.cols * 0.25)-0.5);
                        map_y.at<float>(row, col) = 2 * (row - (src.cols * 0.25)-0.5);
                    }
                    // 剩下的区域赋空值
                    else { 
                        map_x.at<float>(row, col) = 0;
                        map_y.at<float>(row, col) = 0;
                    }
                    break;
                case 1:
                //X反转
                    map_x.at<float>(row, col) = (src.cols - col - 1);
                    map_y.at<float>(row, col) = row;
                    break;
                case 2:
                //Y反转
                    map_x.at<float>(row, col) = col;
                    map_y.at<float>(row, col) = (src.rows - row - 1);
                    break;
                case 3:
                //XY翻转
                    map_x.at<float>(row, col) = (src.cols - col - 1);
                    map_y.at<float>(row, col) = (src.rows - row - 1);
                    break;
            }
        }
    }
}