#pragma once

#include "Trajectory/Trajectory.h"

#include "Detector/Detector.h"

cv::Point3d Trajectory::solve(cv::Point3d pos, double v) {
	auto p = pos - _offset;
	auto tmp = pos;

	auto s = EuDis(cv::Point2d(pos.x,pos.y));
	auto d = EuDis(pos);

	

	return tmp;
}