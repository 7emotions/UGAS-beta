// Detector.cpp: 定义应用程序的入口点。
//

#include "Detector.h"

#define DEBUG


using namespace cv;
using namespace std;

std::string dir =
R"(/home/alliance/gitrepo/UGAS-beta/Detector/resources/)";

int main() {

	std::vector<std::string> files;
	Detector detector;

	cv::glob(dir, files);
	for (auto i = 0; i < files.size(); i++)
	{
		std::cout<<files[i]<<std::endl;
		Mat img = imread(files[i]);	
		img = detector.fetchLights(img,Detector::COLOR_TAG::RED);
		imshow("image", img);
		waitKey(0);
	}

	cv::destroyAllWindows();

	return 0;
}
