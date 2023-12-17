/**
 * @file HikCameraInterface.h
 * @author Lorenzo Feng (lorenzo.feng@njust.edu.cn)
 * @brief 海康相机接口
 * @version 0.1
 * @date 2023-12-15
 *
 * @copyright Alliance, Nan Jing University of Science & Technology
 * 
 */
#pragma once

#include <opencv2/core/mat.hpp>

#include "CameraParams.h"

class HikCameraInterface {
   public:
	virtual bool isUSEDevice(MV_CC_DEVICE_INFO *pstMVCCDI);
	virtual bool connectDeivce();
	virtual cv::Mat fetchFrame();
};