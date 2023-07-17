#include <math.h>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int numRun = 0;
Rect rect;
bool init = false;
Mat src, image;
Mat mask, bgModel, fgModel;
const char* winTitle = "input image";

/*回调函数*/
void onMouse(int event, int x, int y, int flags, void* param);
void setROIMask();
void showImage();
void runGrabCut();


int main(int argc, char** argv) {
    src = imread("/home/jhr/catkin_ws/image/people.jpg", 1);
    if (src.empty()) {
        printf("could not load image...\n");
        return -1;
    }
    mask.create(src.size(), CV_8UC1);
    mask.setTo(Scalar::all(GC_BGD));//转化图片

    namedWindow(winTitle, WINDOW_AUTOSIZE);
    setMouseCallback(winTitle, onMouse, 0);//鼠标回调函数
    imshow(winTitle, src);

    while (true) {
        char c = (char)waitKey(0);
        if (c == 'n') {
            runGrabCut();
            numRun++;
            showImage();
            printf("current iteative times : %d\n", numRun);
        }
        if ((int)c == 27) {
            break;
        }
    }

    waitKey(0);
    return 0;
}

//显示矩形
void showImage() {
    Mat result, binMask;
    binMask.create(mask.size(), CV_8UC1);
    binMask = mask & 1;
    if (init) {
        src.copyTo(result, binMask);
    } else {
        src.copyTo(result);
    }
    rectangle(result, rect, Scalar(0, 0, 255), 2, 8);//绘制矩形
    imshow(winTitle, result);
}

//创建掩膜
void setROIMask() {
    // GC_FGD = 1//前景
    // GC_BGD =0;//背景
    // GC_PR_FGD = 3//可能前景
    // GC_PR_BGD = 2//可能背景
    mask.setTo(GC_BGD);
    rect.x = max(0, rect.x);
    rect.y = max(0, rect.y);
    //防止越界
    rect.width = min(rect.width, src.cols - rect.x);
    rect.height = min(rect.height, src.rows - rect.y);
    mask(rect).setTo(Scalar(GC_PR_FGD));//从mask中截取rect范围的图像
}

/*鼠标回调函数*/
void onMouse(int event, int x, int y, int flags, void* param) {
    switch (event) {
        case EVENT_LBUTTONDOWN://左键按下
            rect.x = x;
            rect.y = y;
            rect.width = 1;
            rect.height = 1;
            init = false;
            numRun = 0;
            break;
        case EVENT_MOUSEMOVE://鼠标移动
            if (flags & EVENT_FLAG_LBUTTON) {
                rect = Rect(Point(rect.x, rect.y), Point(x, y));//生成矩形
                showImage();//显示矩形
            }
            break;
        case EVENT_LBUTTONUP://左键松开
            if (rect.width > 1 && rect.height > 1) {
                setROIMask();
                showImage();//显示矩形
            }
            break;
        default:
            break;
    }
}

/*进行grabcut处理*/
void runGrabCut() {
    if (rect.width < 2 || rect.height < 2) {
        return;
    }

    if (init) {
        grabCut(src, mask, rect, bgModel, fgModel, 1);
    }
    {
        grabCut(src, mask, rect, bgModel, fgModel, 1, GC_INIT_WITH_RECT);
        init = true;
    }
}