#include "stdafx.h"

#include "Functions.h"


// function that serve to track any car in frame_diff 
// returns the list of the coordinates of the cars because we have to save these coordinates to compare them at the next frame to the new ones (to see if a car has crossed the border)

std::vector<cv::Point> tracking_function(cv::Mat frame_diff, cv::Mat frame_convex, cv::Mat frame_final, std::vector<cv::Point> old_cars_coordinates, int compteur1, int compteur2, cv::Point pointA_line, cv::Point pointB_line)
{
	std::vector<cv::Point> cars_coordinates;
	
	// create a copy of frame_diff to ensure that the original is not modified by findContours
	cv::Mat frame_diff_copy = frame_diff.clone();

	//		1. find the contours of the image
	std::vector<std::vector<cv::Point> > contours;

	cv::findContours(frame_diff_copy, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	// show what we get (no needed because with convexHull, we will redraw on final image)
	//cv::drawContours(frame_final, contours, -1, cv::Scalar(0, 0, 255), -1);

	//		2. fill those contours with convexHull
	// we have to declare convexHulls after we use findContours, because we need the final size of findcontours
	std::vector<std::vector<cv::Point> > convexHulls(contours.size());

	for (unsigned int i = 0; i < contours.size(); i++) 
	{
		cv::convexHull(contours[i], convexHulls[i]);
	}

	// show what we get
	cv::drawContours(frame_convex, convexHulls, -1, cv::Scalar(0, 255, 0), -1);

	//		3. trace the shape of the contours on the final frame
	for (int i = 0; i < convexHulls.size(); i++)
	{
		// get the x and y of the rectangle we get from the contour
		cv::Rect bounding_car = cv::boundingRect(convexHulls[i]);
		cv::Point center;
		center.x = bounding_car.x + (bounding_car.width) / 2;
		center.y = bounding_car.y + (bounding_car.height) / 2;

		// draw on the image
		//cv::drawContours(frame_final, convexHulls, i, cv::Scalar(0, 0, 255), 2);
		cv::rectangle(frame_final, bounding_car, cv::Scalar(0, 0, 255), 2);
		
		cars_coordinates.push_back(center);

		for(int i=0;i<cars_coordinates.size();i++)
			counting_crossline(compteur1, compteur2, pointA_line, pointB_line, cars_coordinates[i], old_cars_coordinates[i]);
	}
	return cars_coordinates;
}

// if a car croww the road, a counter must be incremented (2 conters because two roads)
void counting_crossline(int compteur1, int compteur2, cv::Point pointA_line, cv::Point pointB_line, cv::Point pointCar, cv::Point pointCar_old)
{
	if (abs(pointA_line.x - pointB_line.x) < 50 && abs(pointA_line.y - pointB_line.y) > 200)
	{
		// the line is vertical
		if (pointCar.x > (pointA_line.x + pointB_line.x) / 2 && pointCar_old.x < (pointA_line.x + pointB_line.x) / 2)
		{
			// the car has crossed the line and is heading to the top of the frame
			compteur1++;
		}
		if (pointCar.x <(pointA_line.x + pointB_line.x) / 2 && pointCar_old.x >(pointA_line.x + pointB_line.x) / 2)
		{
			// the car has crossed the line and is heading to the bottom of the frame
			compteur2++;
		}
	}

	if (abs(pointA_line.y - pointB_line.y) < 50 && abs(pointA_line.x - pointB_line.x) > 200)
	{
		// the line is horizontal
		if (pointCar.y > (pointA_line.y + pointB_line.y) / 2 && pointCar_old.y < (pointA_line.y + pointB_line.y) / 2)
		{
			// the car has crossed the line and is heading to the top of the frame
			compteur1++;
		}
		if (pointCar.y <(pointA_line.y + pointB_line.y) / 2 && pointCar_old.y > (pointA_line.y + pointB_line.y) / 2)
		{
			// the car has crossed the line and is heading to the bottom of the frame
			compteur2++;
		}
	}
}