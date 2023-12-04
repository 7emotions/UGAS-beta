#include  "Serial/Serial.h"
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <ios>
#include <opencv2/core/hal/interface.h>
#include <unistd.h>


void SerialUtil::send(){
	uint8_t pkg[pkg_size]={0};

	auto ptr = (uint8_t*)&package;

	pkg[0]=0xff;

	for(size_t i=0;i<pkg_size-1;i++){
		pkg[i+1]=*(ptr+i);
	}

	Append_CRC8_Check_Sum(pkg,pkg_size);

	auto count = write(pkg, pkg_size);
	if(count != pkg_size){
		std::cout << "send error" << std::endl;
	}
}

