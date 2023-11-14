#pragma once

#include "ArmorDescriptor/ArmorDescriptor.h"
#include <opencv2/core/mat.hpp>

class PnPSolver{

public:
    PnPSolver()=default;
    ~PnPSolver()=default;

	void solve(ArmorDescriptor armor, cv::Mat &rot, cv::Mat &t);

private:
};