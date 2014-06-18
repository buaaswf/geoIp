#include <stdio.h>

#include "vol_math_2D_filter_Interface.h"
#include <assert.h>

#include <stdio.h>
#include <tchar.h>
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "KDetectMemoryLeak.h"
#include "testDemo.cpp"

void p(int type,int total ,int step,bool &cancled)
{

	printf(" %f\n",(float)step/total);
}

void Exit()
{
	int i = _CrtDumpMemoryLeaks();
	assert( i == 0);
}
 void testRaw2DTrilaterlfilter()
{
	int l = 281, m = 481, n =1; 
	RawImage test;
	unsigned char * indata = new unsigned char [l*m*n];
	//unsigned char  *result = indata; 
	//test.readImage(indata,"F:\\3DVdata\\1\\mig100.3dv.raw",l*m*n*sizeof(unsigned char));//G:\geo\data
	test.readImage(indata,"F:\\lab\\VTKproj\\mig.raw",l*m*sizeof(unsigned char));

	unsigned char * outdata = new unsigned char[l*m*n]; 
	Image2D *src = new Image2D(l,m,1);
	memcpy(src->data,indata,src->width*src->height);
	Image2D *ret = new Image2D(l,m,1);
	memcpy(ret->data,outdata,ret->width*ret->height);
	TrilateralfilterI *tri =new TrilateralfilterI(3,3,1);
	ret=(Image2D*)doTrilateralfilterI2D(*src,*tri);

	test.writeImagesesmicarray(ret->data ,ret->width,ret->height,1);

}
 void testRaw2DGaussFilter()
 {
	 int l = 281, m = 481, n =1; 
	 RawImage test;
	 unsigned char * indata = new unsigned char [l*m*n];
	 //unsigned char  *result = indata; 
	 //test.readImage(indata,"F:\\3DVdata\\1\\mig100.3dv.raw",l*m*n*sizeof(unsigned char));//G:\geo\data
	 test.readImage(indata,"F:\\lab\\VTKproj\\mig.raw",l*m*sizeof(unsigned char));

	 unsigned char * outdata = new unsigned char[l*m*n]; 
	 Image2D *src = new Image2D(l,m,1);
	 memcpy(src->data,indata,src->width*src->height);
	 Image2D *ret = new Image2D(l,m,1);
	 memcpy(ret->data,outdata,ret->width*ret->height);
	 GuassFilterI *tri =new GuassFilterI(5,1);
	 ret=(Image2D*)doGuassFilterI2D(*src,*tri);

	 test.writeImagesesmicarray(ret->data ,ret->width,ret->height,1);

 }
 void testRaw2DBilateralFilter()
 {
	 int l = 481, m = 281, n =1; 
	 RawImage test;
	 unsigned char * indata = new unsigned char [l*m*n];
	 //unsigned char  *result = indata; 
	 //test.readImage(indata,"F:\\3DVdata\\1\\mig100.3dv.raw",l*m*n*sizeof(unsigned char));//G:\geo\data
	 test.readImage(indata,"F:\\lab\\VTKproj\\mig.raw",l*m*sizeof(unsigned char));

	 unsigned char * outdata = new unsigned char[l*m*n]; 
	 Image2D *src = new Image2D(l,m,1);
	 memcpy(src->data,indata,src->width*src->height);
	 Image2D *ret = new Image2D(l,m,1);
	 memcpy(ret->data,outdata,ret->width*ret->height);
	 BilateralFilterI *tri =new BilateralFilterI(3,3,1);
	 ret=(Image2D*)doBilateralI2D(*src,*tri);

	 test.writeImagesesmicarray(ret->data ,ret->width,ret->height,1);

 }
 void testRaw2DAnistropicFilter()
 {
	 int l = 481, m = 281, n =1; 
	 RawImage test;
	 unsigned char * indata = new unsigned char [l*m*n];
	 //unsigned char  *result = indata; 
	 //test.readImage(indata,"F:\\3DVdata\\1\\mig100.3dv.raw",l*m*n*sizeof(unsigned char));//G:\geo\data
	 test.readImage(indata,"F:\\lab\\VTKproj\\mig.raw",l*m*sizeof(unsigned char));

	 unsigned char * outdata = new unsigned char[l*m*n]; 
	 Image2D *src = new Image2D(l,m,1);
	 memcpy(src->data,indata,src->width*src->height);
	 Image2D *ret = new Image2D(l,m,1);
	 memcpy(ret->data,outdata,ret->width*ret->height);
	 AnistropicI *tri =new AnistropicI(3,3,1,1);
	 ret=(Image2D*)doAnistropicI2D(*src,*tri);

	 test.writeImagesesmicarray(ret->data ,ret->width,ret->height,1);

 }
int main(int argc, char* argv[])
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//testRaw2D();

	//_CrtSetBreakAlloc(520);
	//testRaw2DTrilaterlfilter();
	//testRaw2DGaussFilter();
	//testRaw2DBilateralFilter();
	testRaw2DAnistropicFilter();
	int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );
	tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag( tmpFlag );
	//testbigdata();
	//testprocess();
	 _CrtDumpMemoryLeaks();
	// _CrtDumpMemoryLeaks;
	//atexit(Exit);
	system("pause");
	return 1;

}
