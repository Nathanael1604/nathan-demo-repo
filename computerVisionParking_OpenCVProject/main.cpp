// Author: Nathanael Park
// main.cpp for COSC428 main Project: COCS428_ParkingProject_NathanPark
// Development Set-up: OpenCV library, OpenFrameworks, Visual Studios 2012 IDE, Windows 7

#include <stdio.h>
#include <stdlib.h>
#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <opencv2\imgproc\imgproc_c.h>
#include <opencv2\imgproc\types_c.h>
#include <opencv2\imgproc\imgproc.hpp> 
#include <math.h>
#include <iostream>

#define ARRAY_LENGTH(x) (sizeof(x)/sizeof(*(x)))
#define NUM_LOTS 19

using namespace cv;
using namespace std;

//void space_select_callback( int event, int x, int y, int flags, void* param );

bool destroy=false;
CvRect boxArray[65];
int occupiedArray[NUM_LOTS] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int original_state[NUM_LOTS] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
bool drawing_box = false;
int box_i = 0; //Used to count the number of boxes drawn
CvMemStorage *memStorage = cvCreateMemStorage(0);



CvPoint  Lot1[4]={13,396, 95,398, 120,305, 55,305};
CvPoint  Lot2[4]={103,397, 196,397, 197,305, 126,305};
CvPoint  Lot3[4]={203,398, 290,396, 270,303, 205,304};
CvPoint  Lot4[4]={307,394, 400,388, 352,300, 278,300};
CvPoint  Lot5[4]={406,385, 493,378, 431,299, 359,300};
CvPoint  Lot6[4]={579,366, 638,356, 560,293, 506,296};
CvPoint  Lot7[4]={1,298, 51,300, 78,247, 33,247};
CvPoint  Lot8[4]={58,300, 122,300, 136,247, 85,247};
CvPoint  Lot9[4]={130,300, 198,300, 199,248, 143,247};
CvPoint  Lot10[4]={204,300, 274,299, 263,248, 204,248};
CvPoint  Lot11[4]={281,297, 353,297, 325,247, 269,248};
CvPoint  Lot12[4]={361,295, 425,294, 386,246, 332,246};
CvPoint  Lot13[4]={434,294, 495,293, 447,247, 393,247};
CvPoint  Lot14[4]={500,292, 553,290, 506,245, 453,245};
CvPoint  Lot15[4]={349,203, 390,203, 368,177, 335,177};
CvPoint  Lot16[4]={158,202, 198,202, 200,176, 164,176};
CvPoint  Lot17[4]={204,202, 246,202, 243,176, 206,176};
CvPoint  Lot18[4]={309,201, 345,201, 330,177, 300,177};
CvPoint  Lot19[4]={69,203, 105,203, 115,180, 86,180};


CvSeq** edges[NUM_LOTS] = {0};
CvSeq* contour = 0;

//CvSeq* seq = cvCreateSeq(CV_32SC2 | CV_SEQ_KIND_CURVE, sizeof(CvSeq), sizeof(CvPoint), memStorage); 

CvSeq* sequence[NUM_LOTS] = {0};
CvPoint* parkingArray[NUM_LOTS]={Lot1, Lot2, Lot3, Lot4, Lot5, Lot6, Lot7, Lot8, Lot9, Lot10, Lot11, Lot12, Lot13, Lot14, Lot15, Lot16, Lot17, Lot18, Lot19};
int      parkingLots[NUM_LOTS]={4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4};
int      numLots=NUM_LOTS;
int      isCurveClosed=1;
int      lineWidth=2;

const char* occupiedText = "Please click in the pre-occupied spaces";
CvPoint textPoint;
bool allocated = 0;


void draw_box(IplImage* img, CvRect rect)
{
  //Draws a red border around the passed rect object
  cvRectangle(img, cvPoint(rect.x, rect.y), cvPoint(rect.x+rect.width,rect.y+rect.height), cvScalar(0,0,255),4);
}

