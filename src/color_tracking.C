#include "color_tracking.h"

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

void processYawPitchAngles(int vectY, int vectX, int& yawAngle, int& pitchAngle){
	int valToaddX = 0, valToaddY = 0;

	valToaddX = abs(vectX / SEUIL_PIXEL_VIT);
	valToaddY = abs(vectY / SEUIL_PIXEL_VIT);

	//std::cout << "vx: " << valToaddX << " vy: " << valToaddY << std::endl;

	if(vectY<-SEUIL_CENTER && vectX<-SEUIL_CENTER){
        yawAngle-=valToaddX;pitchAngle+=valToaddY;
  	} else if(vectY>SEUIL_CENTER && vectX>SEUIL_CENTER){
    	yawAngle+=valToaddX;pitchAngle-=valToaddY;
  	} else if(-SEUIL_CENTER<vectY && vectY<SEUIL_CENTER && vectX<-SEUIL_CENTER){
    	yawAngle-=valToaddX;
  	} else if(-SEUIL_CENTER<vectY && vectY<SEUIL_CENTER && vectX>SEUIL_CENTER){
    	yawAngle+=valToaddX;
  	} else if(-SEUIL_CENTER<vectX && vectX<SEUIL_CENTER && vectY<-SEUIL_CENTER){
    	pitchAngle+=valToaddY;
  	} else if(-SEUIL_CENTER<vectX && vectX<SEUIL_CENTER && vectY>SEUIL_CENTER){
    	pitchAngle-=valToaddY;
  	} else if(vectY>SEUIL_CENTER && vectX<-SEUIL_CENTER){
    	yawAngle-=valToaddX;pitchAngle-=valToaddY;
  	} else if(vectY<-SEUIL_CENTER && vectX>SEUIL_CENTER){
    	yawAngle+=valToaddX;pitchAngle+=valToaddY;
  	}

  	if(yawAngle>180)
  		yawAngle = 180;
  	else if(yawAngle<0)
  		yawAngle = 0;

  	if(pitchAngle>180)
  		pitchAngle = 180;
  	else if(pitchAngle<0)
  		pitchAngle = 0;
}

void colour_red_detection(Mat& frame,int& minX, int& minY, int& maxX, int& maxY, int& xMilieu, int& yMilieu){
	double sumX = 0, sumY = 0, nbpixel = 0;
	xMilieu=0; yMilieu=0;

	for (int i=0;i<frame.rows;i++){
		for (int j=0;j<frame.cols;j++){
			int b = frame.at<Vec3b>(i,j)[0];
			int g = frame.at<Vec3b>(i,j)[1];
			int r = frame.at<Vec3b>(i,j)[2];
			
	  		if ( (r > SEUIL_MAX) && ((g+b) < SEUIL_OTHERS)){
			   frame.at<Vec3b>(i,j)[0] = 0;
			   frame.at<Vec3b>(i,j)[1] = 0;	
			   frame.at<Vec3b>(i,j)[2] = 255;
				
			   /*if (j<minX) minX=j;
			   if (i<minY) minY=i;
			   if (j>maxX) maxX=j;
			   if(i>maxY) maxY=i;*/
			   sumX+=j;
			   sumY+=i;
			   nbpixel++;

		
			} else {
		   	   frame.at<Vec3b>(i,j)[0] = 0;
			   frame.at<Vec3b>(i,j)[1] = 0;	
			   frame.at<Vec3b>(i,j)[2] = 0;
			}
		}   
	}

	if(nbpixel != 0){
		xMilieu=round(sumX/nbpixel);
		yMilieu=round(sumY/nbpixel);
	}
	//std::cout << "xM: " << xMilieu << " yM: " << yMilieu << " nb: " << nbpixel << std::endl; 
}

int main(int argc, char** argv){

  int maxX =0,maxY=0,minX=0,minY=0;
  int xMilieu, yMilieu;
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
   serial_open(&sp, argv[1]);
   //sleep(1);
    
   cap.set(CV_CAP_PROP_FRAME_WIDTH,WIDTH);  //taille de la fenetre
   cap.set(CV_CAP_PROP_FRAME_HEIGHT,HEIGHT); //au dela de 320*240, image qui lag

   //MatIterator_<Vec3b> it, end;
  gettimeofday(&start, 0);

  while(1){
    if(cap.read(frame)){// get a new frame from camera
      frame.copyTo(frameOrig);
	
	  colour_red_detection(frame,minX,minY,maxX,maxY,xMilieu,yMilieu);
      //if(minX != 20000 | minY != 20000 | maxX != 0 | maxY !=0){
	  if(xMilieu != 0 && yMilieu != 0){
        /*xMilieu=(minX+maxX)/2;
        yMilieu=(minY+maxY)/2;
        frame.at<Vec3b>(xMilieu,yMilieu)[0] = 255;*/
        
        circle(frame, Point(xMilieu, yMilieu), 8, Scalar(0, 255, 0), 2);
        //rectangle(frame, Point(minX, minY), Point(maxX, maxY), Scalar(0, 255, 0)); 

        vectX = xMilieu-frameMX;
        vectY = yMilieu-frameMY;

        taskPeriodValid = taskPeriodCheck();

        if(taskPeriodValid){
          
        	processYawPitchAngles(vectY, vectX, yawAngle, pitchAngle);
        	servAngle(&sp, yawAngle, pitchAngle);
        }
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