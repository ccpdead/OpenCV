/**
 * 自定义线滤波
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    Mat src, dst;
    src = imread("/home/jhr/catkin_ws/image/jasoo-2.jpg");
    if (src.empty()) {
        printf("could not load image\n");
        return -1;
    }
    int ksize = 0;
    const char* input_win = "input_image";
    const char* output_win = "output_image";
    namedWindow(input_win, WINDOW_AUTOSIZE);
    namedWindow(output_win, WINDOW_AUTOSIZE);

    imshow(input_win, src);

    // Sobel X 方向
    // Mat kernel_x = (Mat_<int>(3, 3) << -1, 0, -1, -2, 0, 2, -1, 0, 1);
    // filter2D(src, dst, -1, kernel_x, Point(-1, -1), 0.0);

    // Sobel Y 方向
    // Mat yimg;
    // Mat kernel_y = (Mat_<int>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
    // filter2D(src, yimg, -1, kernel_y, Point(-1, -1), 0.0);

    // 拉普拉斯算子
    // Mat kernel_y = (Mat_<int>(3, 3) << 0, -1, 0, -1, 4, -1, 0, -1, 0);
    // filter2D(src, dst, -1, kernel_y, Point(-1, -1), 0.0);

    int c = 0;
    int index = 0;
    //通过循环变更kernel尺寸
    while (true) {
        c = waitKey(500);
        if ((char)c == 27) {  // ESC
            break;
        }
        ksize = 5 + (index % 8) * 2;
        Mat kernel = Mat::ones(Size(ksize, ksize), CV_32F) / (float)(ksize * ksize);
        filter2D(src, dst, -1, kernel, Point(-1, -1));
        index++;
        imshow(output_win, dst);
    }

    // imshow(output_win, dst);
    waitKey(0);
    return 0;
}