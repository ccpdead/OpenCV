/**
 * 点多边形
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
RNG rng(12345);
int main(int argc, char** argv) {
    const int r = 100;
    Mat src = Mat::zeros(r * 4, r * 4, CV_8UC1);

    vector<Point2f> vert(6);  // 定义六个点
    vert[0] = Point(3 * r / 2, static_cast<int>(1.34 * r));
    vert[1] = Point(1 * r, 2 * r);
    vert[2] = Point(3 * r / 2, static_cast<int>(2.866 * r));
    vert[3] = Point(5 * r / 2, static_cast<int>(2.866 * r));
    vert[4] = Point(3 * r, 2 * r);
    vert[5] = Point(5 * r / 2, static_cast<int>(1.34 * r));

    for (int i = 0; i < 6; i++) {
        // Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        line(src, vert[i], vert[(i + 1) % 6], Scalar(255), 3, 8, 0);
    }

    vector<vector<Point>> contours;
    vector<Vec4i> hierachy;
    Mat csrc;
    src.copyTo(csrc);
    // imshow("csrc", csrc);
    findContours(csrc, contours, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

    // draw contours;
    //  Mat contoursImg = Mat::zeros(src.size(),CV_8UC3);
    //  for(size_t i = 0; i <contours.size(); i++) {
    //      drawContours(contoursImg,contours,i,Scalar(0,0,255),2,LINE_8,hierachy,0,Point(0,0));
    //  }
    //  imshow("contoursImg",contoursImg);

    Mat raw_dist = Mat::zeros(csrc.size(), CV_32FC1);  // 创建图层

    // 遍历图片每一个像素
    for (int row = 0; row < raw_dist.rows; row++) {
        for (int col = 0; col < raw_dist.cols; col++) {
            double dist = pointPolygonTest(contours[0], Point2f(static_cast<float>(col), static_cast<float>(row)), true);
            raw_dist.at<float>(row, col) = static_cast<float>(dist);  // 使用最大值代替
        }
    }

    imshow("raw_dist", raw_dist);

    double minValue = 0, maxValue = 0;
    minMaxLoc(raw_dist, &minValue, &maxValue, 0, 0, Mat());  // 提取最大值最低值
    printf("min%.2f,max%.2f\n", minValue, maxValue);

    // opencv图像是三通道，bgr
    Mat drawImg = Mat::zeros(src.size(), CV_8UC3);
    // 遍历图片每一个像素
    for (int row = 0; row < drawImg.rows; row++) {
        for (int col = 0; col < drawImg.cols; col++) {
            float dist = raw_dist.at<float>(row, col);  // 计算距离
            if (dist > 0) {                             // 外部
                // 在第一通道绘制，blue
                drawImg.at<Vec3b>(row, col)[0] = (uchar)(abs(255.0 - (dist / maxValue))* 255);
            } else if (dist < 0) {  // 内部
                // 在第三通道绘制，red
                drawImg.at<Vec3b>(row, col)[2] = (uchar)(abs(1.0 - (dist / minValue)) * 255);
            } else {  // 边缘
                // 在三通道绘制
                drawImg.at<Vec3b>(row, col)[0] = (uchar)(abs(255 - dist));
                drawImg.at<Vec3b>(row, col)[1] = (uchar)(abs(255 - dist));
                drawImg.at<Vec3b>(row, col)[2] = (uchar)(abs(255 - dist));
            }
        }
    }

    imshow("src", src);
    imshow("output", drawImg);
    waitKey(0);
    return 0;
}