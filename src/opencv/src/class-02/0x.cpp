#include <iostream>
#include <opencv2/opencv.hpp>
// #include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

int main(int argc, char** argv) {
    Mat src = imread("/home/jhr/catkin_ws/image/people2.jpg");
    if (src.empty()) {
    	printf("could not load image...\n");
    	return -1;
    }

    // Mat src;
    // VideoCapture cap(0);
    // if (!cap.isOpened()) {
    //     printf("cloud not open camera\n");
    //     return -1;
    // }

    // while (1) {
    //     cap >> src;
    //     if (src.empty()) {
    //         break;
    //     }
        resize(src, src, Size(640, 480), 0, 0, 1);
        cvtColor(src,src,COLOR_BGR2GRAY);
        namedWindow("input image", WINDOW_AUTOSIZE);
        imshow("input image", src);

        // SURF特征检测
        int minHessian = 100;
        Ptr<SURF> detector = SURF::create(minHessian);
        vector<KeyPoint> keypoints;
        detector->detect(src, keypoints, Mat());
        printf("total keypoints:%d\n",keypoints.size());

        // // 绘制关键点
        // Mat keypoint_img;
        // drawKeypoints(src, keypoints, keypoint_img, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
        // imshow("KeyPoints Image", keypoint_img);

        waitKey(0);
        // char c = waitKey(20);
        // if ((int)c == 27) {
        //     break;
        // }
    // }
    return 0;
}