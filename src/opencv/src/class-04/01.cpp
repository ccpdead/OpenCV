/**
 * 人脸检测
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

string filename = "/home/jhr/Downloads/opencv_build/opencv/data/haarcascades/haarcascade_frontalface_alt.xml";
string filename2 = "/home/jhr/Downloads/opencv_build/opencv/data/haarcascades/haarcascade_frontalcatface_extended.xml";
string myfile = "/home/jhr/catkin_ws/image/cascade.xml";
CascadeClassifier face_classifier;

int main(int argc, char** argv) {
    if (!face_classifier.load(filename)) {
        printf("could not load face feature data....\n");
        return -1;
    }

    // Mat src = imread("/home/jhr/catkin_ws/image/people_face.png");
    // if (src.empty()) {
    //     printf("could not load image ....\n");
    //     return -1;
    // }

    Mat src;
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        printf("could not open img\n");
        return -1;
    }
    while (1) {
    cap >> src;
    if (src.empty()) {
        break;
    }
    resize(src, src, Size(640, 480), 0, 0, 1);
    imshow("input image", src);
    Mat gray;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    // blur(gray,gray,Size(3,3));
    GaussianBlur(gray,gray,Size(3,3),5,5);
    // equalizeHist(gray, gray);//直方图均衡化
    imshow("gray",gray);

    vector<Rect> face;
    face_classifier.detectMultiScale(gray, face, 1.03, 2, 0, Size(30, 30));
    for (size_t t = 0; t < face.size(); t++) {
        putText(src, "face", Point(face[static_cast<int>(t)].x, face[static_cast<int>(t)].y), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255), 1, LINE_8);
        rectangle(src, face[static_cast<int>(t)], Scalar(0, 0, 255), 2, 8, 0);
    }

    imshow("detect faces", src);
    // waitKey(0);

    char c = (char)waitKey(20);
    if (c == 27) {
        break;
    }
    }
    return 0;
}