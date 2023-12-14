#pragma once

#include <opencv2/core/mat.hpp>

#include "ArmorDescriptor/ArmorDescriptor.h"

class PnPSolver {
   public:
	PnPSolver() = default;
	~PnPSolver() = default;

	void solve(ArmorDescriptor armor, cv::Mat &rot, cv::Mat &t);

   private:
};