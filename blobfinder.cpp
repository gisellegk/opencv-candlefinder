#include <iostream>
#include "opencv2/opencv.hpp"
using namespace cv;
// Read image
int main(int, char**)
{
  SimpleBlobDetector::Params params;

// Change thresholds
params.minThreshold = 1;
params.maxThreshold = 500;

// Filter by Area.
params.filterByArea = true;
params.minArea = 10;

Mat im = imread( "blob.jpg", IMREAD_GRAYSCALE );

// Set up the detector with default parameters.
Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

// Detect blobs.
std::vector<KeyPoint> keypoints;
detector->detect( im, keypoints);

// Draw detected blobs as red circles.
// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
Mat im_with_keypoints;
drawKeypoints( im, keypoints, im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

// Show blobs
imshow("keypoints", im_with_keypoints );
waitKey(0);
}
