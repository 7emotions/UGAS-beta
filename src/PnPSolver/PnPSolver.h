#pragma once

#include <opencv2/core/mat.hpp>

#include "ArmorDescriptor/ArmorDescriptor.h"

class PnPSolver {
   public:
	PnPSolver() = default;
	~PnPSolver() = default;

	/**
	 * @brief PnP解算，根据armor的pts（2D）得到旋转矩阵rot与平移矩阵t
	 * 
	 * @param armor 
	 * @param rot 
	 * @param t 
	 */
	void solve(ArmorDescriptor armor, cv::Mat &rot, cv::Mat &t);

   private:
};