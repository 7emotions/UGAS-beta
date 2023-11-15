#include "PnPSolver/PnPSolver.h"
#include "ArmorDescriptor/ArmorDescriptor.h"
#include "PnPSolver/Parameters.h"
#include <opencv2/calib3d.hpp>
#include <opencv2/core/hal/interface.h>
#include <opencv2/core/types.hpp>
#include <vector>

namespace param {
	double CameraMatrixData6[3][3] = {
        {1.722231837421459e+03, 0, 7.013056440882832e+02},
        {0, 1.724876404292754e+03, 5.645821718351237e+02},
        {0, 0, 1}
    };
    double CameraDistCoeffsData6[5] = {-0.064232403853946, -0.087667493884102, 0, 0, 0.792381808294582};
	const cv::Mat CameraMatrix6(3, 3, CV_64FC1, CameraMatrixData6);
	const cv::Mat CameraDistCoeffs6(1, 5, CV_64FC1, CameraDistCoeffsData6);
}

void PnPSolver::solve(ArmorDescriptor armor, cv::Mat &rot,cv::Mat &trans){
	std::vector<cv::Point3d> object = armor.getArmorType() == ArmorDescriptor::ArmorType::LargeArmor ?
	 param::LargeArmorObjectPoints:param::NormalArmorObjectPoints;
	
	cv::solvePnP(object, armor.getPoints(), param::CameraMatrix6, param::CameraDistCoeffs6, rot, trans,false,cv::SOLVEPNP_IPPE);

}