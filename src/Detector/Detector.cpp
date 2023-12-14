#include "Detector.h"

#include <math.h>

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/matx.hpp>
#include <opencv2/core/operations.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <ostream>
#include <vector>

#include "ArmorDescriptor/ArmorDescriptor.h"
#include "LightDescriptor/LightDescriptor.h"


void sortPts(std::vector<cv::Point2f> &points) {
	std::sort(points.begin(), points.end(),
			  [](const cv::Point2f &a, const cv::Point2f &b) {
				  return a.x + a.y > b.x + b.y;
			  });
	auto tl = points[0];
	cv::Point2f bl, br, tr;
	auto a = points[1] - tl;
	auto b = points[2] - tl;
	auto c = points[3] - tl;

	auto e1 = crossProduct(a, b) > 0;
	auto e2 = crossProduct(a, c) > 0;
	auto e3 = crossProduct(b, c) > 0;

	if (e1 && e2) {
		bl = points[1];

		if (e3) {
			br = points[2];
			tr = points[3];
		} else {
			tr = points[2];
			br = points[3];
		}
	} else if (!e1 && !e2) {
		tr = points[1];

		if (e3) {
			br = points[3];
			bl = points[2];
		} else {
			bl = points[3];
			br = points[2];
		}
	} else {
		br = points[1];

		if (e3) {
			bl = points[3];
			tr = points[2];
		} else {
			tr = points[3];
			bl = points[2];
		}
	}

	points.clear();
	points.push_back(tl);
	points.push_back(bl);
	points.push_back(tr);
	points.push_back(br);
}
enum { BR, BL, TR, TL };

void perspective(cv::Mat &img, std::vector<cv::Point2f> &points, cv::Mat &roi) {
	auto leftHeight = EuDis(points[TL], points[BL]);
	auto rightHeight = EuDis(points[TR], points[BR]);
	auto maxHeight = std::max(leftHeight, rightHeight);

	auto upWidth = EuDis(points[TL], points[TR]);
	auto downWidth = EuDis(points[BL], points[BR]);
	auto maxWidth = std::max(upWidth, downWidth);

	cv::Point2f srcAffinePts[4] = {
		cv::Point2f(points[TL]), cv::Point2f(points[TR]),
		cv::Point2f(points[BR]), cv::Point2f(points[BL])};
	cv::Point2f dstAffinePts[4] = {cv::Point2f(0, 0), cv::Point2f(maxWidth, 0),
								   cv::Point2f(maxWidth, maxHeight),
								   cv::Point2f(0, maxHeight)};

	auto affineMat = cv::getPerspectiveTransform(srcAffinePts, dstAffinePts);
	cv::warpPerspective(img, roi, affineMat, cv::Point(maxWidth, maxHeight));
}

cv::Mat Detector::preprocess(cv::Mat img, COLOR_TAG tagToDetect) {
	cv::Mat channels[3];
	cv::Mat binary, gaussian, dilate;
	cv::split(img, channels);
	cv::threshold(channels[tagToDetect == BLUE ? 0 : 2] -
					  channels[tagToDetect == BLUE ? 2 : 0],
				  binary, 125, 255, cv::THRESH_BINARY);

	GaussianBlur(binary, gaussian, cv::Size(5, 5), 0);

	auto element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
	cv::dilate(gaussian, dilate, element);

	return dilate;
	std::vector<cv::Point2f> points;
}

