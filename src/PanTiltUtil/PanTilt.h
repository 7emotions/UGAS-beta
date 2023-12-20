/**
 * @file PanTilt.h
 * @author Lorenzo Feng (lorenzo.feng@njust.edu)
 * @brief 云台控制类
 * @version 0.1
 * @date 2023-12-15
 * 
 * @copyright Copyright (c) Alliance, Nan Jing University of Science & Technology
 * 
 */
#pragma once

#include <opencv2/core/types.hpp>
#include "ArmorDescriptor/ArmorDescriptor.h"
#include "PnPSolver/PnPSolver.h"
#include "Serial/Serial.h"

class PanTiltUtil {
   public:
	PanTiltUtil() = default;
	~PanTiltUtil() = default;

	void aim(ArmorDescriptor armor);
	void aim(float yaw, float pitch);
	void aim(cv::Point3d pos);
	void aim() {
		sender.pack();
		sender.send();
	}

   private:
	SerialUtil sender;
	PnPSolver solver;
};