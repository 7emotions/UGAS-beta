#pragma once

#include <opencv2/core/mat.hpp>

class PnPSolver{

public:
    PnPSolver();
    ~PnPSolver();

	void solve(const std::vector<cv::Point3f>& points3D, const std::vector<cv::Point2f>& points2D);

private:
};