// Implement mouse callback
void space_select_callback( int event, int x, int y, int flags, void* param )
{
  //Used to draw boxes
  IplImage* frame = (IplImage*) param;
  int i = 0;

  switch( event )
  {
      case CV_EVENT_MOUSEMOVE: 
      {
          if( drawing_box )
          {
              boxArray[box_i].width = 0; //x-boxArray[box_i].x;
              boxArray[box_i].height = 0; //y-boxArray[box_i].y;
          }
      }
      break;

      case CV_EVENT_LBUTTONDOWN:
      {
          drawing_box = true;
          boxArray[box_i] = cvRect( x, y, 0, 0 );
      }
      break;

      case CV_EVENT_LBUTTONUP:
      {
          drawing_box = false;
          if( boxArray[box_i].width < 0 )
          {
              boxArray[box_i].x += boxArray[box_i].width;
              boxArray[box_i].width *= -1;
          }

          if( boxArray[box_i].height < 0 )
          {
              boxArray[box_i].y += boxArray[box_i].height;
              boxArray[box_i].height *= -1;
          }

          draw_box(frame, boxArray[box_i]);
		  allocated = 1;
		  box_i++; //Box drawn, move to next element in array
      }
      break;

      case CV_EVENT_RBUTTONUP:
      {
		  //Destroy temp window, and begin
		 for(i=0;i<ARRAY_LENGTH(boxArray);i++)
		 {
			printf("BOX DRAWING ARRAY %d: (%d, %d, %d, %d)\n",i, boxArray[i].height, boxArray[i].width, boxArray[i].x, boxArray[i].y);
		 }
		 //allocated = 1;
		 //destroy = true;
      }
      break;

      default:
      break;
   }
}

void occupied_callback( int event, int x, int y, int flags, void* param )
{
  //Used to draw boxes
  IplImage* frame = (IplImage*) param;
  int i = 0;

  switch( event )
  {

      case CV_EVENT_LBUTTONUP:
      {
		  int dist[NUM_LOTS] = {0};
		  for(i=0;i<(NUM_LOTS);i++)
		  {
			  dist[i] = cvPointPolygonTest((CvSeq*) sequence[i], cvPoint2D32f(x,y), true);
			  printf("distance%i: %d\n",i,dist[i]);
			  if (cvPointPolygonTest((CvSeq*) sequence[i], cvPoint2D32f(x,y), true)>0)
			  {
				  //occupiedArray[i] = 1;
				  original_state[i] = 1;
			  }
		  }
      }
      break;

      default:
      break;
   }
}


