#include <iostream>
#include "opencv2/opencv.hpp"

#define CAMERAINTERFACE 1

using namespace cv;

int main(int, char**)
{
  /* Get camera feed */
  system(("./setExposure.sh "+ std::to_string(CAMERAINTERFACE)).c_str()); // sets exposure of video1 to 1
  VideoCapture cap(CAMERAINTERFACE); // open camera

  if(!cap.isOpened()) {  // check if we succeeded
    std::cout << "it didn't open :(" << std::endl;
    return -1;
  }

  namedWindow("robovision",1); // computer output window

  /* Simple Blob Detector parameters */
  SimpleBlobDetector::Params params;
  // Change thresholds
  params.minThreshold = 10;
  params.maxThreshold = 200;
  // Filter by Area.
  params.filterByArea = true;
  params.minArea = 50;

  Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

  for(;;) //repeat forever
  {
    Mat frame;
    cap >> frame; // get a new frame from camera

    /* Process frame image */
    cvtColor(frame, frame, COLOR_BGR2GRAY);
    GaussianBlur(frame, frame, Size(3,3), 1.5, 1.5); // remove noise
    bitwise_not(frame, frame); // invert colors - you are looking for the light but this searches for dark

    /* Search for flame and draw a red circle around it */
    std::vector<KeyPoint> keypoints;
    detector->detect( frame, keypoints);
    drawKeypoints( frame, keypoints, frame, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

    //std::cout << keypoints.size() << std::endl;

    imshow("robovision", frame ); // show frame on computer output window

    /* Escape = kill program */
    int keypress = waitKey(10)%255;
    if(keypress == 27){
      std::cout << "key pressed: " << keypress << std::endl;
      break;
    }
  }
  // the camera will be deinitialized automatically in VideoCapture destructor
  return 0;
}