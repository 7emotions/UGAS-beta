/**
 * @file LightDescriptor.h
 * @author Lorenzo Feng (lorenzo.feng@njust.edu.cn)
 * @brief 灯条描述
 * @version 0.1
 * @date 2023-12-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <opencv2/core/base.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

class LightDescriptor {
   public:
	cv::Point2f center;
	cv::Point2f pts[4];
	cv::Point2f v;
	double length;

	enum { LEFT, RIGHT };

	LightDescriptor(const cv::RotatedRect &light) : center(light.center) {
		light.points(pts);
		if (light.size.height > light.size.width) {
			v = pts[1] - pts[0];
			v /= light.size.height;

			length = light.size.height;
		} else {
			v = pts[3] - pts[0];
			v /= light.size.width;

			length = light.size.width;
		}
	}

	/**
	 * @brief 将
	 * 
	 * @param pnpPts 
	 * @param flag 
	 */
	void pnpPts(std::vector<cv::Point2f> &pnpPts) {
		pnpPts.push_back(center + length / 2 * v);
		pnpPts.push_back(center - length / 2 * v);
	}

	void extend(std::vector<cv::Point2f> &points, double ml) {
		points.push_back(center + ml * 120 / 56 / 2 * v);
		points.push_back(center - ml * 120 / 56 / 2 * v);
	}
};