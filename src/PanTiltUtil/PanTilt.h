#pragma once

#include "ArmorDescriptor/ArmorDescriptor.h"
#include "PnPSolver/PnPSolver.h"
#include "Serial/Serial.h"

class PanTiltUtil {
   public:
	PanTiltUtil() = default;
	~PanTiltUtil() = default;

	void aim(ArmorDescriptor armor);
	void aim() {
		sender.pack(0, 0, 0, 0, 0);
		sender.send();
	}

   private:
	SerialUtil sender;
	PnPSolver solver;
};