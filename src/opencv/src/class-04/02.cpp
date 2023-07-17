/**
 * 人脸与人眼检测
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

CascadeClassifier face_cascader;
CascadeClassifier eye_cascader;

string facefile = "/home/jhr/Downloads/opencv_build/opencv/data/haarcascades/haarcascade_frontalface_alt.xml";
string eyefile = "/home/jhr/Downloads/opencv_build/opencv/data/haarcascades/haarcascade_eye.xml";

int main(int argc, char** argv) {
    if (!face_cascader.load(facefile)) {
        printf("could load facefile\n");
        return -1;
    }
    if (!eye_cascader.load(eyefile)) {
        printf("could load eyefile\n");
        return -1;
    }
    namedWindow("camera-demo", WINDOW_AUTOSIZE);

    Mat src = imread("/home/jhr/catkin_ws/image/people2.jpg");
    // Mat src;
    // VideoCapture cap(0);
    // if (!cap.isOpened()) {
    //     printf("could not open img\n");
    //     return -1;
    // }

    // while (1) {
    //     cap >> src;
    //     if (src.empty()) {
    //         break;
    //     }
        // resize(src,src,Size(640,480),0,0,1);

        Mat gray;
        vector<Rect> faces;
        vector<Rect> eyes;
        cvtColor(src, gray, COLOR_BGR2GRAY);  // 灰度化
        equalizeHist(gray, gray);             // 均衡直方图
        imshow("equ",gray);
        // 检测人脸
        face_cascader.detectMultiScale(gray, faces, 1.2, 4, 0, Size(40, 40));
        // 检测人眼
        for (size_t t = 0; t < faces.size(); t++) {
            Rect roi;
            roi.x = faces[static_cast<int>(t)].x;
            roi.y = faces[static_cast<int>(t)].y;
            roi.width = faces[static_cast<int>(t)].width;
            roi.height = faces[static_cast<int>(t)].height / 2;
            Mat faceROI = src(roi);  // 定义掩膜
            eye_cascader.detectMultiScale(faceROI, eyes, 1.2, 3, 0, Size(20, 20));
            for (size_t k = 0; k < eyes.size(); k++) {
                Rect rect;
                rect.x = faces[static_cast<int>(t)].x + eyes[k].x;
                rect.y = faces[static_cast<int>(t)].y + eyes[k].y;
                rect.width = eyes[k].width;
                rect.height = eyes[k].height;
                putText(src, "eye", Point(rect.x, (rect.y + rect.height * 1.5)), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 0, 0), 1, LINE_8);

                rectangle(src, rect, Scalar(0, 255, 0), 2, 8, 0);
            }
            putText(src, "face", Point(faces[static_cast<int>(t)].x, faces[static_cast<int>(t)].y), FONT_HERSHEY_COMPLEX, 0.7, Scalar(0, 255, 0), 1,
                    LINE_8);
            rectangle(src, faces[static_cast<int>(t)], Scalar(0, 0, 255), 2, 8, 0);
        }

        imshow("camera-demo", src);
        waitKey(0);
        // char c = (char)waitKey(25);
        // if (c == 27) {
        //     break;
        // }
    // }
    return 0;
}