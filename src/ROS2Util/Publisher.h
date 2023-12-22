/**
 * @file Publisher.h
 * @author Lorenzo Feng (lorenzo.feng@njust.edu.cn)
 * @brief ros2 Topic 发布器
 * @version 0.1
 * @date 2023-12-20
 *
 * @copyright Copyright (c) Alliance, Nan Jing University of Science &
 * Technology
 *
 */

#include <rclcpp/node.hpp>

using namespace std::chrono_literals;

template <typename MsgType>
class RoSPublisher : public rclcpp::Node {
   public:
	RoSPublisher(const std::string topicName, rclcpp::QoS qos = 10)
		: rclcpp::Node("Publisher"), count_(0), topicName_(topicName) {
		publisher_ = this->create_publisher<MsgType>(topicName, qos);
		timer_ = this->create_wall_timer(
			500ms, std::bind(&RoSPublisher::timerCallback, this));
	}

   protected:
	virtual void timerCallback() {
		auto msg = MsgType();
		msg.data = "Hello World";
		RCLCPP_INFO(this->get_logger(), "Publisher: ", msg.data.c_str());
		publisher_->publish(msg);
	}

	const std::string topicName_;
	typename rclcpp::Publisher<MsgType>::SharedPtr publisher_;
	rclcpp::TimerBase::SharedPtr timer_;
	size_t count_;
};
