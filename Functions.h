#pragma once
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

std::vector<cv::Point> tracking_function(cv::Mat, cv::Mat, cv::Mat, std::vector<cv::Point>, cv::Point, cv::Point, int, int, int);
void counting_crossline(cv::Point, cv::Point, cv::Point, cv::Point);
void print_compters(int, int, cv::Mat);
void onMouse(int, int, int, int, void*);
