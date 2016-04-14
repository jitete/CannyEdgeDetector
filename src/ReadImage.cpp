/*
 * ReadImage.cpp
 *
 *  Created on: Mar 3, 2016
 *      Author: jitete
 */
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cv.h>
#include <highgui.h>
#include "ReadImage.h"

using namespace std;
using namespace cv;

Mat readImage(char *filename)
{
	FILE *fp = NULL;
	fp = fopen(filename, "rb");

	short resolution[2], width, height;
	fread(resolution, sizeof(short), 2, fp);
	width = resolution[0];
	height = resolution[1];
	cout << "The image size is ";
	cout << width << "X" << height << endl;

	uchar imgdata[width*height];
	fseek(fp, 5, SEEK_SET);
	fread(imgdata, sizeof(uchar), width*height, fp);

	Mat img(height, width, CV_8UC1, imgdata);
	Mat image = img.clone();

	fclose(fp);

	return image;
}
