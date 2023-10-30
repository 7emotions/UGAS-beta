#include <opencv2/opencv.hpp>
#include <iostream>
#include "NumberIdentifyInterface.h"

class NumberIdentify:public NumberIdentifyInterface
{
    public:
        explicit NumberIdentify(std::string modelPath);
        std::tuple<int,double> Identify(cv::Mat &img);
    private:
        cv::dnn::Net _net;
        void _softmax(cv::Mat &modelInferResult);
        cv::Mat _BlobImage(cv::Mat& img);
};