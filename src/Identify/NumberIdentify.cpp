#include "NumberIdentify.h"

NumberIdentify::NumberIdentify(std::string modelPath)
{
    _net=cv::dnn::readNetFromONNX(modelPath);
    if (_net.empty())
        throw std::runtime_error("Cannot open model file!");
}

std::tuple<int,double> NumberIdentify::Identify(cv::Mat &img)
{
    cv::Mat inputImage = _BlobImage(img);
	cv::Mat blobImage = cv::dnn::blobFromImage(inputImage, 1.0, cv::Size(36, 36), false, false);
    _net.setInput(blobImage);
	cv::Mat pred = _net.forward();
	cv::normalize(pred, pred, 1, 10, cv::NORM_MINMAX);
	_softmax(pred);
	double maxValue;
	cv::Point maxLoc;
	minMaxLoc(pred, NULL, &maxValue, NULL, &maxLoc);
    std::tuple<int,double> resultMsg(maxLoc.x,maxValue);
    return resultMsg;
}

cv::Mat NumberIdentify::_BlobImage(cv::Mat &img)
{
    cv::resize(img, img, cv::Size(36, 36));
	cv::threshold(img, img, 127, 255, cv::THRESH_BINARY);
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 1));
	cv::GaussianBlur(img, img, cv::Size(3, 3), 0);
	cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
	return img;
}

void NumberIdentify::_softmax(cv::Mat &modelInferResult)
{
	cv::Mat expMat;
	cv::exp(modelInferResult, expMat);

	double sumExp = cv::sum(expMat)[0];

	modelInferResult = expMat / sumExp;
}