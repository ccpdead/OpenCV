#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

/*将图像三彩色数据转化为二维样本数据*/
Mat mat_to_samples(Mat& image);

int main(int argc, char** argv) {
    Mat src = imread("/home/jhr/catkin_ws/image/tx.png");
    if (src.empty()) {
        printf("could not load image...\n");
        return -1;
    }
    namedWindow("input image", WINDOW_AUTOSIZE);
    imshow("input image", src);

    // 组装数据
    Mat points = mat_to_samples(src);

    // 运行KMeans
    int numCluster = 5;
    Mat labels;
    Mat centers;
    TermCriteria criteria = TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 0.1);
    //Keans数据
    kmeans(points, numCluster, labels, criteria, 5, KMEANS_PP_CENTERS, centers);

    // 去背景+遮罩生成
    Mat mask = Mat::zeros(src.size(), CV_8UC1);
    int index = src.rows * 2 + 2;//2，4，6........
    int cindex = labels.at<int>(index, 0);
    int height = src.rows;
    int width = src.cols;
    // Mat dst;
    // src.copyTo(dst);
    //循环每个像素
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            index = row * width + col;
            int label = labels.at<int>(index, 0);
            if (label == cindex) {  // 将背景配置为黑色
                // dst.at<Vec3b>(row, col)[0] = 0;
                // dst.at<Vec3b>(row, col)[1] = 0;
                // dst.at<Vec3b>(row, col)[2] = 0;
                mask.at<uchar>(row, col) = 0;
            } else {
                mask.at<uchar>(row, col) = 255;//非背景配置为白色
            }
        }
    }
    imshow("mask", mask);

    // 腐蚀 + 高斯模糊
    Mat k = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
    erode(mask, mask, k);
    // imshow("erode-mask", mask);
    GaussianBlur(mask, mask, Size(3, 3), 0, 0);
    // imshow("Blur Mask", mask);

    // 通道混合
    RNG rng(12345);
    Vec3b color;
    color[0] = 40;  // rng.uniform(0, 255);
    color[1] = 40;   // rng.uniform(0, 255);
    color[2] = 200;  // rng.uniform(0, 255);
    Mat result(src.size(), src.type());

    double w = 0.0;
    int b = 0, g = 0, r = 0;
    int b1 = 0, g1 = 0, r1 = 0;
    int b2 = 0, g2 = 0, r2 = 0;

    //循环每个像素
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int m = mask.at<uchar>(row, col);
            if (m == 255) {
                result.at<Vec3b>(row, col) = src.at<Vec3b>(row, col);  // 前景
            } else if (m == 0) {
                result.at<Vec3b>(row, col) = color;  // 背景
            } else {
                w = m / 255.0;
                //原图像素值
                b1 = src.at<Vec3b>(row, col)[0];
                g1 = src.at<Vec3b>(row, col)[1];
                r1 = src.at<Vec3b>(row, col)[2];

                //背景像素值
                b2 = color[0];
                g2 = color[1];
                r2 = color[2];

                //进行混合,实现过渡像素
                b = b1 * w + b2 * (1.0 - w);
                g = g1 * w + g2 * (1.0 - w);
                r = r1 * w + r2 * (1.0 - w);

                result.at<Vec3b>(row, col)[0] = b;
                result.at<Vec3b>(row, col)[1] = g;
                result.at<Vec3b>(row, col)[2] = r;
            }
        }
    }
    imshow("背景替换", result);

    waitKey(0);
    return 0;
}

Mat mat_to_samples(Mat& image) {
    int w = image.cols;
    int h = image.rows;
    int samplecount = w * h;
    int dims = image.channels();
    Mat points(samplecount, dims, CV_32F, Scalar(10));

    //图片像素数据化，将像素的三维彩色信息转化为二维数据
    int index = 0;
    for (int row = 0; row < h; row++) {
        for (int col = 0; col < w; col++) {
            index = row * w + col;//存放三维像素每位的位置
            Vec3b bgr = image.at<Vec3b>(row, col);
            points.at<float>(index, 0) = static_cast<int>(bgr[0]);
            points.at<float>(index, 1) = static_cast<int>(bgr[1]);
            points.at<float>(index, 2) = static_cast<int>(bgr[2]);
        }
    }
    //返回参数化图像数据
    return points;
}