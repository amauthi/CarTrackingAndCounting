#include "stdafx.h"

#include "Functions.h"
#include <string>

// variables for counting cars crossing the line drawed by user
int compteur1 = 0; int compteur2 = 0;

// function that serve to track any car in frame_diff 
// returns the list of the coordinates of the cars because we have to save these coordinates to compare them at the next frame to the new ones (to see if a car has crossed the border)

std::vector<cv::Point> tracking_function(cv::Mat frame_diff, cv::Mat frame_convex, cv::Mat frame_final, std::vector<cv::Point> old_cars_coordinates, cv::Point pointA_line, cv::Point pointB_line, int frame_number, int centers_printed, int distance_old_new)
{
	std::vector<cv::Point> cars_coordinates;
	
//	std::cout << "after init cars_coord vect" << std::endl;
	// create a copy of frame_diff to ensure that the original is not modified by findContours
	cv::Mat frame_diff_copy = frame_diff.clone();

	//		1. find the contours of the image
	std::vector<std::vector<cv::Point> > contours;
//	std::cout << "after init contours" << std::endl;
	
	cv::findContours(frame_diff_copy, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

//	std::cout << "after using FindContours" << std::endl;

	// show what we get (no needed because with convexHull, we will redraw on final image)
	//cv::drawContours(frame_final, contours, -1, cv::Scalar(0, 0, 255), -1);

	//		2. fill those contours with convexHull
	// we have to declare convexHulls after we use findContours, because we need the final size of findcontours
	std::vector<std::vector<cv::Point> > convexHulls(contours.size());

//	std::cout << "after init convexHulls" << std::endl;

	for (unsigned int i = 0; i < contours.size(); i++) 
	{
		cv::convexHull(contours[i], convexHulls[i]);
	}

//	std::cout << "after using convexHulls" << std::endl;

	// show what we get
	cv::drawContours(frame_convex, convexHulls, -1, cv::Scalar(0, 255, 0), -1);

//	std::cout << "after DRAWING ON Frame_convex" << std::endl;

	//		3. trace the shape of the contours on the final frame 
	// TODO : find a way to remove contours that are outside of the roads
	for (int i = 0; i < convexHulls.size(); i++)
	{
		// get the x and y of the rectangle we get from the contour
		cv::Rect bounding_car = cv::boundingRect(convexHulls[i]);
		cv::Point center;
		center.x = bounding_car.x + (bounding_car.width) / 2;
		center.y = bounding_car.y + (bounding_car.height) / 2;

		// draw on the image
		cv::rectangle(frame_final, bounding_car, cv::Scalar(0, 0, 255), 2);
		
		// printf the centers of the cars (x in yellow and y in blue)
		if (centers_printed == 1)
		{
			cv::putText(frame_final, std::to_string(center.x), cv::Point(center.x - 50, center.y), cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 255, 255));
			cv::putText(frame_final, std::to_string(center.y), cv::Point(center.x + 50, center.y), cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(255, 0, 0));
		}

		cars_coordinates.push_back(center);
	}

//	std::cout << "after parcouring convexHulls" << std::endl;

//	for (int i = 0; i < cars_coordinates.size(); i++)
//		std::cout << "cars_coordinates[" << i << "] = " << cars_coordinates[i] << std::endl;
//	std::cout << std::endl;
//	for (int i = 0; i < old_cars_coordinates.size(); i++)
//		std::cout << "old_cars_coordinates[" << i << "] = " << old_cars_coordinates[i] << std::endl;
//	std::cout << std::endl;

	//		4. calling of counting_crossline methode
	// we add this condition to be sure that we had time to fill the old coordinates 
	if (frame_number > 1)
	{
		for (int i = 0; i < cars_coordinates.size(); i++)
		{
	//		printf("i = %d, pointCar.x = %d, pointCar.y = %d\n", i, cars_coordinates[i].x, cars_coordinates[i].y);
	//		printf("i = %d, pointCar_old.x = %d, pointCar_old.y = %d\n", i, old_cars_coordinates[i].x, old_cars_coordinates[i].y);
	//		printf("\n");

			// we search in the old_cars_coord vector which center is closed to the center of cars_coord (meaning they belong to the same car)
			// we then compare the two inside function counting_crossline
			for (int j = 0; j < old_cars_coordinates.size(); j++)
			{
				if(abs(old_cars_coordinates[j].x-cars_coordinates[i].x) <distance_old_new && abs(old_cars_coordinates[j].y - cars_coordinates[i].y) <distance_old_new)
					counting_crossline(pointA_line, pointB_line, cars_coordinates[i], old_cars_coordinates[j]);
			}
		}
	}
//	std::cout << "after calling couting_crossline" << std::endl;
	print_compters(compteur1, compteur2, frame_final);
//	std::cout << "after calling print_compters" << std::endl;
	return cars_coordinates;
}

// if a car croww the road, a counter must be incremented (2 conters because two roads)
void counting_crossline(cv::Point pointA_line, cv::Point pointB_line, cv::Point pointCar, cv::Point pointCar_old)
{
	if (abs(pointA_line.x - pointB_line.x) < 50 && abs(pointA_line.y - pointB_line.y) > 200)
	{
		// the line is vertical
		if (pointCar.x > (pointA_line.x + pointB_line.x) / 2 && pointCar_old.x < (pointA_line.x + pointB_line.x) / 2)
		{
			// the car has crossed the line and is heading to the top of the frame
			compteur1++;
			printf("compteur1++ verticaly\n");
		}
		if (pointCar.x <(pointA_line.x + pointB_line.x) / 2 && pointCar_old.x >(pointA_line.x + pointB_line.x) / 2)
		{
			// the car has crossed the line and is heading to the bottom of the frame
			compteur2++;
			printf("compteur2++ verticaly\n");
		}
	}
	// y increase as we head to the bottom of the image
	if (abs(pointA_line.y - pointB_line.y) < 50 && abs(pointA_line.x - pointB_line.x) > 200)
	{
		// the line is horizontal
		if ((pointCar.y < (pointA_line.y + pointB_line.y) / 2) && (pointCar_old.y > (pointA_line.y + pointB_line.y) / 2))
		{
			// the car has crossed the line and is heading to the top of the frame
			compteur1++;
			printf("compteur1++ horizontaly\n");
		}
		if ((pointCar.y > (pointA_line.y + pointB_line.y) / 2) && (pointCar_old.y < (pointA_line.y + pointB_line.y) / 2))
		{
			// the car has crossed the line and is heading to the bottom of the frame
			compteur2++;
			printf("compteur2++ horizontaly\n");
		}
	}
//	printf("compteur1 inside counting = %d\n", compteur1);
//	printf("compteur2 inside counting = %d\n", compteur2);
	printf("\n");
}


void print_compters(int compteur1, int compteur2, cv::Mat frame_final)
{
	std::string compt1 = std::to_string(compteur1);
	std::string compt2 = std::to_string(compteur2);
	cv::putText(frame_final, compt1, cv::Point(10, 30), cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 0, 255, 255));
	cv::putText(frame_final, compt2, cv::Point(500, 480), cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 0, 255, 255));
}
