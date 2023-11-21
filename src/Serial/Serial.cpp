#include  "Serial/Serial.h"
#include <unistd.h>


void SerialUtil::send(){
		clear();
	
	*pkg = pkg_head;
	
	for(size_t i=1;i<=pkg_size;i++){
		*(pkg+i)=*(ptr+i-1);
	}

	*(pkg+pkg_size+1) = crc();
	

	write(serial, pkg, pkg_size);
}

