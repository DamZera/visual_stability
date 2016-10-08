#include "tracking.h"

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

Mat maskFiltrageSobel(Mat& grey){

	long sumY, sumX;
	long gradX, gradY, tmp;
	Mat greyMat(grey.rows, grey.cols, CV_8UC1);

	//cvtColor(frame, grey, CV_BGR2GRAY);

	for (int i=1;i<grey.rows-1;i++){
	   	for (int j=1;j<grey.cols-1;j++){

	   		gradX = (-1 * grey.at<uchar>(i-1,j-1)) + grey.at<uchar>(i-1,j+1)
	   			+ (-2 * grey.at<uchar>(i,j-1)) + (2 * grey.at<uchar>(i,j+1))
	   			+ (-1 * grey.at<uchar>(i+1,j-1)) + grey.at<uchar>(i+1,j+1);

	   		gradY = (-1 * grey.at<uchar>(i-1,j-1)) + grey.at<uchar>(i+1,j-1)
	   			+ (-2 * grey.at<uchar>(i-1,j)) + (2 * grey.at<uchar>(i+1,j))
	   			+ (-1 * grey.at<uchar>(i-1,j+1)) + grey.at<uchar>(i+1,j+1);
	   		
	   		tmp=sqrt((gradX*gradX) + (gradY*gradY));
	   		greyMat.at<uchar>(i, j)=(fabs(tmp)/1020)*255;
	   
	   	}
	}
	return greyMat;
}

double distance(double xA, double  yA, double xB, double yB){
	return sqrt(pow(xB-xA,2)+pow(yB-yA,2));
}

