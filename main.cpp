// CarTrackingAndCounting.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

Mat frame;
Mat frame_old;
Mat frame_diff;


void on_trackbar(int, void*) {}

int main(int argc, char *argv[])
{
	// variable for tresholding
	int seuil = 10;

	// variables for the morphological operations
	int morph_elem = 0;
	int morph_size = 0;
	int morph_operator = 1;
	int const max_operator = 4;
	int const max_elem = 2;
	int const max_kernel_size = 21;
	namedWindow("Morphological operations");

	// creation of a trackbar inside a panel to slow down the video
	int slow = 10;
	namedWindow("Slow motion");
	createTrackbar("Slowing", "Slow motion", &slow, 1000, on_trackbar);

	VideoCapture stream1(argv[1]);

	if (!stream1.isOpened())
	{
		cout << "no video ! :o" << endl;
		return -1;
	}

	while (char(waitKey(1)) != 'f')
	{
		stream1.read(frame);

		// resize video
		Size size(800, 500);
		resize(frame, frame, size);

		if (!frame_old.empty())
		{
			// it is not the first frame of the video
			frame_diff = frame - frame_old;

			// trackbar to choose treshold
			createTrackbar("threshold", "diff", &seuil, 255, on_trackbar);
			
			// conversion to gray 
			cvtColor(frame_diff, frame_diff, CV_BGR2GRAY);

			// call to method that do a treshold 
			threshold(frame_diff, frame_diff, seuil, 255, 0);


			/// Create Trackbar to select Morphology operation
			createTrackbar("Operator:\n 0: Opening - 1: Closing \n 2: Gradient - 3: Top Hat \n 4: Black Hat", "Morphological operations", 
				&morph_operator, max_operator, on_trackbar);

			/// Create Trackbar to select kernel type
			createTrackbar("Element:\n 0: Rect - 1: Cross - 2: Ellipse", "Morphological operations",
				&morph_elem, max_elem,
				on_trackbar);

			/// Create Trackbar to choose kernel size
			createTrackbar("Kernel size:\n 2n +1", "Morphological operations",
				&morph_size, max_kernel_size,
				on_trackbar);

			int operation = morph_operator + 2;

			Mat element = getStructuringElement(morph_elem, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));

			/// Apply the specified morphology operation
			morphologyEx(frame_diff, frame_diff, operation, element);

			resize(frame_diff, frame_diff, size);
			imshow("diff", frame_diff);
		}

		frame_old = frame;
		imshow("video", frame);

		int c;
		if (slow == 0) c = waitKey(1);
		else
			c = waitKey(slow);
		if ((char)c == 'c') { break; }
	}

    return 0;
}

