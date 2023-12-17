/**
 * @file Trajectory.h
 * @author Lorenzo Feng (lorenzo.feng@njust.edu.cn)
 * @brief 弹道解算器
 * @version 0.1
 * @date 2023-12-15
 *
 * @copyright Alliance, Nan Jing University of Science & Technology
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
	Trajectory(float offset, cv::Point3d position) : _cameraOffset(position), _gunpointOffset(offset){}

	cv::Point3d solve(cv::Point3d pos, double v);

   private:
	void projectTransform(cv::Point3d &org, cv::Point2d &dst);

	cv::Point3d _cameraOffset;
	float _gunpointOffset;
};