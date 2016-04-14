/*
 * CannyEdgeDetector.cpp
 *
 *  Created on: Mar 3, 2016
 *      Author: jitete
 */

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <cv.h>
#include <highgui.h>
#include "gradient.h"

using namespace std;
using namespace cv;

Gradient::Gradient(Mat &img, int min, int max)
{
	xMask = (Mat_<int>(3,3) << -1,-2,-1, 0,0,0, 1,2,1);
	yMask = (Mat_<int>(3,3) << -1,0,1, -2,0,2, -1,0,1);

	lowT = min;
	highT = max;

	aImage = img;
	aRow = img.rows;
	aCol = img.cols;

	xEdge = img.clone();
	yEdge = img.clone();
	edgeMap = img.clone();
}

void Gradient::computeGradx()
{
	int xGrad[aRow][aCol];
//	for (int x=0; x<aRow; x++)
//	{
//		for (int y=0; y<aCol; y++)
//		{
//			xGrad[x][y]=0;
//		}
//	}

	for (int x=1; x<aRow-1; x++)
	{
		for (int y=1; y<aCol-1; y++)
		{
			xGrad[x][y] = convolve(x, y, xMask);
			xEdge.at<uchar>(x,y) = abs(xGrad[x][y])/4;
		}
	}


/*  edge with non-Maximum suppression and Threshold
	for (int x=1; x<aRow-1; x++)
	{
		for (int y=1; y<aCol-1; y++)
		{
			if (xGrad[x][y]>xGrad[x-1][y] && xGrad[x][y]>xGrad[x+1][y] && xGrad[x][y]>60)
				xEdge.at<uchar>(x,y) = 255;
			else
				xEdge.at<uchar>(x,y) = 0;
		}
	}
*/
	imshow("xGradient Image", xEdge);
}

void Gradient::computeGrady()
{
	int yGrad[aRow][aCol];
	for (int x=0; x<aRow; x++)
	{
		for (int y=0; y<aCol; y++)
		{
			yGrad[x][y]=0;
		}
	}

	for (int x=1; x<aRow-1; x++)
	{
		for (int y=1; y<aCol-1; y++)
		{
			yGrad[x][y] = convolve(x, y, yMask);
			yEdge.at<uchar>(x,y) = abs(yGrad[x][y])/4;
		}
	}

	imshow("yGradient Image", yEdge);
}

void Gradient::computeEdgeMap()
{
	double strength[aRow][aCol];
	double thinEdge[aRow][aCol];
	double theta[aRow][aCol];
	double delta_x, delta_y;

	for (int x=0; x<aRow; x++)
	{
		for (int y=0; y<aCol; y++)
		{
			strength[x][y]=aImage.at<uchar>(x,y);
			thinEdge[x][y]=aImage.at<uchar>(x,y);
			theta[x][y]=0;
		}
	}

	//===========================================
	//compute gradient magnitude and direction
	//===========================================
	for (int x=1; x<aRow-1; x++)
	{
		for (int y=1; y<aCol-1; y++)
		{
			//compute edge strength
			delta_x = 0.25*convolve(x, y, xMask);
			delta_y = 0.25*convolve(x, y, yMask);
			strength[x][y] = abs(delta_x) + abs(delta_y);
			thinEdge[x][y] = strength[x][y];

			//compute gradient direction
			if (delta_x == 0) theta[x][y] = 90;
			else theta[x][y] = atan(delta_y/delta_x)*180/M_PI;

			//to display the edge strength image in 8UC1 format
			if (strength[x][y]>255)
				edgeMap.at<uchar>(x,y)=255;
			else
				edgeMap.at<uchar>(x,y) = (int)strength[x][y];
		}
	}
	imshow("Edge Strength Map", edgeMap);


	//===========================================
	//non-Maximum Suppression
	//===========================================
	for (int x=1; x<aRow-1; x++)
	{
		for (int y=1; y<aCol-1; y++)
		{
			//gradient direction is 0 degree
			if (theta[x][y]>-22.5 && theta[x][y]<=22.5)
			{
				if (strength[x][y] < strength[x-1][y] || strength[x][y] < strength[x+1][y])
				{
					thinEdge[x][y] = 0;
					edgeMap.at<uchar>(x,y) = 0;
				}
			}
			//gradient direction is 90 degree
			else if ((theta[x][y]>-90 && theta[x][y]<=-67.5) || (theta[x][y]>67.5 && theta[x][y]<=90))
			{
				if (strength[x][y] < strength[x][y-1] || strength[x][y] < strength[x][y+1])
				{
					thinEdge[x][y] = 0;
					edgeMap.at<uchar>(x,y) = 0;
				}
			}
			//gradient direction is 135 degree
			else if (theta[x][y]>-67.5 && theta[x][y]<=-22.5)
			{
				if (strength[x][y] < strength[x+1][y-1] || strength[x][y] < strength[x-1][y+1])
				{
					thinEdge[x][y] = 0;
					edgeMap.at<uchar>(x,y) = 0;
				}
			}
			//gradient direction is 45 degree
			else if (theta[x][y]>22.5 && theta[x][y]<=67.5)
			{
				if (strength[x][y] < strength[x-1][y-1] || strength[x][y] < strength[x+1][y+1])
				{
					thinEdge[x][y] = 0;
					edgeMap.at<uchar>(x,y) = 0;
				}
			}
			else printf("data error!!!\n");
		}
	}
	imshow("Thinned Edge", edgeMap);

	//================================================
	//Double Thresholding and Hysteresis
	//================================================
	for (int x=1; x<aRow-1; x++)
	{
		for (int y=1; y<aCol-1; y++)
		{
			if (thinEdge[x][y] <= lowT)
				edgeMap.at<uchar>(x,y) = 0;

			else if (thinEdge[x][y] >= highT)
				edgeMap.at<uchar>(x,y) = 255;

			else
				edgeMap.at<uchar>(x,y) = (int)thinEdge[x][y];
		}
	}
	imshow("Double Threshold Edge", edgeMap);

	//connected weak edges are marked as edges
	for (int x=1; x<aRow-1; x++)
	{
		for (int y=1; y<aCol-1; y++)
		{
			if (thinEdge[x][y] >= highT)   //contour starts from strong edge
				hysteresis(x, y, lowT, highT);
		}
	}
	//the rest weak edges are suppressed
	for (int x=1; x<aRow-1; x++)
	{
		for (int y=1; y<aCol-1; y++)
		{
			if (edgeMap.at<uchar>(x,y) > lowT && edgeMap.at<uchar>(x,y) < highT)
				edgeMap.at<uchar>(x,y) = 0;
		}
	}
//	imwrite("/home/jitete/Desktop/EE4208results/EdgeMap.png", edgeMap);
	imshow("Edge Map", edgeMap);
}

void Gradient::hysteresis(int x, int y, int lowT, int highT)
{
	for (int u=-1; u<=1; u++)
	{
		for (int v=-1; v<=1; v++)
		{
			if (edgeMap.at<uchar>(x+u,y+v) > lowT && edgeMap.at<uchar>(x+u,y+v) < highT)
				{
					edgeMap.at<uchar>(x+u,y+v) = 255;
					hysteresis(x+u,y+v, lowT, highT);
				}
		}
	}
	return;
}

int Gradient::convolve(int x, int y, Mat mask)
{
	int sum=0;
	int intensity;

	for (int u=-1; u<=1; u++)
		{
			for (int v=-1; v<=1; v++)
			{
				intensity = aImage.at<uchar>(x+u,y+v);
				sum = sum + mask.at<int>(u+1,v+1) * intensity;
			}
		}

	return sum;
}
