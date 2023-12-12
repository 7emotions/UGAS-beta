// Detector.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#include "ArmorDescriptor/ArmorDescriptor.h"
#include "LightDescriptor/LightDescriptor.h"

#include <cstddef>
#include <fstream>
#include <opencv2/opencv.hpp>

const int minArea = 5;
const int minContoursSize = 1;
const double lightLengthRatio = 4.0;
const double minAngleDiff = 15.0;
const double minLengthDiffRatio = 0.8;

class Detector {
public:
	typedef enum { BLUE, RED } COLOR_TAG;

	Detector()=default;
	~Detector()=default;

	cv::Mat preprocess(cv::Mat img, COLOR_TAG tag);
	cv::Mat DetectLights(cv::Mat img, COLOR_TAG tag,std::vector<ArmorDescriptor> &armors);

protected:
    double crossProduct(cv::Point2f a, cv::Point2f b);
    double EuDis(cv::Point2f a, cv::Point2f b);
    void sortPts(std::vector<cv::Point2f> &points);
    void perspective(cv::Mat &img, std::vector<cv::Point2f> &points, cv::Mat &roi);
};

