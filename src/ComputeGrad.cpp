/*
 * Gradient.cpp
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
#include "ComputeGrad.h"

using namespace std;
using namespace cv;

const double     PI = 3.141592653;
const int THRESHOLD = 100;
const int MASKSIZE  = 3;

int convolution(const Mat &image, int x, int y, int mask[][MASKSIZE]);

void computeGrad(const Mat image)
{
	int xMask[][3] = {{-1,-2,-1}, {0,0,0}, {1,2,1}};
	int yMask[][3] = {{-1,0,1}, {-2,0,2}, {-1,0,1}};

	int row=image.rows, col=image.cols;

	int xGrad[row][col], yGrad[row][col], strength[row][col];
	for (int x=0; x<row; x++)
	{
		for (int y=0; y<col; y++)
		{
			xGrad[x][y]=yGrad[x][y]=strength[x][y]=image.at<uchar>(x,y);
		}
	}

	Mat xEdge, yEdge, edgeMap;
	xEdge=image.clone();
	yEdge=image.clone();
	edgeMap=image.clone();
	uchar intensity;
	int sum;

	for (int x=1; x<row-1; x++)
	{
		for (int y=1; y<col-1; y++)
		{
			sum = 0;
			for (int u=-1; u<=1; u++)
			{
				for (int v=-1; v<=1; v++)
				{
					intensity = image.at<uchar>(x+u,y+v);
					sum = sum + xMask[u+1][v+1] * intensity;
				}
			}
			xGrad[x][y] = abs(sum);
		}
	}


	for (int x=1; x<image.rows-1; x++)
	{
		for (int y=1; y<image.cols-1; y++)
		{
			if (xGrad[x][y]>xGrad[x-1][y] && xGrad[x][y]>xGrad[x+1][y] && xGrad[x][y]>60)
				xEdge.at<uchar>(x,y) = 255;
			else
				xEdge.at<uchar>(x,y) = 0;
		}
	}

	for (int x=1; x<row-1; x++)
	{
		for (int y=1; y<col-1; y++)
		{
			sum = 0;
			for (int u=-1; u<=1; u++)
			{
				for (int v=-1; v<=1; v++)
				{
					intensity = image.at<uchar>(x+u,y+v);
					sum = sum + yMask[u+1][v+1] * intensity;
				}
			}
			yGrad[x][y] = abs(sum);
		}
	}

	for (int x=1; x<image.rows-1; x++)
	{
		for (int y=1; y<image.cols-1; y++)
		{
			if (yGrad[x][y]>yGrad[x][y-1] && yGrad[x][y]>yGrad[x][y+1] && yGrad[x][y]>60)
				yEdge.at<uchar>(x,y) = 255;
			else
				yEdge.at<uchar>(x,y) = 0;
		}
	}

	double theta;
	for (int x=1; x<row-1; x++)
	{
		for (int y=1; y<col-1; y++)
		{
//			delta_x = convolution(image, x, y, xMask);
			//delta_y = convolution(image, x, y, yMask);
			//delta = abs(delta_x) + abs(delta_y);
			strength[x][y] = xGrad[x][y]+yGrad[x][y];
		}
	}

	for (int x=1; x<image.rows-1; x++)
	{
		for (int y=1; y<image.cols-1; y++)
		{
			if (xGrad[x][y]==0) theta = 90;
			else theta = atan(double(yGrad[x][y])/double(xGrad[x][y]))*180/3.141592653;

			if (theta>-22.5 && theta<=22.5)
			{
				if ((strength[x][y])>(strength[x-1][y]) && (strength[x][y])>(strength[x+1][y]) && strength[x][y] > THRESHOLD)
					edgeMap.at<uchar>(x,y) = 255;
				else
					edgeMap.at<uchar>(x,y) = 0;
			}
			if ((theta>-90 && theta <=-67.5) || (theta>67.5 && theta<=90))
			{
				if ((strength[x][y])>(strength[x][y-1]) && (strength[x][y])>(strength[x][y+1]) && strength[x][y] > THRESHOLD)
					edgeMap.at<uchar>(x,y) = 255;
				else
					edgeMap.at<uchar>(x,y) = 0;
			}
			if (theta>-67.5 && theta <=-22.5)
			{
				if ((strength[x][y])>(strength[x+1][y-1]) && (strength[x][y])>(strength[x-1][y+1]) && strength[x][y] > THRESHOLD)
					edgeMap.at<uchar>(x,y) = 255;
				else
					edgeMap.at<uchar>(x,y) = 0;
			}
			if (theta>22.5 && theta <=67.5)
			{
				if ((strength[x][y])>(strength[x-1][y-1]) && (strength[x][y])>(strength[x+1][y+1]) && strength[x][y] > THRESHOLD)
					edgeMap.at<uchar>(x,y) = 255;
				else
					edgeMap.at<uchar>(x,y) = 0;
			}
		}
	}


	imshow("ximage", xEdge);
	imshow("yimage", yEdge);
	imshow("gradient image", edgeMap);

}

int convolution(const Mat &image, int x, int y, int mask[][MASKSIZE])
{
	int sum=0, m=1;
	uchar intensity;

	for (int u=-m; u<=m; u++)
		{
			for (int v=-m; v<=m; v++)
			{
				intensity = image.at<uchar>(x+u,y+v);
				sum = sum + mask[u+m][v+m] * intensity;
			}
		}

	return sum;
}

