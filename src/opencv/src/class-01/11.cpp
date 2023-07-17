/**
 * 边缘处理
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
    namedWindow("input image", WINDOW_AUTOSIZE);
    namedWindow("output image", WINDOW_AUTOSIZE);
    imshow("input image", src);

    // int top = (int)(0.05 * src.rows);
    // int bottom = (int)(0.05 * src.rows);
    // int left = (int)(0.05 * src.cols);
    // int right = (int)(0.05 * src.cols);
    // RNG rng(12345);
    // int borderType = BORDER_DEFAULT;

    // int c = 0;
    // while (true) {
    //     c = waitKey(500);
    //     if ((char)c == 27)  // ESC
    //         break;

    //     if ((char)c == 'r') {
    //         borderType = BORDER_REPLICATE;
    //     } else if ((char)c == 'w') {
    //         borderType = BORDER_WRAP;
    //     } else if ((char)c == 'c') {
    //         borderType = BORDER_CONSTANT;
    //     }
    //     Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
    //     copyMakeBorder(src, dst, top, bottom, left, right, borderType, color);
    //     imshow("output image", dst);
    // }

    GaussianBlur(src,dst,Size(5,5),0,0,BORDER_DEFAULT);
    imshow("output image",dst);

    waitKey(0);
    return 0;
}
