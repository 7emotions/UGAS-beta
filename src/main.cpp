#include <unistd.h>

#include <iostream>
#include <opencv2/core/base.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <vector>

#include "ArmorDescriptor/ArmorDescriptor.h"
#include "Detector/Detector.h"
#include "HikCamera/HikCamera.h"
#include "PanTiltUtil/PanTilt.h"

int main() {
	HikCamera camera;
	Detector detector;
	std::vector<ArmorDescriptor> armors;
	PanTiltUtil pan;
	cv::Mat img;

	if (!camera.connectDeivce()) {
		std::cout << "Failed to connect camera." << std::endl;
		return 1;
	}

	auto tag = Detector::COLOR_TAG::RED;

	while (true) {
		armors.clear();
		img = camera.fetchFrame();
		if (img.empty()) {
			std::cout << "Empty image." << std::endl;
			return 1;
		}

		detector.DetectLights(img, tag, armors);
		std::cout << "[Armors] Size:" << armors.size() << std::endl;

		if (armors.empty()) {
			pan.aim();
		}

		for (auto armor : armors) {
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

// int main(){

//     Detector detector;
//     HikCamera hikCamera;
// 	int tag;
// 	std::cout<<"Color to detect(0 for B, 1 for R):";
// 	std::cin >> tag;
// 	if (tag > 1) {
// 		std::cout << "Invalid input" << std::endl;
// 		return 1;
// 	}

//     if (!hikCamera.connectDeivce()) {
//         std::cerr << "Failed to connect to camera" << std::endl;
//         return 1;
//     }

//     std::cout << "Connected to camera" << std::endl;

// 	std::ofstream outfile("../resources/records.csv");

// 	if (!outfile) {
// 		std::cerr << "Failed to open file" << std::endl;
// 		return 1;
// 	}

//     cv::Mat img;
//     do {
//         img = hikCamera.fetchFrame();
//         if (img.empty()) {
//             break;
//         }
//         img = detector.DetectLights(img, (Detector::COLOR_TAG)tag,outfile);

//         //cv::imshow("Detected Lights", img);
//         if (cv::waitKey(10)==27) {
//             break;
//         }
//     }while (true);

// 	outfile.close();
//     return 0;
// }