/**
 * 模板匹配
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat src, temp, dst;
int match_method = TM_SQDIFF;
int max_track = 5;
void Match_Demo(int, void*);

int main(int argc, char** argv) {
    // 加载待检测图片
    src = imread("/home/jhr/catkin_ws/image/cup.jpg");
    // 加载模板
    temp = imread("/home/jhr/catkin_ws/image/cup_temp.jpg");
    if (src.empty() || temp.empty()) {
        printf("could not load image\n");
        return -1;
    }
    cvtColor(src, src, COLOR_BGR2GRAY);
    cvtColor(temp, temp, COLOR_BGR2GRAY);

    //namedWindow("input", WINDOW_AUTOSIZE);
    namedWindow("result", WINDOW_AUTOSIZE);
    //imshow("input", src);
    imshow("temp",temp);

    createTrackbar("Match", "result", &match_method, max_track, Match_Demo);
    Match_Demo(0, 0);

    waitKey(0);
    return 0;
}

void Match_Demo(int, void*) {
    int width = src.cols - temp.cols + 1;
    int height = src.rows - temp.rows + 1;
    Mat result(width, height, CV_32FC1);

    matchTemplate(src, temp, result, match_method, Mat());
    normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

    Point minLoc;
    Point maxLoc;
    double min, max;
    src.copyTo(dst);
    Point temLoc;
    minMaxLoc(result, &min, &max, &minLoc, &maxLoc, Mat());
    if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED) {
        temLoc = minLoc;
    } else {
        temLoc = maxLoc;
    }

    // 绘制矩形
    rectangle(dst, Rect(temLoc.x, temLoc.y, temp.cols, temp.rows), Scalar(0, 0, 255), 2, 8);
    rectangle(result, Rect(temLoc.x, temLoc.y, temp.cols, temp.rows), Scalar(0, 0, 255), 2, 8);

    imshow("match", dst);
    imshow("result", result);
}