#pragma once

#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>

namespace param{
	//enum { BR, BL, TR, TL };
    constexpr int NormalArmorWidth = 135, NormalArmorHeight = 56, LargerArmorWidth = 230, LargerArmorHeight = 56;
    
    const std::vector<cv::Point3d> NormalArmorObjectPoints = {
            cv::Point3d( 0.5 * NormalArmorWidth, -0.5 * NormalArmorHeight, 0.0f),
            cv::Point3d(-0.5 * NormalArmorWidth, -0.5 * NormalArmorHeight, 0.0f),
            cv::Point3d( 0.5 * NormalArmorWidth,  0.5 * NormalArmorHeight, 0.0f),
            cv::Point3d(-0.5 * NormalArmorWidth,  0.5 * NormalArmorHeight, 0.0f)
    };
    const std::vector<cv::Point3d> LargeArmorObjectPoints = {
            cv::Point3d( 0.5 * LargerArmorWidth, -0.5 * LargerArmorHeight, 0.0f),
            cv::Point3d(-0.5 * LargerArmorWidth, -0.5 * LargerArmorHeight, 0.0f),
            cv::Point3d( 0.5 * LargerArmorWidth,  0.5 * LargerArmorHeight, 0.0f),
            cv::Point3d(-0.5 * LargerArmorWidth,  0.5 * LargerArmorHeight, 0.0f)
    };
 };
