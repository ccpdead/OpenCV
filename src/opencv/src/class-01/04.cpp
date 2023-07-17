/**
 * 模糊图像
*/
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    Mat src;
    src = imread("/home/jhr/catkin_ws/image/jasoo-2.jpg");
    if (src.empty()) {
        printf("Image could not load.\n");
        return -1;
    }

    char title1[] = "input";
    char title2[] = "blur";
    namedWindow(title1,WINDOW_NORMAL);
    resizeWindow(title1,640,480);
    imshow(title1,src);

    Mat dst;
    Mat kernel = (Mat_<int>(3,3)<<0,-1,0,-1,5,-1,0,-1,0);//掩膜操作
    // blur (src,dst,Size(11,11),Point(-1,-1));//中值滤波s
    // GaussianBlur(src,dst,Size(5,5),5,5);//高斯滤波
    // bilateralFilter(src,dst,15,100,5);//双边高斯滤波
    filter2D(src,dst,-1,kernel ,Point(-1,-1),0);
    namedWindow(title2,WINDOW_NORMAL);
    resizeWindow(title2,640,480);
    imshow(title2,dst);


    waitKey(0);
    return 0;
}