#pragma once
#include <opencv2/opencv.hpp>

class LightDescriptor
{
public:
	float width, length, angle, area;
	cv::Point2f center;

	LightDescriptor(const cv::RotatedRect& light) {
		width = light.size.width;
		length = light.size.height;

		center = light.center;
		angle = light.angle;
		area = light.size.area();
	}

};
