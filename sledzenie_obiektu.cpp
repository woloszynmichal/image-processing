////////////////////////////////////////////////////////////////////////////////////////
////					- Michał Wołoszyn, Paweł Matuszczyk -						////
////			Projekt, Interaktywne Metody Planowania Ruchów 2020r.				////
////////////////////////////////////////////////////////////////////////////////////////

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdlib>
#include <math.h>

using namespace cv;
using namespace std;

int balltracking(int lowH, int highH, int lowS, int highS, int lowV, int highV);
int balltrackinghist();
int balltrackingwithpath(int option);
int balltrackingvehicle();
Point balltracking2(Mat &frame,int lowH, int highH, int lowS, int highS, int lowV, int highV);
int signnum_typical(double x);

int main()
{

	cout << "Please select what type of operation would you like to proceed:" << endl;
	cout << "enter \"1\" for tracking ball with preset HSV values" << endl;
	cout << "enter \"2\" for tracking ball with HSV values based on histogram" << endl;
	cout << "enter \"3\" for tracking ball with ball route" << endl;
	cout << "enter \"4\" for tracking ball with virtual vechicle" << endl;

	int choice;
	do {
		cin >> choice;
	} while (choice != 1 && choice != 2 && choice != 3 && choice != 4);

	if (choice == 1){
		int lowH = 0;       // Set Hue
		int highH = 21;

		int lowS = 200;       // Set Saturation
		int highS = 255;

		int lowV = 120;       // Set Value
		int highV = 255;
		balltracking(lowH, highH, lowS, highS, lowV, highV);
	} 
	if (choice == 2) {
		balltrackinghist();
	}
	if (choice == 3) {
			balltrackingwithpath(1);
	}
	if (choice == 4) {
		balltrackingvehicle();
	}


	system("PAUSE");
	return 0;
}

int balltracking(int lowH, int highH, int lowS, int highS, int lowV, int highV) {
	VideoCapture cap("data/ZadIV.avi");							// Wczytanie pliku nagrania do zmiennej cap

	if (!cap.isOpened()) {
		cout << "Error opening video file" << endl;
		return -1;
	}

	int counter = 1;
	// Petla realizujaca pobieranie kolejnych klatek nagrania i ich analize
	while (1) {

		Mat input_img;

		cap >> input_img;															// Pobranie kolejnej klatki nagrania
		if (input_img.empty()) break;												// Warunek przerywajacy petle po skonczeniu sie klatek


		Mat resized = Mat::zeros(640, 360, input_img.type());
		resize(input_img, resized, resized.size(), 22, 22, INTER_LINEAR);
		// Wyswietlenie pobranej klatki wideo

		Mat aftercoulorblind;
		Mat hsvframe;
		Mat afterdilate;
		Mat ball;

		cvtColor(resized, hsvframe, CV_BGR2HSV);
		inRange(hsvframe, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), aftercoulorblind);
		GaussianBlur(aftercoulorblind, afterdilate, Size(9, 9), 1);
		dilate(afterdilate, afterdilate, Mat(), Point(2, 2), 2, 1, 1);
		erode(afterdilate, afterdilate, Mat(), Point(2, 2), 1);
		//Mat onlyrims = aftercoulorblind - aftererode;
		vector<Vec3f> circles;
		HoughCircles(afterdilate, circles, CV_HOUGH_GRADIENT, 2, afterdilate.rows / 4, 40, 20, 10, 800);

		//threshold(resized, aftercoulorblind, 135, 255, 0);				// Wywo?anie funkcji segmentacji w trybie binarnym
		//threshold(resized, aftercoulorblind, 65, 255, 1);

		for (int i = 0; i < circles.size(); i++) {
			Point center(cvRound(circles[0][0]), cvRound(circles[0][1]));
			int radius = cvRound(circles[0][2]);
			//circle(resized, center, 3, Scalar(0, 255, 0), -4, 3, 0);
			line(resized, Point(center.x - 5, center.y), Point(center.x + 5, center.y), Scalar(0, 255, 0), 2);
			line(resized, Point(center.x, center.y - 5), Point(center.x, center.y + 5), Scalar(0, 255, 0), 2);
			circle(resized, center, 13, Scalar(0, 0, 255), 1, 3, 0);
		}

		imshow("Video", resized);

		imshow("treshold", afterdilate);

		char c = (char)waitKey(65);



		if (input_img.empty() || c == 27)
		{
			destroyAllWindows();
			break;
		}

	}

	cap.release();																	// Zwolnienie nagrania z pamieci programu
	return 1;
}

