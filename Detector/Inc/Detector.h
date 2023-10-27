// Detector.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <algorithm>

#include "TinyXML.h"
#include "LightDescriptor.h"

// TODO: 在此处引用程序需要的其他标头。

class Detector {
public:
	typedef enum { BLUE, RED } COLOR_TAG;

	Detector();
	~Detector();

	cv::Mat preprocess(cv::Mat img, COLOR_TAG tag);

private:
};

Detector::Detector() {}

Detector::~Detector() {}

inline cv::Mat Detector::preprocess(cv::Mat img, COLOR_TAG tagToDetect) {

	cv::Mat channels[3];
	cv::Mat binary, gaussian, dilate;
	cv::split(img, channels);
	cv::threshold(channels[tagToDetect == BLUE ? 0 : 2], binary, 220, 255, 0);

	auto element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));

	cv::GaussianBlur(binary, gaussian, cv::Size(5, 5), 0);
	cv::dilate(gaussian, dilate, element);

	//cv::imshow("pro", dilate);

	return dilate;

	//cv::Mat hsv;
	//cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);
	//cv::Mat mask;

	//if (tagToDetect == COLOR_TAG::BLUE) {
	//	cv::inRange(hsv, cv::Scalar(100, 43, 46), cv::Scalar(124, 255, 255), mask);

	//}
	//else if (tagToDetect == COLOR_TAG::RED) {
	//	cv::inRange(hsv, cv::Scalar(0, 43, 46), cv::Scalar(10, 255, 255), mask);
	//	cv::Mat mask1;
	//	cv::inRange(hsv, cv::Scalar(156, 43, 46), cv::Scalar(180, 255, 255), mask1);

	//	cv::bitwise_and(img, img, img, mask1);
	//}

	//cv::bitwise_and(img, img, img, mask);
	//cv::Mat bin;
	//cv::cvtColor(img, bin, cv::COLOR_RGB2GRAY);
	//cv::threshold(bin, bin, 100, 255, cv::THRESH_BINARY);

	//cv::imshow("bin", bin);

	//return bin;
}
