#pragma once

#include <opencv2/core/types.hpp>
#include <vector>

class ArmorDescriptor{
public:
	ArmorDescriptor(std::vector<cv::Point2f> _pts, int _code):pts(_pts),code(_code){
		if (_code == 1 || (_code < 10 && _code > 6)) {
			armorType = LargeArmor;
		}else {
			armorType = NormalArmor;
		}

		center = (pts[0] + pts[1] + pts[2] + pts[3])/4;
	}

	enum ArmorType{
		NormalArmor,
		LargeArmor
	};

	ArmorType getArmorType() {
		return armorType;
	}


	std::vector<cv::Point2f> getPoints() {
		return pts;
	}

	cv::Point2f getCenter() {
		return center;
	}

	int getCode() {
		return code;
	}

private:
	std::vector<cv::Point2f> pts;
	ArmorType armorType;
	cv::Point2f	center;
	int code;
};