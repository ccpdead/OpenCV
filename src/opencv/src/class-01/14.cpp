/**
 * canny边缘检测
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
Mat src,dst,canny_color;
int t1_value = 50;
int max_vlaue = 255;
void Canny_Demo(int, void*);
int main(int argc, char** argv) {

    src = imread("/home/jhr/catkin_ws/image/cup3.jpg");
    if (src.empty()) {
        printf("could not load image\n");
        return -1;
    }
    namedWindow("input", WINDOW_AUTOSIZE);
    namedWindow("output", WINDOW_AUTOSIZE);
    imshow("input", src);

    cvtColor(src, dst, COLOR_BGR2GRAY);
    createTrackbar("threshold value", "output", &t1_value, max_vlaue, Canny_Demo);
    Canny_Demo(0, 0);

    waitKey(0);
    return 0;
}

void Canny_Demo(int, void*) {
    Mat edge_image,blur_dst;
    blur(dst, blur_dst, Size(3, 3), Point(-1, -1), BORDER_DEFAULT);
    Canny(blur_dst, edge_image, t1_value, t1_value * 2, 3, false);

    // canny_color.create(src.size(),src.type());
    // src.copyTo(canny_color,edge_image);

    imshow("output", ~edge_image);
}