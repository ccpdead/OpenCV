/**
 * 分水岭算法
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    Mat src;
    src = imread("/home/jhr/catkin_ws/image/cards.png");
    if (src.empty()) {
        printf("could not load image\n");
        return -1;
    }
    // imshow("input", src);

    // change background;
    for (int row = 0; row < src.rows; row++) {
        for (int col = 0; col < src.cols; col++) {
            if (src.at<Vec3b>(row, col) == Vec3b(255, 255, 255)) {
                src.at<Vec3b>(row, col)[0] = 0;
                src.at<Vec3b>(row, col)[1] = 0;
                src.at<Vec3b>(row, col)[2] = 0;
            }
        }
    }
    // imshow("background", src);

    // sharpen
    Mat kernel = (Mat_<float>(3, 3) << 1, 1, 1, 1, -8, 1, 1, 1, 1);
    Mat imgLaplance;
    Mat sharpenImg;
    src.convertTo(sharpenImg, CV_32F);
    filter2D(src, imgLaplance, CV_32F, kernel, Point(-1, -1), 0, BORDER_DEFAULT);
    Mat resultImg = sharpenImg - imgLaplance;

    resultImg.convertTo(resultImg, CV_8UC3);
    imgLaplance.convertTo(imgLaplance, CV_8UC3);
    // imshow("resultImg", resultImg);

    // 二值化
    Mat binaryImg;
    cvtColor(resultImg, resultImg, COLOR_BGR2GRAY);
    threshold(resultImg, binaryImg, 40, 255, THRESH_BINARY | THRESH_OTSU);
    imshow("binaryImg", binaryImg);

    // distance transformation
    Mat disImg;
    distanceTransform(binaryImg, disImg, DIST_L1, 3, 5);
    normalize(disImg, disImg, 0, 1, NORM_MINMAX);  // 归一化
    imshow("disImg", disImg);

    // binary again
    threshold(disImg, disImg, .4, 1, THRESH_BINARY);
    Mat k1 = Mat::ones(5, 5, CV_8UC1);  // 定义掩膜
    erode(disImg, disImg, k1, Point(-1, -1));//腐蚀图像
    imshow("binary disimg", disImg);

    // marker
    Mat dist_8u;
    disImg.convertTo(dist_8u, CV_8U);
    vector<vector<Point>> contours;
    findContours(dist_8u, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

    // create makers
    Mat markers = Mat::zeros(src.size(), CV_32SC1);
    for (size_t i = 0; i < contours.size(); i++) {
        drawContours(markers, contours, static_cast<int>(i), Scalar::all(static_cast<int>(i) + 1),1);
    }
    circle(markers, Point(5, 5), 3, Scalar(255, 255, 255), -1);
    // imshow("markers", markers * 1000);

    // perform watershed transform
    watershed(src, markers);
    Mat mark = Mat::zeros(markers.size(), CV_8UC1);
    markers.convertTo(mark, CV_8UC1);
    bitwise_not(mark, mark, Mat());
    imshow("watershed image", mark);

    // generate random color
    vector<Vec3b> colors;
    for (size_t i = 0; i < contours.size(); i++) {
        int r = theRNG().uniform(0, 255);
        int g = theRNG().uniform(0, 255);
        int b = theRNG().uniform(0, 255);

        colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));  // 将颜色放到vector中
    }

    // file with color and display final result
	Mat dst = Mat::zeros(markers.size(), CV_8UC3);
	for (int row = 0; row < markers.rows; row++) {
		for (int col = 0; col < markers.cols; col++) {
			int index = markers.at<int>(row, col);
			if (index > 0 && index <= static_cast<int>(contours.size())) {
				dst.at<Vec3b>(row, col) = colors[index - 1];
			}
			else {
				dst.at<Vec3b>(row, col) = Vec3b(0, 0, 0);
			}
		}
	}

    imshow("final result", dst);

    waitKey(0);
    return 0;
}