#include "HikCamera/HikCamera.h"

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <opencv2/core/base.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>

#include "CameraParams.h"
#include "MvCameraControl.h"
#include "MvErrorDefine.h"

bool HikCamera::isUSEDevice(MV_CC_DEVICE_INFO *pstMVCCDI) {
	if (pstMVCCDI == NULL) {
		return false;
	}
	if (pstMVCCDI->nTLayerType == MV_USB_DEVICE) {
		return true;
	}
	return false;
}

bool HikCamera::connectDeivce()  {
	MV_CC_DEVICE_INFO_LIST stDeviceList;
	memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

	nRet = MV_CC_EnumDevices(MV_USB_DEVICE, &stDeviceList);
	if (nRet != MV_OK) {
		std::cout << "EnumDevices fail! nRet [0x" << std::hex << nRet << "]"
				  << std::endl;
		return false;
	}
	if (stDeviceList.nDeviceNum != 1 ||
		!this->isUSEDevice(stDeviceList.pDeviceInfo[0])) {
		std::cout << "No USB device connected or multiple devices connected!"
				  << std::endl;
		return false;
	}

	nRet = MV_CC_CreateHandle(&handle, stDeviceList.pDeviceInfo[0]);
	if (nRet != MV_OK) {
		std::cout << "Create Handle fail! nRet [0x" << std::hex << nRet << "]"
				  << std::endl;
		return false;
	}
	nRet = MV_CC_OpenDevice(handle);
	if (nRet != MV_OK) {
		std::cout << "Open Device fail! nRet [0x" << std::hex << nRet << "]"
				  << std::endl;
		return false;
	}

	nRet = MV_CC_SetEnumValue(handle, "TriggerMode", 0);
	if (nRet != MV_OK) {
		std::cout << "Set TriggerMode fail! nRet [0x" << std::hex << nRet << "]"
				  << std::endl;
		return false;
	}

	memset(&stParam, 0, sizeof(MVCC_INTVALUE));
	nRet = MV_CC_GetIntValue(handle, "PayloadSize", &stParam);
	if (nRet != MV_OK) {
		std::cout << "Get PayloadSize fail! nRet [0x" << std::hex << nRet << "]"
				  << std::endl;
		return false;
	}

	nRet = MV_CC_StartGrabbing(handle);
	if (nRet != MV_OK) {
		std::cout << "Start Grabbing fail! nRet [0x" << std::hex << nRet << "]"
				  << std::endl;
		return false;
	}

	pData = (unsigned char *)malloc(stParam.nCurValue * sizeof(unsigned char));
	if (pData == NULL) {
		return false;
	}
	isConnected = true;
	return true;
}

cv::Mat HikCamera::fetchFrame() {
	cv::Mat img;
	MV_FRAME_OUT_INFO_EX stImageInfo;
	memset(&stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));

	unsigned int nDataSize = stParam.nCurValue;
	nRet =
		MV_CC_GetOneFrameTimeout(handle, pData, nDataSize, &stImageInfo, 1000);
	try {
		if (nRet == MV_OK) {
#ifdef LOG_ON
			std::cout << "Get One Frame: Width[ " << stImageInfo.nWidth
					  << "], Height[ " << stImageInfo.nHeight
					  << "], nFrameNum[ " << stImageInfo.nFrameNum << "]"
					  << std::endl;
#endif

			if (pDataForRGB) {
				delete[] pDataForRGB;
				pDataForRGB = NULL;
			}
			// pDataForRGB = (unsigned char*)malloc(stImageInfo.nWidth *
			// stImageInfo.nHeight * 4 + 2048);
			pDataForRGB =
				new unsigned char[stImageInfo.nWidth * stImageInfo.nHeight * 4 +
								  2048];
			if (NULL == pDataForRGB) {
				throw "Null pDataForRGB";
			}
			MV_CC_PIXEL_CONVERT_PARAM stConvertParam;
			memset(&stConvertParam, 0, sizeof(MV_CC_PIXEL_CONVERT_PARAM));
			stConvertParam.nWidth = stImageInfo.nWidth;
			stConvertParam.nHeight = stImageInfo.nHeight;
			stConvertParam.pSrcData = pData;
			stConvertParam.nSrcDataLen = stImageInfo.nFrameLen;
			stConvertParam.enSrcPixelType = stImageInfo.enPixelType;
			stConvertParam.enDstPixelType = PixelType_Gvsp_RGB8_Packed;
			stConvertParam.pDstBuffer = pDataForRGB;
			stConvertParam.nDstBufferSize =
				stImageInfo.nWidth * stImageInfo.nHeight * 4 + 2048;
			nRet = MV_CC_ConvertPixelType(handle, &stConvertParam);
			if (MV_OK != nRet) {
				throw "ConvertPixelType fail";
			}
			//
			img = cv::Mat(stImageInfo.nHeight, stImageInfo.nWidth, CV_8UC3,
						  pDataForRGB);
			cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
		} else {
			std::cerr << "Get One Frame fail! nRet [0x" << std::hex << nRet
					  << "]" << std::endl;
		}
	} catch (const char *err) {
		std::cout << err << std::endl;
		return cv::Mat();
	}

	return img;
}
