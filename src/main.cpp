#include "ArmorDescriptor/ArmorDescriptor.h"
#include "Detector/Detector.h"
#include "HikCamera/HikCamera.h"
#include "PanTiltUtil/PanTilt.h"
#include "Serial/Serial.h"
#include "Collector/DataCollector.h"

#include <cstddef>
#include <fstream>
#include <iostream>
#include <opencv2/core/base.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <string>
#include <unistd.h>
#include <vector>

int main(int argc,char *argv[]){
	HikCamera camera;
	DataCollector collector;
    Detector detector;
    std::vector<ArmorDescriptor> armors;
    QApplication app(argc,argv);
	cv::Mat img;
	if(!camera.connectDeivce()){
		std::cout << "Failed to connect camera." << std::endl;
		return 1;
	}

	auto tag = Detector::COLOR_TAG::BLUE;

	while (true) {
        img = camera.fetchFrame();
        cv::imshow("img",img);
        cv::waitKey(1);
        if (img.empty()) {
            std::cout << "Empty image." << std::endl;
            return 1;
        }
        collector.collect(img,tag);
        //detector.DetectLights(img,tag,armors);
    }
    return 0;
}

/*
int main(){
    HikCamera camera;
    Detector detector;
    std::vector<ArmorDescriptor> armors;
    PanTiltUtil pan;
    cv::Mat img;

    if(!camera.connectDeivce()){
        std::cout << "Failed to connect camera." << std::endl;
        return 1;
    }

    auto tag = Detector::COLOR_TAG::RED;


    while (true) {
        armors.clear();
        img = camera.fetchFrame();
        if (img.empty())
        {
            std::cout << "Empty image." << std::endl;
            return 1;
        }

        detector.DetectLights(img, tag,armors);
        std::cout << "[Armors] Size:" << armors.size() << std::endl;

        if (armors.empty())
        {
            pan.aim();
        }

        for(auto armor : armors){
            if (armor.getCode() != 10 && armor.getCode() != 4) {
                pan.aim();
                continue;
            }
            pan.aim(armor);
        }
        usleep(1000 * 10);
    }

    return 0;
}
*/