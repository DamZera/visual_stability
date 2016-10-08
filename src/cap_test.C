#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <sys/time.h>

#include "colour_detection.h"
#include "serial.h"

#define WIDTH 320
#define HEIGHT 240

#define Y_D 90
#define P_D 60

using namespace cv;

#define TASK_DEADLINE 100000 //100ms
#define TASK_PERIOD 15000 //10ms
#define SEUIL_CENTER 15 

struct timeval start, checkpoint;

int taskPeriodCheck(){
  int taskPeriodValid;
  long diff;
  
  gettimeofday(&checkpoint, 0);
  diff=(checkpoint.tv_sec-start.tv_sec) * 1000000L + (checkpoint.tv_usec-start.tv_usec); 
  /* calcul de la difference */
  if (diff < TASK_PERIOD ) taskPeriodValid = 0;
  else {
    gettimeofday(&start, 0); // reinitialisation du temps initial
    if (diff > TASK_PERIOD + TASK_DEADLINE){
      fprintf (stderr,"***echeance manquée %ld \n", diff);
      taskPeriodValid = 0;
    } else {
      taskPeriodValid = 1;
    }
  }
  return taskPeriodValid;
}

int main(int, char**){

  int maxX =0,maxY=0,minX=0,minY=0;
  int xMilieu,yMilieu;
  int frameMX, frameMY;
  int vectX=0, vectY=0;
  int yawAngle=Y_D, pitchAngle=P_D;
  int taskPeriodValid;
  serial_com sp;
  frameMX = WIDTH / 2;
  frameMY = HEIGHT / 2;

  VideoCapture cap(0); // open the default camera
  if(!cap.isOpened())  // check if we succeeded
       return -1;

   Mat frame,frameOrig;
   namedWindow("MyCam",1);
   serial_open(&sp, "/dev/cu.usbmodem1421");
   //sleep(1);
    
   cap.set(CV_CAP_PROP_FRAME_WIDTH,WIDTH);  //taille de la fenetre
   cap.set(CV_CAP_PROP_FRAME_HEIGHT,HEIGHT); //au dela de 320*240, image qui lag

   //MatIterator_<Vec3b> it, end;
  gettimeofday(&start, 0);

  while(1){
    if(cap.read(frame)){// get a new frame from camera
      frame.copyTo(frameOrig);
	
	    colour_red_detection(frame,minX,minY,maxX,maxY);
      if(minX != 20000 | minY != 20000 | maxX != 0 | maxY !=0){
        xMilieu=(minX+maxX)/2;
        yMilieu=(minY+maxY)/2;
        frame.at<Vec3b>(xMilieu,yMilieu)[0] = 255;
        
        circle(frame, Point(xMilieu, yMilieu), 8, Scalar(0, 255, 0), 2);
        //rectangle(frame, Point(minX, minY), Point(maxX, maxY), Scalar(0, 255, 0)); 

        vectX = xMilieu-frameMX;
        vectY = yMilieu-frameMY;

        taskPeriodValid = taskPeriodCheck();

        if(taskPeriodValid){
          if(vectY<-SEUIL_CENTER && vectX<-SEUIL_CENTER){
            yawAngle--;pitchAngle++;
          } else if(vectY>SEUIL_CENTER && vectX>SEUIL_CENTER){
            yawAngle++;pitchAngle--;
          } else if(-SEUIL_CENTER<vectY && vectY<SEUIL_CENTER && vectX<-SEUIL_CENTER){
            yawAngle--;
          } else if(-SEUIL_CENTER<vectY && vectY<SEUIL_CENTER && vectX>SEUIL_CENTER){
            yawAngle++;
          } else if(-SEUIL_CENTER<vectX && vectX<SEUIL_CENTER && vectY<-SEUIL_CENTER){
            pitchAngle++;
          } else if(-SEUIL_CENTER<vectX && vectX<SEUIL_CENTER && vectY>SEUIL_CENTER){
            pitchAngle--;
          } else if(vectY>SEUIL_CENTER && vectX<-SEUIL_CENTER){
            yawAngle--;pitchAngle--;
          } else if(vectY<-SEUIL_CENTER && vectX>SEUIL_CENTER){
            yawAngle++;pitchAngle++;
          }

          servAngle(&sp, yawAngle, pitchAngle);

        }

        std::cout << vectX << " : " << vectY << std::endl;
        std::cout << yawAngle << " : " << pitchAngle << std::endl;
      }
      
      maxX =0;maxY=0;minX=20000;minY=20000;

      imshow("MyCam", frame);
      imshow("Original",frameOrig);
	    
    }
    if(waitKey(5) >= 0) break;
  }
  serial_close(&sp);
  // the camera will be deinitialized automatically in VideoCapture destructor
  return 0;
}
