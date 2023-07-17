// /**
//  * 霍夫直线检测
//  */
// #include <iostream>
// #include <opencv2/opencv.hpp>

// using namespace std;
// using namespace cv;

// int minLineLength = 0;
// int max1 = 30;
// int maxLineGap = 0;
// int max2 = 30;
// int Threshold = 0;
// int max3 = 30;
// Mat src, dst, line_image;

// void line_demo(int, void*);

// int main(int argc, char** argv) {
//     src = imread("/home/jhr/catkin_ws/image/cup.jpg");
//     if (src.empty()) {
//         printf("could not load image\n");
//         return -1;
//     }
//     namedWindow("input", WINDOW_AUTOSIZE);
//     namedWindow("output", WINDOW_AUTOSIZE);
//     imshow("input", src);

//     cvtColor(src, dst, COLOR_BGR2GRAY);
//     Canny(dst, dst, 100, 200);  // 低阈值，高阈值
//     imshow("canny", dst);

//     createTrackbar("threshold", "output", &Threshold, max3, line_demo);
//     createTrackbar("minLineLength", "output", &minLineLength, max1, line_demo);
//     createTrackbar("maxLineGap", "output", &maxLineGap, max2, line_demo);

//     waitKey(0);
//     return 0;
// }

// void line_demo(int, void*) {
//     vector<Vec4f> plines;
//     HoughLinesP(dst, plines, 1, CV_PI / 180.0, Threshold, minLineLength, maxLineGap);
//     Scalar color = Scalar(0, 0, 255);
//     line_image = Mat(src.size(), src.type());

//     for (size_t i = 0; i < plines.size(); i++) {
//         Vec4f hline = plines[i];
//         line(line_image, Point(hline[0], hline[1]), Point(hline[2], hline[3]), color, 3, LINE_AA);
//     }
//     imshow("output", line_image);
// }

/**
 * 霍夫圆检测
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    Mat src, dst;
    src = imread("/home/jhr/catkin_ws/image/cup3.jpg");
    if (src.empty()) {
        printf("could not load image\n");
        return -1;
    }
    namedWindow("input", WINDOW_AUTOSIZE);
    imshow("input", src);

    // 中值滤波
    Mat moutput;
    medianBlur(src, moutput, 3);
    cvtColor(moutput, moutput, COLOR_BGR2GRAY);
    Canny(moutput, moutput, 70, 200);  // 低阈值，高阈值
    imshow("canny", moutput);

    // 霍夫圆检测
    vector<Vec3f> pcircels;
    HoughCircles(moutput, pcircels, HOUGH_GRADIENT, 1, 10, 300, 35, 5, 100);
    src.copyTo(dst);
    for (size_t i = 0; i < pcircels.size(); i++) {
        Vec3f cc = pcircels[i];
        circle(dst, Point(cc[0], cc[1]), cc[2], Scalar(0, 0, 255), 2, LINE_AA);
        circle(dst, Point(cc[0], cc[1]), 2, Scalar(0, 255, 0), 2, LINE_AA);
    }

    imshow("output", dst);

    waitKey(0);
    return 0;
}