// Author: Nathanael Park
// See main.cpp for version description

#ifndef PLATEDETECTOR_H
#define PLATEDETECTOR_H

#include "opencv/cv.h"
#include "PlateReader.h"

#include <string>

class PlateDetector
{
    cv::Mat thresh, grey;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> contourHierarchy;
    PlateReader plateReader;
    
public:
    PlateDetector(std::string filename);
    PlateResult detectPlate(cv::Mat image, cv::Mat* output = NULL);
    float plateScore(cv::Mat image);
    float calculateAverageSaturation(cv::Mat plateImage);
    
};

#endif