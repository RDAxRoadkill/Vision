#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

int main1(int argc, char* argv[])
{
	// Open de camera met nummer 0 in lijst
	VideoCapture cap(0);

	// Controle of de camera wordt herkend.
	if (!cap.isOpened())
	{
		cout << "Cannot open the video cam" << endl;
		return -1;
	}

	// Breedte en hooogte van de frames die de camera genereert ophalen. 
	//double dWidth = cap.get(CAP_PROP_FRAME_WIDTH);
	//double dHeight = cap.get(CAP_PROP_FRAME_HEIGHT);
	//cout << "Frame size : " << dWidth << " x " << dHeight << endl;

	// Window maken waarin de beelden "live" getoond worden
	namedWindow("Live", WINDOW_AUTOSIZE);

	
	Mat frame;
	while (1)
	{
		// Continue loop waarin een beeld wordt opgehaald en wordt getoond in het window

		// Lees een nieuw frame
		bool bSuccess = cap.read(frame);
		//Flippen van frames als dit nodig is. (1 flip around y-axis), 0 (flip around x-axis), -1 (flip both axes)
		flip(frame, frame, 1);

		// Controlleer of het frame goed gelezen is.
		if (!bSuccess)
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		// Het tonen van grijswaarde beeld
		imshow("Live", frame);

		//  Wacht 30 ms op ESC-toets. Als ESC-toets is ingedrukt verlaat dan de loop
		//if (waitKey(1) == 27)
		//{
		//	cout << "esc key is pressed by user" << endl;
		//	break;
		//}

		//Wacht op een Backspace (nummer 32; http://www.asciitable.com/)
		if (waitKey(1) == 32)
		{
			cout << "Spacebar pressed by user" << endl;
			namedWindow("CurrentBottle", WINDOW_AUTOSIZE);// Create a window for display.
			imshow("CurrentBottle", frame);
		}
	}
	return 0;
}