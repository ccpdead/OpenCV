/**
 * 图像矩
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat src, dst;
int threshold_value = 50;
int threshold_max = 255;
RNG rng(12345);
void Demo_Moments(int, void*);

int main(int argc, char** argv) {
    src = imread("/home/jhr/catkin_ws/image/smarties.png");
    if (src.empty()) {
        printf("could not load image\n");
        return -1;
    }

    namedWindow("input", WINDOW_AUTOSIZE);
    namedWindow("output", WINDOW_AUTOSIZE);
    // imshow("input", src);
    cvtColor(src, dst, COLOR_BGR2GRAY);
    GaussianBlur(dst, dst, Size(3, 3), 0, 0);
    imshow("input", dst);

    createTrackbar("threshold", "output", &threshold_value, threshold_max, Demo_Moments);
    Demo_Moments(0, 0);

    waitKey(0);
    return 0;
}

void Demo_Moments(int, void*) {
    Mat canny_output;
    vector<vector<Point>> contours;
    vector<Vec4i> hierachy;

    Canny(dst, canny_output, threshold_value, threshold_value * 2, 3, false);
    findContours(canny_output, contours, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

    vector<Moments> contours_moments(contours.size());
    vector<Point2f> ccs(contours.size());

    for (size_t i = 0; i < contours.size(); i++) {
        contours_moments[i] = moments(contours[i]);
        // 中心
        ccs[i] = Point(static_cast<float>(contours_moments[i].m10 / contours_moments[i].m00),
                       static_cast<float>(contours_moments[i].m01 / contours_moments[i].m00));
    }

    Mat drawImg;
    src.copyTo(drawImg);
    for (size_t k = 0; k < contours.size(); k++) {
        printf("center pointx:%.2f, center pointy:%.2f\n", ccs[k].x, ccs[k].y);
        printf("contours%d,area:%.2f,length%.2f\r\n", k, contourArea(contours[k]), arcLength(contours[k], true));
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        drawContours(drawImg, contours, k, color, 2, 8, hierachy, 0, Point(0, 0));
        circle(drawImg, ccs[k], 2, color, 2, 8);
    }

    imshow("output", drawImg);
}
