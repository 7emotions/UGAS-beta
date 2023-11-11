#include "PnPSolver/PnPSolver.h"
#include "ArmorDescriptor/ArmorDescriptor.h"
#include "PnPSolver/Parameters.h"
#include <opencv2/calib3d.hpp>
#include <opencv2/core/hal/interface.h>
#include <opencv2/core/types.hpp>
#include <vector>

void PnPSolver::solve(const std::vector<cv::Point2f>& points2D, ArmorDescriptor armor){
	std::vector<cv::Point3d> object = armor.getArmorType() == ArmorDescriptor::ArmorType::LargeArmor ?
	 param::LargeArmorObjectPoints:param::NormalArmorObjectPoints;
	cv::Mat rot;
	cv::Mat trans;
	auto martix = cv::Mat(3,3,CV_8UC1,(double *)param::CameraMatrixData6);
	auto offset = cv::Mat(5,1,CV_8UC1,(double *)param::CameraDistCoeffsData6);
	cv::solvePnP(object, points2D, martix, offset, rot, trans);


}