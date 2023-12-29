/**
 * @file PkgSubscriber.h
 * @author Lorenzo Feng (lorenzo.feng@njust.edu.cn)
 * @brief
 * @version 0.1
 * @date 2023-12-21
 *
 * @copyright Copyright (c) Alliance, Nan Jing University of Science &
 * Technology
 *
 */

#pragma once

#include <random>
#include <std_msgs/msg/detail/float32_multi_array__struct.hpp>
#include <std_msgs/msg/float32_multi_array.hpp>

#include "PanTiltUtil/PanTilt.h"
#include "Parameters/Parameters.h"
#include "ROS2Util/Subscriber.h"
#include "Trajectory/Trajectory.h"

class PkgSubscriber : public RoSSubscriber<std_msgs::msg::Float32MultiArray> {
   public:
	PkgSubscriber()
		: RoSSubscriber<std_msgs::msg::Float32MultiArray>("/Launch/pkg") {
		std::cout << "Subscribed to Topic Pkg" << std::endl;
	}

   private:
	void topicCallback(
		const std_msgs::msg::Float32MultiArray::SharedPtr msg) override {
		auto pitch = msg->data[0];
		auto speed = msg->data[1];

		// 弹道静态参数
		auto trajectoryStaticDate =
			param::TrajectoryStaticDate::ODWhellDescriptor;
		// 弹道解算器
		Trajectory trajectoryCalculator(
			trajectoryStaticDate.getGunpointOffset(),
			trajectoryStaticDate.getCameraPosition());
		// 云台控制器(解算航空轴转角)
		PanTiltUtil ptzControllor;

		std::random_device e;
		std::uniform_real_distribution<double> u(0, 1);

		// 计算弹道
		for (size_t i = 0; i < 1000; i++) {
			auto x = u(e) * 100;
			auto y = u(e) * 100;
			auto z = u(e) * 100;

			cv::Point3d target(x, y, z);
			std::cout << "Solving..." << std::endl;

			auto point =
				trajectoryCalculator.solve(target, speed, pitch);

			std::cout << "Target:" << i << "\tdone." << std::endl;\

			float dpitch, dyaw;
			ptzControllor.solve(point, dpitch, dyaw);
			
		}
	}
};
