/*
 * EdgeDetector.cpp
 *
 *  Created on: Mar 2, 2016
 *      Author: jitete
 */
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <cv.h>
#include <highgui.h>

#include "ReadImage.h"
#include "SmoothImage.h"
#include "gradient.h"
#include "LoG.h"

using namespace std;
using namespace cv;

#define LowThreshold  10
#define HighThreshold 25

int main(int argc, char **argv)
{
	double sigma;
	cout << "User-specified standard deviation: ";
	cin >> sigma;

	Mat image = readImage(argv[1]);
	Mat sImage = smoothImage(image, sigma);

	Gradient gradient(sImage, LowThreshold, HighThreshold);

	gradient.computeGradx();
	gradient.computeGrady();
	gradient.computeEdgeMap();

//	LoGEdgeMap(sImage);

	imshow("Raw Image", image);
	imshow("Smoothed Image", sImage);

	for(;;)
	{
		char q = waitKey(-1);
		if (q == 'q')
			break;
	}
	return 0;
}
