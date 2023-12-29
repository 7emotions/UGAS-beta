
/**
 * @file PnPSolver.h
 * @author Lorenzo Feng (lorenzo.feng@njust.edu.cn)
 * @brief PnP解算器，解算机制参考/doc/基于PnP解算器的空间目标位姿估算与追踪.pdf
 * @version 0.1
 * @date 2023-12-15
 *
 * @copyright Copyright (c) Alliance, Nan Jing University of Science & Technology
 *
 */
#pragma once

#include <opencv2/core/mat.hpp>

#include "ArmorDescriptor/ArmorDescriptor.h"

class PnPSolver {
   public:
	PnPSolver() = default;
	~PnPSolver() = default;

	/**
	 * @brief PnP解算，根据armor的pts（2D）得到旋转矩阵rot与平移矩阵t，单位：米（m）
	 *
	 * @param armor 装甲板对象
	 * @param rot 刚体旋转变换矩阵
	 * @param t 刚体平移变换矩阵
	 */
	void solve(ArmorDescriptor armor, cv::Mat &rot, cv::Mat &t);

   private:
};