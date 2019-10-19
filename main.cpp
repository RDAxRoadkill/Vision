 // Laden en tonen van een afbeelding
 // uitleg: http://docs.opencv.org/doc/tutorials/introduction/display_image/display_image.html
 // Jan Oostindie, dd 22-1-2015

//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/cv.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

/// Global variables
Mat src, src_gray, src_gray_bottle, src_gray_cap, imCrop, dst, src_binary;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);


char* window_name = "Threshold Demo";
/// Function header
bool check_bottle();
bool check_bottle_cap();


/** @function main */
int main(int argc, char** argv)
{
	/// Load source image and convert it to gray
	src = imread(argv[1], 1);
	imshow("Source", src);

	/// Convert the image to Gray and Crop
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
	blur(src_gray, src_gray_bottle, Size(3, 3));
	blur(src_gray, src_gray_cap, Size(3, 3));
	
	cout << "Bottlecap check return: " << check_bottle_cap() << endl;
	cout << "Bottlevalue check return: " << check_bottle() << endl;

	waitKey(0);
	return(0);
}

/** Check bottle inhoud */
bool check_bottle()
{
	bool check = false;
	// Array of intrest
	Rect roi(270, 100, 200, 480); //ROI for cap
	imCrop = src_gray_bottle(roi);
	threshold(imCrop, imCrop, 162, 255, 1);

	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	int largest_area = 0;
	int largest_contour_index = 0;
	Rect bounding_rect;

	/// Detect edges using canny
	Canny(imCrop, canny_output, 100, 100 * 2, 3);
	/// Find contours
	findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	
	
	if (contours.size() != 0) {
		// DRAW BIGGEST
		Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
		for (size_t i = 0; i < contours.size(); i++) // iterate through each contour.
		{
			double area = contourArea(contours[i]);  //  Find the area of contour

			if (area > largest_area)
			{
				largest_area = area;
				largest_contour_index = i;               //Store the index of largest contour
				bounding_rect = boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
			}
		}

		vector<vector<Point> > contours_poly(contours.size());
		vector<Rect> boundRect(contours.size());
		vector<Point2f>centers(contours.size());

		int i = largest_contour_index;

		approxPolyDP(contours[i], contours_poly[i], 3, true);
		boundRect[i] = boundingRect(contours_poly[i]);

		Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2);

		if (boundRect[i].height > 350 && boundRect[i].height < 395) {
			cout << "Fles inhoud correct" << endl;
			check = true;
		}
		else {
			cout << "Fles inhoud incorrect" << endl;
		}
		cout << "Heigh: " << boundRect[i].height << endl;

		drawContours(drawing, contours, largest_contour_index, Scalar(0, 255, 0), 2); // Draw the largest contour using previously stored index.

		/// Show in a window
		namedWindow("Contours", WINDOW_AUTOSIZE);
		imshow("Contours", drawing);
	}
	else {
		cout << "Fles is leeg." << endl;
	}

	return check;
	
}

bool check_bottle_cap()
{
	bool check = false;
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	int largest_area = 0;
	int largest_contour_index = 0;
	Rect bounding_rect;

	threshold(src_gray_cap, src_binary, 100, 255, THRESH_BINARY);

	//ROI, make sure to make this changeable
	//Rect roi(290, 50, 175, 500); //Roi for entire bottle
	Rect roi(290, 60, 200, 80); //ROI for cap
	Mat imCrop = src_gray_cap(roi);

	//Detect cappy
	threshold(imCrop, src_binary, 100, 255, THRESH_BINARY);

	/// Detect edges using canny
	Canny(src_binary, canny_output, 100, 100 * 2, 3);
	/// Find contours
	findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));


	if (contours.size() != 0) {
		// DRAW BIGGEST
		Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
		for (size_t i = 0; i < contours.size(); i++) // iterate through each contour.
		{
			double area = contourArea(contours[i]);  //  Find the area of contour

			if (area > largest_area)
			{
				largest_area = area;
				largest_contour_index = i;               //Store the index of largest contour
				bounding_rect = boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
			}
		}

		vector<vector<Point> > contours_poly(contours.size());
		vector<Rect> boundRect(contours.size());
		vector<Point2f>centers(contours.size());

		int i = largest_contour_index;

		approxPolyDP(contours[i], contours_poly[i], 3, true);
		boundRect[i] = boundingRect(contours_poly[i]);

		Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2);

		if (boundRect[i].width > 140 && boundRect[i].width < 175) {
			cout << "Dop inhoud correct" << endl;
			check = true;
		}
		else {
			cout << "Dop is incorrect" << endl;
		}
		cout << "Width: " << boundRect[i].width << endl;

		drawContours(drawing, contours, largest_contour_index, Scalar(0, 255, 0), 2); // Draw the largest contour using previously stored index.

		/// Show in a window
		namedWindow("Contours2", WINDOW_AUTOSIZE);
		imshow("Contours2", drawing);
	}
	else {
		cout << "Fles is leeg." << endl;
	}
	
	return check;
}
