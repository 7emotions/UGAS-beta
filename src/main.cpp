#include "Detector/Detector.h"
#include "HikCamera/HikCamera.h"
#include "Serial/Serial.h"

#include <cstddef>
#include <fstream>
#include <iostream>
#include <opencv2/core/base.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <string>
#include <unistd.h>
#include <vector>

int main(){
	SerialUtil sender;
	while (true) {
		sender.pack(30, 0, 0, 0, 0);
		sender.send();
		sleep(1);
	}
	return 0;
}

// int main(){

//     Detector detector;
//     HikCamera hikCamera;
// 	int tag;
// 	std::cout<<"Color to detect(0 for B, 1 for R):";
// 	std::cin >> tag;
// 	if (tag > 1) {
// 		std::cout << "Invalid input" << std::endl;
// 		return 1;
// 	}

//     if (!hikCamera.connectDeivce()) {
//         std::cerr << "Failed to connect to camera" << std::endl;
//         return 1;
//     }

//     std::cout << "Connected to camera" << std::endl;
    
// 	std::ofstream outfile("../resources/records.csv");

// 	if (!outfile) {
// 		std::cerr << "Failed to open file" << std::endl;
// 		return 1;
// 	}

//     cv::Mat img;
//     do {
//         img = hikCamera.fetchFrame();
//         if (img.empty()) {
//             break;
//         }
//         img = detector.DetectLights(img, (Detector::COLOR_TAG)tag,outfile);

//         //cv::imshow("Detected Lights", img);
//         if (cv::waitKey(10)==27) {
//             break;
//         }
//     }while (true);

// 	outfile.close();
//     return 0;
// }