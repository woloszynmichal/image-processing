/****************************************************************************
* Program do analizy wideo lub zdjec parkingu w celu okreslenia zajetosci	*
*									parkingu 								*
*	   Projekt zosta³ wykonany przy wykorzystaniu biblioteki openCV 4.0		*
*****************************************************************************
* Michal Woloszyn oraz Pawel Matuszczyk, grupa 3TI - 2018/2019			 	*
* Projekt z przedmiotu Przetwarzania Informacji Wizyjnej 					*
****************************************************************************/
#include "stdafx.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdlib>

using namespace cv;
using namespace std;

Mat imagealign(Mat input);
bool iscar(Mat place);
void monitor(bool taken_spots[]);
int video_spots_taken(bool taken_spots[]);
void daily_chart(int T[]);

int main()
{
	/****************************************************************************
	* Interfejs urzytkownika pozwalajacy na wybor oczekiwanej funkcji programu	*
	* zak³adaj¹cy wybor pomiedzy wejsciem bêd¹cym nagraniem lub obrazem			*
	* (dla ci¹g³ego streamu kamery domsylnej nale¿y wprowadzic zmiane w kodzie	*
	* na poziomie sciezki do pliku video - ustawiajac rzadana wartosc na 0)		*
	****************************************************************************/
	cout << "Please select what type of input file you would like to open:" << endl;
	cout << "enter \"1\" for video file" << endl;
	cout << "enter \"2\" for photo file" << endl;
	cout << "enter \"3\" for daily graph" << endl;

	int choice;
    do {
        cin >> choice;
    } while (choice != 1 && choice != 2 && choice != 3);

	 if (choice == 1)
    {
        /****************************************************************************
         * Czesc realizujaca przetwarzanie pliku wideo w celu okreslenia zajetosci	*
         *			parkingu na podstawie analizy co drugiej klatki nagrania		*
         ****************************************************************************/
        
        VideoCapture cap("data/parking/20161106/061019m.avi");							// Wczytanie pliku nagrania do zmiennej cap
        //VideoCapture cap("data/parking/20161107/150249m.avi");
        //VideoCapture cap("data/parking/20161107/061138m.avi");
        
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
            
            
                Mat resized=Mat::zeros(600, 800, input_img.type());
                resize(input_img, resized, resized.size(), 22, 22, INTER_LINEAR );
                imshow("Video", resized);												// Wyswietlenie pobranej klatki wideo
                
                char c = (char)waitKey(65);
                
                
                Mat afterprocessing = imagealign(input_img);							// Wywolanie wewnetrznej funkcji imagealign()
                Mat aftercoulorblind;
                
                // Pobrane wartosci koloru kostek mieszcza sie w przedziale <65;150>
                threshold(afterprocessing, aftercoulorblind, 150, 255, 0);				// Wywo?anie funkcji segmentacji w trybie binarnym
                threshold(afterprocessing, aftercoulorblind, 65, 255, 1);				// Wywo?anie funkcji segmentacji w trybie odwrotnie binarnym
                
                bool taken_spot[25];
                
                // Okreslanie czy auto znajduje sie na wybranym miejscu parkingowym w numeracji 0-24
                // (w okroeslonym przedziale pikseli) przy wykorzystaniu wewnetrzej funkcji iscar()
                {
                    taken_spot[0] = iscar(aftercoulorblind(Range(0, 160), Range(0, 150)));
                    taken_spot[1] = iscar(aftercoulorblind(Range(0, 160), Range(190, 320)));
                    taken_spot[2] = iscar(aftercoulorblind(Range(0, 160), Range(345, 470)));
                    taken_spot[3] = iscar(aftercoulorblind(Range(0, 160), Range(500, 620)));
                    taken_spot[4] = iscar(aftercoulorblind(Range(0, 160), Range(645, 775)));
                    taken_spot[5] = iscar(aftercoulorblind(Range(0, 160), Range(800, 925)));
                    taken_spot[6] = iscar(aftercoulorblind(Range(0, 160), Range(950, 1075)));
                    taken_spot[7] = iscar(aftercoulorblind(Range(0, 160), Range(1095, 1220)));
                    taken_spot[8] = iscar(aftercoulorblind(Range(0, 160), Range(1265, 1370)));
                    taken_spot[9] = iscar(aftercoulorblind(Range(0, 160), Range(1500, 1599)));
                    
                    taken_spot[10] = iscar(aftercoulorblind(Range(445, 620), Range(180, 310)));
                    taken_spot[11] = iscar(aftercoulorblind(Range(445, 620), Range(340, 475)));
                    taken_spot[12] = iscar(aftercoulorblind(Range(445, 620), Range(500, 635)));
                    taken_spot[13] = iscar(aftercoulorblind(Range(445, 620), Range(670, 785)));
                    taken_spot[14] = iscar(aftercoulorblind(Range(445, 620), Range(830, 940)));
                    taken_spot[15] = iscar(aftercoulorblind(Range(445, 620), Range(990, 1090)));
                    taken_spot[16] = iscar(aftercoulorblind(Range(445, 620), Range(1160, 1250)));
                    taken_spot[17] = iscar(aftercoulorblind(Range(445, 620), Range(1300, 1390)));
                    
                    taken_spot[18] = iscar(aftercoulorblind(Range(990, 1090), Range(60, 130)));
                    taken_spot[19] = iscar(aftercoulorblind(Range(910, 1080), Range(230, 300)));
                    taken_spot[20] = iscar(aftercoulorblind(Range(910, 1080), Range(345, 460)));
                    taken_spot[21] = iscar(aftercoulorblind(Range(910, 960), Range(515, 635)));
                    taken_spot[22] = iscar(aftercoulorblind(Range(910, 950), Range(680, 800)));
                    taken_spot[23] = iscar(aftercoulorblind(Range(910, 1080), Range(840, 975)));
                    taken_spot[24] = iscar(aftercoulorblind(Range(910, 935), Range(1000, 1120)));
                }
                
                monitor(taken_spot);													// Wywo?anie wewnetrzej funkcji monior()
            
            
            // Metoda pozwalajaca na zakonczenie pobierania klatek nagrania
            // po wcisnieciu klawisza ESC i zamkniecie wszystkich okien
            if (input_img.empty() || c == 27)
            {
                destroyAllWindows();
                break;
            }
            counter++;
            
        }
        
        cap.release();																	// Zwolnienie nagrania z pamieci programu
    }
    else if (choice == 2)
    {
        /****************************************************************************
         *		Czesc realizujaca przetwarzanie obrazu w celu okreslenia zajetosci	*
         *				parkingu na podstawie analizy zawartosci pikseli			*
         ****************************************************************************/
        Mat input_img;
        
        // Wybieranie przykladowego obrazu do analizy:
        //-------------------------------------------------------------------------//
        input_img = imread("data/test.jpg", 1);
        //input_img = imread("data/test_wieczor.jpg", 1);
        //-------------------------------------------------------------------------//
        
        if (input_img.empty()) {
            cout << "Error opening photo file" << endl;
            return -1;
        }
        
        Mat afterprocessing = imagealign(input_img);
        Mat aftercoulorblind;
    
        Mat resized=Mat::zeros(600, 800, input_img.type());
        resize(input_img, resized, resized.size(), 22, 22, INTER_LINEAR );
        imshow("Input image", resized);
        
        threshold(afterprocessing, aftercoulorblind, 150, 255, 0);
        threshold(afterprocessing, aftercoulorblind, 65, 255, 1);
        
        bool taken_spot[25];
        {
            taken_spot[0] = iscar(aftercoulorblind(Range(0, 160), Range(0, 150)));
            taken_spot[1] = iscar(aftercoulorblind(Range(0, 160), Range(190, 320)));
            taken_spot[2] = iscar(aftercoulorblind(Range(0, 160), Range(345, 470)));
            taken_spot[3] = iscar(aftercoulorblind(Range(0, 160), Range(500, 620)));
            taken_spot[4] = iscar(aftercoulorblind(Range(0, 160), Range(645, 775)));
            taken_spot[5] = iscar(aftercoulorblind(Range(0, 160), Range(800, 925)));
            taken_spot[6] = iscar(aftercoulorblind(Range(0, 160), Range(950, 1075)));
            taken_spot[7] = iscar(aftercoulorblind(Range(0, 160), Range(1095, 1220)));
            taken_spot[8] = iscar(aftercoulorblind(Range(0, 160), Range(1265, 1370)));
            taken_spot[9] = iscar(aftercoulorblind(Range(0, 160), Range(1500, 1599)));
            
            taken_spot[10] = iscar(aftercoulorblind(Range(445, 620), Range(180, 310)));
            taken_spot[11] = iscar(aftercoulorblind(Range(445, 620), Range(340, 475)));
            taken_spot[12] = iscar(aftercoulorblind(Range(445, 620), Range(500, 635)));
            taken_spot[13] = iscar(aftercoulorblind(Range(445, 620), Range(670, 785)));
            taken_spot[14] = iscar(aftercoulorblind(Range(445, 620), Range(830, 940)));
            taken_spot[15] = iscar(aftercoulorblind(Range(445, 620), Range(990, 1090)));
            taken_spot[16] = iscar(aftercoulorblind(Range(445, 620), Range(1160, 1250)));
            taken_spot[17] = iscar(aftercoulorblind(Range(445, 620), Range(1300, 1390)));
            
            taken_spot[18] = iscar(aftercoulorblind(Range(990, 1090), Range(60, 130)));
            taken_spot[19] = iscar(aftercoulorblind(Range(910, 1080), Range(230, 300)));
            taken_spot[20] = iscar(aftercoulorblind(Range(910, 1080), Range(345, 460)));
            taken_spot[21] = iscar(aftercoulorblind(Range(910, 960), Range(515, 635)));
            taken_spot[22] = iscar(aftercoulorblind(Range(910, 950), Range(680, 800)));
            taken_spot[23] = iscar(aftercoulorblind(Range(910, 1080), Range(840, 975)));
            taken_spot[24] = iscar(aftercoulorblind(Range(910, 935), Range(1000, 1120)));
        }
        
        monitor(taken_spot);
        char c = (char)waitKey(1);
        if (input_img.empty() || c == 27)
        {
            destroyAllWindows();
        }
    }
    else
    {
        /****************************************************************************
         * Czesc realizujaca przetwarzanie pliku wideo w celu okreslenia zajetosci	*
         *			parkingu na podstawie analizy co drugiej klatki nagrania		*
         ****************************************************************************/

        string beforepath = "data/parking/20161106/";
        string endpath = "m.avi";
		string startpath = "061019";
        string path;
        
        int T[33];
        bool taken_spot[25];
        int counter = 0;
		int flag=0;

        
        // Petla realizujaca pobieranie kolejnych klatek nagrania i ich analize
        while (1) {
            
            Mat input_img;

				if (counter == 0) path = startpath;
                if(counter==1) path="061338";
                if(counter==2) path="061557";
                if(counter==3) path="063454";
                if(counter==4) path="063900";
                if(counter==5) path="064021";
                if(counter==6) path="064204";
                if(counter==7) path="072132";
                if(counter==8) path="081458";
                if(counter==9) path="082847";
                if(counter==10) path="083159";
                if(counter==11) path="093105";
                if(counter==12) path="093453";
                if(counter==13) path="094638";
                if(counter==14) path="094931";
                if(counter==15) path="102211";
                if(counter==16) path="103738";
                if(counter==17) path="104646";
                if(counter==18) path="111341";
                if(counter==19) path="111802";
                if(counter==20) path="111953";
                if(counter==21) path="114552";
                if(counter==22) path="121657";
                if(counter==23) path="130754";
                if(counter==24) path="131410";
                if(counter==25) path="134458";
                if(counter==26) path="135651";
                if(counter==27) path="142626";
                if(counter==28) path="145132";
                if(counter==29) path="145933";
                if(counter==30) path="150801";
                if(counter==31) path="151155";
                if(counter==32) path="151648";
                if(counter==33) {
                    daily_chart(T);
					system("PAUSE");
					destroyAllWindows();
					return 0;
                }											

			VideoCapture cap(beforepath + path + endpath);							// Wczytanie pliku nagrania do zmiennej cap

			if (!cap.isOpened()) {
				cout << "Error opening video file" << endl;
				return -1;
			}
           
			cap >> input_img;

			while(!input_img.empty()) {
				Mat resized = Mat::zeros(600, 800, input_img.type());
				resize(input_img, resized, resized.size(), 22, 22, INTER_LINEAR);
				imshow("Video", resized);												// Wyswietlenie pobranej klatki wideo

				Mat afterprocessing = imagealign(input_img);							// Wywolanie wewnetrznej funkcji imagealign()
				Mat aftercoulorblind;

				// Pobrane wartosci koloru kostek mieszcza sie w przedziale <65;150>
				threshold(afterprocessing, aftercoulorblind, 150, 255, 0);				// Wywo?anie funkcji segmentacji w trybie binarnym
				threshold(afterprocessing, aftercoulorblind, 65, 255, 1);				// Wywo?anie funkcji segmentacji w trybie odwrotnie binarnym

			// Okreslanie czy auto znajduje sie na wybranym miejscu parkingowym w numeracji 0-24
			// (w okroeslonym przedziale pikseli) przy wykorzystaniu wewnetrzej funkcji iscar()
				{
					taken_spot[0] = iscar(aftercoulorblind(Range(0, 160), Range(0, 150)));
					taken_spot[1] = iscar(aftercoulorblind(Range(0, 160), Range(190, 320)));
					taken_spot[2] = iscar(aftercoulorblind(Range(0, 160), Range(345, 470)));
					taken_spot[3] = iscar(aftercoulorblind(Range(0, 160), Range(500, 620)));
					taken_spot[4] = iscar(aftercoulorblind(Range(0, 160), Range(645, 775)));
					taken_spot[5] = iscar(aftercoulorblind(Range(0, 160), Range(800, 925)));
					taken_spot[6] = iscar(aftercoulorblind(Range(0, 160), Range(950, 1075)));
					taken_spot[7] = iscar(aftercoulorblind(Range(0, 160), Range(1095, 1220)));
					taken_spot[8] = iscar(aftercoulorblind(Range(0, 160), Range(1265, 1370)));
					taken_spot[9] = iscar(aftercoulorblind(Range(0, 160), Range(1500, 1599)));

					taken_spot[10] = iscar(aftercoulorblind(Range(445, 620), Range(180, 310)));
					taken_spot[11] = iscar(aftercoulorblind(Range(445, 620), Range(340, 475)));
					taken_spot[12] = iscar(aftercoulorblind(Range(445, 620), Range(500, 635)));
					taken_spot[13] = iscar(aftercoulorblind(Range(445, 620), Range(670, 785)));
					taken_spot[14] = iscar(aftercoulorblind(Range(445, 620), Range(830, 940)));
					taken_spot[15] = iscar(aftercoulorblind(Range(445, 620), Range(990, 1090)));
					taken_spot[16] = iscar(aftercoulorblind(Range(445, 620), Range(1160, 1250)));
					taken_spot[17] = iscar(aftercoulorblind(Range(445, 620), Range(1300, 1390)));

					taken_spot[18] = iscar(aftercoulorblind(Range(990, 1090), Range(60, 130)));
					taken_spot[19] = iscar(aftercoulorblind(Range(910, 1080), Range(230, 300)));
					taken_spot[20] = iscar(aftercoulorblind(Range(910, 1080), Range(345, 460)));
					taken_spot[21] = iscar(aftercoulorblind(Range(910, 960), Range(515, 635)));
					taken_spot[22] = iscar(aftercoulorblind(Range(910, 950), Range(680, 800)));
					taken_spot[23] = iscar(aftercoulorblind(Range(910, 1080), Range(840, 975)));
					taken_spot[24] = iscar(aftercoulorblind(Range(910, 935), Range(1000, 1120)));
				}
				
				char c = waitKey(1);
				if (c == 27)
				{
					destroyAllWindows();
					return 0;
				}

				cap >> input_img;
			}

			T[counter - 1] = video_spots_taken(taken_spot);
			cout << T[counter - 1] << endl;
			counter++;
			destroyAllWindows();
			flag = 1;
			cap.release();																// Zwolnienie nagrania z pamieci programu

																		
        }
        
        																
    }

	system("PAUSE");
    return 0;
}

