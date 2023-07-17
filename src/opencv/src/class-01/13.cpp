/**
 * laplance算子
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    Mat src, dst;
    src = imread("/home/jhr/catkin_ws/image/cup.jpg");
    if (src.empty()) {
        printf("could not load image\n");
        return -1;
    }
    namedWindow("input", WINDOW_AUTOSIZE);
    imshow("input", src);

    GaussianBlur(src, dst, Size(3, 3), 0, 0);
    Mat gray_dst, edge_image;
    cvtColor(dst, gray_dst, COLOR_BGR2GRAY);

    //Laplocian梯度计算
    Laplacian(gray_dst, edge_image, CV_16S, 3);
    convertScaleAbs(edge_image, edge_image);  // 将图像转化为8bit

    //二值化
    threshold(edge_image,edge_image,0,255,THRESH_OTSU|THRESH_BINARY);

    imshow("edge-image", edge_image);

    waitKey(0);
    return 0;
}
