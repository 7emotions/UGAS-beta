﻿/**
 * @file Detector.h
 * @author Lorenzo Feng (lorenzo.feng@njust.edu.cn)
 * @brief 装甲板检测器
 * @version 0.1
 * @date 2023-12-15
 * 
 * @copyright Copyright (c) Alliance, Nan Jing University of Science & Technology
 * 
 */
#pragma once

#include <opencv2/core/types.hpp>
#include <opencv2/opencv.hpp>

#include "ArmorDescriptor/ArmorDescriptor.h"
#include "Identify/NumberIdentify.h"
#include "PnPSolver/PnPSolver.h"

inline double crossProduct(cv::Point2f a, cv::Point2f b) {
	return a.x * b.y - a.y * b.x;
}
 
template <typename T>
inline double EuDis(T a, T b={0}) {
	return cv::norm(a-b);
}

class Detector {
   public:
	typedef enum { BLUE, RED } COLOR_TAG;

	Detector() : identifier(NumberIdentify("../model/NINNModel.onnx")) {}
	~Detector() = default;

	cv::Mat preprocess(cv::Mat img, COLOR_TAG tag);
	cv::Mat DetectArmors(cv::Mat img, COLOR_TAG tag,
						 std::vector<ArmorDescriptor> &armors);

   private:
	NumberIdentify identifier;
	PnPSolver solver;
};
