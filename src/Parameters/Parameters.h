/**
 * @file Parameters.h
 * @author Lorenzo Feng (lorenzo.feng@njust.edu)
 * @brief
 * @version 0.1
 * @date 2023-12-15
 *
 * @copyright Copyright (c) Alliance, Nan Jing University of Science & Technology
 *
 */
#pragma once

#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/opencv.hpp>

#include "RoboticDescriptor/RoboticDescriptor.h"

namespace param {

namespace DetectorStaticDate {

// 灯条检测
// 灯条面积阈值
const int minArea = 5;
// 轮廓点群数量阈值
const int minContoursSize = 1;
// 灯条长度比例阈值
const double lightLengthRatio = 4.0;

// 平行检测
// 平行角差阈值
const double minAngleDiff = 15.0;
// 极差比阈值
const double minPeakDiffRatio = 0.8;
// 均差比阈值
const double minMeanDiffRatio = 0.5;
// 最小距离均差比 （区间分段）
const double minDisMeanRatio = 3.5;
// 最大距离均差比 （区间分段）
const double maxDisMeanRatio = 0.5;
}  // namespace DetectorStaticDate

/**
 * @brief 装甲板尺寸数据
 * 
 */
constexpr int NormalArmorWidth = 135, NormalArmorHeight = 56,
			  LargerArmorWidth = 230, LargerArmorHeight = 56;

/**
 * @brief PnP解算中图像参考系的点群（以小装甲板中心为原点）
 *
 */
const std::vector<cv::Point3d> NormalArmorObjectPoints = {
	cv::Point3d(0.5 * NormalArmorWidth, -0.5 * NormalArmorHeight, 0.0f),
	cv::Point3d(-0.5 * NormalArmorWidth, -0.5 * NormalArmorHeight, 0.0f),
	cv::Point3d(0.5 * NormalArmorWidth, 0.5 * NormalArmorHeight, 0.0f),
	cv::Point3d(-0.5 * NormalArmorWidth, 0.5 * NormalArmorHeight, 0.0f)};
/**
 * @brief PnP解算中图像参考系的点群（以大装甲板中心为原点）
 *
 */
const std::vector<cv::Point3d> LargeArmorObjectPoints = {
	cv::Point3d(0.5 * LargerArmorWidth, -0.5 * LargerArmorHeight, 0.0f),
	cv::Point3d(-0.5 * LargerArmorWidth, -0.5 * LargerArmorHeight, 0.0f),
	cv::Point3d(0.5 * LargerArmorWidth, 0.5 * LargerArmorHeight, 0.0f),
	cv::Point3d(-0.5 * LargerArmorWidth, 0.5 * LargerArmorHeight, 0.0f)};

/**
 * @brief 相机内参参数
 *
 */
inline double CameraMatrixData6[3][3] = {
	{1.722231837421459e+03, 0, 7.013056440882832e+02},
	{0, 1.724876404292754e+03, 5.645821718351237e+02},
	{0, 0, 1}};

/**
 * @brief 畸变矫正参数
 *
 */
inline double CameraDistCoeffsData6[5] = {
	-0.064232403853946, -0.087667493884102, 0, 0, 0.792381808294582};

/**
 * @brief 相机内参矩阵
 *
 */
const cv::Mat CameraMatrix6(3, 3, CV_64FC1, CameraMatrixData6);

/**
 * @brief 畸变矫正矩阵
 *
 */
const cv::Mat CameraDistCoeffs6(1, 5, CV_64FC1, CameraDistCoeffsData6);

/**
 * @brief 弹道解算静态数据
 *
 */
namespace TrajectoryStaticDate {
const float G = 9.8;
// ODWhell
const RoboticDescriptor ODWhellDescriptor(5, "全向轮", 0.1552,
										  cv::Point3d(0.1055, 0.043, 0));
}  // namespace TrajectoryStaticDate

};	// namespace param
