#include  "Serial/Serial.h"
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <ios>
#include <opencv2/core/hal/interface.h>
#include <unistd.h>


void SerialUtil::send(){
	clear();
	
	*pkg = pkg_head;
	auto ptr = (uint8_t*)&package;
	
	for(size_t i=1;i<=pkg_size - 2;i++){
		*(pkg+i)=*(ptr+i-1);
	}

	*(pkg+pkg_size - 1) = crc();
	

	auto count = write(serial, pkg, pkg_size);

	std::cout << "CRC: " << (uint8_t)*(pkg+pkg_size-1) << std::endl;

	std::cout << "Serial Write Count" << count << std::endl;
}

