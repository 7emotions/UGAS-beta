#include "Detector/Detector.h"
#include "HikCamera/HikCamera.h"

#include <cstddef>
#include <iostream>
#include <opencv2/core/base.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <string>
#include <vector>

int main(){

    Detector detector;
    HikCamera hikCamera;

    if (!hikCamera.connectDeivce()) {
        std::cerr << "Failed to connect to camera" << std::endl;
        return 1;
    }

    std::cout << "Connected to camera" << std::endl;
    

    cv::Mat img;
    do {
        img = hikCamera.fetchFrame();
        if (img.empty()) {
            break;
        }
        img = detector.DetectLights(img, Detector::COLOR_TAG::BLUE);

        cv::imshow("Detected Lights", img);
        if (cv::waitKey(10)==27) {
            break;
        }
    }while (true);
    return 0;
}