int balltrackinghist(){
	VideoCapture cap("data/ZadIV.avi");							// Wczytanie pliku nagrania do zmiennej cap

	if (!cap.isOpened()) {
		cout << "Error opening video file" << endl;
		return -1;
	}
	int frames = 0;

	int maxh=0;
	int desiredh = 0;
	int maxs = 0;
	int desireds = 120;
	int maxv = 0;
	int desiredv = 0;

	int lowH=0, topH=255;
	int lowS=0, topS = 255;
	int lowV = 120, topV = 255;

	while (1) {

		Mat input_img;

		cap >> input_img;															// Pobranie kolejnej klatki nagrania
		char c = (char)waitKey(65);
		if (input_img.empty() || c == 27)
		{
			break;
		}
		
		Mat resized = Mat::zeros(640, 360, input_img.type());
		resize(input_img, resized, resized.size(), 22, 22, INTER_LINEAR);

		Mat hsvframe;
		
		cvtColor(resized, hsvframe, CV_BGR2HSV);
		Mat hsvframeV = hsvframe;

		if (frames <= 30) {

			
			vector<Mat> hsv_planes;
			split(hsvframe, hsv_planes);
			int histSize = 360;
			float range[] = { 0, 360 };
			const float* histRange = { range };
			bool uniform = true, accumulate = false;

			Mat h_hist, s_hist;
			calcHist(&hsv_planes[0], 1, 0, Mat(), h_hist, 1, &histSize, &histRange, uniform, accumulate);
			calcHist(&hsv_planes[1], 1, 0, Mat(), s_hist, 1, &histSize, &histRange, uniform, accumulate);

			int hist_w = 512, hist_h = 600;
			int bin_w = cvRound((double)hist_w / histSize);
			Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
			normalize(h_hist, h_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

			for (int i = 1; i < histSize; i++)
			{
				line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(h_hist.at<float>(i - 1))),
					Point(bin_w * (i), hist_h - cvRound(h_hist.at<float>(i))),
					Scalar(255, 0, 0), 2, 8, 0);
				if (cvRound(h_hist.at<float>(i)) > maxh) {
					maxh = cvRound(h_hist.at<float>(i));
					desiredh = i;
				}
			}
			imshow("H Histogram", histImage);

			if (frames == 30) { 
				cout << "Desired hue was set to: " << desiredh << endl; 
				hsv_planes.clear();
				h_hist.release();
				histImage.release();
			}
		}
		
		if (frames > 31 && frames <=60) {
			
			int topH = desiredh + 10;
			int lowH = desiredh - 10;

			Mat hsvframeS, hsvframeSadd;
			inRange(hsvframe, Scalar(lowH, 0, 0), Scalar(topH, 255, 255), hsvframeS);
			bitwise_and(hsvframe, hsvframe, hsvframeSadd, hsvframeS);
			imshow("Video1", hsvframeSadd);
			vector<Mat> hsv_planesS;
			split(hsvframeSadd, hsv_planesS);
			int histSizeS = 100;
			float rangeS[] = { 0, 100 };
			const float* histRangeS = { rangeS };
			bool uniformS = true, accumulateS = false;

			Mat s_hist;
			calcHist(&hsv_planesS[1], 1, 0, Mat(), s_hist, 1, &histSizeS, &histRangeS, uniformS, accumulateS);

			int hist_wS = 512, hist_hS = 600;
			int bin_wS = cvRound((double)hist_wS / histSizeS);
			Mat histImageS(hist_hS, hist_wS, CV_8UC3, Scalar(0, 0, 0));
			normalize(s_hist, s_hist, 0, histImageS.rows, NORM_MINMAX, -1, Mat());

			for (int i = 1; i < histSizeS; i++)
			{
				line(histImageS, Point(bin_wS * (i - 1), hist_hS - cvRound(s_hist.at<float>(i - 1))), Point(bin_wS * (i), hist_hS - cvRound(s_hist.at<float>(i))), Scalar(255, 255, 255), 2, 3, 0);
				if (cvRound(s_hist.at<float>(i)) > maxs) {
					maxs = cvRound(s_hist.at<float>(i));
					desireds = i;
				}
			}
			imshow("S Histogram", histImageS);

			if (frames == 60) {
				lowS = 255 - desireds - 30;
				cout << "Desired saturation was set to: " << lowS << endl;
				hsv_planesS.clear();
				s_hist.release();
				histImageS.release();
			}
		}



		if (frames >= 61) {
			break;
		}

		imshow("Video", resized);
		frames++;
	}

	destroyAllWindows();
	cap.release();																	// Zwolnienie nagrania z pamieci programu
	

	balltracking(lowH, topH, lowS, topS, lowV, topV);

	return 1;
}

