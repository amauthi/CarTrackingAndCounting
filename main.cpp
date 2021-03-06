// CarTrackingAndCounting.cpp : définit le point d'entrée pour l'application console.
// OpenCV Version : 2.4.11
//

#include "stdafx.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "Functions.h"
#include <iostream>
#include <stdio.h>
#include "Road.h"

using namespace std;
using namespace cv;

Mat frame;
Mat frame_old;
Mat frame_diff;
Mat frame_convex;

Mat frame_final;

// pointA and pointB will be clicked by the user with the mouse
cv::Point pointA_line;
cv::Point pointB_line;

bool already_clicked_once = false;
bool already_clicked_twice = false;

double truc = 0.0;

int number_of_roads = 2;

Line line1 = Line(false, false);
Line line2 = Line(false, false);

Road road1 = Road(false, line1, line2);
Road road2 = Road(false, line1, line2);
// user can select the points forming the counting line and the tracks by clicking on window

void onMouse(int evt, int x, int y, int flags, void* param)
{

	if (evt == CV_EVENT_LBUTTONDOWN)
	{
		// tracks
		if (!road1.road_shaped || !road2.road_shaped)
		{
			if (!road1.line1.pointA_done)
			{
				road1.line1.pointA.x = x;
				road1.line1.pointA.y = y;
				road1.line1.pointA_done = true;
				std::cout << "road1_line1_pointA" << std::endl;
			}
			else if (!road1.line1.pointB_done)
			{
				road1.line1.pointB.x = x;
				road1.line1.pointB.y = y;
				road1.line1.pointB_done = true;
				std::cout << "road1_line1_pointB" << std::endl;
			}
			else if (!road1.line2.pointA_done)
			{
				road1.line2.pointA.x = x;
				road1.line2.pointA.y = y;
				road1.line2.pointA_done = true;
				std::cout << "road1_line2_pointA" << std::endl;
			}
			else if (!road1.line2.pointB_done)
			{
				road1.line2.pointB.x = x;
				road1.line2.pointB.y = y;
				road1.line2.pointB_done = true;
				std::cout << "road1_line2_pointB" << std::endl;
			}
			else if (!road2.line1.pointA_done)
			{
				road2.line1.pointA.x = x;
				road2.line1.pointA.y = y;
				road2.line1.pointA_done = true;
				std::cout << "road2_line1_pointA" << std::endl;
			}
			else if (!road2.line1.pointB_done)
			{
				road2.line1.pointB.x = x;
				road2.line1.pointB.y = y;
				road2.line1.pointB_done = true;
				std::cout << "road2_line1_pointB" << std::endl;
			}
			else if (!road2.line2.pointA_done)
			{
				road2.line2.pointA.x = x;
				road2.line2.pointA.y = y;
				road2.line2.pointA_done = true;
				std::cout << "road2_line2_pointA" << std::endl;
			}
			else if (!road2.line2.pointB_done)
			{
				road2.line2.pointB.x = x;
				road2.line2.pointB.y = y;
				road2.line2.pointB_done = true;
				std::cout << "road2_line2_pointB" << std::endl;
			}
		}

		// counting line : we can only draw the counting line when the tracks are drawed
		else
		{
			if (!already_clicked_twice)
			{
				if (!already_clicked_once)
				{
					already_clicked_once = true;
					pointA_line.x = x;
					pointA_line.y = y;
				}
				else
				{
					already_clicked_twice = true;
					pointB_line.x = x;
					pointB_line.y = y;
				}
			}
		}

		// we indicate when the roads are shaped, and we enter the values of the coeffs of the lines
		if (road1.line1.pointA_done && road1.line1.pointB_done && road1.line2.pointA_done && road1.line2.pointB_done)
		{
			road1.line1.a = ((double)(road1.line1.pointB.y - road1.line1.pointA.y))/(road1.line1.pointB.x - road1.line1.pointA.x);
			road1.line1.b = ((double)(road1.line1.pointA.y*road1.line1.pointB.x - road1.line1.pointB.y*road1.line1.pointA.x)) / (road1.line1.pointB.x - road1.line1.pointA.x);

			road1.line2.a = ((double)(road1.line2.pointB.y - road1.line2.pointA.y)) / (road1.line2.pointB.x - road1.line2.pointA.x);
			road1.line2.b = ((double)(road1.line2.pointA.y*road1.line2.pointB.x - road1.line2.pointB.y*road1.line2.pointA.x)) / (road1.line2.pointB.x - road1.line2.pointA.x);

			road1.road_shaped = true;
		}
		if (road2.line1.pointA_done && road2.line1.pointB_done && road2.line2.pointA_done && road2.line2.pointB_done)
		{
			road2.line1.a = ((double)(road2.line1.pointB.y - road2.line1.pointA.y)) / (road2.line1.pointB.x - road2.line1.pointA.x);
			road2.line1.b = ((double)(road2.line1.pointA.y*road2.line1.pointB.x - road2.line1.pointB.y*road2.line1.pointA.x)) / (road2.line1.pointB.x - road2.line1.pointA.x);

			road2.line2.a = ((double)(road2.line2.pointB.y - road2.line2.pointA.y)) / (road2.line2.pointB.x - road2.line2.pointA.x);
			road2.line2.b = ((double)(road2.line2.pointA.y*road2.line2.pointB.x - road2.line2.pointB.y*road2.line2.pointA.x)) / (road2.line2.pointB.x - road2.line2.pointA.x);

			road2.road_shaped = true;
		}
	}
}


