#include "color_calibration.h"

void colour_red_detection(Mat& frame,int minX, int minY, int maxX, int maxY, int& xMilieu, int& yMilieu){
	double sumR = 0, sumG = 0, sumB = 0, nbpixel = 0;
  int minR = 255, minG = 255, minB = 255;
  int maxR = 0, maxG = 0, maxB = 0;
	xMilieu=0; yMilieu=0;

	for (int i=minY;i<maxY;i++){
		for (int j=minX;j<maxX;j++){
			int b = frame.at<Vec3b>(i,j)[0];
			int g = frame.at<Vec3b>(i,j)[1];
			int r = frame.at<Vec3b>(i,j)[2];

      sumR += r;
      sumG += g;
      sumB += b;

      nbpixel++;

      if(r > maxR){
        maxR = r;
      } else if(r < minR){
        minR = r;
      }
      if(g > maxG){
        maxG = g;
      } else if(g < minG){
        minG = g;
      }
      if(b > maxB){
        maxB = b;
      } else if(r < minB){
        minB = b;
      }
			
    }  
	}

  for (int i=0;i<frame.rows;i++){
    for (int j=0;j<frame.cols;j++){
      int b = frame.at<Vec3b>(i,j)[0];
      int g = frame.at<Vec3b>(i,j)[1];
      int r = frame.at<Vec3b>(i,j)[2];

      if ( (r > S_RMIN) && (r < S_RMAX) && (g > S_GMIN) && (g < S_GMAX) && (b > S_BMIN) && (b < S_BMAX)){
         frame.at<Vec3b>(i,j)[0] = 0;
         frame.at<Vec3b>(i,j)[1] = 0; 
         frame.at<Vec3b>(i,j)[2] = 255;
        

    
      } else {
           frame.at<Vec3b>(i,j)[0] = 0;
         frame.at<Vec3b>(i,j)[1] = 0; 
         frame.at<Vec3b>(i,j)[2] = 0;
      }
    }
  }

	std::cout << "R.avg: " << sumR/nbpixel << " G.avg: " << sumG/nbpixel << " B.avg: " << sumB/nbpixel << std::endl;
  std::cout << "R.min: " << minR << " R.max: " << maxR <<  "G.min: " << minG << " G.max: " << maxG <<  "B.min: " << minB << " B.max: " << maxB << std::endl; 
}

int main(int argc, char** argv){

  int maxX =0,maxY=0,minX=0,minY=0;
  int xMilieu, yMilieu;
  int frameMX, frameMY;
  int vectX=0, vectY=0;
  int yawAngle=Y_D, pitchAngle=P_D;
  int taskPeriodValid;
  frameMX = WIDTH / 2;
  frameMY = HEIGHT / 2;

  VideoCapture cap(0); // open the default camera
  if(!cap.isOpened())  // check if we succeeded
       return -1;

   Mat frame,frameOrig;
   namedWindow("MyCam",1);
    
   cap.set(CV_CAP_PROP_FRAME_WIDTH,WIDTH);  //taille de la fenetre
   cap.set(CV_CAP_PROP_FRAME_HEIGHT,HEIGHT); //au dela de 320*240, image qui lag

  while(1){
    if(cap.read(frame)){// get a new frame from camera
      frame.copyTo(frameOrig);
	
	  colour_red_detection(frame,frameMX-20,frameMY-20,frameMX+20,frameMY+20,xMilieu,yMilieu);
    rectangle(frame, Point(frameMX-20, frameMY-20), Point(frameMX+20, frameMY+20), Scalar(0, 255, 0));
    rectangle(frameOrig, Point(frameMX-20, frameMY-20), Point(frameMX+20, frameMY+20), Scalar(0, 255, 0)); 


    imshow("MyCam", frame);
    imshow("Original",frameOrig);
	    
    }
    if(waitKey(5) >= 0) break;
  }
  // the camera will be deinitialized automatically in VideoCapture destructor
  return 0;
}