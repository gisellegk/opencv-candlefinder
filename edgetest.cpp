#include <iostream>
#include "opencv2/opencv.hpp"

#define CAMERAINTERFACE 1

using namespace cv;

int main(int, char**)
{
    system("./setExposure.sh 1"); // sets exposure of video1 to 1
    VideoCapture cap(CAMERAINTERFACE); // open camera
    if(!cap.isOpened()){  // check if we succeeded
    std::cout << "it didn't open :(" << std::endl;
    return -1;
    }

    Mat edges;
    namedWindow("edges",1);
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, edges, COLOR_BGR2GRAY);
        GaussianBlur(edges, edges, Size(3,3), 1.5, 1.5);
        Canny(edges, edges, 37, 37*3, 3);
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
