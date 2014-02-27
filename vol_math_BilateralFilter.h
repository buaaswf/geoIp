#pragma once
#include "vol_math_RawImage.h"
#include "vol_math_Raw3D_Independt.h"
#define M_EXP 2.6
class BilateralFilter
{
private:
	Raw2D src;
	double kernelRadius;
	double ** kernelD;
	double *gaussSimilarity;
public:
	BilateralFilter(Raw2D *image,double sigmaD, double sigmaR);
	~BilateralFilter(void);
	void BilateralFilter_Struct_Oriented(Raw2D &src);
	//Raw2D & runFilter(Raw2D &src );
	void apply(Raw2D &src);
	bool isInsideBoundaries(int m,int n);
	double similarity(int p,int s);
	double gauss(double sigma,int x, int y);
	double BilateralFilter::getSpatialWeight(int m, int n,int i,int j);
	 void bilateral(Raw *temp);

};


