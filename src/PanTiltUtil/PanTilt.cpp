#include "PanTiltUtil/PanTilt.h"

#include <iostream>

void PanTiltUtil::aim(ArmorDescriptor armor) {
	auto p = armor.get3DPoint();
	auto yaw = -atanf(p.x / p.z) * 180.0 / CV_PI;
	auto pitch = -atanf(-p.y / p.z) * 180.0 / CV_PI;

	std::cout << "[PTZ] d yaw: " << yaw << "\td pitch: " << pitch << std::endl;
	sender.pack(yaw, pitch, 0, 0, 0);
	sender.send();
}