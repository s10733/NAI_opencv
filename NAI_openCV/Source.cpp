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
	VideoCapture cap(0); 
    if(!cap.isOpened())  
		return -1;
	while (waitKey(30) !=27){
		
		
	cap>>frame;
	namedWindow( "oryginal", WINDOW_AUTOSIZE );	
	imshow("oryginal",frame);
	}

}