/**
 * @file PitchSubscriber.h
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


#include <std_msgs/msg/float32.hpp>
#include "ROS2Util/Subscriber.h"

class PitchSubscriber : public RoSSubscriber<std_msgs::msg::Float32> {
   public:
	PitchSubscriber() : RoSSubscriber<std_msgs::msg::Float32>("Pitch") {
		std::cout << "Subscribed to Topic Pitch" << std::endl;
	}

   private:
	void topicCallback(const std_msgs::msg::Float32::SharedPtr msg) override {
		std::cout << "Msg : " << msg->data << std::endl;
	}
};
