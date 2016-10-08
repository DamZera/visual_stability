#include "tracking.h"

int main(void){

   Mat frame,frameOrig, sobel;
   namedWindow("MyCam",1);
   std::vector<Point> vect;
   VideoCapture cap(0); // open the default camera
  if(!cap.isOpened())  // check if we succeeded
       return -1;

    
   cap.set(CV_CAP_PROP_FRAME_WIDTH,WIDTH);  //taille de la fenetre
   cap.set(CV_CAP_PROP_FRAME_HEIGHT,HEIGHT); //au dela de 320*240, image qui lag
  while(1){
   //frame = imread("../imageTest.jpg", CV_LOAD_IMAGE_COLOR);

   if(cap.read(frame)){// get a new frame from camera
	frame.copyTo(frameOrig);
	cvtColor(frame, frameOrig, CV_BGR2GRAY);

	sobel = maskFiltrageSobel(frameOrig);
	
	/*vect = Harris(frame);

	for(std::vector<Point>::iterator i = vect.begin(); i != vect.end();++i){
		Point &p = *i;
		std::cout << p << std::endl;
		circle(frame, p, 1, Scalar(0, 255, 0), 1);
	}*/

	namedWindow( "Display window", WINDOW_AUTOSIZE );
	imshow("Display window", sobel);
	//imshow("Original",frameOrig);
	    
    }
    if(waitKey(5) >= 0) break;
  }
	waitKey(0);
	return 0;
}