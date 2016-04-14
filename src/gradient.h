/*
 * CannyEdgeDetector.h
 *
 *  Created on: Mar 3, 2016
 *      Author: jitete
 */

#ifndef SRC_GRADIENT_H_
#define SRC_GRADIENT_H_

class Gradient
{
	public:
		Gradient(cv::Mat &img, int min, int max);
		void computeGradx();
		void computeGrady();
		void computeEdgeMap();

	private:
		void hysteresis(int x, int y, int lowT, int highT);
		int convolve(int x, int y, cv::Mat mask);

		int aRow, aCol;
		int lowT, highT;
		cv::Mat xMask, yMask;
		cv::Mat aImage, xEdge, yEdge, edgeMap;
};


#endif /* SRC_GRADIENT_H_ */
