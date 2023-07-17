/**
 * 混阔周围绘制矩形圆形框
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

VideoCapture cap("/home/jhr/catkin_ws/image/cup.mp4");
Mat src, dst;
RNG rng(12345);
int threshold_v = 170;
int threshold_max = 255;
void Contours_Callback(int, void*);

int main(int argc, char** argv) {
    namedWindow("input", WINDOW_AUTOSIZE);
    namedWindow("output", WINDOW_AUTOSIZE);

    // image
    // src = imread("/home/jhr/catkin_ws/image/cup3.jpg");
    // if (src.empty()) {
    //     printf("could not load image\n");
    //     return -1;
    // }

    // vedio
    if (!cap.isOpened()) {
        printf("could not load vedio\n");
        return -1;
    }
    while (1) {
        cap >> src;
        if (src.empty())
            break;
        cvtColor(src, src, COLOR_BGR2GRAY);
        blur(src, src, Size(3, 3), Point(-1, -1));
        // imshow("input", src);

        createTrackbar("Threshold", "output", &threshold_v, threshold_max, Contours_Callback);
        Contours_Callback(0, 0);
        char c = (char)waitKey(25);
        if (c == 27)
            break;
    }

    // when everything is done, release the vedio capture object
    cap.release();

    // waitKey(0);
    return 0;
}

void Contours_Callback(int, void*) {
    Mat binary_output;
    vector<vector<Point>> contours;
    vector<Vec4i> hierachy;
    threshold(src, binary_output, threshold_v, threshold_max, THRESH_BINARY);
    imshow("input", binary_output);
    findContours(binary_output, contours, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(-1, -1));

    vector<vector<Point>> contours_ploy(contours.size());  // 轮廓
    vector<Rect> ploy_rects(contours.size());              // 矩形
    vector<Point2f> ccs(contours.size());                  // 圆心
    vector<float> radius(contours.size());                 // 圆半径

    vector<RotatedRect> minRects(contours.size());
    vector<RotatedRect> myellipse(contours.size());

    for (size_t i = 0; i < contours.size(); i++) {
        approxPolyDP(Mat(contours[i]), contours_ploy[i], 3, true);
        ploy_rects[i] = boundingRect(contours_ploy[i]);
        minEnclosingCircle(contours_ploy[i], ccs[i], radius[i]);

        if (contours_ploy[i].size() > 5) {
            myellipse[i] = fitEllipse(contours_ploy[i]);
            minRects[i] = minAreaRect(contours_ploy[i]);
        }
    }

    // Draw
    // Mat drawImage = Mat::zeros(src.size(),src.type());
    dst = Mat::zeros(src.size(), CV_8UC3);
    Point2f pts[4];

    for (size_t k = 0; k < contours.size(); k++) {
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        // rectangle(dst, ploy_rects[k], color, 2, 8);//绘制矩形
        // circle(dst, ccs[k], radius[k], color, 2, 8);//绘制圆形

        if (contours_ploy[k].size() > 5) {
            ellipse(dst, myellipse[k], color, 2, 8);
            minRects[k].points(pts);
            for (int r = 0; r < 4; r++) {
                line(dst, pts[r], pts[(r + 1) % 4], color, 2, 8);
            }
        }
    }

    imshow("output", dst);
    return;
}
