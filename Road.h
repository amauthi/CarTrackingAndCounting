#pragma once

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "Line.h"
using namespace cv;

class Road
{
public:
	Road(bool road_shaped, Line line1, Line line2)
	{
		this->road_shaped = road_shaped;
		this->line1 = line1;
		this->line2 = line2;
	}
	Line line1; //= Line(false, false);
	Line line2; //= Line(false, false);
	bool road_shaped;
};
