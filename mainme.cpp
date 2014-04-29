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

void p(int type,int total ,int step,bool &cancled)
{

	printf(" %f\n",(float)step/total);
}
ImageVolume * testinterface()
{
	//int l = 4338, m = 3353, n = 3;
	int l = 41, m = 41, n = 300; 
	//int l= 281,m=481,n=20;
	RawImage test;
	unsigned char * indata = new unsigned char [l*m*n];
	//unsigned char  *result = indata; 
	//test.readImage(indata,"F:\\3DVdata\\1\\mig100.3dv.raw",l*m*n*sizeof(unsigned char));//G:\geo\data
	//test.readImage(indata,"F:\\lab\\VTKproj\\mig.raw",l*m*n*sizeof(unsigned char));//E:\geo\0000geoimageprov2\data
	 test.readImage(indata,"E:\\geo\\0000geoimageprov2\\data\\Probe_Amp.probe.raw",l*m*n*sizeof(unsigned char));
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
	ImageVolume *src = new ImageVolume(l,m,n,1,indata,false);
	ImageVolume *ret = new ImageVolume(l,m,n,1,outdata,false);
	//testinterface(src,ret);
	//unsigned char* data = (unsigned char*)Raw2ImageVolume(*ret,1);
	//AnistropicI anis(2,30,1,4);
	//doAnistropicIYproqt(src,ret,anis,6,p);
	//MultiOstuI mul(1,3);
	//doMultiOstuI(src,ret,mul,p);
	//AnistropicI anis(2,30,1,4);
	//doAnistropicIY(src,ret,anis);
	// trilateral 20140211============
	//TrilateralfilterI tril(4,3);
	//doTrilateralfilterIY(src,ret,tril);
	// //=======================
	//Raw *ret=(Raw *)doTrilateralfilterI(src,tril);
	//BilateralFilterI bil(3,3,10);
	//doBilateralIY(src,ret,bil);
	//Raw *ret=(Raw *)doAnistropicykfour_diff(src,anis);
	//Raw *ret =(Raw*)doAnistropicI(src,anis);
	//Raw *ret=(Raw *)doTrilateralfilterI(src,tril);zzz
	//BilateralFilterI bil(3,3,10);
	//Raw * ret = (Raw *)doBilateralI(src,bil);
	//Raw *ret=(Raw *)doAnistropicykfour_diff(src,anis);
	// swf 20140415
	AnistropicI ani(6,30,2,16);
	////doAnistropicI(src,ret,ani);
	doAnistropicIYproqt(src,ret,ani,10,p);
	// end 20140415
	//TrilateralfilterI tri(8,5,5);
	//doTrilateralproqt(src,ret,tri,6,p);
	//BilateralFilterI bia(9,9,5);
	//doBilateralproqt(src,ret,bia,3,p);
	//GuassFilterI gs(7,15);
	//doGaussproqt(src,ret,gs,20,p);
	//WaterShedsI &water=WaterShedsI();
	//doWaterShedsI(src,ret,water,p);
	//lowPassI lpass(5000.0);
	//Raw *ret = (Raw *)dolowPassI(src,lpass);
	test.writeImagesesmicarray((unsigned char*)ret->Data ,ret->Width,ret->Height,ret->Depth);
	delete outdata;
	return ret;
	

}
Raw * testinterface1(Process &src)
{
	AnistropicI anis(3,30,1,2);
	//TrilateralfilterI tril(3,2);
	//Raw *ret=(Raw *)doTrilateralfilterI(src,tril);
	//BilateralFilterI bil(3,3,5);
	//Raw * ret=(Raw *)doBilateralI(src,bil);
	//Raw *ret=(Raw *)doAnistropicykfour_diff(src,anis);
	//Raw *ret =(Raw*)doAnistropicI(src);
	//GuassFilterI gs(3,2);
	Raw *ret = (Raw *)doGuassFilterI(src);
	//Raw * ret = (Raw *)doGuassFilterI(src , gs);
	//lowPassI lpass(5000.0);
	//Raw *ret = (Raw *)dolowPassI(src,lpass);

	return ret;


}
void testprocess()
{

	int l = 281, m = 481,total =3, n = 3;
	RawImage test;
	unsigned char * indata = new unsigned char [l*m*n];
	unsigned char  *result = indata; 
	test.readImage(indata,"F:\\lab\\VTKproj\\mig.raw",l*m*n*sizeof(unsigned char));
	unsigned char **slice =new unsigned char *[n];
	for (int k = 0; k < n; k++)
	{
		slice[k] = new unsigned char[l*m];
		memcpy(slice[k],result,l*m);
		//slice++;
		result += l*m;
	}
	ImageVolume *imagevol=new ImageVolume(l,m,n,1,indata);

	//Process para(1,1,l,m,3,slice,indata,3,3,3,3,2);
	//Raw  * ret = testinterface1(para);
	//testinterface1(para);
	unsigned char * outdata = new unsigned char[l*m*n]; 
	void** slice1=(void**)slice;
	void *outdata1=(void *)outdata;
	//ImageVolume *src = new ImageVolume(l,m,n,1,indata);
	//ImageVolume *ret = new ImageVolume(l,m,n,1,outdata);
	//testinterface(src,ret);
	//unsigned char* data = (unsigned char*)Raw2ImageVolume(*ret,1);
	TrilateralfilterI gs(3,3,15);
	doTrilateralFilterFileMode(slice1,l,m,n,outdata1,gs,1);
	//BilateralFilterI bila(3,3,4);
	//doBilateralFilterFileMode(slice1,l,m,n,outdata1,bila,1);
	//GuassFilterI gs(3,3);
	//bool flag=false;
	//p(1,2,3,flag);
	//doGuassFilterFileMode(slice1,l,m,n,outdata1,gs,1,p);
	//(void **src,int width,int height ,int count,void * ret,GuassFilterI &para,int datatype,void(*ProgressChanged)(int,int,int,bool &));
	test.writeImageSesmicRecursive(outdata1 ,l,m,n);
}
void testbigdata()
{
	//int l = 989, m = 1241, total = 9, n = 3;
	int l = 281, m = 481, total = 500, n = 3;
	//int l = 4338, m = 3753, total = 4, n = 3;

	unsigned char * indata = new unsigned char [l*m*n];
	unsigned char * outdata = new unsigned char[l*m*n]; 
	for (int i=0; i < total-n+1 ; i++)
	{
		RawImage test;
		
		//test.readImagerecursive(indata,"F:\\3DVdata\\1\\mig100.3dv.raw", l, m,i,n);
		//test.readImagerecursive(indata,"F:\\3DVdata\\4\\mig.3dv.raw", l, m,i);//F:\lab\VTKproj
		test.readImagerecursive(indata,"F:\\lab\\VTKproj\\mig.raw", l, m,i,n);
		//test.readImagerecursive(indata,"G:\\geo\\data\\mig.vol", l, m,i,n);
		//test.readImagerecursive(indata,"F:\\3DVdata\\3\\mig8.3dv.raw", l, m,i,n);
		ImageVolume *src = new ImageVolume(l,m,n,1,indata);
		ImageVolume *src_bak = new ImageVolume(l,m,n,1,indata);
		ImageVolume *ret = new ImageVolume(l,m,n,1,outdata);


		//GuassFilterI gs(3,20);
		//doGuassFilterIY(src,ret,gs);
		//AnistropicI ani( 1, 1, 2,1);
		//doAnistropicI(src,ret,ani);
		//MultiOstuI mutilostu(1,3);
		//doMultiOstuI(src,ret,mutilostu);
		
		TrilateralfilterI tri(4,1.0,1.0);
		doTrilateralfilterIY(src,ret,tri);
		//BilateralFilterI bila(3,3,4);
		//doBilateralI(src, ret, bila);
		//lowPassI lowpass(1000);
		//dolowPassI(src,ret,lowpass);

		test.writeImageSesmicRecursive(ret->Data ,ret->Width,ret->Height,ret->Depth);
		delete src;
		delete src_bak;
		delete ret;
	}
	delete [] indata;
	indata = NULL;
	delete [] outdata;
	outdata = NULL;

}
void Exit()
{
	int i = _CrtDumpMemoryLeaks();
	assert( i == 0);
}
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
int main(int argc, char* argv[])
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//testRaw2D();

	//_CrtSetBreakAlloc(520);
	testinterface();
	

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
