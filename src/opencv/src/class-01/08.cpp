/**
 * 图像金字塔-上采样与下采样
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

    char input_win[] = "input image";

    namedWindow(input_win, WINDOW_NORMAL);
    resizeWindow(input_win, 640, 480);
    imshow(input_win, src);

    //上采样
    pyrUp(src,dst,Size(src.cols*2,src.rows*2));
    imshow("up", dst);
    //降采样
    Mat dst2;
    pyrDown(src,dst2,Size(src.cols/2,src.rows/2));
    imshow("down", dst2);

    //dog（高斯分差）
    Mat gray_src,g1,g2,dogImg;
    cvtColor(src,gray_src,COLOR_BGR2GRAY);
    GaussianBlur(gray_src,g1,Size(5,5),0,0);
    GaussianBlur(g1,g2,Size(5,5),0,0);
    subtract(g1,g2,dogImg,Mat());

    //归一化显示
    normalize(dogImg,dogImg,255,0,NORM_MINMAX);
    imshow("dogImag",dogImg);

    waitKey(0);
    return 0;
}