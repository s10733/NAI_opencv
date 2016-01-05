
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>
#include <time.h>
#include <iostream>
#include <conio.h>
using namespace cv;
using namespace std;


int main(int, char**)
{
	Mat frame;
	Mat frame2;
	Mat imgTemp;
	Mat imgLines;
	VideoCapture cap(0); 
    if(!cap.isOpened())  
		return -1;
	
	int iLowH = 170;
	int iHighH = 179;

	int iLowS = 150; 
	int iHighS = 255;

	int iLowV = 60;
	int iHighV = 255;
	
	int iLastX = -1; 
	int iLastY = -1;
	cap.read(imgTemp);
	imgLines = Mat::zeros(imgTemp.size(),CV_8UC3);
	while (waitKey(30) !=27){
		cap>>frame;
		cvtColor(frame,frame2,COLOR_BGR2HSV);
		//wydzielenie czerwonego obrazu;
		Mat imgTres;
		inRange(frame2,Scalar(iLowH,iLowS,iLowV),Scalar(iHighH,iHighS,iHighV),imgTres);
		//usuniêcie wszystkich ma³ych obiektów tak by tylko bylo widac wykryty
		erode(imgTres, imgTres, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate( imgTres, imgTres, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate( imgTres, imgTres, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
		erode(imgTres, imgTres, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

		namedWindow( "oryginal", WINDOW_AUTOSIZE );
		imshow("oryginal",frame);
		namedWindow( "wykryty czerwony", WINDOW_AUTOSIZE );
		imshow("wykryty czerwony",imgTres);
	}

}