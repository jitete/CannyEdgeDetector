/*
 * LoG.cpp
 *
 *  Created on: Mar 28, 2016
 *      Author: jitete
 */
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <cv.h>
#include <highgui.h>
#include "LoG.h"

using namespace std;
using namespace cv;

//int Lmask[3][3] = {{ 2,-1,2}, {-1,-4,-1}, {2,-1,2}};
//int Lmask[3][3] = {{ 0,1,0}, {1,-4,1}, {0,1,0}};
int Lmask[3][3] = {{ 1,1,1}, {1,-8,1}, {1,1,1}};
int T=5;

//HOW TO DECIDE ZERO-CROSSINGS IN GRID??
void LoGEdgeMap(const Mat &image)
{
	int m=1;
	int sum;
	int intensity;
	Mat LoGimage(image.rows, image.cols, CV_32F);

	//compute Laplacian of Gaussian smoothed image
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
					sum = sum + Lmask[u+m][v+m] * intensity;
				}
			}
			LoGimage.at<float>(x,y) = sum;
		}
	}

//	Mat logmap;
//	convertScaleAbs(LoGimage,logmap);
//	imshow("LoG Operator Computation", logmap);

	Mat edgemap(image.rows, image.cols, CV_8U, Scalar(0));

	for (int x=m; x<image.rows-m; x++)
	{
		for (int y=m; y<image.cols-m; y++)
		{
			//zero-crossing always associated with a positive and a negative LoG
			//check only negative pixel to reduce computation
			if(LoGimage.at<float>(x,y)<0)
			{
				for (int u=-1; u<=1; u++)
				{
					for (int v=-1; v<=1; v++)
					{
						//zero-crossing occurs where LoG changes sign
						if (LoGimage.at<float>(x+u,y+v)>0)
						{
							//edge assigned to the pixel with smaller magnitude
							if(abs(LoGimage.at<float>(x,y)) <= abs(LoGimage.at<float>(x+u,y+v)))
								edgemap.at<uchar>(x,y)=255;
							else
								edgemap.at<uchar>(x+u,y+v)=255;
						}
					}
				}
			}
		}
	}

	imshow("LoG Edge Map", edgemap);
	for (int x=100; x<120; x++)
	{
		for (int y=80; y<100; y++)
		{
			cout << LoGimage.at<float>(x,y) << '('<<int(image.at<uchar>(x,y))<<')' << " ";
		}
		cout << endl;
	}
}
