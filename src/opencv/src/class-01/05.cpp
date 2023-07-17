/**
 * 膨胀与腐蚀
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat src, dst;

char title1[] = "input";
char title2[] = "output";
int element_size =3;
int max_size = 21;
void CallBack(int, void*);  // 滑动条回调函数
int main(int argc, char** argv) {
    
    src = imread("/home/jhr/catkin_ws/image/LinuxLogo.jpg");
    if (src.empty()) {
        printf("could not read image\n");
        return -1;
    }

    namedWindow(title1, WINDOW_NORMAL);
    resizeWindow(title1, 640, 480);
    imshow(title1, src);

    namedWindow(title2, WINDOW_NORMAL);
    resizeWindow(title2, 640, 480);

    createTrackbar("Element size", title2, &element_size, max_size, CallBack);//创建滑动条
    CallBack(0, 0);

    waitKey(0);
}

//滑动条回调函数
void CallBack(int, void*) {
    int s = element_size*2+1;//保证掩膜为奇数
    Mat structureBlement = getStructuringElement(MORPH_RECT, Size(s, s), Point(-1, -1));//创建掩膜参数
    // erode(src, dst, structureBlement);//腐蚀
    dilate(src, dst, structureBlement,Point(-1,-1),1);//膨胀
    imshow(title2, dst);
    return;
}