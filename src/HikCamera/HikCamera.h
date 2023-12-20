/**
 * @file HikCamera.h
 * @author Lorenzo Feng (lorenzo.feng@njust.edu.cn)
 * @brief 海康威视SDK 二次封装
 * @version 0.1
 * @date 2023-12-15
 * 
 * @copyright Copyright (c) Alliance, Nan Jing University of Science & Technology
 * 
 */
#pragma once

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <opencv2/core/base.hpp>

#include "CameraParams.h"
#include "HikCamera/HikCameraInterface.h"
#include "MvCameraControl.h"
#include "MvErrorDefine.h"

class HikCamera : public HikCameraInterface{
   public:
	explicit HikCamera() = default;
	~HikCamera() override {
		if (!isConnected) {
			return;
		}
		nRet = MV_CC_StopGrabbing(handle);
		if (nRet != MV_OK) {
			std::cout << "Stop Grabbing failed" << std::endl;
		}
		nRet = MV_CC_DestroyHandle(handle);
		if (nRet != MV_OK) {
			std::cout << "Destroy Handle failed" << std::endl;
		}
		if (handle != NULL) {
			handle = NULL;
		}
		if (pData) {
			free(pData);
			pData = NULL;
		}
	}

	bool isUSEDevice(MV_CC_DEVICE_INFO *pstMVCCDI) override;
	bool connectDeivce() override;
	cv::Mat fetchFrame() override;

   private:
	int nRet = MV_OK;
	void *handle = NULL;
	bool isConnected = false;
	MVCC_INTVALUE stParam;
	unsigned char *pData = NULL;
	unsigned char *pDataForRGB = NULL;
};