#ifndef INTERFACE_SWF
#define INTERFACE_SWF

//#include "KDetectMemoryLeak.h"
#include "vol_math_ImageVolume.h"
#include "vol_math_RawImage.h"
//#include "vol_math_WipeNioisePde.h" //3d ansitropic filter
//#include "vol_math_BilateralFilter.h" //2d bilateralfilter
//#include "vol_math_ThreeDim_Bilateral.h"//3d bilateralfilter
//#include "vol_math_GuassFilter.h"//2d \3d guass filter
//#include "vol_math_trilateralfilter.h"//3d trilaterfilter
//#include "vol_math_Anistropic2D.h"
//#include "vol_math_2Dtrilateralfilter.h"
//#include "vol_math_Otsu.h"
//#include"vol_math_FourierFilter2.h"
//#include "vol_math_FourierFilter3.h"

struct lowPassI
{
	float threshold;
	lowPassI(float threshold)
	{
		this->threshold =threshold;
	}


};

extern void *dolowPassI (ImageVolume *src,lowPassI &);


#endif
