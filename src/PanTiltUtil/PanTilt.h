/**
 * @file PanTilt.h
 * @author Lorenzo Feng (lorenzo.feng@njust.edu)
 * @brief 云台控制类
 * @version 0.1
 * @date 2023-12-15
 * 
 * @copyright 南京理工大学 Alliance
 * 
 */
#pragma once

#include "ArmorDescriptor/ArmorDescriptor.h"
#include "PnPSolver/PnPSolver.h"
#include "Serial/Serial.h"

class PanTiltUtil {
   public:
	PanTiltUtil() = default;
	~PanTiltUtil() = default;

	void aim(ArmorDescriptor armor);
	void aim(cv::Point3d pos);
	void aim() {
		sender.pack();
		sender.send();
	}

   private:
	SerialUtil sender;
	PnPSolver solver;
};