#pragma once

#include "ArmorDescriptor/ArmorDescriptor.h"
#include <opencv2/core/mat.hpp>

class PnPSolver{

public:
    PnPSolver();
    ~PnPSolver();

	void solve(const std::vector<cv::Point2f>& points2D, ArmorDescriptor armor);

private:
};