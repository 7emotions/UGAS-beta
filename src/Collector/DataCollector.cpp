#include "Collector/DataCollector.h"

void DataCollector::collect(cv::Mat img,COLOR_TAG color_tag) {
    cv::Point2i origin(img.cols / 2, img.rows / 2);

    std::vector<LightDescriptor> lights;


    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    std::vector<cv::Point2f> centers;

    cv::Mat pre = preprocess(img, color_tag);

    cv::findContours(pre, contours, hierarchy, cv::RETR_TREE,
                     cv::CHAIN_APPROX_NONE, cv::Point());

    if (!contours.empty()) {
        std::vector<cv::RotatedRect> boundRect(contours.size());
        for (size_t i = 0; i < contours.size(); i++) {
            auto area = contourArea(contours[i]);
            if (area < minArea || contours[i].size() <= minContoursSize) {
                continue;
            }

            auto rec = fitEllipse(contours[i]);

            if (rec.size.width / rec.size.height > lightLengthRatio) {
                continue;
            }

            rec.size.height *= 1.2;
            rec.size.width *= 1.2;

            lights.push_back(rec);
        }

        for (size_t i = 0; i < lights.size(); i++) {
            for (size_t j = i + 1; j < lights.size(); j++) {
                bool loopFlag = true;
                auto center = (lights[i].center + lights[j].center) / 2;
                for (size_t i = 0; i < centers.size(); i++) {
                    if (EuDis(center, centers[i]) < 10) {
                        loopFlag = false;
                        break;
                    }
                }
                if (!loopFlag) {
                    continue;
                }
                if (asin(abs(crossProduct(lights[i].v, lights[j].v)) > minAngleDiff)) {
                    continue;
                }

                auto ml = (lights[i].length + lights[j].length) / 2;

                auto rpd = abs(lights[i].length - lights[j].length) / std::max(lights[i].length, lights[j].length);

                if (rpd > 0.8) {
                    continue;
                }

                auto rdfm = abs(lights[i].length - lights[j].length) / ml;

                if (rdfm > 0.5) {
                    continue;
                }

                auto distance = EuDis(lights[i].center, lights[j].center);
                auto rdsm = distance / ml;

                if (rdsm > 3.5 || rdsm < 0.5) {
                    continue;
                }

                std::vector<cv::Point2f> points;
                lights[i].extend(points, ml,
                                 lights[i].center.x < lights[j].center.x ? LightDescriptor::LEFT
                                                                         : LightDescriptor::RIGHT);
                lights[j].extend(points, ml,
                                 lights[i].center.x < lights[j].center.x ? LightDescriptor::RIGHT
                                                                         : LightDescriptor::LEFT);

                sortPts(points);
                cv::Mat roi;
                perspective(img, points, roi);
                roi=blobImage(roi);
                cv::imshow("roi",roi);
                cv::waitKey(0);
                UISave(roi);
            }
        }
    }
}

cv::Mat DataCollector::blobImage(cv::Mat &img)
{
    cv::resize(img, img, cv::Size(36, 36));
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(img, img, cv::Size(5,5), 0,0);
    cv::threshold(img, img, 0, 255, cv::THRESH_BINARY|cv::THRESH_OTSU);
    // cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 1));
    // cv::GaussianBlur(img, img, cv::Size(3, 3), 0);
    //cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
    return img;
}

void DataCollector::UISave(cv::Mat img) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(nullptr, "Save Image", "Do you want to save roi image?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // 弹出文件夹选择对话框
        QString folderPath = QFileDialog::getExistingDirectory(nullptr, "Select Folder");

        if (!folderPath.isEmpty()) {
            // 遍历文件夹中的子文件数量
            QDir folderDir(folderPath);
            int numberOfFiles = folderDir.entryList(QDir::Files).count();
            int numberOfCopies=10; //存1张=10张
            for(int i=1;i<=numberOfCopies;i++) {
                // 构建保存的文件名
                QString fileName = QString::number(numberOfFiles + i) + ".png";

                // 构建保存的完整路径
                QString filePath = folderPath + "/" + fileName;

                // 保存图片
                cv::imwrite(filePath.toStdString(), img);
            }
        }
    }
}
