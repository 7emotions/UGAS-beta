/**
 * @file RoboticDescriptor.h
 * @author Lorenzo Feng (lorenzo.feng@njust.edu.cn)
 * @brief
 * @version 0.1
 * @date 2023-12-17
 *
 * @copyright Copyright (c) Alliance, Nan Jing University of Science & Technology
 *
 */

#include <opencv2/core/types.hpp>

class RoboticDescriptor {
   public:
	RoboticDescriptor(const int id, const char* name,
					  const float gunpointOffset,
					  const cv::Point3d cameraPosition)
		: _id(id),
		  _name(name),
		  _gunpointOffset(gunpointOffset),
		  cameraPosition(cameraPosition) {}

	int getId() const { return _id; }
	const char* getName() const { return _name; }
	float getGunpointOffset() const { return _gunpointOffset; }
	cv::Point3d getCameraPosition() const { return cameraPosition; }

   private:
	const int _id;
	const char* _name;
	const float _gunpointOffset;
	const cv::Point3d cameraPosition;
};