/****************************************************************************
* Funkcja realizujaca zmiane perspektwy klatki lub obrazu do takiej, ktora	*
*	umorzliwia dalsze przetwarzanie w celu okreslania czy wybrane miejsce	*
*							parkingowe jest zajete							*
****************************************************************************/
Mat imagealign(Mat input)
{
	Point2f inputQuad[4];
	Point2f outputQuad[4];
	Mat lambda(2, 4, CV_32FC1);
	Mat output;

	lambda = Mat::zeros(input.rows, input.cols, input.type());							// Wypelnienie macierzy o wymiarach obrazu przetwarzanego zerami 

	// Wspolrzedne punktow obrazu pierwotnego okreslajace zakres analizowanego parkingu
	inputQuad[0] = Point2f(450, 430);
	inputQuad[1] = Point2f(1310, 470);
	inputQuad[2] = Point2f(1599, 990);
	inputQuad[3] = Point2f(350, 939);

	// Wspolrzedne punktow obrazu winikowego bedacego rozciagnieciem perspektwycznym obrazu przetwarzanego
	outputQuad[0] = Point2f(0, 0);
	outputQuad[1] = Point2f(input.cols - 1, 0);
	outputQuad[2] = Point2f(input.cols - 1, input.rows - 1);
	outputQuad[3] = Point2f(0, input.rows - 1);

	lambda = getPerspectiveTransform(inputQuad, outputQuad);							// Utworzenie macierzy transformacji perspektywicznej
	warpPerspective(input, output, lambda, output.size());								// Wykorzystanie przetworzenia transformacji perspektywicznej obrazu przetwarzanego 

	return output;																		// Zwrocenie obrazu (macierzy danych) wynikowego funkcji
}


