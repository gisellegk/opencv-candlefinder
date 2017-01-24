#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;
int lowThreshold;

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened()){  // check if we succeeded
    std::cout << "we died" << std::endl;
    return -1;
    }

    Mat edges;
    namedWindow("edges",1);
    createTrackbar("min thresh", "edges", &lowThreshold, 100);
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, edges, COLOR_BGR2GRAY);
        GaussianBlur(edges, edges, Size(3,3), 1.5, 1.5);
        Canny(edges, edges, lowThreshold, lowThreshold*3, 3);
        imshow("edges", edges);
	int keypress = waitKey(10)%255;
        if(keypress == 27){
		std::cout << "key pressed: " << keypress << std::endl;
		break;
	}
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
