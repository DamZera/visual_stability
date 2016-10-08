#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;

int main(int, char**){

  int maxX =0,maxY=0,minX=0,minY=0;
  int xMilieu,yMilieu;

  VideoCapture cap(0); // open the default camera
  if(!cap.isOpened())  // check if we succeeded
       return -1;

   Mat frame,frameOrig;
   namedWindow("MyCam",1);
    
   cap.set(CV_CAP_PROP_FRAME_WIDTH,320);  //taille de la fenetre
   cap.set(CV_CAP_PROP_FRAME_HEIGHT,240); //au dela de 320*240, image qui lag

   //MatIterator_<Vec3b> it, end;

   while(1){
       if(cap.read(frame)){// get a new frame from camera
	frame.copyTo(frameOrig);
	for (int i=0;i<frame.rows;i++){
		for (int j=0;j<frame.cols;j++){
		  int b = frame.at<Vec3b>(i,j)[0];
		  int g = frame.at<Vec3b>(i,j)[1];
		  int r = frame.at<Vec3b>(i,j)[2];
		
		  if ( (r > 150) && ((g+b) < 150)){
			frame.at<Vec3b>(i,j)[0] = 0;
			frame.at<Vec3b>(i,j)[1] = 0;	
			frame.at<Vec3b>(i,j)[2] = 255;
			
			if (i<minX) minX=i;
			if (j<minY) minY=j;
			if (i>maxX) maxX=i;
			if(j>maxY) maxY=j;			
	
		  } else {
			frame.at<Vec3b>(i,j)[0] = 0;
			frame.at<Vec3b>(i,j)[1] = 0;	
			frame.at<Vec3b>(i,j)[2] = 0;	

		  }
		}
	
	   
	 }
		xMilieu=(minX+maxX)/2;
		yMilieu=(minY+maxY)/2;
		frame.at<Vec3b>(xMilieu,yMilieu)[0] = 255;

		//std::cout << xMilieu;
		//std::cout << yMilieu;
		maxX =0;maxY=0;minX=20000;minY=20000;

	     imshow("MyCam", frame);
	     imshow("Original",frameOrig);
	    
        }
        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
