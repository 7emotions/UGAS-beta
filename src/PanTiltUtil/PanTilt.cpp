#include "PanTiltUtil/PanTilt.h"

#include <iostream>

void PanTiltUtil::aim(ArmorDescriptor armor) {
	cv::Mat rot;
	cv::Mat t;

	solver.solve(armor, rot, t);

	cv::Point3d p = {t.at<double>(0), t.at<double>(1), t.at<double>(2)};

	auto yaw = -atanf(p.x / p.z) * 180.0 / CV_PI;
	auto pitch = -atanf(-p.y / p.z) * 180.0 / CV_PI;

	std::cout << "[PTZ] d yaw: " << yaw << "\td pitch: " << pitch << std::endl;
	sender.pack(yaw, pitch, 0, 0, 0);
	sender.send();
}