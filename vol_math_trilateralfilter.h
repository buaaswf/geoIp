#ifndef VOL_MATH_TRILATERLFILTER_H
#define VOL_MATH_TRILATERLFILTER_H
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <assert.h>
#include "vol_math_RawImage.h"
#include <vector>
#include <pthread.h>
#define M_EXP 2.7182818284590452353602874713527
class RawArray;
struct Tri_para 
{
	Raw *src;
	PIXTYPE sigmaC;
	Tri_para()
	{

	}
	Tri_para(Raw *src, PIXTYPE sigmaC)
	{
		this->src = src;
		this->sigmaC = sigmaC;
	}
};
class Trilateralfilter
{
public:
	Trilateralfilter(Raw* RawImage);
	~Trilateralfilter(void);
	void TrilateralFilter(PIXTYPE sigmaC); 
	void TrilateralFilter_Multi(PIXTYPE sigmaC,int threadcount);
	
private:
	//Computes X , Y and Z gradients of the input RawImage
	void ComputeGradients(Raw* pX, Raw* pY,Raw *pZ); 

	//Bilaterally filters  gradients pX and pY 
	void BilateralGradientFilter(Raw* pX, Raw* pY,Raw*pZ, Raw* pSmoothX, 
		Raw* pSmoothY,Raw * pSmoothZ,float sigmaC,float sigmaS, int filterSize); //sigmaC,sugmaS is change  from PIXTYPE to float

	//Builds the stack of min-max RawImage gradients; returns the range variance
	PIXTYPE buildMinMaxImageStack(Raw* pX, Raw* pY, Raw* pZ,RawArray* pMinStack,
		RawArray* pMaxStack , int levelMax, float beta);		// beta is changed from PIXTYPE to float

	//Finds the adaptive neighborhood size (stack level) 
	//from the min-max gradient stack
	void findAdaptiveRegion(RawArray* pMinStack, RawArray* pMaxStack, PIXTYPE R, int levelMax); 

	//Filters the detail signal and computes the final output RawImage	
	void DetailBilateralFilter(Raw* srcImg, Raw* pSmoothX, Raw* pSmoothY, Raw* pSmoothZ, 
		Raw* fTheta, float sigmaCTheta, float sigmaRTheta); // sigmaCTheta sigmaRTheta is changed from PIXTYPE to float

	RawImage* img;
	Raw* raw;
	RawArray* rawarraydata;
	/*Raw* rawarray(RawImage* img)
	{
		int xsize=img->width;
		int ysize=img->height;
		int zsize=img->length;
		PIXTYPE *yy=img->buf;

		int i=0,j=0,k=0,kk=0;
		int thread=100;
		for(kk=0;kk<(zsize/thread);kk++)
		{
			try
			{
				PIXTYPE *p=new PIXTYPE[xsize*ysize*thread];
				for(i=0;i<xsize*ysize*thread;i++)
				{
					*(p+i)=*(yy+i+kk*(xsize*ysize*thread));
				}	
				try
				{
					Raw *rawData=new Raw(xsize,ysize,thread,p);
					return rawData;
				}
				catch(std::bad_alloc)
				{
					delete [] p;
					p=NULL;
					i--;
				}
				printf("no  %d\n" ,i+1);
			}
			catch (const std::bad_alloc& e)
			{
				printf("p alloc failed");
			}
		}
	}*/
};
#endif 





