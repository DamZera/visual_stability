#include "colour_detection.h"

using namespace cv;

void colour_red_detection(Mat& frame,int& minX, int& minY, int& maxX, int& maxY){

	

	for (int i=0;i<frame.rows;i++){
	   	for (int j=0;j<frame.cols;j++){
			
	  		if ( (r > 150) && ((g+b) < 150)){
			   frame.at<Vec3b>(i,j)[0] = 0;
			   frame.at<Vec3b>(i,j)[1] = 0;	
			   frame.at<Vec3b>(i,j)[2] = 255;
				
			   if (j<minX) minX=j;
			   if (i<minY) minY=i;
			   if (j>maxX) maxX=j;
			   if(i>maxY) maxY=i;			
		
			} else {
		   	   frame.at<Vec3b>(i,j)[0] = 0;
			   frame.at<Vec3b>(i,j)[1] = 0;	
			   frame.at<Vec3b>(i,j)[2] = 0;
			}
		}   
	}	
}

