/**
 * 基本的图像掩膜操作学习记录
*/
#include <math.h>
#include <iostream>
#include <opencv4/opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    Mat image1, image2;
    image1 = imread("/home/jhr/catkin_ws/image/hdr.png");
    if (!image1.data) {
        printf("could not load image ......\n");
        return -1;
    }

    namedWindow("input image", WINDOW_NORMAL);
    resizeWindow("input image", 640, 480);
    imshow("input image", image1);

    // int cols = (image1.cols - 1) * image1.channels();  // 列
    // int offsetx = image1.channels();                   // 通道数
    // int rows = image1.rows;                            // 行
    // image2 = Mat(image1.size(), image1.type());

    // for (int row = 1; row < (rows - 1); row++) {            // 每一行
    //     const uchar* current = image1.ptr<uchar>(row - 1);  // 上一行的指针
    //     const uchar* previous = image1.ptr<uchar>(row);     // 当前行指针
    //     const uchar* next = image1.ptr<uchar>(row + 1);     // 下一行指针
    //     uchar* output = image2.ptr<uchar>(row);
    //     for (int col = offsetx; col < cols; col++) {
    //         output[col] = saturate_cast<uchar>(5 * current[col] - (current[col - offsetx] + current[col + offsetx] + previous[col] + next[col]));
    //     }
    // }
    double t = getTickCount();
    Mat kernel = (Mat_<char>(3,3)<<0,-1,0,-1,5,-1,0,-1,0);//生成掩膜
    filter2D(image1,image2,image1.depth(),kernel);
    double timeconlume =(getTickCount() -t)/getTickFrequency();
    printf("time :%f\n",timeconlume);

    namedWindow("output image", WINDOW_NORMAL);
    resizeWindow("output image", 640, 480);
    imshow("output image", image2);
    waitKey(0);
    return 0;
}