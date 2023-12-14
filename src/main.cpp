#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <opencv2/core/base.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
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
			std::cout << "Empty image." << std::endl;
			return 1;
		}

		detector.DetectLights(img, tag, armors);
		std::cout << "[Armors] Size:" << armors.size() << std::endl;

		if (armors.empty()) {
			pan.aim();
		}

		std::sort(armors.begin(), armors.end(), [](const ArmorDescriptor &a, const ArmorDescriptor &b){
			auto d1 = EuDis(a.get3DPoint());
			auto d2 = EuDis(b.get3DPoint());
			return d1 < d2;
		});

		pan.aim(armors[0]);

		usleep(1000 * 10);
	}

	return 0;
}
