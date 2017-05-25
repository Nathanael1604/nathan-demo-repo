// Author: Nathanael Park
// See main.cpp for version description

#include "PlateDetector.h"
#include <iostream>

#include "opencv/highgui.h"

using namespace std;
using namespace cv;

PlateDetector::PlateDetector(std::string filename) : plateReader(filename)
{
    
}

//Locate and Read a plate
PlateResult PlateDetector::detectPlate(Mat image, Mat* output)
{
    	cv::cvtColor(image, grey, CV_RGB2GRAY);
	cv::cvtColor(image, thresh, CV_RGB2GRAY);
	
	
	//cv::threshold(thresh, thresh, 220, 255, CV_THRESH_BINARY );
	//cv::threshold(grey, thresh, 220, 255, CV_THRESH_BINARY );
	
	//Threshold image
	cv::threshold(grey, thresh, 180, 255, CV_THRESH_BINARY );
	//cv::morphologyEx(thresh, thresh, MORPH_CLOSE, cv::getStructuringElement(MORPH_RECT, cv::Size(2,2)));
	
	
	//cv::adaptiveThreshold(thresh, thresh, 245, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 15);
	
	//imshow("THRESH", thresh);
	
	//Get image cvontours
	cv::findContours( thresh, contours, contourHierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_TC89_KCOS);
	
	cv::RotatedRect plateRect;
	
	cv::Mat selectedPlate;
	float plateHighScore = 0.0f;
	
	cv::Rect childRects[10];
	int childCount;
	
	if( contours.size() > 0 )
	{
	    if (output != NULL) cv::drawContours(*output, contours, -1, CV_RGB(0,100,255), 2);
	    
	    cout << "Found " << contours.size() << " countours\n";

	    int plateIndex = -1;
	    

	    //Loop over all contours
	    for(int i = 0; i < contours.size() && plateIndex == -1; i++ )
	    {
        
                vector<cv::Point>& contour = contours[i];
		
		//Get bounding box of each contour
        cv::RotatedRect rrect = cv::minAreaRect(cv::Mat(contour));
		cv::Point2f pts[4];
		cv::Rect rect = rrect.boundingRect();
		//rect.points(pts);
		
		float relativeArea = (rrect.size.area()) / (image.size().area());
		float aspect = (float)rect.width/(float)rect.height;
		//if (output != NULL) cv::rectangle(*output, pts[1], pts[3], CV_RGB(0,100,100));
		
		//Check plate criteria
		if (relativeArea > 0.01 && aspect > 2.5 && aspect < 3.5)
		{
		    printf("Plate Aspect: %f", aspect);
		    cv::Rect plateRect = rect;
		    plateRect.x -= 1;
		    plateRect.y -= 1;
		    plateRect.width += 2;
		    plateRect.height += 2;
		    

		    try
		    {
			//Try to score the plate
			Mat plate(image,plateRect);
			
			float score = plateScore(plate);

			printf("PlateScore: %f\n", score);
			
			if (score > plateHighScore)
			{
			    selectedPlate = plate;
			    plateHighScore = score;
			}
		    }
		    catch (cv::Exception e)
		    {
			
		    }
		    
		    /*cout << "Rect area: " << relativeArea << endl;
		    float aspect = rect.size.width/rect.size.height;
		    
		    cout << "Rectangle aspect: " << aspect << ", angle: " << rect.angle << endl;
		    //cout << "Contour child: " <<  << endl;*/
		    
		    /*int childIndex = contourHierarchy[i][2];
		    
		    
		    
		    childCount = 0;

		    
		    while (childIndex >= 0)
		    {
			vector<cv::Point>& childContour = contours[childIndex];
			cv::RotatedRect childRect = cv::minAreaRect(cv::Mat(childContour));

			float childAspect = childRect.size.width / childRect.size.height;
			if (childRect.angle > 45) childAspect = 1.0f / childAspect;
			
			if ((childAspect > 1.55 && childAspect < 1.92) || ((childRect.size.width*childRect.size.height) > 0.05 * (rect.size.width * rect.size.height)))
			{
			    childCount++;
			    if (childCount > 6) childCount = 6; 
			    childRects[childCount - 1] = childRect.boundingRect();
			}
			
			childIndex = contourHierarchy[childIndex][0];
		    }
		    
		    if (childCount >= 4)
		    {
			plateIndex = i;
			plateRect = rect;
		    }*/
		    

		}

	    }
	    
	    if (plateHighScore > 0.0f)
	    {
		//return "";
		//imshow("PLATE", selectedPlate);
		/*cv::Point2f pts[4];
		cout << "FOUND PLATE" << endl;
		plateRect.points(pts);
		    
		if (output != NULL) cv::rectangle(*output, pts[1], pts[3], CV_RGB(0,100,255));
		    
		Mat plate(image,plateRect.boundingRect());*/
		//imshow("PLATE", selectedPlate);
		
		//return plateReader.readPlate(image, childRects, childCount);
		
		//Return the result of reading the palte with the highest score
		return plateReader.readPlate(selectedPlate);
		//return "LOL";
	    }
	    
	}
	//No plate found...
	PlateResult failplate;
	failplate.plate = "";
	failplate.confidence = 0;
	return failplate;
}
/*
Mat imHist(Mat hist, float scaleX=1, float scaleY=1){
  double maxVal=0;
  minMaxLoc(hist, 0, &maxVal, 0, 0);
  int rows = 64; //default height size
  int cols = hist.rows; //get the width size from the histogram
  Mat histImg = Mat::zeros(rows*scaleX, cols*scaleY, CV_8UC3);
  //for each bin
  for(int i=0;i<cols-1;i++) {
    float histValue = hist.at<float>(i,0);
    float nextValue = hist.at<float>(i+1,0);
    Point pt1 = Point(i*scaleX, rows*scaleY);
    Point pt2 = Point(i*scaleX+scaleX, rows*scaleY);
    Point pt3 = Point(i*scaleX+scaleX, (rows-nextValue*rows/maxVal)*scaleY);
    Point pt4 = Point(i*scaleX, (rows-nextValue*rows/maxVal)*scaleY);

    int numPts = 5;
    Point pts[] = {pt1, pt2, pt3, pt4, pt1};

    fillConvexPoly(histImg, pts, numPts, Scalar(255,255,255));
  }
  return histImg;
}
*/

