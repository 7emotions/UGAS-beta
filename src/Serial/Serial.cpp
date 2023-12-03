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

	for (size_t i = 0; i < pkg_size; i++)
	{
		std::cout << (uchar)*(pkg+i) << std::endl;
	}	

	std::cout << "Serial Write Count" << count << std::endl;
}

