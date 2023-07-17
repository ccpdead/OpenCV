/**
 * Kemeans图像聚类
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    Mat src = imread("/home/jhr/catkin_ws/image/cup6.jpg");
    if (src.empty()) {
    	printf("could not load image...\n");
    	return -1;
    }

    // Mat src;
    // VideoCapture cap("/home/jhr/catkin_ws/image/cup2.mp4");
    // if (!cap.isOpened()) {
    //     printf("cloud load img\n");
    //     return -1;
    // }

    // while (1) {
        // cap >> src;
        // if (src.empty()) {
        //     break;
        // }
        namedWindow("input image", WINDOW_AUTOSIZE);
        imshow("input image", src);

        // 创建colorTab
        Scalar colorTab[] = {Scalar(0, 0, 255), Scalar(0, 255, 0), Scalar(255, 0, 0), Scalar(0, 255, 255), Scalar(255, 0, 255)};

        int width = src.cols;
        int height = src.rows;
        int dims = src.channels();

        // 初始化定义
        int sampleCount = width * height;  // 数据点数
        int clusterCount = 5;              // 类型数
        Mat points(sampleCount, dims, CV_32F, Scalar(10));//定义三维的point点

        Mat labels;
        Mat centers(clusterCount, 1, points.type());//定义中心点

        // 将RGB数据转化为样本数据点
        int index = 0;
        for (int row = 0; row < height; row++) {
            for (int col = 0; col < width; col++) {
                index = row * width + col;
                Vec3b bgr = src.at<Vec3b>(row, col);
                // 将三维颜色数据转化为三个两维数据
                points.at<float>(index, 0) = static_cast<int>(bgr[0]);
                points.at<float>(index, 1) = static_cast<int>(bgr[1]);
                points.at<float>(index, 2) = static_cast<int>(bgr[2]);
            }
        }

        // 对颜色进行KMeans分类
        TermCriteria criteria = TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 0.1);
        kmeans(points, clusterCount, labels, criteria, 3, KMEANS_PP_CENTERS, centers);
        // 显示图像分割结果
        Mat result = Mat::zeros(src.size(), src.type());

        // 通过指针对图片三通道进行颜色处理
        for (int row = 0; row < height; row++) {
            for (int col = 0; col < width; col++) {
                index = row * width + col;
                int label = labels.at<int>(index, 0);
                result.at<Vec3b>(row, col)[0] = colorTab[label][0];
                result.at<Vec3b>(row, col)[1] = colorTab[label][1];
                result.at<Vec3b>(row, col)[2] = colorTab[label][2];
            }
        }

        for (int i = 0; i < centers.rows; i++) {
            int x = centers.at<float>(i, 0);
            int y = centers.at<float>(i, 1);
            printf("center %d = c.x : %d, c.y : %d\n", i, x, y);
        }

        imshow("KMeans Image Segmentation Demo", result);
        // char c = (char)waitKey(25);
        // if (c == 27)
        //     break;
    // }

    waitKey(0);
    return 0;
}