/****************************************************************************
*	Funkcja okreslajaca czy auto znajduje sie w danym przedziale obrazu		*
*					bedacym zbiorem punktow w macierzy						*
****************************************************************************/
bool iscar(Mat place)
{
	int sum = 0;
	int intensity = 0, counter = 0;
	double average;

	// Petla realizujaca pobieranie kolejnych wartosci piksela oraz sumowanie
	// tych wartosci z jedoczesnym zliczaniem ilosci pikseli w badanym przedziale
	for (int x = 0; x <= place.rows-1; x++)
	{
		for (int y = 0; y <= place.cols-1; y++)
		{
			intensity = place.at<uchar>(x, y);											// Pobieranie wartosci piksela o polorzeniu (x,y) z zakresu 0-255 do zmiennej
			sum = sum + intensity;
			counter++;
		}
	}

	average = (double)sum / (double)counter;											// Wyliczenie sredniej wartosci pikseli w badanym przedziale 

	if (average > 5)
	{
		return true;																	// Zwrocenie wartosci 1 jesli srednia wartosc pikseli przekracza 5, 
																						// auto znajduje sie w badanym przedziale
	}
	else
	{
		return false;																	// Zwrocenie wartosci 0 jesli srednia wartosc pikseli nie przekracza 5, 
																						// auto nie znajduje sie w badanym przedziale
	}
	
}


