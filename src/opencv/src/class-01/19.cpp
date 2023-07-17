/**
 * 轮廓检测
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
Mat src, dst;

int threshold_value = 100;
int threshold_max = 255;

void Demo(int,void*);
int main(int argc, char** argv) {
    
    src = imread("/home/jhr/catkin_ws/image/cup2.jpg");
    if (src.empty()) {
        printf("could not load image\n");
        return -1;
    }
    namedWindow("input", WINDOW_AUTOSIZE);
    namedWindow("output", WINDOW_AUTOSIZE);
    imshow("input", src);

    cvtColor(src,src,COLOR_BGR2GRAY);
    createTrackbar("Threshold","output",&threshold_value,threshold_max,Demo);
    Demo(0,0);

    waitKey(0);
    return 0;
}

void Demo(int,void*){
    Mat canny_output;
    vector<vector<Point>>contours;
    vector<Vec4i>hierachy;
    Canny(src,canny_output,threshold_value,threshold_value*2,3,false);
    findContours(canny_output,contours,hierachy,RETR_TREE,CHAIN_APPROX_SIMPLE,Point(0,0));

    dst = Mat::zeros(src.size(),CV_8UC3);
    RNG rng(12345);
    for(size_t i = 0; i < contours.size(); i++) {
        Scalar color = Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
        drawContours(dst,contours,i,color,2,8,hierachy,0,Point(0,0));
    }

    imshow("output",dst);
}