int main()
{
  int i=0, clicked = 0, j=0,k=0,v=0,w=0,p =0;
  const char* name = "Window 1";
  CvRect entryBox;
  cvNamedWindow( name );
  bool first_loop = true;
  int cannyBase[NUM_LOTS] = {0};
  int hsvBase[NUM_LOTS] = {0};
  int cannyCurrent = 0, prev_nonZ = 0, hsvCurrent = 0;
  int numPx[NUM_LOTS] = {0};
  float cannyChange = 0, hsvChange = 0;
  int cannyStable = 0, hsvStable = 0;
  int canstateChange[NUM_LOTS] = {0};
  int hsvStateChange[NUM_LOTS] = {0};
  int lowThreshold = 80;
  float ratio = 1;
  int kernel_size = 3;
  
  int x_diff0t3;
  int x_diff1t2;
  int y_diff0t3;
  int y_diff1t2;

  for(j=0;j<NUM_LOTS;j++)
  {
	  for (k=0;k<4;k++)
	  {
		  if (k%2 == 1) 
		  {
			  parkingArray[j][k].y -= abs(parkingArray[j][k].x-parkingArray[j][k-1].x)/2;
		  }
		  else 
		  {
			  parkingArray[j][k].y -= abs(parkingArray[j][k].x-parkingArray[j][k+1].x)/2;
		  }
	  }
  }

  for(j=0;j<NUM_LOTS;j++)
  {
	  x_diff0t3 = parkingArray[j][0].x-parkingArray[j][3].x;
	  x_diff1t2 = parkingArray[j][1].x-parkingArray[j][2].x;
	  y_diff0t3 = parkingArray[j][1].y-parkingArray[j][2].y;
	  y_diff1t2 = parkingArray[j][1].y-parkingArray[j][2].y;
	  parkingArray[j][1].x += (x_diff1t2)/4;
	  parkingArray[j][1].y += (y_diff1t2)/4;
	  parkingArray[j][0].x += (x_diff0t3)/4;
	  parkingArray[j][0].y += (y_diff0t3)/4;

	  if ((j>=0)&&(j<=5))
	  {
		  parkingArray[j][2].x += (x_diff1t2)/4;
		  parkingArray[j][2].y += (y_diff1t2)/4;
		  parkingArray[j][3].x += (x_diff0t3)/4;
		  parkingArray[j][3].y += (y_diff0t3)/4;
	  }
	  else if ((j>=14)&&(j<=18))
	  {
		  parkingArray[j][2].x -= (x_diff1t2)/4;
		  parkingArray[j][2].y -= (y_diff1t2)/4;
		  parkingArray[j][3].x -= (x_diff0t3)/4;
		  parkingArray[j][3].y -= (y_diff0t3)/4;
	  }
  }

  entryBox.x = 456; 
  entryBox.y = 163;
  entryBox.height = 27;
  entryBox.width = 30;

  textPoint.x = 30;
  textPoint.y = 40;
  CvFont font;
  cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX, 0.8, 0.8, 0, 1, 8);

  CvCapture* capture = cvCaptureFromAVI("..\\carpark.avi"); 

  if (!capture)
  {
    printf("!!! Failed cvCaptureFromCAM\n");
    return 1;
  }

  IplImage* image = cvQueryFrame(capture); // get frame of video
  if (!image)
  {
    printf("!!! Failed cvQueryFrame #1\n");
    return 2;
  }
  
  IplImage* temp = cvCloneImage(image);
  IplImage* temp1 = cvCloneImage(image);
  IplImage* src = cvCloneImage(image);
  IplImage* base_frame = cvCloneImage(image);
  IplImage* diff_frame = cvCloneImage(image);
  IplImage* roi[NUM_LOTS];
  IplImage* roi_mask[NUM_LOTS];
  IplImage* gray_roi[NUM_LOTS];
  IplImage* gray_roi_tst = cvCreateImage(cvGetSize(src),8,1);
  //IplImage* hsv_roi[NUM_LOTS];
  IplImage* rgb_roi = cvCreateImage(cvGetSize(src),8,3);
  IplImage* hsv_roi[NUM_LOTS];
  IplImage* harris_roi[NUM_LOTS];
  IplImage* hsv_frame = cvCreateImage(cvGetSize(src),8,3);
  IplImage* rgb_frame = cvCreateImage(cvGetSize(src),8,3);
  IplImage* harris_frame = cvCreateImage(cvGetSize(src),IPL_DEPTH_32F,1);
  IplImage* harris_gray = cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
  IplImage* hsv_thresh = cvCreateImage(cvGetSize(src),8,1);
  IplImage* gray_frame = cvCreateImage(cvGetSize(src),8,1);
  IplImage* gray_threshed = cvCreateImage(cvGetSize(src),8,1);
  IplImage* diff_threshed = cvCreateImage(cvGetSize(src),8,1);
  IplImage* thresh_hsv[NUM_LOTS];
  IplImage* shadow_mask = cvCreateImage(cvGetSize(src),8,1);
  IplImage* det_edges[NUM_LOTS];
  IplImage* canny_whole = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
  IplImage* dst = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
  IplImage* temp_conv = cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,3);
  IplImage* temp_conv1 = cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,1);
  IplImage* temp_res = cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,1);
  IplImage* temp_gray = cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,1);

  cvPolyLine(temp,(CvPoint**) parkingArray, parkingLots,NUM_LOTS,isCurveClosed,cvScalar(0,255,255),lineWidth);

  cvPutText(temp, "Define Parking Spaces", textPoint, &font, CV_RGB(255,255,0));
  cvSetMouseCallback(name, space_select_callback, (void*) temp); // Initialize the mouse callback to space_select_callback function

  // Select car spaces
  while( 1 )
  {
    if (destroy) 
    {
      cvDestroyWindow(name); break;
    }
    //cvCopyImage(image, temp);

    cvMoveWindow(name, 200, 100);
    cvShowImage(name, temp);

    if (cvWaitKey(15) == 27) 
        break;
  }

  for(k=0;k<(NUM_LOTS);k++)
  {
	  sequence[k] = cvCreateSeq(CV_32SC2 | CV_SEQ_KIND_CURVE, sizeof(CvSeq), sizeof(CvPoint), memStorage);
	  det_edges[k] = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	  gray_roi[k] = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U,1);
	  roi_mask[k] = cvCreateImage(cvGetSize(src),8,1);
	  roi[k] = cvCreateImage(cvGetSize(src),8,3);
	  hsv_roi[k] = cvCreateImage(cvGetSize(src),8,1);
	  harris_roi[k] = cvCreateImage(cvGetSize(src),IPL_DEPTH_32F,1);
	  thresh_hsv[k] = cvCreateImage(cvGetSize(src),8,1);
	  for(v=0;v<4;v++)
	  {
		  cvSeqPush((CvSeq*) sequence[k],&parkingArray[k][v]);
	  }
  }

  for(w=0;w<NUM_LOTS;w++)
  {
	  cvDrawContours(temp, (CvSeq*) sequence[w],cvScalar(0,0,255),cvScalar(0,0,255),0,2);
	  numPx[w] = cvContourArea((CvSeq*) sequence[w],CV_WHOLE_SEQ,false);
  }

  cvSetMouseCallback(name, occupied_callback, (void*) temp);
  // Initialisation loop
  while( 1 )
  {
    if (destroy) 
    {
      cvDestroyWindow(name); break;
    }
    //cvCopyImage(image, temp2);


    cvMoveWindow(name, 200, 100);
    cvShowImage(name, temp);

    if (cvWaitKey(15) == 27) 
        break;
  }
  //Done

  cvReleaseImage(&temp);
  cvDestroyWindow(name);
  cvNamedWindow("Window 2", CV_WINDOW_AUTOSIZE);
  cvMoveWindow("Window 2", 150, 150);
    
  IplImage* vid_frame = cvQueryFrame(capture);
  if (!vid_frame)
  {
    printf("!!! Failed cvQueryFrame #2\n");
    return 2;
  }

  // Retrieve a single frame from the device
  for(w=0;w<NUM_LOTS;w++)
  {
	  cvZero(roi_mask[w]);
	  cvZero(det_edges[w]);
	  cvZero(roi[w]);
	  cvZero(hsv_roi[w]);
	  cvZero(gray_roi[w]);
	  cvDrawContours(roi_mask[w], (CvSeq*) sequence[w],cvScalarAll(255),cvScalarAll(255),0,-1,8);
  }

  // Main while loop
  while(1) 
  {
	if (!vid_frame)
	{
		vid_frame = cvQueryFrame(capture);
		if (!vid_frame)
		{
			printf("!!! Failed cvQueryFrame #3\n");
			break;
		}
	}

	if (first_loop == true)
	{
		base_frame = cvCloneImage(vid_frame);
	}

	//cvCopyImage(vid_frame, temp1);

	cvCopyImage(vid_frame, src);
	cvAnd(src,src,rgb_roi,roi_mask[2]);
	// Harris Corner Detection
	/*cvCvtColor(src, harris_gray, CV_BGR2GRAY);
	cvCornerHarris(harris_gray, harris_frame, 2,7,0.04);
	cvNot(harris_frame, harris_frame);*/

	// Canny Edge Detection
	cvCvtColor(src, gray_frame, CV_BGR2GRAY);
	cvSmooth(gray_frame, gray_frame, CV_GAUSSIAN, 5, 1, (1), (1)); 
	cvCanny(gray_frame, gray_frame, 50, 50, 3);
	cvInRangeS(gray_frame,cvScalarAll(0),cvScalarAll(254),gray_threshed);
	//cvOr(gray_threshed,shadow_mask, gray_threshed,shadow_mask);

	// HSV Color Detection & Shadow Removal
	cvCvtColor(src, hsv_frame,CV_BGR2HSV);
	cvZero(shadow_mask);
	//cvSmooth(hsv_frame,hsv_frame, CV_GAUSSIAN, 5, 1, (1), (1));
	//cvInRangeS(hsv_frame,cvScalar(0,30,85),cvScalar(255,250,254),hsv_thresh);
	//cvInRangeS(hsv_frame,cvScalar(0,40,0),cvScalar(255,255,200),hsv_thresh);
	cvInRangeS(hsv_frame,cvScalar(0,0,0),cvScalar(255,30,200),hsv_thresh);
	cvInRangeS(hsv_frame,cvScalar(110,0,0),cvScalar(170,90,80),shadow_mask);
	//cvNot(shadow_mask, shadow_mask);
	cvOr(hsv_thresh,shadow_mask,hsv_thresh,shadow_mask);

	// Absolute Difference Comparison 
	cvAbsDiff(vid_frame, base_frame, diff_frame);
	cvThreshold(diff_frame, diff_frame, 70, 255, CV_THRESH_BINARY_INV);
	cvInRangeS(diff_frame,cvScalarAll(1),cvScalarAll(255),diff_threshed);
	
	// RGB Shadow Removal
	cvSmooth(src,rgb_frame, CV_GAUSSIAN, 5, 1, (1), (1));
	cvInRangeS(rgb_frame,cvScalarAll(140),cvScalarAll(190),temp_res);
	//cvOr(temp_res,shadow_mask,temp_res,shadow_mask);
	//cvNot(temp_res,temp_res);


	// Prototype Work
	//cvCvtColor(src, temp_conv,CV_BGR2YCrCb);
	//cvCvtColor(src, temp_gray, CV_BGR2GRAY);
	//cvCvtColor(src, temp_conv,CV_BGR2HSV);
	//cvSmooth(temp_conv,temp_conv, CV_GAUSSIAN, 5, 1, (1), (1)); 
	//cvInRangeS(temp_conv,cvScalar(0,30,85),cvScalar(255,250,254),temp_res);
	//cvInRangeS(temp_conv,cvScalar(0,15,90),cvScalar(255,255,255),temp_res);
	//cvInRangeS(temp_conv,cvScalar(0,0,0),cvScalar(68,69,88),temp_res);
	//cvNot(temp_res,temp_res);
	//cvCvtColor(temp_conv, temp_conv1,CV_YCrCb2BGR);
	//cvAnd(temp_gray,temp_gray,temp_conv1,temp_res);
	//cvCvtColor(temp_res, temp_conv,CV_YCrCb2BGR);

	for(p=0;p<NUM_LOTS;p++)
	{
		//cvZero(det_edges[p]);
		//cvZero(roi[p]);
		//cvZero(gray_roi[p]);

		cvAnd(gray_threshed,gray_threshed,gray_roi[p],roi_mask[p]);
		cvAnd(hsv_thresh,hsv_thresh,hsv_roi[p],roi_mask[p]);

		if (first_loop == true)
		{
			cannyBase[p] = cvCountNonZero(gray_roi[p]);
			hsvBase[p] = cvCountNonZero(hsv_roi[p]);
			//prev_nonZ = cvCountNonZero(det_edges);
		}

		cannyCurrent = cvCountNonZero(gray_roi[p]);
		hsvCurrent = cvCountNonZero(hsv_roi[p]);
		//numPx = cvContourArea((CvSeq*) sequence[i],CV_WHOLE_SEQ,false);
		cannyChange = abs(((float)cannyCurrent / (float)numPx[p] * 1000.0) - ((float)cannyBase[p] / (float)numPx[p] * 1000.0));
		hsvChange = abs(((float)hsvCurrent / (float)numPx[p] * 1000.0) - ((float)hsvBase[p] / (float)numPx[p] * 1000.0));

		if (cannyChange >30)
		{
			cannyStable++;
			if (cannyStable >= 5)
			{
				canstateChange[p] = 1;
				cannyStable = 0;
			}
		}
		else
		{
			canstateChange[p] = 0;
		}

		
		if (hsvChange > 150)
		{
			//hsvStable++;
			//if (hsvStable >= 5)
			//{
				hsvStateChange[p] = 1;
				//hsvStable = 0;
			//}
		}
		else
		{
			hsvStateChange[p] = 0;
		}
		
		if ((hsvStateChange[p] == 1) && (original_state[p] == 1))
		{
			occupiedArray[p] = 0;
		}
		else if ((hsvStateChange[p] == 1) && (original_state[p] == 0))
		{
			occupiedArray[p] = 1;
		}
		else 
		{
			occupiedArray[p] = original_state[p];
		}

		/*if ((canstateChange[p] == 1) && (original_state[p] == 1))
		{
			occupiedArray[p] = 0;
		}
		else if ((canstateChange[p] == 1) && (original_state[p] == 0))
		{
			occupiedArray[p] = 1;
		}
		else 
		{
			occupiedArray[p] = original_state[p];
		}*/


		if (occupiedArray[p] == 1)
		{
			cvPolyLine(vid_frame,&parkingArray[p],&parkingLots[p],1,isCurveClosed,cvScalar(0,255,0),lineWidth);
		}
		else
		{
			cvPolyLine(vid_frame,&parkingArray[p],&parkingLots[p],1,isCurveClosed,cvScalar(255,0,0),lineWidth);
		}

	}
    cvShowImage("Window 2", vid_frame);
	//cvShowImage("Window 2", res);

    char c = cvWaitKey(1); //char c = cvWaitKey(0);

	if( c == 27 ) break;
	else if (c == 32)
	{
		c = 0;
		//cvPutText(image, occupiedText, textPoint, &font, CV_RGB(255,255,0));
		//IplImage* temp_frame = cvCloneImage(vid_frame);
		cvSetMouseCallback("Window 2", space_select_callback, (void*) vid_frame); // Initialize the mouse callback to space_select_callback function

		while (c != 32) {
			c = cvWaitKey(10);
			//cvCopyImage(vid_frame, temp_frame);
			//cvMoveWindow("Window 2", 150, 150);
			cvShowImage("Window 2", vid_frame);
			if (allocated == 1)
			{
				//cvPolyLine(temp1,(CvPoint**) parkingArray, parkingLots,NUM_LOTS,isCurveClosed,cvScalar(255,0,255),lineWidth);
				cvShowImage("Window 2",diff_frame);
			}
			if( c == 27 ) break;
		}
		allocated = 0;
		if (c==27) break;
	}

    vid_frame = NULL;
	if (first_loop == true)
	{
		first_loop = false;
	}
  }

  //Release all data from memory
  cvReleaseCapture( &capture );
  cvDestroyWindow( "Window 2" );

  return 0;
}