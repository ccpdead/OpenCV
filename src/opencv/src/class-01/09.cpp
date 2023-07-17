/**
 * 基本阈值操作
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat dst, src, gray_src;
int threshold_value = 127;
int threshold_max = 255;
int type_value = 2;
int type_max = 5;
const char* output_title = "binary image";
void Threshold_Demo(int, void*);

int main(int argc, char** argv) {
    src = imread("/home/jhr/catkin_ws/image/jasoo-2.jpg");
    if (src.empty()) {
        printf("could not load image\n");
        return -1;
    }
    char input_win[] = "input image";

    namedWindow(input_win, WINDOW_AUTOSIZE);
    namedWindow(output_title, WINDOW_AUTOSIZE);
    imshow(input_win, src);

    createTrackbar("Threshold value", output_title, &threshold_value, threshold_max, Threshold_Demo);
    createTrackbar("Type value", output_title, &type_value, type_max, Threshold_Demo);

    Threshold_Demo(0, 0);

    waitKey(0);
    return 0;
}

void Threshold_Demo(int, void*) {
    cvtColor(src, gray_src, COLOR_BGR2GRAY);
    threshold(gray_src, dst, threshold_value, threshold_max, THRESH_TRIANGLE | type_value);
    imshow(output_title, dst);
}