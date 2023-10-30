//
// Created by iruma on 23-10-30.
//
#include <opencv2/opencv.hpp>
#ifndef TEST_NUMBERIDENTIFYINTERFACE_H
#define TEST_NUMBERIDENTIFYINTERFACE_H

class NumberIdentifyInterface
{
public:
    virtual ~NumberIdentifyInterface()=default;
    virtual std::tuple<int,double> Identify(cv::Mat &img)=0;
};

#endif //TEST_NUMBERIDENTIFYINTERFACE_H
