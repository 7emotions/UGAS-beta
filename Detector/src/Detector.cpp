// Detector.cpp: 定义应用程序的入口点。
//

#include "Detector.h"

#define DEBUG

const int minArea = 5;
const int minContoursSize = 1;
const double lightLengthRatio = 4.0;
const double minAngleDiff = 10.0;
const double minLengthDiffRatio = 0.8;

using namespace cv;
using namespace std;

std::string dir =
R"(D:\RMWorks\images\red\processed)";

void test(cv::Mat img, Detector::COLOR_TAG color_tag) {
	img.resize(img.rows, img.cols);

	Detector myDetector;
	vector<LightDescriptor> lights;

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;


	cv::findContours(myDetector.preprocess(img, color_tag), contours,
		hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE,
		cv::Point());

	std::vector<cv::RotatedRect> boundRect(contours.size());
	cv::Point2f rect[4];

	for (int i = 0; i < contours.size(); i++) {
		double area = contourArea(contours[i]);
		if (area < minArea || contours[i].size() <= minContoursSize)
		{
			continue;
		}

		RotatedRect rec = fitEllipse(contours[i]);

		if (rec.size.width / rec.size.height > lightLengthRatio)
		{
			continue;
		}

		rec.size.height *= 1.2;
		rec.size.width *= 1.2;

		lights.push_back(rec);
#pragma region pre
		//RotatedRect rec = fitEllipse(contours[i]);


		//boundRect[i] = cv::minAreaRect(cv::Mat(contours[i]));
		//boundRect[i].points(rect);

		//for (int j = 0; j < 4; j++) {
		//	cv::line(img, rect[j], rect[(j + 1) % 4],
		//		color_tag == Detector::COLOR_TAG::BLUE ?
		//		cv::Scalar(0, 0, 255) :
		//		cv::Scalar(255, 0, 0), 2, 8);
		//}
#pragma endregion
	}

	sort(lights.begin(), lights.end(), [&](const LightDescriptor& a, const LightDescriptor& b) {
		return a.angle == b.angle ? a.angle < b.angle : a.angle > b.angle;
		});

	for (int i = 0; i < lights.size(); i++)
	{
		int j = i + 1;
		for (; j < lights.size(); j++)
		{
			if (abs(lights[i].angle - lights[j].angle) > 10)
			{
				break;
			}
		}

	}
}

int main() {
	
	std::vector<std::string> files;

	cv::glob(dir, files);
	for (auto i = 0; i < files.size(); i++)
	{
#pragma region label
		//try
		//{
		//	tinyxml2::XMLDocument doc;
		//	doc.LoadFile(files[i].c_str());
		//	auto annotation = doc.RootElement();
		//	auto filename = annotation->FirstChildElement("filename");
		//	if (!filename)
		//	{
		//		break;
		//	}
		//	std::cout << filename->GetText() << std::endl;

		//	auto img = cv::imread(dir + "image\\" + filename->GetText());

		//	auto object = annotation->FirstChildElement("object");

		//	while (object)
		//	{
		//		auto color = object->FirstChildElement("armor_color");
		//		if (!color)
		//		{
		//			object = object->NextSiblingElement();
		//			continue;
		//		}
		//		auto bndbox = object->FirstChildElement("bndbox");
		//		auto xmin = atoi(bndbox->FirstChildElement("xmin")->GetText());
		//		auto xmax = atoi(bndbox->FirstChildElement("xmax")->GetText());
		//		auto ymin = atoi(bndbox->FirstChildElement("ymin")->GetText());
		//		auto ymax = atoi(bndbox->FirstChildElement("ymax")->GetText());
		//		
		//		test(img(cv::Range(ymin, ymax), cv::Range(xmin, xmax)),
		//			strcmp(color->GetText(), "blue") == 0 ?
		//			Detector::BLUE :
		//			Detector::RED);

		//		object = object->NextSiblingElement();
		//	}

		//	//test(img);
		//}
		//catch (const std::exception& e)
		//{
		//	std::cout << e.what() << std::endl;
		//}
#pragma endregion
		Mat img = imread(files[i]);

		test(img, Detector::BLUE);
		waitKey();
	}

	cv::destroyAllWindows();


#if false

	bool hash[90] = { false };

	for (int i = 0; i < boundRect.size(); i++) {
		if (hash[(int)boundRect[i].angle]) {

		}
		else {
			hash[(int)boundRect[i].angle] = true;
	}
}


#endif
	return 0;
}
