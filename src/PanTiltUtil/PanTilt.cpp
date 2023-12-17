#include "PanTiltUtil/PanTilt.h"

#include <iostream>

void PanTiltUtil::aim(ArmorDescriptor armor) {
	auto p = armor.get3DPoint();
	aim(p);
}

void PanTiltUtil::aim(cv::Point3d pos){
	auto yaw = -atanf(-pos.y / pos.x) * 180.0 / CV_PI;
	auto pitch = -atanf(pos.z / pos.x) * 180.0 / CV_PI;
	aim(yaw,pitch);
}

void PanTiltUtil::aim(float yaw, float pitch){

	std::cout << "[PTZ] d yaw: " << yaw << "\td pitch: " << pitch << std::endl;
	sender.pack(yaw, pitch, 0, 0, 0);
	sender.send();

}
