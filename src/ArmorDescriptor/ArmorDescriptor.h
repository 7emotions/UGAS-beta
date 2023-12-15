/**
 * @file ArmorDescriptor.h
 * @author Lorenzo Feng (lorenzo.feng@njust.edu.cn)
 * @brief 装甲板描述类
 * @version 0.1
 * @date 2023-12-15
 * 
 * @copyright 南京理工大学 Alliance
 * 
 */
#pragma once

#include <opencv2/core/types.hpp>
#include <vector>

class ArmorDescriptor {
   public:
	ArmorDescriptor(std::vector<cv::Point2f> _pts, int _code)
		: pts(_pts), code(_code) {
		if (_code == 1 || (_code < 10 && _code > 6)) {
			armorType = LargeArmor;
		} else {
			armorType = NormalArmor;
		}

		center = (pts[0] + pts[1] + pts[2] + pts[3]) / 4;
	}

	enum ArmorType { NormalArmor, LargeArmor };

	ArmorType getArmorType() { return armorType; }
	std::vector<cv::Point2f> getPoints() const { return pts; }
	cv::Point3d get3DPoint() const {return _3dpts;}
	cv::Point2f getCenter() { return center; }
	int getCode() { return code; }

	void set3DPoint(cv::Point3d _3dpts) { this->_3dpts = _3dpts; }

   private:
	std::vector<cv::Point2f> pts;
	cv::Point3d _3dpts;
	ArmorType armorType;
	cv::Point2f center;
	int code;
};