// Detector.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#include <opencv2/opencv.hpp>

#include "ArmorDescriptor/ArmorDescriptor.h"
#include "Identify/NumberIdentify.h"

const int minArea = 5;
const int minContoursSize = 1;
const double lightLengthRatio = 4.0;
const double minAngleDiff = 15.0;
const double minLengthDiffRatio = 0.8;

class Detector {
   public:
	typedef enum { BLUE, RED } COLOR_TAG;

	Detector() : identifier(NumberIdentify("../model/NINNModel.onnx")) {}
	~Detector() = default;

	cv::Mat preprocess(cv::Mat img, COLOR_TAG tag);
	cv::Mat DetectLights(cv::Mat img, COLOR_TAG tag,
						 std::vector<ArmorDescriptor> &armors);

   private:
	NumberIdentify identifier;
};