/****************************************************************************
*	Funkcja tworzaca nowe okno bedace reprezentacja zajetosci parkingu		*
*	  oraz wyswietlajaca komunikaty tekstowe o ilosci wolnych miejsc		*
*	     i stopniu zajetosci analizowanego parkingu w procentach			*
****************************************************************************/
void monitor(bool taken_spots[])
{
	Mat space = Mat::zeros(300, 230, CV_8UC3);											// Okreslenie wymiarow okna monitorujacego 
	int taken = 0;
	string message_parking_overview;

	for (int i = 0; i <= 24; i++)
	{
		taken = taken + taken_spots[i];													// Zliczenie ilosci zajetych miejsc parkingowych

		// Tworzenie wizualizacji miejsc parkingowych o numerach od 0 do 9,
		// bedacych jednoczescie reprezentacja miejsc najbardziej wysunietych na parkingu
		// Kolor zielony reprezentuje miejsce wolne, czerwony natomiast miejsce zajete
		if (i < 10) 
		{
			if (taken_spots[i] == 1)
			{
				rectangle(space, Point(5, 200 - (i * 20)), Point(45, 180 - i * 20), Scalar(0, 0, 255), +1, 4);
			}
			else
			{
				rectangle(space, Point(5, 200 - (i * 20)), Point(45, 180 - i * 20), Scalar(0, 255, 0), +1, 4);
			}
		}

		// Wyswietlanie wizualizacji miejsc parkingowych o numerach od 9 do 17
		// bedacych jednoczescie reprezentacja miejsc w srodkowej czesci parkingu
		if (i > 9 && i<18)
		{
			if (taken_spots[i] == 1)
			{
				rectangle(space, Point(100, 180 - (i - 10) * 20), Point(140, 160 - (i - 10) * 20), Scalar(0, 0, 255), +1, 4);
			}
			else
			{
				rectangle(space, Point(100, 180 - (i - 10) * 20), Point(140, 160 - (i - 10) * 20), Scalar(0, 255, 0), +1, 4);
			}
		}

		// Wyswietlanie wizualizacji miejsc parkingowych o numerach od 18 do 24 
		// bedacych jednoczescie reprezentacja miejsc znajdujacych sie najblizej kamery 
		if (i >= 18)
		{
			if (taken_spots[i] == 1)
			{
				rectangle(space, Point(180, 200 - (i - 18) * 20), Point(220, 180 - (i - 18) * 20), Scalar(0, 0, 255), +1, 4);
			}
			else
			{
				rectangle(space, Point(180, 200 - (i - 18) * 20), Point(220, 180 - (i - 18) * 20), Scalar(0, 255, 0), +1, 4);
			}
		}

	}
	
	// funkjca switch() realizujaca utworzenie komunikatu w zaleznosci od ilosci zajetych miejsc 
	switch (taken)
	{
	case 0:
		message_parking_overview = "Parking is empty!";
		break;
	case 1:
		message_parking_overview = "There is " + to_string(taken) + " spot taken";
		break;
	case 25:
		message_parking_overview = "Parking is full!";
		break;

	default:
		message_parking_overview = "There are " + to_string(taken) + " spots taken";
		break;
	}

	int percentage = (double)(taken / 25.0) * 100;

	// Umieszczenie utworzonych komunikatow w oknie monitoringu 
	putText(space, message_parking_overview , Point(0,255), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1, 8, false);
	putText(space, "Parking is in " + to_string(percentage) + "% full", Point(0, 280), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1, 8, false);

	char* source_window = "Parking overwiev";										// Nazwa okna monitoringu
	imshow(source_window, space);													// Wyswietlenie okna monitoringu 
}

