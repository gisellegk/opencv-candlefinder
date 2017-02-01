#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <iostream>

using namespace std;

int main()
{
    CvCapture* capture = cvCaptureFromCAM( 1 ); //CV_CAP_ANY ); 
//CV_CAP_ANY
    if ( !capture )
    {
        fprintf( stderr, "ERROR: capture is NULL \n" );
        getchar();
        return -1;
    }

    cvNamedWindow( "mywindow", CV_WINDOW_AUTOSIZE );

    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 640);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 480);

    int width = (int)(cvGetCaptureProperty(capture, 
CV_CAP_PROP_FRAME_WIDTH));
    int height = (int)(cvGetCaptureProperty(capture, 
CV_CAP_PROP_FRAME_HEIGHT));
    IplImage* image = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 
3);

    IplImage* frame;
    char c;
    while ( true )
    {
        frame = cvQueryFrame( capture );
        if ( !frame ) 
        {
            fprintf( stderr, "ERROR: frame is null...\n" );
            getchar();
            break;
        }
        cvShowImage( "mywindow", frame );
        cvSetCaptureProperty(capture, CV_CAP_PROP_EXPOSURE, 2);
        c = cvWaitKey(33);
        if( (c & 255) == 27 ) break;
    }
    cvReleaseCapture( &capture );
    cvDestroyWindow( "mywindow" );
    return 0;
}
