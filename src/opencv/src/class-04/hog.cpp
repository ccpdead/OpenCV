#include <opencv2/opencv.hpp>

int main() {
    // 加载图像
    cv::Mat image = cv::imread("/home/jhr/catkin_ws/image/cup1.jpg", cv::IMREAD_COLOR);

    // 将图像转换为灰度图
    cv::Mat grayImage;
    cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);

    // 创建HOG描述符
    cv::HOGDescriptor hog;

    // 设置HOG参数（可选）
    hog.winSize = cv::Size(64, 128);  // HOG窗口大小
    hog.blockSize = cv::Size(16, 16);  // 块大小
    hog.blockStride = cv::Size(8, 8);  // 块步长
    hog.cellSize = cv::Size(8, 8);  // 单元格大小
    hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());  // 使用默认的SVM检测器

    // 对图像进行HOG特征检测
    std::vector<cv::Rect> detections;
    hog.detectMultiScale(grayImage, detections);

    // 在图像上绘制检测结果矩形框
    for (const auto& detection : detections) {
        cv::rectangle(image, detection, cv::Scalar(0, 255, 0), 2);
    }

    // 显示结果图像
    cv::imshow("HOG Detection", image);
    cv::waitKey(0);

    return 0;
}
