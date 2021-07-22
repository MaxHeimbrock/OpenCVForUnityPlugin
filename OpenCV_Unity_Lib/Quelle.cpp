#include "Header.h"
#include <stdio.h>
#include <iostream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

VideoCapture *cap;
vector<float> *testVector;

void ProcessImage(Mat image)
{
	Mat image_gray;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	cvtColor(image, image_gray, COLOR_BGR2GRAY);
	
	Mat canny_output;
	Canny(image_gray, canny_output, 100, 200);	
	findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);	

	Scalar color = Scalar(250, 0, 0);
	for (size_t i = 0; i < contours.size(); i++)
	{		
		drawContours(image, contours, (int)i, color, 2, LINE_8, hierarchy, 0);
	}
}

extern "C"
{
	__declspec(dllexport) void FindBottleCap(Mat image, float* result)
	{
		Scalar lowerColor = Scalar(40, 90, 30);
		Scalar upperColor = Scalar(100, 255, 255);

		Mat image_hsv;
		Mat image_inRange;
		Mat image_gray;

		cvtColor(image, image_gray, COLOR_BGR2GRAY);
		cvtColor(image, image_hsv, COLOR_BGR2HSV);

		inRange(image_hsv, lowerColor, upperColor, image_inRange);
		bitwise_and(image_gray, image_inRange, image_gray);

		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		findContours(image_gray, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

		// If no contour found
		if (contours.size() == 0)
		{
			result[0] = -1;
			result[1] = -1;
		}
		else
		{
			int maxContourId = getMaxAreaContourId(contours);

			Scalar color = Scalar(250, 0, 0);
			drawContours(image, contours, maxContourId, color, 2, LINE_8, hierarchy, 0);

			Moments m = cv::moments(contours[maxContourId]);

			float cx, cy;
			if ((m.m00 * m.m01 * m.m10) != 0)
			{
				cx = m.m10 / m.m00;   cy = m.m01 / m.m00;

				cv::drawMarker(image, Point(cx, cy), Scalar(0, 250, 0), MARKER_CROSS, 10);

				result[0] = cx;
				result[1] = cy;
			}
		}
	}
	__declspec(dllexport) void FindPlane(Mat image, float* result)
	{
		for (int i = 0; i < 8; i++)
		{
			result[i] = -1;
		}

		Scalar lowerColor = Scalar(90, 150, 70);
		Scalar upperColor = Scalar(140, 255, 255);

		Mat image_hsv;
		Mat image_inRange;
		Mat image_gray;

		cvtColor(image, image_gray, COLOR_BGR2GRAY);
		cvtColor(image, image_hsv, COLOR_BGR2HSV);

		inRange(image_hsv, lowerColor, upperColor, image_inRange);
		bitwise_and(image_gray, image_inRange, image_gray);

		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		findContours(image_gray, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

		// If no contour found
		if (contours.size() == 0)
		{
			return;
		}
		else
		{
			int maxContourId = getMaxAreaContourId(contours);
			vector<Point> maxContour = contours[maxContourId];

			Scalar color = Scalar(250, 0, 0);
			//drawContours(image, contours, maxContourId, color, 2, LINE_8, hierarchy, 0);

			vector<Point> poly;
			cv::approxPolyDP(maxContour, poly, 10, true);
			cv::polylines(image, poly, true, color, 2, LINE_8, 0);

			if (poly.size() != 4)
			{
				return;
			}
			else
			{
				vector<Point2f> imagePoints;
				imagePoints.push_back(poly.at(0));
				imagePoints.push_back(poly.at(1));
				imagePoints.push_back(poly.at(2));
				imagePoints.push_back(poly.at(3));

				vector<Point3f> objectPoints;
				objectPoints.push_back(Point3f(0, 0, 0));
				objectPoints.push_back(Point3f(210, 0, 0));
				objectPoints.push_back(Point3f(210, 297, 0));
				objectPoints.push_back(Point3f(0, 297, 0));

				cv::Mat_<float> cameraIntrinsics(3,3);
				cameraIntrinsics(0, 0) = 982.18440222970639;
				cameraIntrinsics(0, 1) = 0;
				cameraIntrinsics(0, 2) = 662.60715208248371;
				cameraIntrinsics(1, 0) = 0;
				cameraIntrinsics(1, 1) = 980.06566941461074;
				cameraIntrinsics(1, 2) = 341.91458999493193;
				cameraIntrinsics(2, 0) = 0;
				cameraIntrinsics(2, 1) = 0;
				cameraIntrinsics(2, 2) = 1;

				vector<float> dist;
				dist.push_back(0.058486684466621953);
				dist.push_back(0.0015971215896586845);
				dist.push_back(0.0013990238034683347);
				dist.push_back(-0.00017088115369920796);
				dist.push_back(-0.35689872046512439);
		
				Mat rvec;
				Mat rmat;
				Mat tvec;

				//vector<float> rvec;
				//vector<float> tvec;

				// did not work with my distortion vector, results were all -1218301280362073601
				cv::solvePnP(objectPoints, imagePoints, cameraIntrinsics, Mat(4, 1, CV_64FC1, Scalar(0)), rvec, tvec, false);

				Rodrigues(rvec, rmat);

				//std::cout << "tvec: " << std::to_string(tvec.at(0)) << ", " << std::to_string(tvec.at(1)) << ", " << std::to_string(tvec.at(2)) << std::endl;

				// Mat 3x1 is the print here
				//std::cout << "tvec: " << std::to_string(tvec.rows) << ", " << std::to_string(tvec.cols) << std::endl;
				// type is CV_32F
				//std::cout << "tvec: " << std::to_string(tvec.type()) << std::endl;

				// Mat 3x3 is the print here
				//std::cout << "tvec: " << std::to_string(rmat.rows) << ", " << std::to_string(rmat.cols) << std::endl;

				//std::cout << "tvec: " << std::to_string(rmat.at<float>(0, 0)) << ", " << std::to_string(rmat.at<float>(1, 0)) << ", " << std::to_string(rmat.at<float>(2, 0)) << std::endl;

				// pass forward vector
				result[2] = rmat.at<float>(0, 2);
				result[3] = rmat.at<float>(1, 2);
				result[4] = rmat.at<float>(2, 2);										  
				result[5] = rmat.at<float>(0, 1);
				result[6] = rmat.at<float>(1, 1);
				result[7] = rmat.at<float>(2, 1);										  
				result[8] = tvec.at<float>(0, 0);
				result[9] = tvec.at<float>(1, 0);
				result[10] = tvec.at<float>(2, 0);
			}
		}
	}
	
	__declspec(dllexport) void InitCap(int* resolution)
	{		
		//cap = new VideoCapture(0);
		cap = new VideoCapture("MarkerMovie.mp4");
		
		cap->set(CV_CAP_PROP_FRAME_WIDTH, 1280);
		cap->set(CV_CAP_PROP_FRAME_HEIGHT, 720);

		Mat queryFrame;
		cap->read(queryFrame);

		resolution[0] = queryFrame.size().width;
		resolution[1] = queryFrame.size().height;
	}

	__declspec(dllexport) uint8_t* GetFrameUnity(float* results)
	{
		Mat image;
		Mat betterImage;

		uint8_t* unityFrame;

		(*cap) >> image;

		flip(image, image, 0);
		flip(image, image, 1);

		FindBottleCap(image, results);
		FindPlane(image, results);

		cvtColor(image, betterImage, CV_BGR2BGRA, 4);

		unityFrame = new uint8_t[image.cols * image.rows * 4];

		memcpy(unityFrame, betterImage.data, betterImage.cols * image.rows * 4);
		return unityFrame;
	}

	__declspec(dllexport) Mat GetFrameRunner()
	{
		Mat image;
		Mat betterImage;
		
		(*cap) >> image;

		flip(image, image, 1);

		//ProcessImage(image);
		//FindBottleCap(image);

		cvtColor(image, betterImage, CV_BGR2BGRA, 4);

		return betterImage;
	}
}

int getMaxAreaContourId(vector <vector<cv::Point>> contours) {
	double maxArea = 0;
	int maxAreaContourId = -1;
	for (int j = 0; j < contours.size(); j++) {
		double newArea = cv::contourArea(contours.at(j));
		if (newArea > maxArea) {
			maxArea = newArea;
			maxAreaContourId = j;
		} // End if
	} // End for
	return maxAreaContourId;
} // End function


