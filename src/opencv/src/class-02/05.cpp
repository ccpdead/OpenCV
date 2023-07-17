/**
 * Shi-Tomasi角点检测
 */

#include "iostream"
#include "opencv2/opencv.hpp"
#include "opencv2/xfeatures2d.hpp"

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

int main(int argc, char** argv) {
    Mat src;

    // src = imread("/home/jhr/catkin_ws/image/people2.jpg");
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
        cvtColor(src, src, COLOR_BGR2GRAY);
        imshow("input", src);

        int numFeatures = 200;
        Ptr<SIFT> detector = SIFT::create(numFeatures);//创建检测器
        vector<KeyPoint> keypoints;
        detector->detect(src, keypoints, Mat());
        printf("total keyPoints:%d\n", keypoints.size());

        Mat keypoint_img;
        drawKeypoints(src, keypoints, keypoint_img, Scalar(0, 0, 255), DrawMatchesFlags::DEFAULT);
        imshow("sift keyPoints", keypoint_img);

        // waitKey(0);
        char c = waitKey(20);
        if ((int)c == 27) {
            break;
        }
    }
    return 0;
}