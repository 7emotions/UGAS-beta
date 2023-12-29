#include "PnPSolver/PnPSolver.h"

#include <opencv2/core/hal/interface.h>

#include <opencv2/calib3d.hpp>
#include <opencv2/core/types.hpp>
#include <vector>

#include "ArmorDescriptor/ArmorDescriptor.h"
#include "Parameters/Parameters.h"


void PnPSolver::solve(ArmorDescriptor armor, cv::Mat &rot, cv::Mat &trans) {
	std::vector<cv::Point3d> object =
		armor.getArmorType() == ArmorDescriptor::ArmorType::LargeArmor
			? param::LargeArmorObjectPoints
			: param::NormalArmorObjectPoints;

	cv::solvePnP(object, armor.getPoints(), param::CameraMatrix6,
				 param::CameraDistCoeffs6, rot, trans, false,
				 cv::SOLVEPNP_IPPE);
	
	// mm -> m
	trans /= 1000;
}