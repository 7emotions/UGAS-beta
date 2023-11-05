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

inline double EuDis(cv::Point2f a, cv::Point2f b) {
	return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}

cv::Mat Detector::preprocess(cv::Mat img, COLOR_TAG tagToDetect) {

	cv::Mat channels[3];
	cv::Mat binary, gaussian, dilate;
	cv::split(img, channels);
	cv::threshold(channels[tagToDetect == BLUE ? 0 : 2]-channels[tagToDetect == BLUE ? 2 : 0], binary, 125, 255, cv::THRESH_BINARY);
	
	GaussianBlur(binary, gaussian, cv::Size(5, 5), 0);

	auto element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
	cv::dilate(gaussian, dilate, element);

	return dilate;std::vector<cv::Point2f> points;

}


cv::Mat Detector::DetectLights(cv::Mat img, COLOR_TAG color_tag) {

	Detector myDetector;
	std::vector<LightDescriptor> lights;

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	NumberIdentify identifier("../model/NINNModel.onnx");

	cv::Mat pre = myDetector.preprocess(img, color_tag);

	//cv::imshow("pre",pre);
	cv::findContours(pre, contours,
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
			auto distance = EuDis(lights[i].center, lights[j].center);
			auto rdsm = distance / ml;
			if (rdsm > 3.5 || rdsm < 0.5){
				continue;
			}

			cv::Point2f center((lights[i].center.x+lights[j].center.x)/2,
					(lights[i].center.y+lights[j].center.y)/2);
			auto angle = (lights[i].angle + lights[j].angle)/2;
			
			cv::RotatedRect rect(center, cv::Size(distance, ml*125/56), angle);
			//std::cout<<angle<<std::endl;
			cv::RotatedRect pnpRect(center, cv::Size(distance,ml), angle);
			std::vector<cv::Point2f> points;
			std::vector<cv::Point2f> pnpPs;
			rect.points(points);
			pnpRect.points(pnpPs);

			std::sort(points.begin(), points.end(), [](const cv::Point2f &a, const cv::Point2f &b){
				return a.y < b.y || a.x < b.x;
			});
			
			enum {
				TL,
				TR,
				BL,
				BR,
			};

			auto leftHeight = EuDis(points[TL], points[BL]);
			auto rightHeight = EuDis(points[TR], points[BR]);
			auto maxHeight = std::max(leftHeight, rightHeight);

			auto upWidth = EuDis(points[TL], points[TR]);
			auto downWidth = EuDis(points[BL], points[BR]);
			auto maxWidth = std::max(upWidth, downWidth);

			cv::Point2f srcAffinePts[4] = {cv::Point2f(points[TL]),cv::Point2f(points[TR]),cv::Point2f(points[BR]),cv::Point2f(points[BL])};
			cv::Point2f dstAffinePts[4] = {cv::Point2f(0,0),cv::Point2f(maxWidth,0),cv::Point2f(maxWidth,maxHeight),cv::Point2f(0,maxHeight)};

			auto affineMat = cv::getPerspectiveTransform(srcAffinePts, dstAffinePts);

			cv::Mat roi;
			try {
				cv::warpPerspective(img, roi, affineMat, cv::Point(maxWidth, maxHeight));
				cv::imshow("perspective", roi);
				std::tuple<int,double> res = identifier.Identify(roi);
				auto code = (int)std::get<0>(res);
				auto confidence = (double)std::get<1>(res);

				if (code == 0 || confidence<0.5) {
					continue;
				}

				cv::putText(img, std::to_string(code), center, cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0,255,0),
							2, cv::LINE_AA);
				std::cout<<"Code:"<<code<<"\t"<<"Confidence" << confidence<<std::endl;

				
				cv::line(img, points[TL], points[TR], cv::Scalar(0, 255, 0));
				cv::line(img, points[TR], points[BR], cv::Scalar(0, 255, 0));
				cv::line(img, points[BR], points[BL], cv::Scalar(0, 255, 0));
				cv::line(img, points[BL], points[TL], cv::Scalar(0, 255, 0));
				
			} catch (cv::Exception &e) {
				std::cout << e.what() << std::endl;
			}

		}
	}
	return img;
}


