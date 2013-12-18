#include "vol_math_ImageVolume.h"
#include "vol_math_RawImage.h"
#include "vol_math_WipeNioisePde.h" //3d ansitropic filter
#include "vol_math_BilateralFilter.h" //2d bilateralfilter
#include "vol_math_ThreeDim_Bilateral.h"//3d bilateralfilter
#include "vol_math_Filter.h"//2d \3d guass filter
#include "vol_math_trilateralfilter.h"//3d trilaterfilter
#include "vol_math_Anistropic2D.h"
#include "vol_math_2Dtrilateralfilter.h"
#include "MultiThread.h"


struct  AnistropicI
{
	int time;
	int val; //val=1
	int method; 
	int threadcount;
	/*
	method:
	1:Perona_Malik();
	2:four_diff
	*/
	AnistropicI(int time,int val,int method,int threadcount)
	{
		this->time=time;
		this->val=val;
		this->method=method;
		this->threadcount=threadcount;
	}


};
struct  TrilateralfilterI
{
	float sigmaC;//sigmaC=1
	int threadcount;
	TrilateralfilterI(float sigmaC =1)
	{
		this->sigmaC = sigmaC;
		this->threadcount=threadcount;
	}
};

struct BilateralFilterI
{
	double sigmaD;
	double sigmaR;
	int threadcount;
	BilateralFilterI(double sigmaD,double sigmaR,int threadcount)
	{
		this->sigmaD=sigmaD;
		this->sigmaR=sigmaR;
		this->threadcount=threadcount;
	}
};

struct GuassFilterI
{
	int halfsize;
	int threadcount;
	GuassFilterI(int halfsize,int threadcount)
	{
		this->halfsize=halfsize;
		this->threadcount=threadcount;
	}
};

struct lowPassI
{

};
struct MultiOstuI
{

};
extern void * doAnistropicI (ImageVolume &src,AnistropicI &);
extern void * doBilateralI (ImageVolume &, BilateralFilterI &);
extern void *doGuassFilterI (ImageVolume &, GuassFilterI &);
extern void *doTrilateralfilterI ( ImageVolume &, TrilateralfilterI &);
extern void *doMultiOstuI (ImageVolume &src,MultiOstuI &);
extern void *dolowPassI (ImageVolume &src,lowPassI &);
extern void *doAnistropicI2D (Image2D &src,AnistropicI &);
extern void *doBilateralI2D (Image2D&, BilateralFilterI &);
extern void *doGuassFilterI2D (Image2D &, GuassFilterI &);
extern void *doTrilateralfilterI2D ( Image2D &, TrilateralfilterI &);
extern void *doMultiOstuI2D (Image2D &src,MultiOstuI &);
extern void *dolowPassI2D (Image2D &src,lowPassI &);
//extern void *domultiThread (int medthod,int threadcount);
/*
1
2
3
4
5
6
7
8
9
10
11
12
*/