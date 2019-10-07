#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;


Mat src; Mat src_gray; Mat src_binary; Mat im_keypoints;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

/// Function header
void thresh_callback(int, void*);

/** @function main */
int main(int argc, char** argv)
{
	/// Load source image and convert it to gray
	src = imread(argv[1], 1);

	/// Convert image to gray and blur it
	cvtColor(src, src_gray, COLOR_BayerGB2GRAY);
	blur(src_gray, src_gray, Size(3, 3));
	threshold(src_gray, src_binary, 100, 255, THRESH_BINARY);

	//ROI, make sure to make this changeable
	//Rect roi(290, 50, 175, 500); //Roi for entire bottle
	Rect roi(290, 60, 200, 80); //ROI for cap
	Mat imCrop = src_gray(roi);

	//Detect cappy
	threshold(imCrop, src_binary, 100, 255, THRESH_BINARY);
	// Setup SimpleBlobDetector parameters.
	SimpleBlobDetector::Params params;

	// Change thresholds
	params.minThreshold = 10;
	params.maxThreshold = 2000;

	//Filter by Color.
	params.filterByColor = true;
	params.blobColor = 0;

	// Filter by Area.
	//params.filterByArea = true;
	//params.minArea = 150;

	// Filter by Circularity
	//params.filterByCircularity = true;
	//params.minCircularity = 0.1;

	// Filter by Convexity
	//params.filterByConvexity = true;
	//params.minConvexity = 0.87;

	// Filter by Inertia
	//params.filterByInertia = true;
	//params.minInertiaRatio = 0.01;


	// Storage for blobs
	vector<KeyPoint> keypoints;

	// Set up detector with params
	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

	// Detect blobs
	detector->detect(src_binary, keypoints);

	// Draw detected blobs as red circles.
	// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
	// the size of the circle corresponds to the size of blob

	Mat im_with_keypoints;
	drawKeypoints(src_binary, keypoints, im_with_keypoints, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	cout << "Number of Blobs " << keypoints.size() << endl;

	// Show blobs
	imshow("keypoints", im_with_keypoints);
	waitKey(0);
	
	/// Create Window
	char* source_window = "Source";
	namedWindow(source_window, WINDOW_AUTOSIZE);
	imshow(source_window, src);
	imshow("Binary", src_binary);
	imshow("ROI", imCrop);


	createTrackbar(" Canny thresh:", "Source", &thresh, max_thresh, thresh_callback);

	thresh_callback(0, 0);

	waitKey(0);
	return(0);
}

/** @function thresh_callback */
void thresh_callback(int, void*)
{
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Detect edges using canny
	Canny(src_gray, canny_output, thresh, thresh * 2, 3);
	/// Find contours
	findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Draw contours
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
	}

	/// Show in a window
	namedWindow("Contours", WINDOW_AUTOSIZE);
	imshow("Contours", drawing);
}