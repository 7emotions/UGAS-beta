#include "PanTiltUtil/PanTilt.h"

#include <iostream>

void PanTiltUtil::aim(ArmorDescriptor armor) {
	auto p = armor.get3DPoint();
	aim(p);
}

void PanTiltUtil::aim(cv::Point3d pos){
	float pitch, yaw;
	solve(pos, pitch, yaw);
	aim(yaw,pitch);
}

void PanTiltUtil::aim(float yaw, float pitch){

	std::cout << "[PTZ] d yaw: " << yaw << "\td pitch: " << pitch << std::endl;
	sender.pack(yaw, pitch, 0, 0, 0);
	sender.send();

}

void PanTiltUtil::solve(cv::Point3d pos, float &pitch, float &yaw){
	yaw = -atanf(-pos.y / pos.x) * 180.0 / CV_PI;
	pitch = -atanf(pos.z / pos.x) * 180.0 / CV_PI;
}