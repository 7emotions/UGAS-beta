
/**
 * @file Trajectory.h
 * @author Lorenzo Feng (lorenzo.feng@njust.edu.cn)
 * @brief 弹道解算器，解算机制参考doc/TrajectorySolution.md
 * @version 0.1
 * @date 2023-12-15
 *
 * @copyright Copyright (c) Alliance, Nan Jing University of Science &
 * Technology
 *
 */

#pragma once

#include <opencv2/core/types.hpp>

class Trajectory {
   public:
	/**
	 * @brief Construct a new Trajectory object
	 *
	 * @param offset 云台原点到枪口的偏差
	 * @param posture 在云台参考系下,光心参考系的坐标
	 */
	Trajectory(float offset, cv::Point3d position)
		: _cameraOffset(position), _gunpointOffset(offset) {}

	/**
	 * @brief 弹道解算，用于测试
	 *
	 * @param pos 目标
	 * @param v 初速度
	 * @param count 解算迭代次数
	 * @param err 解算误差
	 * @return cv::Point3d 补偿点
	 */
	cv::Point3d solve(cv::Point3d pos, double v, size_t &count, double &err);

	/**
	 * @brief 弹道解算
	 * 
	 * @param pos 目标坐标
	 * @param v 子弹初速度
	 * @param pitch_ 初始pitch角，用于回归绝对参考系
	 * @return cv::Point3d 补偿目标点
	 */
	cv::Point3d solve(cv::Point3d pos, double v, double pitch_);

   protected:
	/**
	 * @brief 抛体运动计算
	 *
	 * @param error 误差
	 * @param pos 目标
	 * @param v 初速度
	 * @param pitch 抛射角
	 */
	void errorCalculate(double &error, cv::Point2d pos, double v, double pitch);

   private:
	/**
	 * @brief 投影变换
	 * 
	 * @param org 
	 * @param dst 
	 */
	void projectTransform(const cv::Point3d &org, cv::Point2d &dst);

	void rotateTransform(const cv::Point2d &org, cv::Point2d &dst, double pitch);

	cv::Point3d _cameraOffset;
	float _gunpointOffset;
	
};