//Generate a score for a plate based on saturation value
float PlateDetector::plateScore(cv::Mat plateImage)
{
    
    /*int hsize[] = {256};
    float xranges[] = { 0, 256 };
    const float  *ranges[] = { xranges };
    
    cv::Mat greyPlate;
    cv::cvtColor( plateImage, greyPlate, CV_BGR2GRAY );
    MatND hist;
    int channels[] = {0}, histSize[] = {256};
    
    //hist = cvCreateHist( 1, hsize, CV_HIST_ARRAY, ranges,1);
    cv::calcHist( &greyPlate, 1, channels, Mat(), hist, 1, histSize, ranges, true, false);*/
    
    //cv::Mat imgHistogram = imHist(hist,3,3);

    
    /*cv::Mat imgHistogram(256, 256,CV_8UC1);
    //cvRectangle(imgHistogram, cvPoint(0,0), cvPoint(256,50), CV_RGB(255,255,255),-1);
    cv::rectangle(imgHistogram, cvPoint(0,0), cvPoint(256,256), CV_RGB(255,255,255),-1);


    for(int i=0; i < 256; i++){
	    float value = hist.at<int>(i);
	    float normalized = (min(256,value)/256.0f);
	    //cv::line(imgHistogram,cvPoint(i,50), cvPoint(i,50-normalized), CV_RGB(0,0,0));
    }*/
    
    //imshow("HISTOGRAM", imgHistogram);
    
    //cvWaitKey();
    
    return 255.0f - calculateAverageSaturation(plateImage);
}


//Find average saturation of an image
float PlateDetector::calculateAverageSaturation(cv::Mat plateImage)
{
    Mat hsvImage;
    cv::cvtColor(plateImage, hsvImage, CV_BGR2HSV);
    float totalSaturation = 0.0f;
    for(int i=0; i<hsvImage.rows; i++)
    {
	for(int j=0; j<hsvImage.cols; j++)
	{
           //A.data[A.step[0]*i + A.step[1]* j + 0];
           totalSaturation += hsvImage.data[hsvImage.step[0]*i + hsvImage.step[1]* j + 1];
           //A.data[A.step[0]*i + A.step[1]* j + 2];
	}
    } 
    
    totalSaturation /= plateImage.size().area();
    return totalSaturation;
    
}

