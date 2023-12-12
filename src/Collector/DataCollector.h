#pragma once
#ifndef UGAS_BETA_DATACOLLECTOR_H
#define UGAS_BETA_DATACOLLECTOR_H

#include "Detector/Detector.h"
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>

class DataCollector:public Detector{
public:
    void UISave(cv::Mat img);
    void collect(cv::Mat img,COLOR_TAG color_tag);
    cv::Mat blobImage(cv::Mat &img);
};
#endif //UGAS_BETA_DATACOLLECTOR_H