int balltrackingwithpath(int option) {
	
	VideoCapture cap("data/ZadIV.avi");							// Wczytanie pliku nagrania do zmiennej cap
	int step = 1;
	Point earlierpathpoint;
	Mat input_img;
	cap >> input_img;															// Pobranie kolejnej klatki nagrania
	Mat path = Mat::zeros(640, 360, input_img.type());
	string message;

	if (!cap.isOpened()) {
		cout << "Error opening video file" << endl;
		return -1;
	}

	if (option == 1){
		
		while (1) {
			Mat pathindicator = Mat::zeros(200, 360, input_img.type());
			if (input_img.empty()) break;												// Warunek przerywajacy petle po skonczeniu sie klatek

			int lowH = 0;       // Set Hue
			int highH = 21;

			int lowS = 200;       // Set Saturation
			int highS = 255;

			int lowV = 120;       // Set Value
			int highV = 255;

			Mat resized = Mat::zeros(640, 360, input_img.type());
			resize(input_img, resized, resized.size(), 22, 22, INTER_LINEAR);

			Point pathpoint = balltracking2(resized, lowH, highH, lowS, highS, lowV, highV);


			imshow("Video", resized);

			if (step == 1) {
				earlierpathpoint = pathpoint;
			}
			
			if (step > 1) {
				line(path, earlierpathpoint, pathpoint, Scalar(0, 255, 0), 2);
				message = "Pilka jest w pozycji : (" + to_string(pathpoint.x) + " , " + to_string(pathpoint.y)+")";
				putText(pathindicator, message, Point(50, 50), 1, 1, Scalar(255, 255, 255));
				earlierpathpoint = pathpoint;
				imshow("Ball path", path);
				imshow("Ball path - position", pathindicator);
			}

			char c = (char)waitKey(65);

			if (input_img.empty() || c == 27)
			{
				destroyAllWindows();
				break;
			}
			step++;
			cap >> input_img;
		}

	}
	waitKey();
	destroyAllWindows();
	cap.release();																	// Zwolnienie nagrania z pamieci programu
	return 1;
}

