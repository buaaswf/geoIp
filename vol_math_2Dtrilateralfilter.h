#pragma once
#include <stdlib.h>
#include "vol_math_RawImage.h"
#include "vol_math_Raw3D_Independt.h"
#define BOOL int
#define TRUE 1
#define FALSE 0
//#define PIXTYPE unsigned char
#define M_EXP 2.7182818284590452353602874713527
class Trilateralfilter2D
{
public:
	Trilateralfilter2D(void);
	~Trilateralfilter2D(void);

	//Trilateral filter consisting of gradient filter, adaptive neighborhood
	//computation and detail filter
	Raw2D * TrilateralFilter2D(Raw2D* srcImg, PIXTYPE sigmaC); 


};

class Trilateral2d
{
	Trilateral2d();
	~Trilateral2d();
};