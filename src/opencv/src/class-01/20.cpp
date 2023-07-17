/**
 * 凸包
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat src, dst;
RNG rng(12345);
int threshold_value = 100;
int threshold_max = 255;
void Demo_Contours(int, void*);

int main(int argc, char** argv) {
    src = imread("/home/jhr/catkin_ws/image/cup_temp.png");
    if (src.empty()) {
        printf("could not load image\n");
        return -1;
    }
    namedWindow("input", WINDOW_AUTOSIZE);
    namedWindow("output", WINDOW_AUTOSIZE);

    cvtColor(src, src, COLOR_BGR2GRAY);
    blur(src, src, Size(3, 3), Point(-1, -1), BORDER_DEFAULT);
    // imshow("input", src);
    createTrackbar("Threshold", "output", &threshold_value, threshold_max, Demo_Contours);
    Demo_Contours(0, 0);

    waitKey(0);
    return 0;
}

void Demo_Contours(int, void*) {
    Mat bin_output;
    vector<vector<Point>> contours;
    vector<Vec4i> hierachy;

    //二值化
    threshold(src, bin_output, threshold_value, threshold_max, !THRESH_BINARY);
    imshow("input",bin_output);
    findContours(bin_output, contours, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

    
    vector<vector<Point>> convexs(contours.size());
    for (size_t i = 0; i < contours.size(); i++) {
        convexHull(contours[i], convexs[i],false,true);
    }

    //绘制凸包
    dst = Mat::zeros(src.size(),CV_8UC3);
    vector<Vec4i>empty(0);
    for(size_t k=0;k<contours.size();k++) {
        Scalar color = Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
        drawContours(dst,contours,k,color,2,LINE_8,hierachy,0,Point(0,0));
        drawContours(dst,convexs,k,color,2,LINE_8,empty,0,Point(0,0));
    }

    imshow("output",dst);
}