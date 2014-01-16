#ifndef THREEDIM_BILATERAL
#define THREEDIM_BILATERAL

#include "vol_math_Raw3D_Independt.h"
#include "vol_math_RawImage.h"
#include "vol_math_filter_Interface.h"
class ThreeDim_Bilateral
{
	
private:
	Raw *src;
	Raw *ret;
	Raw *temp;
	double kernelRadius;
	double *** kernelD;
	double *gaussSimilarity;
public:
	void (*ProgressChanged)(int ,int ,int ,bool &);
	ThreeDim_Bilateral(Raw *src,double sigmaD, double sigmaR);
	ThreeDim_Bilateral(Raw *image,Raw &ret,double sigmaD, double sigmaR,void(*ProgressChanged)(int,int,int,bool &));
	~ThreeDim_Bilateral(void);
	void apply(Raw &ret);
	void applySipl(int iter);
	bool isInsideBoundaries(int m,int n,int l);
	double similarity(int p,int s);
	double gauss(double sigma,int x, int y,int z);
	double getSpatialWeight(int m, int n,int l,int i,int j,int k);
};
#endif