#pragma once
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

std::vector<cv::Point> tracking_function(cv::Mat, cv::Mat, cv::Mat, std::vector<cv::Point>, int , int, cv::Point, cv::Point);
void counting_crossline(int,int , cv::Point, cv::Point, cv::Point, cv::Point);