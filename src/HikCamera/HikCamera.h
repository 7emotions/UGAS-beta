#pragma once

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <opencv2/core/base.hpp>
#include <vector>

#include "CameraParams.h"
#include "MvCameraControl.h"
#include "MvErrorDefine.h"


class HikCamera{
public:
    HikCamera() = default;
    ~HikCamera(){
        nRet = MV_CC_StopGrabbing(handle);
        if (nRet != MV_OK) {
            std::cout << "Stop Grabbing failed" << std::endl;
        }
        nRet = MV_CC_DestroyHandle(handle);
        if (nRet!=MV_OK) {
            std::cout << "Destroy Handle failed" << std::endl;
        }
        if(handle!=NULL){
            handle = NULL;
        }
        if (pData) {
            free(pData);
            pData = NULL;
        }
        if (pDataForRGB) {
            free(pDataForRGB);
            pDataForRGB=NULL;
        }
    }

    bool isUSEDevice(MV_CC_DEVICE_INFO* pstMVCCDI);
    bool connectDeivce();
    cv::Mat fetchFrame();

private:
    int nRet = MV_OK;
    void *handle=NULL;
    MVCC_INTVALUE stParam;
    unsigned char *pData = NULL;
    unsigned char *pDataForRGB = NULL;
};