#include <unistd.h>

#include <opencv2/core/base.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <rclcpp/rclcpp.hpp>

// 忽略数字神经网络致信度（神经网络模型需更新）
#define _IGNORE_CODE_CONFIDENCE

#define ROS2_DEBUG

#ifdef ROS2_DEBUG

#include "ROS2Util/PkgSubscriber.h"

int main(int argc, char **args) {
	rclcpp::init(argc, args);
	rclcpp::spin(std::make_shared<PkgSubscriber>());
	rclcpp::shutdown();
	return 0;
}

#endif

#ifdef TRAJECTORY_DEBUG

#include <fstream>
#include <random>

int main() {
	// 弹道静态参数
	auto trajectoryStaticDate = param::TrajectoryStaticDate::ODWhellDescriptor;
	// 弹道解算器
	Trajectory trajectoryCalculator(trajectoryStaticDate.getGunpointOffset(),
									trajectoryStaticDate.getCameraPosition());

	std::random_device e;
	std::uniform_real_distribution<double> u(0, 1);

	auto v = 30.0;

	std::ofstream fp;
	fp.open("log.csv");

	fp << "id,count,error" << std::endl;

	for (size_t i = 0; i < 1000; i++) {
		size_t count = 0;
		double error = 0.0;
		auto x = u(e) * 100;
		auto y = u(e) * 100;
		auto z = u(e) * 100;

		cv::Point3d target(x, y, z);
		std::cout << "Solving..." << std::endl;

		trajectoryCalculator.solve(target, v, count, error);

		std::cout << "Target:" << i << "\tdone." << std::endl;

		fp << i << "," << count << "," << error << std::endl;
	}

	return 0;
}

#endif

#ifdef RELEASE
#include <algorithm>
#include <iostream>
#include <vector>

#include "ArmorDescriptor/ArmorDescriptor.h"
#include "Detector/Detector.h"
#include "HikCamera/HikCamera.h"
#include "PanTiltUtil/PanTilt.h"
#include "Parameters/Parameters.h"
#include "Trajectory/Trajectory.h"

int main() {
	HikCamera camera;
	Detector detector;
	std::vector<ArmorDescriptor> armors;
	PanTiltUtil pan;
	auto trajectoryStaticDate = param::TrajectoryStaticDate::ODWhellDescriptor;
	Trajectory trajectoryCalculator(trajectoryStaticDate.getGunpointOffset(),
									trajectoryStaticDate.getCameraPosition());
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

		detector.DetectArmors(img, tag, armors);
		std::cout << "[Armors] Size:" << armors.size() << std::endl;

		if (armors.empty()) {
			pan.aim();
		}

		std::sort(armors.begin(), armors.end(),
				  [](const ArmorDescriptor &a, const ArmorDescriptor &b) {
					  auto d1 = EuDis(a.get3DPoint());
					  auto d2 = EuDis(b.get3DPoint());
					  return d1 < d2;
				  });

		auto v = 30.0;
		std::cout << "Aim to armor [" << armors[0].getCode() << "]"
				  << std::endl;
		pan.aim(trajectoryCalculator.solve(armors[0].get3DPoint(), v, 0));

		usleep(1000 * 10);
	}

	return 0;
}

#endif