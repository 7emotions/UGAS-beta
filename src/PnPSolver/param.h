#pragma once

#include <opencv2/opencv.hpp>

namespace param{
    constexpr int NormalArmorWidth = 135, NormalArmorHeight = 56, LargerArmorWidth = 230, LargerArmorHeight = 56;
    const double CameraMatrixData6[3][3] = {
        {1.722231837421459e+03, 0, 7.013056440882832e+02},
        {0, 1.724876404292754e+03, 5.645821718351237e+02},
        {0, 0, 1}
    };
    const double CameraDistCoeffsData6[5] = {-0.064232403853946, -0.087667493884102, 0, 0, 0.792381808294582};
    const std::vector<cv::Point3d> NormalArmorObjectPoints = {
            cv::Point3d(-0.5 * NormalArmorWidth,  0.5 * NormalArmorHeight, 0.0f),
            cv::Point3d(-0.5 * NormalArmorWidth, -0.5 * NormalArmorHeight, 0.0f),
            cv::Point3d( 0.5 * NormalArmorWidth, -0.5 * NormalArmorHeight, 0.0f),
            cv::Point3d( 0.5 * NormalArmorWidth,  0.5 * NormalArmorHeight, 0.0f)
    };
    const std::vector<cv::Point3d> LargeArmorObjectPoints = {
            cv::Point3d(-0.5 * LargerArmorWidth,  0.5 * LargerArmorHeight, 0.0f),
            cv::Point3d(-0.5 * LargerArmorWidth, -0.5 * LargerArmorHeight, 0.0f),
            cv::Point3d( 0.5 * LargerArmorWidth, -0.5 * LargerArmorHeight, 0.0f),
            cv::Point3d( 0.5 * LargerArmorWidth,  0.5 * LargerArmorHeight, 0.0f)
    };
 };
