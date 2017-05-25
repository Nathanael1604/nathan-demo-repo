// Author: Nathanael Park
// See main.cpp for version description

#include "PlateReader.h"
#include "opencv/highgui.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <cstdio>
#include <utility>
#include <algorithm>
using namespace std;
using namespace cv;


char PlateReader::letterMap[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'}; 

//Size PlateReader::charSize(34, 49);
Size PlateReader::charSize(70, 100);

PlateReader::PlateReader(string filename)
{
      //Load the character library
    letterLibrary = imread(filename);
    if (letterLibrary.data != NULL) 
    {
	for (int i = 0; i < sizeof(letterMap); i++)
	{
	    vector<vector<Point> > contours;
	    
	    Mat currentLetter(letterLibrary, Rect(charSize.width * i, 0, charSize.width, charSize.height));
	    
	    cv::cvtColor(currentLetter, letter[i], CV_RGB2GRAY);
	    
	    autoCrop(letter[i]);
	    
	}
	letter[0].copyTo(compValue);
    }
}
//UNUSED
string PlateReader::readPlate(Mat& image, Rect* characters, int numCharacters)
{
    if (letterLibrary.data == NULL) return "_BadFile";
    char plateString[6];
    
    cout << "Reading " << numCharacters << " characters" << endl;
    
    calculateCharacterPositions(characters, numCharacters);
    
    for (int i = 0; i < min(6,numCharacters); i++)
    {
	cv::cvtColor(Mat(image, characters[i]),character, CV_RGB2GRAY);
	cv::threshold(character, character, 200, 255, CV_THRESH_BINARY );
	
	plateString[characterPosition[i]] = matchCharacter(character).letter;
	//plateString[i] = matchCharacter(character);
	
	//imshow(numbers[i], character);
	//cout << "Character has area: " << characters[i].area() << endl;
	
    }
    
    return string(plateString,numCharacters);
}

bool poscharCompare (PosChar a, PosChar b) { return (a.xPos < b.xPos); }

//Read a plate from an image
PlateResult PlateReader::readPlate(Mat& image)
{
    PlateResult result;
    result.confidence = 1;
    cv::cvtColor(image, plateThresh, CV_RGB2GRAY);
    cv::threshold(plateThresh, plateThresh, 190, 255, CV_THRESH_BINARY );
    
    cv::rectangle(plateThresh, cv::Point(0,0), cv::Point(image.cols-1, image.rows-1), 255, 6);
    
    //imshow("PostRec", plateThresh);
    
    //Find contours
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> contourHierarchy;
    cv::findContours( plateThresh, contours, contourHierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_TC89_KCOS);
    
    //cv::drawContours(image, contours, -1, CV_RGB(0,100,255));
    
    stringstream plateString;
    
    Vector<PosChar> readLetters;
    
    //Loop over contours...
    cv::RotatedRect plateRect;
    for(int i = 0; i < contours.size(); i++ )
    {
	try
	{
	    vector<cv::Point>& contour = contours[i];
	    cv::RotatedRect rrect = cv::minAreaRect(cv::Mat(contour));
	    cv::Rect rect = rrect.boundingRect();
	    cv::Point2f pts[4];
	    
	    rrect.points(pts);

	    
	    //float relativeArea = (rect.size.area()) / (image.size().area());
	    float relativeHeight = (max(rrect.size.height, rrect.size.width)) / (image.size().height);
	    //float relativeHeight = (maxrrect.size.height) / (image.size().height);
	    float aspect = (float)rect.width/(float)rect.height;
	    
	    //Check aspect ratio and relative height
	    if (relativeHeight > 0.5 && aspect < 0.9f )
	    {
		//printf ("X: %d\n", rect.x);
		//printf("Relative Height: %f, Aspect : %f\n", relativeHeight, aspect );
		//cv::rectangle(image, pts[1], pts[3], CV_RGB(255,0,0));
		cv::cvtColor(Mat(image, rect),character, CV_RGB2GRAY);
		cv::threshold(character, character, 200, 255, CV_THRESH_BINARY );
		PosChar poschar;
		//Find the character that matches
		poschar = matchCharacter(character);
		
		//Character x position on the palte for finding position in string
		poschar.xPos = rect.x;
		
		printf("Read character %c with confidence %f\n", poschar.letter, poschar.confidence);
		result.confidence = min(result.confidence, poschar.confidence);
		//plateString << matchCharacter(character);
		readLetters.push_back(poschar);
	    }
	}
	catch (cv::Exception e)
	{
	    
	}
	
    }
    
    //imshow("PlateThresh", image);
    
    sort(readLetters.begin(), readLetters.end(), poscharCompare);
    
    for (int i = 0; i < readLetters.size(); i++) plateString << readLetters[i].letter;
    
    //return string(plateString,numCharacters);
    
    result.plate = plateString.str();
    
    return result;
}

//Find the closest character match of a letter image
PosChar PlateReader::matchCharacter(Mat& image)
{
    PosChar result;
    result.confidence = 0;
    //cv::resize(image, resizedCharacter, charSize, 0, 0, INTER_NEAREST);
    image.copyTo(resizedCharacter);
    autoCrop(resizedCharacter);
    char lol[2];
    lol[1] = 0;
    
    int bestScore = 0;
    int bestMatch = 0;
    
    for (int i = 0; i < sizeof(letterMap); i++)
    {
	compare(resizedCharacter, letter[i], compValue, CMP_EQ);
	int score = countNonZero(compValue);
	
	
	
	//int score = 0;
	
	if (score > bestScore)
	{
	    bestScore = score;
	    bestMatch = i;
	}
	

	
    }
    
    result.confidence = (float)bestScore / (float)charSize.area();
    
    lol[0] = letterMap[bestMatch];
    //imshow(lol, resizedCharacter);
    
    result.letter = letterMap[bestMatch];
    return result;
}

//Crop and normalise an image
void PlateReader::autoCrop(Mat& image)
{
    int minX = image.size().width+1;
    int maxX = -1;
    int minY = image.size().height+1;
    int maxY = -1;
    
    
    for (int y = 0; y < image.size().height; y++)
    {
	for (int x = 0; x < image.size().width; x++)
	{
	    if(image.at<uchar>(y,x) == 0)
	    {
		minX = min(minX, x);
		maxX = max(maxX, x);
		minY = min(minY, y);
		maxY = max(maxY, y);
		
	    }
	}
    }

    Mat temp;
    image.copyTo(temp);
    
    Mat tempRoi(temp, Rect(minX, minY, maxX - minX, maxY - minY));
    
    cv::resize(tempRoi, image, charSize, 0, 0, INTER_NEAREST);
}


//UNUSED
bool platePosCompare (pair<int, Rect*> a,pair<int, Rect*> b) { return (a.second->x < b.second->x); }

void PlateReader::calculateCharacterPositions(Rect* characters, int numCharacters)
{
    //int maxWidth = 0;
    //for (int i = 0; i < numCharacters; i++) maxWidth = max(maxWidth, characters[i].width);
    
    vector< pair<int, Rect*> > list;
    for (int i = 0; i < numCharacters; i++)
    {
	list.push_back(pair<int, Rect*>(i, &characters[i]));
    }
    sort(list.begin(), list.end(), platePosCompare);
    
    for (int i = 0; i < numCharacters; i++)
    {
	characterPosition[i] = list[i].first;
	cout << list[i].second->x << " " << list[i].second->width << endl;
    }
    
    for (int i = 5; i > numCharacters - 1; i--) characterPosition[i] = i;
    
    for (int i = 0; i < 6; i++) cout << characterPosition[i] << " ";
    cout << endl;

}