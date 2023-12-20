/**
 * @file Subscriber.h
 * @author Lorenzo Feng (lorenzo.feng@njust.edu.cn)
 * @brief ros2 Topic 订阅器
 * @version 0.1
 * @date 2023-12-20
 *
 * @copyright Copyright (c) Alliance, Nan Jing University of Science & Technology
 *
 */
#pragma once

#include <rclcpp/node.hpp>
#include <std_msgs/msg/detail/string__struct.hpp>

/**
 * @brief 消息类型
 * 
 * @tparam MsgType 
 */
template <typename MsgType>
class Subscriber : public rclcpp::Node {
   public:
	Subscriber(const std::string topicName)
		: rclcpp::Node("Subscriber"), topicName_(topicName) {
		subscription_ = this->create_subscription<MsgType>(
			topicName, 10,
			std::bind(&Subscriber::topicCallback, this, std::placeholders::_1));
	}

   private:
	void topicCallback(const typename MsgType::SharedPtr msg) const {
        
    }

	const std::string topicName_;

	typename rclcpp::Subscription<MsgType>::SharedPtr subscription_;
};