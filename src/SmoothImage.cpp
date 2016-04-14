/*
 * SmoothImage.cpp
 *
 *  Created on: Feb 29, 2016
 *      Author: jitete
 */
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <cv.h>
#include <highgui.h>
#include "SmoothImage.h"

using namespace std;
using namespace cv;

Mat smoothImage(const Mat &image, double sigma)
{
	//==================================================
	//compute mask size based on deviation sigma
	//==================================================
	int masksize;

	if (int(ceil(5*sigma))%2 == 0)
		masksize = ceil(5*sigma) + 1;
	else
		masksize = ceil(5*sigma);

	cout << "Gaussian mask size is " << masksize << endl;


	//==================================================
	//create Gaussian filter
	//==================================================
	double filter[masksize][masksize];
	int mask[masksize][masksize];  //normalized (integer) mask
	int A=0;                       //scaling factor
	int m = masksize/2;

	for (int i=-m; i<=m; i++)
	{
		for (int j=-m; j<=m; j++)
		{
			filter[i+m][j+m] = exp(-(i*i+j*j)/(2*sigma*sigma));
			mask[i+m][j+m] = round(filter[i+m][j+m]/filter[0][0]);
			A = A + mask[i+m][j+m];
			cout << mask[i+m][j+m] << " ";
			if (j==m) cout << endl;
		}
	}
	cout << "Scaling factor is 1/" << A << endl;
;


	//=================================================
	//smooth image using Gaussian filter
	//=================================================
	Mat sImage = image.clone();
	int sum;
	int intensity;

	for (int x=m; x<image.rows-m; x++)
	{
		for (int y=m; y<image.cols-m; y++)
		{
			sum = 0;
			for (int u=-m; u<=m; u++)
			{
				for (int v=-m; v<=m; v++)
				{
					intensity = image.at<uchar>(x+u,y+v);
					sum = sum + mask[u+m][v+m] * intensity;
				}
			}
			sImage.at<uchar>(x,y) = sum/A;
		}
	}

	return sImage;
}
