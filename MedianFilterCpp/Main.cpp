// Author: Nathan Park
// Originally ran a simple median blur filter, now Nathans code foe just testing CV lib functions and experimenting with random concepts
// Current: display the difference between adjacent video frames (press any key to exit)


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
//#include <cstdio>
//#include <string>
//#include <stdio.h>
//#include <stdlib.h>

using namespace cv;
using namespace std;


class myFrameClass {
	public:
		virtual void setMyImage(Mat im) = 0;
		virtual Mat getMyImage() = 0;
	private:
	protected:
};

class baseFrame: public myFrameClass {
	public:
		void setMyImage(Mat im)
		{
			im.copyTo(origImage);
		};

		Mat getMyImage()
		{
			return origImage;
		};
	private:
	protected:
		Mat origImage;
};


class prevFrame: public myFrameClass {
	public:
		void setMyImage(Mat im)
		{
			im.copyTo(prevImage);
		};

		Mat getMyImage()
		{
			return prevImage;
		};
	private:
	protected:
		Mat prevImage;
};

class diffFrame: public myFrameClass {
	public:
		Mat diffImage;
		void setMyImage(Mat im)
		{
			im.copyTo(diffImage);
		};

		Mat getMyImage()
		{
			return diffImage;
		};
	private:
	protected:
};

/*
class prevFrame {
	public:
		void setPrevImage(Mat im)
		{
			//prevImage = im;
			im.copyTo(prevImage); 
		};

		Mat getPrevImage()
		{
			return prevImage;
		};
	private:
	protected:
		Mat prevImage;
};
*/

int main()
{
	prevFrame prevImageObj;
	diffFrame diffImageObj;
	baseFrame baseImageObj;
	//myFrameClass *dIO = &diffImageObj, *bIO = &baseImageObj, *pIO = &prevImageObj;
	Mat image1st, image2nd; //differenceImage, previousFrame;
	VideoCapture capture(0); // connect to a camera
	//VideoCapture capture ("..\\redball.avi");

	if(!capture.isOpened())  // check if capture succeeded
	{
		cout << "!!! Failed cvCaptureFromCAM connection" << endl;
		return 1;
	}

	capture >> image1st;
	if (image1st.empty())
	{
		cout << "!!! Failed cvQueryFrame #1\n" << endl;
		//return 2;
	}
	else {
		imshow("1st image", image1st);
	}

	capture >> image2nd;
	if (image2nd.empty())
	{
		cout << "!!! Failed cvQueryFrame #2" << endl;
		return 3;
	}
	else {
		baseImageObj.setMyImage(image2nd);
		imshow("original image", baseImageObj.getMyImage());// display original frame pixels
	}

	//image2nd.copyTo(prevImageObj.prevImage);
	prevImageObj.setMyImage(image2nd);
	diffImageObj.setMyImage(image2nd);
	//prevImageObj.setPrevImage(image2nd);
	//image2nd.copyTo(previousFrame); // save it to the previousFrame
	//image2nd.copyTo(differenceImage); // save it to the differenceFrame

	//cout << "Stop Point #1" << endl;

	while(waitKey(10) < 0) {
		capture >> image2nd; // get the next frame of video
		if(image2nd.empty()) break; // stop after last frame (end-of-file)
		absdiff(image2nd, prevImageObj.getMyImage(), diffImageObj.diffImage); // calculate difference image
		//absdiff(image2nd, previousFrame, differenceImage); // calculate difference image
		//absdiff(image2nd, prevImageObj.getPrevImage(), differenceImage);
		threshold( diffImageObj.diffImage, diffImageObj.diffImage, 32, 0, CV_THRESH_TOZERO ); //threshold
		threshold( diffImageObj.diffImage, diffImageObj.diffImage, 0, 255, CV_THRESH_BINARY );//highlight
		//threshold( differenceImage, differenceImage, 32, 0, CV_THRESH_TOZERO ); //threshold
		//threshold( differenceImage, differenceImage, 0, 255, CV_THRESH_BINARY );//highlight
		
		// insert one line of code here to median filter out noise in difference image:
		medianBlur (diffImageObj.diffImage, diffImageObj.diffImage, 3);
		
		//image2nd.copyTo(previousFrame); // backup current frame
		//prevImageObj.setPrevImage(image2nd); // backup current frame
		prevImageObj.setMyImage(image2nd);

		imshow("difference images", diffImageObj.diffImage);// display highlighted pixels
	}

	//Release the capture object
	capture.release();
	return 0;
}


/*
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int main()
{
	// Camera/video capture testing code
    Mat image, image2nd;
	VideoCapture capture(0);
	//VideoCapture capture ("..\\redball.avi");

	capture >> image;
	//capture >> image2nd;

    while(waitKey(10) < 0)
    {    
        capture >> image;
		if(image.empty()) break; // stop after last frame (end-of-file)
        imshow("Video", image);
    }

	//Release the capture object
    capture.release();
    return 0;
}
*/