int balltrackingvehicle() {

	VideoCapture cap("data/ZadIV.avi");							// Wczytanie pliku nagrania do zmiennej cap
	int step = 1;
	Point earlierpathpoint;
	Point earliervehiclepoint;
	Point vehiclepoint = Point(50, 50);
	Mat input_img;
	cap >> input_img;															// Pobranie kolejnej klatki nagrania
	Mat path = Mat::zeros(640, 360, input_img.type());
	string message;
	string messagevehicle;
	double etx = 0;
	double ety = 0;
	double detx = 0;
	double dety = 0;
	double utx = 0;
	double uty = 0;
	double ldetx = 0;
	double ldety = 0;

	if (!cap.isOpened()) {
		cout << "Error opening video file" << endl;
		return -1;
	}

	while (1) {
			Mat pathindicator = Mat::zeros(300, 360, input_img.type());
			if (input_img.empty()) break;												// Warunek przerywajacy petle po skonczeniu sie klatek

			int lowH = 0;       // Set Hue
			int highH = 21;

			int lowS = 200;       // Set Saturation
			int highS = 255;

			int lowV = 120;       // Set Value
			int highV = 255;

			Mat resized = Mat::zeros(640, 360, input_img.type());
			resize(input_img, resized, resized.size(), 22, 22, INTER_LINEAR);

			Point pathpoint = balltracking2(resized, lowH, highH, lowS, highS, lowV, highV);

			line(resized, Point(vehiclepoint.x - 5, vehiclepoint.y), Point(vehiclepoint.x + 5, vehiclepoint.y), Scalar(0, 0, 255), 2);
			line(resized, Point(vehiclepoint.x, vehiclepoint.y - 5), Point(vehiclepoint.x, vehiclepoint.y + 5), Scalar(0, 0, 255), 2);

			imshow("Video", resized);

			

			if (step == 1) {
				earlierpathpoint = pathpoint;
				earliervehiclepoint = vehiclepoint;
				etx = abs(etx - pathpoint.x - vehiclepoint.x);
				ety = abs(etx - pathpoint.y - vehiclepoint.y);
			}

			if (step > 1) {
				line(path, earlierpathpoint, pathpoint, Scalar(0, 255, 0), 2);
				line(path, earliervehiclepoint, vehiclepoint, Scalar(0, 0, 255), 2);
				message = "Pilka jest w pozycji : (" + to_string(pathpoint.x) + " , " + to_string(pathpoint.y) + ")";
				messagevehicle = "Pojazd jest w pozycji : (" + to_string(vehiclepoint.x) + " , " + to_string(vehiclepoint.y) + ")";
				putText(pathindicator, message, Point(50, 50), 1, 1, Scalar(255, 255, 255));
				putText(pathindicator, messagevehicle, Point(50, 150), 1, 1, Scalar(255, 255, 255));
				earlierpathpoint = pathpoint;
				earliervehiclepoint = vehiclepoint;
				imshow("Ball path", path);
				imshow("Ball path - position", pathindicator);

				/* PID kontroler dla virtualnego pojazdu */
				detx = abs(etx - pathpoint.x - vehiclepoint.x);
				dety = abs(etx - pathpoint.y - vehiclepoint.y);
				etx = abs(etx - pathpoint.x - vehiclepoint.x);
				ety = abs(etx - pathpoint.y - vehiclepoint.y);
				ldetx = ldetx + abs(detx);
				ldety = ldety + abs(dety);
				utx = 0.006 * etx + 0.0006 * detx + 0.000005 * ldetx;
				uty = 0.006 * ety + 0.0006 * dety + 0.000005 * ldety;

				if (utx > 10){
					utx = 10;
				}
				if (uty > 10){
					uty = 10;
				}
				vehiclepoint = Point(vehiclepoint.x + signnum_typical(pathpoint.x - vehiclepoint.x) * int(utx), vehiclepoint.y + signnum_typical(pathpoint.y - vehiclepoint.y) * int(uty));
				//line(resized, earliervehiclepoint, vehiclepoint, Scalar(0, 0, 0), 1);
				//cout << int(utx) << ","<<int(uty) << endl;

			}

			char c = (char)waitKey(65);

			if (input_img.empty() || c == 27)
			{
				destroyAllWindows();
				break;
			}
			step++;
			cap >> input_img;
	}

	waitKey();
	destroyAllWindows();
	cap.release();																	// Zwolnienie nagrania z pamieci programu
	return 1;
}

Point balltracking2(Mat &frame, int lowH, int highH, int lowS, int highS, int lowV, int highV) {
	Mat aftercoulorblind;
	Mat hsvframe;
	Mat afterdilate;
	Mat ball;

	cvtColor(frame, hsvframe, CV_BGR2HSV);
	inRange(hsvframe, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), aftercoulorblind);
	GaussianBlur(aftercoulorblind, afterdilate, Size(9, 9), 1);
	dilate(afterdilate, afterdilate, Mat(), Point(2, 2), 2, 1, 1);
	erode(afterdilate, afterdilate, Mat(), Point(2, 2), 1);
	//Mat onlyrims = aftercoulorblind - aftererode;
	vector<Vec3f> circles;
	HoughCircles(afterdilate, circles, CV_HOUGH_GRADIENT, 2, afterdilate.rows / 4, 40, 20, 10, 800);

	//threshold(resized, aftercoulorblind, 135, 255, 0);				// Wywo?anie funkcji segmentacji w trybie binarnym
	//threshold(resized, aftercoulorblind, 65, 255, 1);

	for (int i = 0; i < circles.size(); i++) {
		Point center(cvRound(circles[0][0]), cvRound(circles[0][1]));
		int radius = cvRound(circles[0][2]);
		//circle(resized, center, 3, Scalar(0, 255, 0), -4, 3, 0);
		line(frame, Point(center.x - 5, center.y), Point(center.x + 5, center.y), Scalar(0, 255, 0), 2);
		line(frame, Point(center.x, center.y - 5), Point(center.x, center.y + 5), Scalar(0, 255, 0), 2);
		circle(frame, center, 13, Scalar(0, 0, 255), 1, 3, 0);
		return center;
	}
}

int signnum_typical(double x) {
	if (x > 0.0) return 1;
	if (x < 0.0) return -1;
	return 0;
}