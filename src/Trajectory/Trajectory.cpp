#pragma once

#include "Trajectory/Trajectory.h"

#include <cmath>
#include <complex>
#include <opencv2/core/types.hpp>

#include "Detector/Detector.h"
#include "Parameters/Parameters.h"

void projectTransform(cv::Point3d &org, cv::Point2d &dst) {
	auto s = EuDis(cv::Point2d(org.x, org.y));
	dst.x = s;
	dst.y = org.z;
}

inline void motionCalculate(double &error, cv::Point2d pos, double v,
							double pitch) {
	error = pos.x * tan(pitch) -
			param::TrajectoryStaticDate::G * pos.x * pos.x / (2.0 * v * v * cos(pitch) * cos(pitch)) -
			pos.y;
}

cv::Point3d Trajectory::solve(cv::Point3d pos, double v) {
	auto dpos = _cameraOffset + pos;
	cv::Point2d proPos;

	projectTransform(dpos, proPos);
	auto error = 0.0;
	auto pitch = error;
	auto tmp = proPos.y;

	do {
		tmp = proPos.y - error;
		pitch = atan(tmp / proPos.x);
		motionCalculate(error, proPos, v, pitch);
	}while (fabs(error) > 1e-6);

	return cv::Point3d(pos.x , pos.y, tmp);
}