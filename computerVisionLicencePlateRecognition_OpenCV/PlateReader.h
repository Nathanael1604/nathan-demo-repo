// Author: Nathanael Park
// See main.cpp for version description

#ifndef PLATEREADER_H
#define PLATEREADER_H

#include "opencv/cv.h"

#include <string>

struct PlateResult 
{
    std::string plate;
    float confidence;
};

struct PosChar
{
    int xPos;
    char letter;
    float confidence;
};

class PlateReader
{
    cv::Mat character, resizedCharacter, compValue;
    cv::Mat letterLibrary;
    cv::Mat letter[36];
   
    cv::Mat plateThresh;
    
    int characterPosition[6];
    
    static char letterMap[];
    
    static cv::Size charSize;
    
    PosChar matchCharacter(cv::Mat& image);
    
    void autoCrop(cv::Mat& image);
    
    void calculateCharacterPositions(cv::Rect* characters, int numCharacters);
    
public:  
    
    PlateReader(std::string filename);
    
    std::string readPlate(cv::Mat& image, cv::Rect* characters, int numCharacters);
    
    PlateResult readPlate(cv::Mat& image);
    
    
};

#endif