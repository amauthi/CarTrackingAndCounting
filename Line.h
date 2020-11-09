#pragma once

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;

class Line
{
public:
	Line(){}
	Line(bool pointA_done, bool pointB_done)
	{
		this->pointA_done = pointA_done;
		this->pointB_done = pointB_done;
	}
	cv::Point pointA;
	cv::Point pointB;
	bool pointA_done;
	bool pointB_done;

	// coeffs for the equation of the line : y = a*x+ b
	double a;
	double b;
};

