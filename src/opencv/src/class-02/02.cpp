/**
 * Shi-Tomasi角点检测
 */

#include "iostream"
#include "opencv2/opencv.hpp"
#include "opencv2/xfeatures2d.hpp"

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;
Mat src, gray_src;
int num_corners = 25;
int max_corners = 200;
char* output_title = "shitomasi detector";
void ShiTomasi_Demo(int, void*);
RNG rng(12345);
int main(int argc, char** argv) {

    // src = imread("/home/jhr/catkin_ws/image/hdr.png");
    // if (src.empty()) {
    //     printf("could not load image\n");
    //     return -1;
    // }

    VideoCapture cap(0);
    if (!cap.isOpened()) {
        printf("could not open camera");
        return -1;
    }

    while (1) {
        cap >> src;
        if (src.empty()) {
            break;
        }

        resize(src, src, Size(640, 480), 0, 0, 1);

        imshow("input", src);
        namedWindow(output_title, WINDOW_AUTOSIZE);
        cvtColor(src, gray_src, COLOR_BGR2GRAY);
        createTrackbar("num corners", output_title, &num_corners, max_corners, ShiTomasi_Demo);
        ShiTomasi_Demo(0, 0);

        // waitKey(0);
        char c = waitKey(20);
        if ((int)c == 27) {
            break;
        }
    }
    return 0;
}

void ShiTomasi_Demo(int, void*) {
    if (num_corners < 5)
        num_corners = 5;
    vector<Point2f> corners;

    double qualityLevel = 0.01;
    double minDistance = 10;
    int blockSize = 3;
    bool useHarris = false;
    double k = 0.04;
    Mat resultImg = gray_src.clone();
    cvtColor(resultImg, resultImg, COLOR_GRAY2BGR);
    goodFeaturesToTrack(gray_src, corners, num_corners, qualityLevel, minDistance, Mat(), blockSize, useHarris, k);
    printf("number of detected conners: %d\n", corners.size());

    for (size_t t = 0; t < corners.size(); t++) {
        circle(resultImg, corners[t], 2, Scalar(0,0,255), 1, 8, 0);
    }
    imshow(output_title, resultImg);
}