/**
 * 人脸检测
 * 46714028 //haar
 * 12374580 //lbp
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

string haarfile = "/home/jhr/Downloads/opencv_build/opencv/data/haarcascades/haarcascade_frontalface_alt.xml";
string lbpfile = "/home/jhr/Downloads/opencv_build/opencv/data/lbpcascades/lbpcascade_frontalface.xml";

CascadeClassifier face_classifier;

int main(int argc, char** argv) {
    if (!face_classifier.load(lbpfile)) {
        printf("could not load face feature data....\n");
        return -1;
    }

    Mat src = imread("/home/jhr/catkin_ws/image/people.jpg");
    if (src.empty()) {
        printf("could not load image ....\n");
        return -1;
    }
    imshow("input image", src);
    Mat gray;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    equalizeHist(gray, gray);

    vector<Rect> face;
    int st = getTickCount();  // start
    face_classifier.detectMultiScale(gray, face, 1.2, 3, 0, Size(24, 24));
    int et = (getTickCount() - st);

    for (size_t t = 0; t < face.size(); t++) {
        rectangle(src, face[static_cast<int>(t)], Scalar(0, 0, 255), 2, 8, 0);
    }

    imshow("detect faces", src);
    printf("time:%d\n", et);
    waitKey(0);
    return 0;
}