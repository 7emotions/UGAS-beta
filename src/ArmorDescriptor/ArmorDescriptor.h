#pragma once

#include <opencv2/core/types.hpp>
#include <vector>

class ArmorDescriptor{
public:
	ArmorDescriptor(std::vector<cv::Point2f> _pts, int code):pts(_pts){
		if (code == 1 || (code < 10 && code > 6)) {
			armorType = LargeArmor;
		}else {
			armorType = NormalArmor;
		}
	}

	enum ArmorType{
		NormalArmor,
		LargeArmor
	};

	ArmorType getArmorType() {
		return armorType;
	}

private:
	std::vector<cv::Point2f> pts;
	ArmorType armorType;
};