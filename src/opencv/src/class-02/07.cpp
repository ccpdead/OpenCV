/**
 * HOG特征检测
 *
 */

#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    Mat src;
    src = imread("/home/jhr/catkin_ws/image/face_detect.png");
    if (src.empty()) {
        printf("could not load image\n");
        return -1;
    }
    imshow("input", src);

	HOGDescriptor hog = HOGDescriptor();
	hog.setSVMDetector(hog.getDefaultPeopleDetector());

	vector<Rect> foundLocations;
	hog.detectMultiScale(src, foundLocations, 0, Size(8, 8), Size(32, 32), 1.05, 2);
	Mat result = src.clone();
	for (size_t t = 0; t < foundLocations.size(); t++) {
		rectangle(result, foundLocations[t], Scalar(0, 0, 255), 2, 8, 0);
	}
	namedWindow("HOG SVM Detector Demo", WINDOW_AUTOSIZE);
	imshow("HOG SVM Detector Demo", result);

    waitKey(0);
    return 0;
}