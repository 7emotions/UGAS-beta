#include "Trajectory/Trajectory.h"

#include <cmath>
#include <opencv2/core/types.hpp>

#include "Detector/Detector.h"
#include "Parameters/Parameters.h"

void Trajectory::errorCalculate(double &error, cv::Point2d pos, double v,
								double pitch) {
	error = pos.x * tan(pitch) -
			param::TrajectoryStaticDate::G * pos.x * pos.x /
				(2.0 * v * v * cos(pitch) * cos(pitch)) -
			pos.y;
}

void Trajectory::projectTransform(const cv::Point3d &org, cv::Point2d &dst) {
	auto s = EuDis(cv::Point2d(org.x, org.y));
	dst.x = s - _gunpointOffset;
	dst.y = org.z;
}

void Trajectory::rotateTransform(const cv::Point2d &org, cv::Point2d &dst, double pitch) {
	auto s = EuDis(cv::Point2d(org.x, org.y));
	auto alpha = std::atan(org.x / org.y);
	auto beta = alpha - pitch;
	dst.x = s * std::cos(beta);
	dst.y = s * std::sin(beta);
}


cv::Point3d Trajectory::solve(cv::Point3d pos, double v, size_t &count,
							  double &err) {
	auto dpos = _cameraOffset + pos;
	cv::Point2d projPos;

	projectTransform(dpos, projPos);

	auto error = 0.0;
	auto pitch = error;
	auto tmp = projPos.y;
	auto i = 0;

	do {
		tmp = tmp - error;
		pitch = atan(tmp / projPos.x);
		errorCalculate(error, projPos, v, pitch);
		i++;
	} while (fabs(error) > 1e-6 && i < 30);

	count = i;
	err = error;

	return cv::Point3d(pos.x, pos.y, tmp);
}

cv::Point3d Trajectory::solve(cv::Point3d pos, double v, double pitch_) {
	auto dpos = _cameraOffset + pos;
	cv::Point2d projPos;

	projectTransform(dpos, projPos);
	rotateTransform(projPos, projPos, pitch_);

	auto error = 0.0;
	auto pitch = error;
	auto tmp = projPos.y;
	auto i = 0;

	do {
		tmp = tmp - error;
		pitch = atan(tmp / projPos.x);
		errorCalculate(error, projPos, v, pitch);
		i++;
	} while (fabs(error) > 1e-6 && i < 30);

	return cv::Point3d(pos.x, pos.y, tmp);
}
