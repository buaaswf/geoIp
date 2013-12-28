#ifndef THREEDIM_BILATERAL
#define THREEDIM_BILATERAL

#include "vol_math_Raw3D_Independt.h"
#include "vol_math_RawImage.h"

class ThreeDim_Bilateral
{
	
private:
	Raw src;
	double kernelRadius;
	double *** kernelD;
	double *gaussSimilarity;
public:
	ThreeDim_Bilateral(Raw *image,double sigmaD, double sigmaR);
	~ThreeDim_Bilateral(void);
	void apply(Raw &src);
	bool isInsideBoundaries(int m,int n,int l);
	double similarity(int p,int s);
	double gauss(double sigma,int x, int y,int z);
	double ThreeDim_Bilateral::getSpatialWeight(int m, int n,int l,int i,int j,int k);
};
#endif