cv::Mat Detector::DetectLights(cv::Mat img, COLOR_TAG color_tag,
							   std::vector<ArmorDescriptor> &armors) {
	cv::Point2i origin(img.cols / 2, img.rows / 2);

	std::vector<LightDescriptor> lights;

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	std::vector<cv::Point2f> centers;

	cv::Mat pre = preprocess(img, color_tag);

	// cv::imshow("pre",pre);
	cv::findContours(pre, contours, hierarchy, cv::RETR_TREE,
					 cv::CHAIN_APPROX_NONE, cv::Point());

	std::vector<cv::RotatedRect> boundRect(contours.size());

	for (size_t i = 0; i < contours.size(); i++) {
		auto area = contourArea(contours[i]);
		if (area < minArea || contours[i].size() <= minContoursSize) {
			continue;
		}

		auto rec = fitEllipse(contours[i]);

		if (rec.size.width / rec.size.height > lightLengthRatio) {
			continue;
		}

		rec.size.height *= 1.2;
		rec.size.width *= 1.2;

		lights.push_back(rec);
	}

	for (size_t i = 0; i < lights.size(); i++) {
		for (size_t j = i + 1; j < lights.size(); j++) {
			bool loopFlag = true;
			auto center = (lights[i].center + lights[j].center) / 2;
			for (size_t i = 0; i < centers.size(); i++) {
				if (EuDis(center, centers[i]) < 10) {
					loopFlag = false;
					break;
				}
			}
			if (!loopFlag) {
				continue;
			}
			if (asin(abs(crossProduct(lights[i].v, lights[j].v)) >
					 minAngleDiff)) {
				continue;
			}

			auto ml = (lights[i].length + lights[j].length) / 2;

			auto rpd = abs(lights[i].length - lights[j].length) /
					   std::max(lights[i].length, lights[j].length);

			if (rpd > 0.8) {
				continue;
			}

			auto rdfm = abs(lights[i].length - lights[j].length) / ml;

			if (rdfm > 0.5) {
				continue;
			}

			auto distance = EuDis(lights[i].center, lights[j].center);
			auto rdsm = distance / ml;

			if (rdsm > 3.5 || rdsm < 0.5) {
				continue;
			}

			std::vector<cv::Point2f> points;
			lights[i].extend(points, ml,
							 lights[i].center.x < lights[j].center.x
								 ? LightDescriptor::LEFT
								 : LightDescriptor::RIGHT);
			lights[j].extend(points, ml,
							 lights[i].center.x < lights[j].center.x
								 ? LightDescriptor::RIGHT
								 : LightDescriptor::LEFT);

			sortPts(points);

			// cv::Mat frame=img.clone();
			// cv::putText(frame, "TL", points[TL], cv::FONT_HERSHEY_SIMPLEX, 2,
			// cv::Scalar(0,255,0)); cv::putText(frame, "TR", points[TR],
			// cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0,255,0));
			// cv::putText(frame, "BL", points[BL], cv::FONT_HERSHEY_SIMPLEX, 2,
			// cv::Scalar(0,255,0)); cv::putText(frame, "BR", points[BR],
			// cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0,255,0));

			// cv::imshow("frame", frame);

			cv::Mat roi;
			perspective(img, points, roi);

			auto res = identifier.Identify(roi);
			auto code = std::get<0>(res);
			auto confidence = std::get<1>(res);

			if (code == 0) {
				points.clear();
				continue;
			}

			points.clear();

			lights[i].pnpPts(points, lights[i].center.x < lights[j].center.x
										 ? LightDescriptor::LEFT
										 : LightDescriptor::RIGHT);
			lights[j].pnpPts(points, lights[i].center.x < lights[j].center.x
										 ? LightDescriptor::RIGHT
										 : LightDescriptor::LEFT);

			sortPts(points);
			// cv::Mat frame=img.clone();
			// cv::putText(frame, "TL", points[TL], cv::FONT_HERSHEY_SIMPLEX, 2,
			// cv::Scalar(0,255,0)); cv::putText(frame, "TR", points[TR],
			// cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0,255,0));
			// cv::putText(frame, "BL", points[BL], cv::FONT_HERSHEY_SIMPLEX, 2,
			// cv::Scalar(0,255,0)); cv::putText(frame, "BR", points[BR],
			// cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0,255,0));

			// cv::imshow("frame", frame);

			cv::line(img, points[TL], points[TR], cv::Scalar(0, 255, 0));
			cv::line(img, points[TR], points[BR], cv::Scalar(0, 255, 0));
			cv::line(img, points[BR], points[BL], cv::Scalar(0, 255, 0));
			cv::line(img, points[BL], points[TL], cv::Scalar(0, 255, 0));

			// cv::putText(img, std::to_string(code), center, );
			std::cout << "Code: " << code << "\tConfidence: " << confidence
					  << std::endl;

			if (code == 0) {  //|| confidence < 0.65) {
				continue;
			}

			centers.push_back(center);
			auto armor = ArmorDescriptor(points, code);
			cv::Mat rot,t;
			solver.solve(armor, rot, t);
			cv::Point3d p = {t.at<double>(0), t.at<double>(1), t.at<double>(2)};
			armor.set3DPoint(p);
			armors.push_back(armor);

			points.clear();
		}
	}

	return img;
}