int video_spots_taken(bool taken_spots[])
{
    int taken = 0;
    
    for (int i = 0; i <= 24; i++)
    {
        taken = taken + taken_spots[i];													// Zliczenie ilosci zajetych miejsc parkingowych
        
    }
    return taken;
    
}

void daily_chart(int T[])
{
	Mat space = Mat::zeros(900, 1080, CV_8UC3);

	string path;

	line(space, Point(50, 850), Point(50, 50), Scalar(255, 255, 255), +1, 4);
	line(space, Point(50, 850), Point(1030, 850), Scalar(255, 255, 255), +1, 4);
	line(space, Point(40, 80), Point(50, 50), Scalar(255, 255, 255), +1, 4);
	line(space, Point(60, 80), Point(50, 50), Scalar(255, 255, 255), +1, 4);
	line(space, Point(1000, 840), Point(1030, 850), Scalar(255, 255, 255), +1, 4);
	line(space, Point(1000, 860), Point(1030, 850), Scalar(255, 255, 255), +1, 4);

	for (int k = 0; k <= 25; k++)
	{
		line(space, Point(50, 850 - 30 * k), Point(40, 850 - 30 * k), Scalar(255, 255, 255), +1, 4);
		putText(space, to_string(k), Point(15, 850 - 30 * k), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1, 8, false);
	}


	for (int i = 0; i <= 32; i++)
	{
		circle(space, Point(50 + i * 30, 850 - 30 * T[i]), 4, Scalar(0, 0, 255), +4, 4);
		line(space, Point(50 + i * 30, 850), Point(50 + i * 30, 860), Scalar(255, 255, 255), +1, 4);
		if (i == 0) path = "06:10";
		if (i == 1) path = "06:13";
		if (i == 2) path = "06:15";
		if (i == 3) path = "06:34";
		if (i == 4) path = "06:39";
		if (i == 5) path = "06:40";
		if (i == 6) path = "06:42";
		if (i == 7) path = "07:21";
		if (i == 8) path = "08:14";
		if (i == 9) path = "08:28";
		if (i == 10) path = "08:31";
		if (i == 11) path = "09:31";
		if (i == 12) path = "09:34";
		if (i == 13) path = "09:46";
		if (i == 14) path = "09:49";
		if (i == 15) path = "10:22";
		if (i == 16) path = "10:37";
		if (i == 17) path = "10:46";
		if (i == 18) path = "11:13";
		if (i == 19) path = "11:18";
		if (i == 20) path = "11:19";
		if (i == 21) path = "11:45";
		if (i == 22) path = "12:16";
		if (i == 23) path = "13:07";
		if (i == 24) path = "13:14";
		if (i == 25) path = "13:44";
		if (i == 26) path = "13:56";
		if (i == 27) path = "14:26";
		if (i == 28) path = "14:51";
		if (i == 29) path = "14:59";
		if (i == 30) path = "15:08";
		if (i == 31) path = "15:11";
		if (i == 32) path = "15:16";
		if (i % 2 == 1)  putText(space, path, Point(35 + i * 30, 890), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 255, 255), 1, 8, false);
		if (i % 2 == 0)  putText(space, path, Point(35 + i * 30, 870), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 255, 255), 1, 8, false);
	}

	imshow("Daily chart", space);
	moveWindow("Daily chart", 20, 20);
	waitKey(1);
}