Mat morph_op; 
Mat pannel;
Mat slow_pause; 

Mat diff_2;
Mat diff_3;


void on_trackbar(int, void*) {}

int main(int argc, char *argv[])
{
	std::cout << "OpencCV version = " << CV_VERSION << std::endl;
	// variable for tresholding
	int seuil = 25;
	
	// int variable to show or no the centers alognside the cars (1 : show / 0 : not show)
	int centers_printed = 0;
	// variable to adjust counting efficiency (the more this value is high, the more we risk to have errors)
	int distance_old_new = 0;
	 
	// int variable for pause (0 : not pause, 1 : pause)
	int pause = 0;

	// list of coordinates of the car
	std::vector<cv::Point> liste_new_Carcoordinates;
	std::vector<cv::Point> liste_old_Carcoordinates;
	int frame_number = 1; // start at 1 because frame 0 is not used to calculate diff

	// variables for the morphological operations
	int morph_elem = 0;
	int morph_size = 5;
	int morph_operator = 1;
	int const max_operator = 5;
	int const max_elem = 2;
	int const max_kernel_size = 21;
	namedWindow("Morphological operations");

	// creation of trackbars inside a panel to slow down the video and to pause it
	int slow = 1000;
	namedWindow("Slow motion / Pause");
	// Slow the video (1000 = 1 s between frames)
	createTrackbar("S", "Slow motion / Pause", &slow, 1000, on_trackbar);
	// Pause the video (press P to advance frame by frame)
	createTrackbar("P", "Slow motion / Pause", &pause, 1, on_trackbar);

	// creation of trackbars to enable showing of cars coordinates and to choose parameter to adjust counting efficiency
	namedWindow("Pannel");
	createTrackbar("C", "Pannel", &centers_printed, 1,on_trackbar);
	// distance_old_new : distance between the position of the car on the old frame and its position on the new frame
	createTrackbar("P", "Pannel", &distance_old_new, 100, on_trackbar);

	VideoCapture stream1(argv[1]);

	if (!stream1.isOpened())
	{
		cout << "no video ! :o" << endl;
		return -1;
	}

	while (char(waitKey(1)) != 'f')
	{

		stream1.read(frame);
		stream1.read(frame_final);

		// resize video
		Size size(800, 500);
		resize(frame, frame, size);
		resize(frame_final, frame_final, size);

	
		// calling onMouse function with video final window
		setMouseCallback("video final", onMouse);

		if (!frame_old.empty())
		{
			// it is not the first frame of the video
			frame_diff = frame - frame_old;
			
			// to test stuff about morphology

			diff_2 = frame - frame_old;
			cvtColor(diff_2, diff_2, CV_BGR2GRAY);
			threshold(diff_2, diff_2, seuil, 255, 0);

			diff_3 = frame - frame_old;
			cvtColor(diff_3, diff_3, CV_BGR2GRAY);
			threshold(diff_3, diff_3, seuil, 255, 0);


			frame_convex = frame_diff.clone();
			// trackbar to choose treshold
			createTrackbar("threshold", "diff", &seuil, 255, on_trackbar);
			
			// conversion to gray 
			cvtColor(frame_diff, frame_diff, CV_BGR2GRAY);

			// call to method that do a treshold 
			threshold(frame_diff, frame_diff, seuil, 255, 0);


			/// Create Trackbar to select Morphology operation (if we choose 5 : no operation)
			// Operator:\n 0: Opening - 1: Closing \n 2: Gradient - 3: Top Hat \n 4: Black Hat \n 5: Nothing
			createTrackbar("O", "Morphological operations", 
			&morph_operator, max_operator, on_trackbar);

			/// Create Trackbar to select kernel type
			// Element:\n 0: Rect - 1: Cross - 2: Ellipse
			createTrackbar("K", "Morphological operations",
				&morph_elem, max_elem,
				on_trackbar);

			/// Create Trackbar to choose kernel size
			// Kernel size:\n 2n +1
			createTrackbar("S", "Morphological operations",
				&morph_size, max_kernel_size,
				on_trackbar);



			int operation = morph_operator + 2;

			Mat element = getStructuringElement(morph_elem, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));
			

			if (operation != 7)
			{
				/// Apply the specified morphology operation (?)
				morphologyEx(frame_diff, frame_diff, operation, element);

				// erosion to eliminate noise of one pixel
				erode(frame_diff, frame_diff, element);
				dilate(frame_diff, frame_diff, element);

				morphologyEx(diff_2, diff_2, operation, element);


				// again a morph op but an opening to differencite two close cars
	//			morphologyEx(frame_diff, frame_diff, 0, element);
				morphologyEx(diff_3, diff_3, operation, element);
				erode(diff_3, diff_3, element);
	//			morphologyEx(diff_3, diff_3, 0, element);

			}
			else
			{
				std::cout << "Dangerous ! Program could crash unless threshold for frame_diff is high (over 100)" << std::endl;
			}


			erode(diff_2, diff_2, element);
			
			dilate(diff_3, diff_3, element);

			resize(frame_diff, frame_diff, size);
			
			resize(diff_2, diff_2, size);
			resize(diff_3, diff_3, size);

			//TODO		for(int i = 0; i<number_of_roads;i++) (adapt the code to number_of_roads with a loop)
			draw_road(frame_final, road1.line1.pointA, road1.line1.pointB, road1.line2.pointA, road1.line2.pointB, 1);
			draw_road(frame_final, road2.line1.pointA, road2.line1.pointB, road2.line2.pointA, road2.line2.pointB, 2);
//			draw_road(frame_final, pointA_line1_road1, pointB_line1_road1, pointA_line2_road1, pointB_line2_road1, 1);
//			draw_road(frame_final, pointA_line1_road2, pointB_line1_road2, pointA_line2_road2, pointB_line2_road2, 2);

			// if the user has drawned the two roads, then we can define the counting line
			if (road1.road_shaped && road2.road_shaped)
			{
				// counting line drawed in red
				cv::line(frame_final, pointA_line, pointB_line, cv::Scalar(0, 0, 255), 2);
			}

			
			liste_new_Carcoordinates = tracking_function(diff_3, frame_convex, frame_final, liste_old_Carcoordinates, pointA_line, pointB_line, frame_number, centers_printed, distance_old_new, road1, road2);

//			std::cout << "after calling tracking function" << std::endl;
//			std::cout <<""<<std::endl;
			imshow("diff", frame_diff);
			imshow("convex", frame_convex);
			liste_old_Carcoordinates = liste_new_Carcoordinates;
			frame_number++;

//			imshow("diff 2 : after erosion", diff_2);
//			imshow("diff 3 : after erosion and dilation", diff_3);
		}

		frame_old = frame;
		imshow("video", frame);
		imshow("video final", frame_final);

		// load explanations images
		morph_op = imread("Explanations_Morph_op.png", 1);
		pannel = imread("Explanations_Pannel.png", 1);
		slow_pause = imread("Explanations_Slow_Pause.png", 1);

		// resizing
		Size size_pannels(300, 500);
		resize(morph_op, morph_op, size_pannels);
		resize(pannel, pannel, size_pannels);
		resize(slow_pause, slow_pause, size_pannels);

		// show these images
		imshow("Morphological operations", morph_op);
		imshow("Pannel", pannel);
		imshow("Slow motion / Pause", slow_pause);

		int c;
		if (slow == 0) c = waitKey(1);
		else
			c = waitKey(slow);
		if ((char)c == 'c') { break; }

		while (pause == 1)
		{
			// user can advance frame by frame by pressing 'p' 
			if ((char(waitKey(1)) == 'p'))
			{
				break;
			}
		}
	}

    return 0;
}


