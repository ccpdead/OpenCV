/**
 * GMM数据聚类
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace cv::ml;

int main(int argc, char** argv) {
    Mat src = imread("/home/jhr/catkin_ws/image/cup3.jpg");
    if (src.empty()) {
        printf("cloud not load image\n");
        return -1;
    }

    int width = src.cols;
    int height = src.rows;
    int dims = src.channels();
    int nsamples = width * height;

    Mat points(nsamples, dims, CV_64FC1);
    Mat labels;
    Mat result = Mat ::zeros(src.size(), CV_8UC3);

    int numCluster = 3;  // 样本数
    Scalar colorTab[] = {Scalar(0, 0, 255), Scalar(0, 255, 0), Scalar(255, 0, 0), Scalar(0, 255, 255), Scalar(255, 0, 255)};

    // 图像RGB像素转化为样本数据
    int index = 0;
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            index = row * width + col;
            Vec3b rgb = src.at<Vec3b>(row, col);
            points.at<double>(index, 0) = static_cast<int>(rgb[0]);
            points.at<double>(index, 1) = static_cast<int>(rgb[1]);
            points.at<double>(index, 2) = static_cast<int>(rgb[2]);
        }
    }

    // EM Cluster train
    /*注意，EM需要调用cv::ML*/
    Ptr<EM> em_model = EM::create();
    em_model->setClustersNumber(numCluster);
    em_model->setCovarianceMatrixType(EM::COV_MAT_SPHERICAL);                                    // 设置矩阵类型
    em_model->setTermCriteria(TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 100, 0.2));  // 设置参数
    em_model->trainEM(points, noArray(), labels, noArray());                                     // 训练数据

    // 对每个像素标记颜色与显示
    Mat sample(dims, 1, CV_64FC1);
    int time = getTickCount();
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
          index = row*width+col;
          int label = labels.at<int>(index,0);
          Scalar c=colorTab[label];
          result.at<Vec3b>(row,col)[0] = c[0];
          result.at<Vec3b>(row,col)[1] = c[1];
          result.at<Vec3b>(row,col)[2] = c[2];
        }
    }

    imshow("input", src);
    imshow("EM-Segmentation",result);

    printf("execution time: %d\n",(time-getTickCount())*1000/getTickFrequency());
    waitKey(0);
    return 0;
}