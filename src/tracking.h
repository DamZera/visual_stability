#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <stdio.h>
#include <vector>
#include <algorithm> 
#include <sys/time.h>

#define TASK_DEADLINE 100000 //100ms
#define TASK_PERIOD 15000 //15ms

#define ALPHA 0.1
#define SEUIL 2000

#define WIDTH 320
#define HEIGHT 240

#define PI 3.14159265

using namespace cv;

Mat maskFiltrageSobel(Mat& frame);
Mat Harris (Mat& grey);
bool compare (int i,int j);
int taskPeriodCheck();

struct timeval start, checkpoint;

double harris[HEIGHT][WIDTH];

double gauss[5][5] = {
	0.00390625, 0.015625, 0.0234375, 0.015625, 0.00390625,
	0.015625, 0.0625, 0.09375, 0.0625, 0.015625,
	0.0234375, 0.09375, 0.140625, 0.09375, 0.0234375,
	0.015625, 0.0625, 0.09375, 0.0625, 0.015625,
	0.00390625, 0.015625, 0.0234375, 0.015625, 0.00390625,
};