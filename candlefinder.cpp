#include <iostream>
#include "opencv2/opencv.hpp"

#define CAMERAINTERFACE 1

using namespace cv;

int main(int argc, char* argv[])
{
  int cameraNum = CAMERAINTERFACE;
  if(argc > 1) {
    char * p;
    cameraNum = strtol(argv[1], &p, 10);
  }

  std::cout << cameraNum << std::endl;

  /* Get camera feed */
  system(("./setExposure.sh "+ std::to_string(cameraNum)).c_str()); // sets exposure of video1 to 1
  VideoCapture cap(cameraNum); // open camera

  if(!cap.isOpened()) {  // check if we succeeded
    std::cout << "it didn't open :(" << std::endl;
    return -1;
  }

  namedWindow("robovision",1); // computer output window
  namedWindow("robovision2",1);

  RNG rng(12345);
  for(;;) //repeat forever
  {
    Mat frame;
    cap >> frame; // get a new frame from camera

    /* Process frame image */
    cvtColor(frame, frame, COLOR_BGR2GRAY);
    GaussianBlur(frame, frame, Size(3,3), 1.5, 1.5); // remove noise

    //threshold hopefully
    // Set threshold and maxValue
    double thresh = 127;
    double maxValue = 255;

    // Binary Threshold
    threshold(frame,frame, thresh, maxValue, 1);
    imshow("robovision2", frame);
    bitwise_not(frame, frame); // invert colors - possibly unnecessary idk

    /* Search for flame and draw a rectangle around it */
    std::vector<std::vector<Point> > contours;
    std::vector<Vec4i> hierarchy;
    findContours( frame, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    /// Approximate contours to polygons + get bounding rects and circles
    std::vector<std::vector<Point> > contours_poly( contours.size() );
    std::vector<Rect> boundRect( contours.size() );
    //std::vector<Point2f>center( contours.size() );
    //std::vector<float>radius( contours.size() );

    for( int i = 0; i < contours.size(); i++ )
    { approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
      boundRect[i] = boundingRect( Mat(contours_poly[i]) );
      //minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
    }
    /// Draw polygonal contour + bonding rects + circles
    Mat drawing = Mat::zeros( frame.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
      Rect r = boundRect[i];
      std::cout << "(" << (r.x+(r.width/2.0)) << ", " << (r.y+(r.height/2.0)) << ")" << std::endl;
      Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
      drawContours( drawing, contours_poly, i, color, 1, 8, std::vector<Vec4i>(), 0, Point() );
      rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
      //circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
    }

    imshow("robovision", drawing ); // show frame on computer output window

    /* Escape = kill program */
    int keypress = waitKey(10)%255;
    if(keypress == 27){
      std::cout << "key pressed: " << keypress << std::endl;
      std::cout << "see you later! :)" << std::endl;
      break;
    }
  }
  // the camera will be deinitialized automatically in VideoCapture destructor
  return 0;
}
