/**
 * 形态学操作
*/
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
  char title1[] = "input";
  Mat src,dst;
  src = imread("/home/jhr/catkin_ws/image/LinuxLogo.jpg");
  namedWindow(title1,WINDOW_NORMAL);
  resizeWindow(title1,640,480);
  imshow(title1,src);

  Mat kernel = getStructuringElement(MORPH_RECT,Size(3,3),Point(-1,-1));
  morphologyEx(src,dst,MORPH_BLACKHAT,kernel);
  char title2[]="output";
  namedWindow(title2,WINDOW_NORMAL);
  resizeWindow(title2,640,480);
  imshow(title2,dst);

  waitKey(0);
}