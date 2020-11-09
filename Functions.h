#pragma once
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include "Road.h"

std::vector<cv::Point> tracking_function(cv::Mat, cv::Mat, cv::Mat, std::vector<cv::Point>, cv::Point, cv::Point, int, int, int, Road, Road);
void counting_crossline(cv::Point, cv::Point, cv::Point, cv::Point);
void print_compters(int, int, cv::Mat);
void onMouse(int, int, int, int, void*);
void draw_road(cv::Mat image, cv::Point pt1, cv::Point pt2, cv::Point pt3, cv::Point pt4, int road_number);