#pragma once
#include <opencv2/core/base.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

class LightDescriptor
{
public:
	cv::Point2f center;
	cv::Point2f pts[4];
	cv::Point2f v;
	cv::Point2f t;
	double length;

	enum{
		LEFT,
		RIGHT
	};

	LightDescriptor(const cv::RotatedRect& light):center(light.center) {
		light.points(pts);
		if (light.size.height > light.size.width) {
			v = pts[1] - pts[0];
			v /= light.size.height;

			t = pts[3] - pts[0];
			t /= 2;

			length = light.size.height;
		}else {
			v = pts[3] - pts[0];
			v /= light.size.width;

			t = pts[0] - pts[1];
			t /= 2;

			length = light.size.width;
		}
	}

	void pnpPts(std::vector<cv::Point2f> &pnpPts, bool flag=LEFT){
		if (flag) {
			pnpPts.push_back(center+t+length/2*v);
			pnpPts.push_back(center+t-length/2*v);
		}else {
			pnpPts.push_back(center-t+length/2*v);
			pnpPts.push_back(center-t-length/2*v);
		}
	}

	void extend(std::vector<cv::Point2f> &points, double ml,bool flag=LEFT){
		if (flag) {
			points.push_back(center+t+ml*120/56/2*v); //TL
			points.push_back(center+t-ml*120/56/2*v); //BL
		}else {
			points.push_back(center-t+ml*120/56/2*v); //TR
			points.push_back(center-t-ml*120/56/2*v); //BR
		}
	}
};