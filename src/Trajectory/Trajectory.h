/**
 * @file Trajectory.h
 * @author Lorenzo Feng (lorenzo.feng@njust.edu.cn)
 * @brief 弹道解算器
 * @version 0.1
 * @date 2023-12-15
 *
 * @copyright 南京理工大学 Alliance
 *
 */

#pragma once

#include <opencv2/core/types.hpp>

class Trajectory {
   public:
	/**
	 * @brief Construct a new Trajectory object
	 * 
	 * @param offset 光轴与
	 */
	Trajectory(cv::Point3d offset) : _offset(offset) {}

	cv::Point3d solve(cv::Point3d pos, double v);

   private:
	cv::Point3d _offset;
};