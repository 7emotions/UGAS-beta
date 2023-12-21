/**
 * @file Subscriber.h
 * @author Lorenzo Feng (lorenzo.feng@njust.edu.cn)
 * @brief ros2 Topic 订阅器
 * @version 0.1
 * @date 2023-12-20
 *
 * @copyright Copyright (c) Alliance, Nan Jing University of Science &
 * Technology
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
class RoSSubscriber : public rclcpp::Node {
   public:
	RoSSubscriber(const std::string topicName, rclcpp::QoS qos = 10)
		: rclcpp::Node("Subscriber"), topicName_(topicName) {
		subscription_ = this->create_subscription<MsgType>(
			topicName, qos,
			std::bind(&RoSSubscriber::topicCallback, this,
					  std::placeholders::_1));
	}

   protected:
	virtual void topicCallback(const typename MsgType::SharedPtr msg) = 0;

	const std::string topicName_;

	typename rclcpp::Subscription<MsgType>::SharedPtr subscription_;
};