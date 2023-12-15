#include "PanTiltUtil/PanTilt.h"

#include <iostream>

void PanTiltUtil::aim(ArmorDescriptor armor) {
	auto p = armor.get3DPoint();
	aim(p);
}

/**
 * @brief 令云台瞄准点pos
 * 
 * @param pos 
 */
void PanTiltUtil::aim(cv::Point3d pos){
	auto yaw = -atanf(-pos.y / pos.x) * 180.0 / CV_PI;
	auto pitch = -atanf(pos.z / pos.x) * 180.0 / CV_PI;

	std::cout << "[PTZ] d yaw: " << yaw << "\td pitch: " << pitch << std::endl;
	sender.pack(yaw, pitch, 0, 0, 0);
	sender.send();
}