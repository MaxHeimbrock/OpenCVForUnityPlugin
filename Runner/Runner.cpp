// Runner.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include "Header.h"

#include "opencv2/highgui.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;

int main()
{
    std::cout << "Hello World!\n" << std::endl;

	Mat frame;

	int* resolution = new int[2];
	float* bottlecap = new float[2];
	float* plane = new float[8];

	// USE DLL
	/*
	InitCap(resolution);
		
	std::cout << "Resolution: " << std::to_string(resolution[0]) << " x " << std::to_string(resolution[1]) << std::endl;

	while (true) {
		
		//frame = GetFrameRunner();

		//FindBottleCap(frame, bottlecap);
		FindPlane(frame, plane);

		imshow("Main Frame", frame);
		//std::cout << "Bottlecap: " << std::to_string(bottlecap[0]) << " x " << std::to_string(bottlecap[1]) << std::endl;
		std::cout << "Plane: " << std::to_string(plane[0]) << ", " << std::to_string(plane[1]) << ", " << std::to_string(plane[2]) << std::endl;
		
		char key = (char)waitKey(33);
		if (key == 27) break;
	}
	*/

	// OPEN FRONT CAMERA
	/*
	VideoCapture cap = VideoCapture(0);

	cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 720);

	frame = cap.read(frame);
	std::cout << "Resolution: " << std::to_string(frame.size().width) << " x " << std::to_string(frame.size().height) << std::endl;

	while (cap.read(frame)) {
		FindPlane(frame, plane);
		imshow("Main Frame", frame);

		char key = (char)waitKey(33);   
		if (key == 27) break;
	}
	//*/

	// USE MARKERMOVIE
	///*
	VideoCapture cap;
	cap.open("MarkerMovie.mp4");
	if (cap.isOpened() == false) {
		cout << "No video!" << endl;
		exit(0);
		return -1;
	}

	while (cap.read(frame)) {
		FindPlane(frame, plane);
		imshow("Main Frame", frame);
		/*
		//std::cout << "Plane: " << std::to_string(plane[0]) << ", " << std::to_string(plane[1]) << ", " << std::to_string(plane[2]) << std::endl;
		std::cout << "1: (" << std::to_string(plane[0]) << ", " << std::to_string(plane[1]) << ")";
		std::cout << " (" << std::to_string(plane[2]) << ", " << std::to_string(plane[3]) << ")";
		std::cout << " (" << std::to_string(plane[4]) << ", " << std::to_string(plane[5]) << ")";
		std::cout << " (" << std::to_string(plane[6]) << ", " << std::to_string(plane[7]) << ")" << std::endl;
		*/
		char key = (char)waitKey(33);
		if (key == 27) break;
	}
	//*/

	delete(resolution);
	delete(bottlecap);
	delete(plane);

	std::cout << "End\n" << std::endl;
}
