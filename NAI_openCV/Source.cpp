#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>
#include <time.h>
#include <iostream>
#include <conio.h>
using namespace cv;
using namespace std;
void drawLine();
void drawTriangles(Mat&, const vector< vector<Point> >&);

Mat img_rgb,img_gray,canny_output,drawing;

int thresh = 100;
int max_thresh = 255;


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
		//wyliczenie momentów progowych
		Moments oMoments = moments(imgTres);
		
		double dM01 = oMoments.m01;
		double dM10 = oMoments.m10;
		double dArea = oMoments.m00;

		//jeœli jest area jest wiêksze to znaczy ¿e jest obiekt na ekranie
		if (dArea > 10000)
		{
			//wyliczenie pozycji obiektu na ekranie
			int posX = dM10 / dArea;
			int posY = dM01 / dArea;        
        
		   if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
		   {
			//rysowanie od pocz¹tku do koñca lini.
			line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0,0,255), 2);
		   }
   
		   iLastX = posX;
		   iLastY = posY;
		}
		namedWindow( "oryginal", WINDOW_AUTOSIZE );
		imshow("oryginal",frame);
		namedWindow( "wykryty czerwony", WINDOW_AUTOSIZE );
		imshow("wykryty czerwony",imgTres);
		namedWindow("linia", WINDOW_AUTOSIZE);
		imshow("linia",imgLines);
		
		vector<int> compression_params;
		compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
		compression_params.push_back(9);
		
		if(waitKey(10)!=27){
			Mat RGB;
			cvtColor(frame+imgLines,RGB,CV_RGB2HLS);
			imwrite("C:/Users/£ukasz/Desktop/Nowy folder/ima.png",RGB, compression_params);
		}

		
	}
	while(true){
		destroyWindow("oryginal");
		destroyWindow("linia");
		destroyWindow("wykryty czerwony");
		img_rgb  = imread("C:/Users/£ukasz/Desktop/Nowy folder/ima.png");
		cvtColor(img_rgb,img_gray,CV_RGB2GRAY);
		imshow("Klatka",img_rgb);
		drawLine();
		(waitKey(10)!=27);
	}   

}
void drawLine(){
	vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
	Canny( img_gray, canny_output, thresh, thresh*2, 3 );
    imshow("Kontury",canny_output);
    findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    drawing = Mat::zeros( canny_output.size(), CV_8UC3 );

    drawTriangles(drawing,contours);
    imshow("Trojkaty",drawing);;
}
void drawTriangles(Mat& img, const vector< vector<Point> >& contours){
    vector<Point> approxTriangle;
    for(size_t i = 0; i < contours.size(); i++){
        approxPolyDP(contours[i], approxTriangle, arcLength(Mat(contours[i]), true)*0.05, true);
        if(approxTriangle.size() == 3){
            drawContours(img, contours, i, Scalar(0, 255, 255), CV_FILLED); // fill GREEN
            vector<Point>::iterator vertex;
            for(vertex = approxTriangle.begin(); vertex != approxTriangle.end(); ++vertex){
                circle(img, *vertex, 3, Scalar(0, 0, 255), 1);
            }
        }
    }
}