Mat Harris (Mat& grey, Mat& frame, std::vector<Point>& vectBefore, std::vector<Point>& vectAfter){
	double sumY, sumX, dX, dY;
	double sumIX,sumIY,sumIXIY;

	Mat IX(grey.rows, grey.cols, CV_8UC1);
	Mat IY(grey.rows, grey.cols, CV_8UC1);
	Mat IXIY(grey.rows, grey.cols, CV_8UC1);
	Mat test(grey.rows, grey.cols, CV_8UC1);

	std::vector<Point3d> vect_harris;

	int accu[100][360]={0};


	for (int i=0;i<grey.rows-5;i++){
	   	for (int j=0;j<grey.cols-5;j++){
	   		double a = 0;
			double b = 0;
			double c = 0;

			// Calcul de dérivées avec le grandient Sobel 

			sumX = (-1 * grey.at<uchar>(i-1,j-1)) + grey.at<uchar>(i-1,j+1)
	   			+ (-2 * grey.at<uchar>(i,j-1)) + (2 * grey.at<uchar>(i,j+1))
	   			+ (-1 * grey.at<uchar>(i+1,j-1)) + grey.at<uchar>(i+1,j+1);
	   		
	   		dX=(std::abs(sumX)/1020)*255;

	   		sumY = (-1 * grey.at<uchar>(i-1,j-1)) + grey.at<uchar>(i+1,j-1)
	   			+ (-2 * grey.at<uchar>(i-1,j)) + (2 * grey.at<uchar>(i+1,j))
	   			+ (-1 * grey.at<uchar>(i-1,j+1)) + grey.at<uchar>(i+1,j+1);
	   		
	   		dY=(std::abs(sumY)/1020)*255;

	   		IX.at<uchar>(i,j) = (uchar)(dX*dX);
			IY.at<uchar>(i,j)  = (uchar)(dY*dY);
			IXIY.at<uchar>(i,j)  = (uchar)(dX*dY);

			// Convolution avec gaussienne
			for (int x = 0; x < 5; x++){
				for (int y = 0; y < 5; y++){
					a += IX.at<uchar>(i + y, j + x) * gauss[x][y];
					b += IY.at<uchar>(i + y, j + x) * gauss[x][y];
					c += IXIY.at<uchar>(i + y, j + x) * gauss[x][y];
				}
			}

			//lambda entre 0.04 et 0.06
			double responce = (a * b) - c - (ALPHA * (a + b) * (a + b));
			
			//entre 104 et 107
			// 1000 tres bon seuil
			if(responce > SEUIL){
                harris[i][j] = responce;
                //vect_harris.push_back(Point3d(i,j,responce));
                //circle(frame, Point(j,i), 1, Scalar(0, 255, 0), 1);
            }else{
                harris[i][j] = 0;
            }
        }
	}

	/*std::sort(vect_harris.begin(), vect_harris.end(), [](Point3d const & a, Point3d const & b) { return a.z > b.z; });
	int count = 0;
	for(std::vector<Point3d>::iterator i = vect_harris.begin(); i != vect_harris.end();++i){
	   	if(count<30)circle(frame, Point((*i).x,(*i).y), 1, Scalar(0, 255, 0), 1);
	   	else break;
	   	count++;
	}*/
	
	// Calculer la force de coin pour chaque pixel  
	for (int i=0;i<grey.rows-14;i+=15){
	   	for (int j=0;j<grey.cols-14;j+=15){

	   		double tmp;
	   		double max=0;
	   		int xM = 0, yM = 0;

	   		for (int x=0;x<15;x++){
	   			for (int y=0;y<15;y++){
	   				tmp = harris[y+i][x+j];
	   				if(tmp>max){
	   					max=tmp;
	   					yM = y+i; xM = x+j;
	   				}
	   			}
	   		}
	   		
	   		if(harris[i][j]>0){
	   			//vectAfter.push_back(Point(xM,yM));
	   			circle(frame, Point(xM,yM), 1, Scalar(0, 255, 0), 1);

	   			/*if(!vectBefore.empty()){
	   				double tmp, min=1000000000000000;
	   				int minX, minY;
	   				for(std::vector<Point>::iterator i = vectBefore.begin(); i != vectBefore.end();++i){
	   					
	   					tmp = distance(xM,yM,(*i).x, (*i).y);
	   					if(tmp<min){
	   						min = tmp;
	   						minX = (*i).x;
	   						minY = (*i).y;
	   					}
	   					//std::cout << (*i).x << " : " << (*i).y << std::endl;
	   				}
	   				int ang;
	   				if (min==0) ang = 0;
	   				else ang = acos ((minX-xM)/min) * 180.0 / PI;
	   				//std::cout <<min <<":"<< minX <<":"<< xM << ":" << ang << std::endl;

	   				if (min<100) accu[(int)min][ang]++;
	   				//line(frame, Point(xM,yM), Point(minX, minY),  Scalar(255, 0, 0));
	   			}*/
	   		}
	   	}
	}

	/*int best = 0;
	int bestDist, bestAng;
	for(int i=0; i<100; i++){
		for(int j=0; j<360; j++){
			int tmp = accu[i][j];
			if(tmp>best){
				best = tmp;
				bestDist = i;
				bestAng = j;
			}
		}
	}*/

	//line(frame, Point(0, 0), Point(bestDist*cos(bestAng), bestDist*sin(bestAng)),  Scalar(255, 0, 0));
	//vectBefore.swap(vectAfter);
	//vectAfter.clear();
	
	
	return grey;

}

int main(void){

   Mat frame,frameOrig, sobel;
   namedWindow("MyCam",1);
   std::vector<Point> vectBefore;
   std::vector<Point> vectAfter;
   VideoCapture cap(0); // open the default camera
   int taskPeriodValid;

  if(!cap.isOpened())  // check if we succeeded
       return -1;

    
   cap.set(CV_CAP_PROP_FRAME_WIDTH,WIDTH);  //taille de la fenetre
   cap.set(CV_CAP_PROP_FRAME_HEIGHT,HEIGHT); //au dela de 320*240, image qui lag

   gettimeofday(&start, 0);

   while(1){

   if(cap.read(frame)){// get a new frame from camera
	frame.copyTo(frameOrig);
	cvtColor(frame, frameOrig, CV_BGR2GRAY);

	taskPeriodValid = taskPeriodCheck();

    if(taskPeriodValid){
		frameOrig = Harris(frameOrig, frame, vectBefore, vectAfter);
		imshow("Original",frameOrig);
	}

	/*for(std::vector<Point>::iterator i = vect.begin(); i != vect.end();++i){
		Point &p = *i;
		circle(frame, p, 1, Scalar(0, 255, 0), 1);
	}*/

	namedWindow( "Display window", WINDOW_AUTOSIZE );
	imshow("Display window", frame);
	//imshow("Original",frameOrig);
	    
    }
    if(waitKey(0) >= 0) break;
    //waitKey(0);
  }
	return 0;
}