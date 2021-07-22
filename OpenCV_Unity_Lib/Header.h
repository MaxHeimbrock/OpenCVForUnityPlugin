#pragma once

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

struct Vector3Unity
{
	float x;
	float y;
	float z;
};

void ProcessImage(Mat image);
extern "C" __declspec(dllexport) void FindBottleCap(Mat image, float* bottlecap);
extern "C" __declspec(dllexport) void FindPlane(Mat image, float* plane);
int getMaxAreaContourId(vector <vector<cv::Point>> contours);

extern "C" __declspec(dllexport) void InitCap(int* resolution);
extern "C" __declspec(dllexport) uint8_t* GetFrameUnity(float* results);
extern "C" __declspec(dllexport) Mat GetFrameRunner();