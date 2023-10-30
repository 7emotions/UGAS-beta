#include "Detector.h"
#include "Identify/NumberIdentify.h"

#include <complex>
#include <cstddef>
#include <exception>
#include <iostream>
#include <algorithm>
#include <string>
#include <tuple>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

cv::Mat Detector::preprocess(cv::Mat img, COLOR_TAG tagToDetect) {

	cv::Mat channels[3];
	cv::Mat binary, gaussian, dilate;
	cv::split(img, channels);
	cv::threshold(channels[tagToDetect == BLUE ? 0 : 2], binary, 220, 255, 0);
	
	GaussianBlur(binary, gaussian, cv::Size(5, 5), 0);

	auto element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
	cv::dilate(gaussian, dilate, element);

	return dilate;

}


cv::Mat Detector::DetectLights(cv::Mat img, COLOR_TAG color_tag) {

	Detector myDetector;
	std::vector<LightDescriptor> lights;

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	NumberIdentify identifier("../model/NINNModel.onnx");

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
			
			cv::RotatedRect rect(center, cv::Size(distance, distance), angle);
			std::vector<cv::Point2f> points;
			rect.points(points);

			cv::Mat rot = getRotationMatrix2D(center, angle, 1);
			cv::Mat rot_img;
			cv::warpAffine(img, rot_img, rot, img.size());
			auto k = 1.8;
			cv::Mat roi;
			try {
				roi = rot_img(cv::Rect(center.x - ml * k / 2, center.y - ml * k / 2, ml * k, ml * k));
				
				try {
					std::tuple<int, double> res = identifier.Identify(roi);
					std::cout << "code:" << std::get<0>(res) << std::endl;
					std::cout<<"confidence:"<<std::get<1>(res)<<std::endl;
				} catch (std::exception &err) {
					std::cout << err.what() << std::endl;
				}

			} catch (cv::Exception &e) {
				std::cout << e.what() << std::endl;
			}

		}
	}
	return img;
}


