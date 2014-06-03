#include <stdio.h>

//#include "CImg.h"
//#include "test.h"
#include <assert.h>
#include "vol_math_filter_interface.h"
#include <stdio.h>
#include <tchar.h>
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "KDetectMemoryLeak.h"
void testRaw2D()
{
	int l = 281, m = 481, n =1; 
	RawImage test;
	unsigned char * indata = new unsigned char [l*m*n];
	//unsigned char  *result = indata; 
	//test.readImage(indata,"F:\\3DVdata\\1\\mig100.3dv.raw",l*m*n*sizeof(unsigned char));//G:\geo\data
	test.readImage(indata,"F:\\lab\\VTKproj\\mig.raw",l*m*sizeof(unsigned char));
	//test.readImage(indata,"G:\\geo\\data\\mig.vol",l*m*n*sizeof(unsigned char));//G:\geo\data
	//PIXTYPE **slice =new PIXTYPE *[n];
	//for (int k = 0; k < n; k++)
	//{
	//	slice[k] = new PIXTYPE[l*m];
	//	memcpy(slice[k],result,l*m);
	//	//slice++;
	//	result += l*m;
	//}
	//ImageVolume *imagevol=new ImageVolume(l,m,n,1,indata);

	//Process para(1,l,m,3,slice,indata,3,3,3,3,2);
	//Raw  * ret = testinterface1(para);
	unsigned char * outdata = new unsigned char[l*m*n]; 
	Image2D *src = new Image2D(l,m,1);
	memcpy(src->data,indata,src->width*src->height);
	Image2D *ret = new Image2D(l,m,1);
	memcpy(ret->data,outdata,ret->width*ret->height);

	WaterShedsI &para=WaterShedsI();
	//doMWaterShed2D(src,ret,para);
	doWaterSheds2D(src,ret,para);
	test.writeImagesesmicarray(ret->data ,ret->width,ret->height,1);

}