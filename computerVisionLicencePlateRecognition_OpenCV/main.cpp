// Author: Nathanael Park
// main.cpp
// Initially started as minor project when first gaining exposure to the OpenCV library
// Have added bits and pieces over the years as a side hobby project

// >> Version Function <<
// Current version performs: threshold & contour extraction, plate detection, plate localization,
// character segmentation, and Optical Character Recognition (OCR)

// >> Version Results <<
// Pretty successful at threshold & contour extraction, plate detection, plate localization.
// Some ongoing issues with character segmentation and OCR.
// Struggles with O/0, 5/S, W/V, 7/Y, 6/b, 1/I etc...
// For example, successfully identifies car1.jpg (FSU681) with almost 90% confidence
// and successfully identifies car4.jpg (ERH960) with ~80% confidence,
// but struggles with car5.jpg (WW5543), identifying it as IVSS43
// or car7.jpg (RY2540), identifying it as R72S40...LOL
// Development Set-up: OpenCV library, OpenFrameworks, Visual Studios 2012 IDE, Windows 7
// Will also compile and run on Linux from command line, had it set up on Ubuntu 14.04.1 at one point.

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cstdio>
#include <vector>
#include <iostream>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "PlateDetector.h"
#include "PlateReader.h"


using namespace std;
using namespace cv;

#define r(j) differenceImage->imageData[j+2] // red pixel in difference image
#define g(j) differenceImage->imageData[j+1] // green pixel in difference image
#define b(j) differenceImage->imageData[j]	 // blue pixel in difference image

int main(/*int argc, char* argv[]*/)
{
    //CvCapture* capture = cvCreateCameraCapture(0); // connect to a camera
    
    //cv::Ptr<IplImage> iplimage = cvLoadImage("car7.jpg");
    cv::Mat image;
    //if (argc > 1) image = cv::imread(argv[1]);
    //else image = cv::imread("car7.jpg");
    
	image = cv::imread("car4.jpg");

    cv::Mat testPlate = cv::Mat(image);
    //PlateReader plateReader("chars2.png");
    PlateReader plateReader("chars2.png");
	PlateResult result = plateReader.readPlate(testPlate);
    cout << "Plate Only plate: " << result.plate << " with confidence " << result.confidence << endl;
    
    float avg = 0;
    for (int i = 0; i < image.rows; i++)
    {
	for (int j = 0; j < image.cols; j++) 
	{
	    avg += image.at<uint8_t>(i,j);
	}
	    
	
    }
    avg /= image.rows * image.cols;
    printf("Average colour: %f (%d, %d)\n", avg, image.rows, image.cols);
    
    
    //IplImage* image = cvQueryFrame(capture);
    // get the first frame of video
    //cv::Ptr<IplImage> output = cvCloneImage( image );
    cv::Mat output;
    // allocate image buffer for previous frame
    //IplImage* differenceImage = cvCloneImage( image );
    //cv::Ptr<IplImage> thresh = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
    
    
    //IplImage* thresh = cvCloneImage( image );
    // allocate image buffer for difference image
    //cv::namedWindow("ANPR", 0);
    
    //printf("%c%c%c%c\n", image->colorModel[0],image->colorModel[1],image->colorModel[2],image->colorModel[3]);
    //printf("%c%c%c%c\n", thresh->colorModel[0],thresh->colorModel[1],thresh->colorModel[2],thresh->colorModel[3]);
    
    //printf("Canny %d, Source %d", thresh->nChannels, image->nChannels);
    
    PlateDetector plateDetector("chars2.png");
    
    int key;
    
    int threshThresh1 = 10;
    int threshThresh2 = 250;
    
    
    // title display window
    //while (key = cvWaitKey(10) != 'q')
    {
	//capture >> image;
	
	//image = cvQueryFrame(capture); // get the next frame of video
	//output = cvCloneImage( image );
	
	image.copyTo(output);
	PlateResult plate = plateDetector.detectPlate(image, NULL);
	
	cout << "Found plate: " << plate.plate << " with confidence " << plate.confidence << endl;
	
	//cvAbsDiff( image, previousFrame, differenceImage );//diff current frame with previous frame
	//cvThreshold( differenceImage, differenceImage, 32, 0, CV_THRESH_TOZERO );//threshold difference image
	//for(int i=0; i<image->imageSize; i+=3)
	  //  if (r(i)||g(i)||b(i)) r(i)=g(i)=b(i)=255;
	//highlight all remaining difference pixels
	// insert one line of code here to median filter out the salt & pepper noise in the difference image
	//cvSmooth(image, image, CV_MEDIAN);
	////cvCvtColor(image, thresh, CV_RGB2GRAY); 


	
	//cvShowImage("difference images", thresh); // display highlighted difference image
	//cvShowImage("difference images", output); // display highlighted difference image
	
	//imshow("ANPR", output);
	//imshow("ANPR", thresh);
	
	//cvCopy( image, previousFrame );// backup current frame
    }
    //cvReleaseCapture( &capture );
    
    while (key = cvWaitKey(10) != 'q');
    return 0;
}
