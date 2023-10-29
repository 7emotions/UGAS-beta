#include "Detector.h"

#include <cstddef>
#include <iostream>
#include <algorithm>

#include <opencv2/core/types.hpp>
#include <opencv2/opencv.hpp>

cv::Mat Detector::preprocess(cv::Mat img, COLOR_TAG tagToDetect) {

	cv::Mat channels[3];
	cv::Mat binary, gaussian, dilate;
	cv::split(img, channels);
	cv::threshold(channels[tagToDetect == BLUE ? 0 : 2], binary, 220, 255, 0);
	std::cout<<binary.size()<<std::endl;
	GaussianBlur(binary, gaussian, cv::Size(5, 5), 0);

	auto element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
	cv::dilate(gaussian, dilate, element);

	return dilate;

}


cv::Mat Detector::fetchLights(cv::Mat img, COLOR_TAG color_tag) {
	Detector myDetector;
	std::vector<LightDescriptor> lights;

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;


	cv::findContours(myDetector.preprocess(img, color_tag), contours,
		hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE,
		cv::Point());

	std::vector<cv::RotatedRect> boundRect(contours.size());

	for (size_t i = 0; i < contours.size(); i++) {
		auto area = contourArea(contours[i]);
		if (area < minArea || contours[i].size() <= minContoursSize)
		{
			continue;
		}

		auto rec = fitEllipse(contours[i]);

		if (rec.size.width / rec.size.height > lightLengthRatio)
		{
			continue;
		}

		rec.size.height *= 1.2;
		rec.size.width *= 1.2;

		lights.push_back(rec);
	}

	sort(lights.begin(), lights.end(), [&](const LightDescriptor& a, const LightDescriptor& b) {
		return a.angle == b.angle ? a.angle < b.angle : a.angle > b.angle;
		});

	for (size_t i = 0; i < lights.size(); i++)
	{
		int j = i + 1;
		for (; (size_t)j < lights.size(); j++)
		{
			if (abs(lights[i].angle - lights[j].angle) > minAngleDiff)
			{
				break;
			}
		}

		for(;j>=0;j--){
			auto ml = (lights[i].center.x - lights[j].center.x) / 2;
			auto rpd = abs(lights[i].length-lights[j].length)/std::max(lights[i].length,lights[j].length);
			if(rpd > 0.8){
				continue;
			}
			auto rdfm = abs(lights[i].length - ml) / ml;
			if (rdfm > 0.5){
				continue;
			}
			auto distance = 
				sqrt(pow(lights[i].center.x - lights[j].center.x, 2) + pow(lights[i].center.y - lights[j].center.y, 2));
			auto rdsm = distance / ml;
			if (rdsm > 3.5 || rdsm < 0.5){
				continue;
			}
			auto dx = abs(lights[i].center.x - lights[j].center.x);
			auto dy = abs(lights[i].center.y - lights[j].center.y);
			auto dxr = dx / ml;
			auto dyr = dy / ml;
			if(dxr > 2 || dxr<0.6){
				continue;
			}
			if(dyr>1.2){
				continue;
			}

			cv::Point2f center((lights[i].center.x+lights[j].center.x)/2,
					(lights[i].center.y+lights[j].center.y)/2);
			auto angle = (lights[i].angle + lights[j].angle)/2;
			cv::RotatedRect rect(center, cv::Size(distance, ml), angle);
			std::vector<cv::Point2f> points;
			rect.points(points);

			for(size_t k=0;k<points.size();k++){
				line(img, points[k], points[(k+1)%points.size()], 
				color_tag == Detector::COLOR_TAG::BLUE? cv::Scalar(0, 0, 255):cv::Scalar(255,0,0)
				, 5);
			}
		}
	}
	return img;
}


