#pragma once

#include <cstdint>
typedef struct PackageInfo{
	float yaw,pitch;
	uint16_t rect_x,rect_y;
	uint8_t color;
} Package;

class SerialUtil{
public:
	SerialUtil()=default;
	~SerialUtil()=default;

	void pack(float yaw,float pitch,uint16_t rect_x,uint16_t rect_y,uint8_t color){
		package.yaw=yaw;
		package.pitch=pitch;
		package.rect_x=rect_x;
		package.rect_y=rect_y;
		package.color=color;
	}
	
	void send();

private:
	Package package;
};