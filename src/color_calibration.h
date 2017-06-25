#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <sys/time.h>

#define WIDTH 320
#define HEIGHT 240

#define Y_D 90
#define P_D 60

#define TASK_DEADLINE 100000 //100ms
#define TASK_PERIOD 15000 //15ms
#define SEUIL_CENTER 15
#define S_RMIN 75
#define S_RMAX 205
#define S_GMIN 10
#define S_GMAX 50
#define S_BMIN 16
#define S_BMAX 126

#define SEUIL_PIXEL_VIT 50

using namespace cv;

void colour_red_detection(Mat& frame,int minX, int minY, int maxX, int maxY, int& xMilieu, int& yMilieu);