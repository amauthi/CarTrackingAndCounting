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

void on_trackbar(int, void*) {}

int main(int argc, char *argv[])
{
	// creation of a trackbar inside a panel to slow down the video
	int slow = 0;
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

		imshow("video", frame);

		int c;
		if (slow == 0) c = waitKey(1);
		else
			c = waitKey(slow);
		if ((char)c == 'c') { break; }
	}

    return 0;
}

