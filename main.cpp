#include <stdio.h>
#include <stdlib.h>
//#include "CImg.h"
//#include "test.h"
#include <assert.h>
#include "vol_math_filter_interface.h"
#include <stdio.h>
#include <tchar.h>

ImageVolume * testinterface()
{
	int l = 281, m = 481, n = 50;
	RawImage test;
	unsigned char * indata = new unsigned char [l*m*n];
	//unsigned char  *result = indata; 
	test.readImage(indata,"G:\\geo\\data\\mig.vol",l*m*n*sizeof(unsigned char));
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
	ImageVolume *src = new ImageVolume(l,m,n,1,indata);
	ImageVolume *ret = new ImageVolume(l,m,n,1,outdata);
	//testinterface(src,ret);
	//unsigned char* data = (unsigned char*)Raw2ImageVolume(*ret,1);
	
	//AnistropicI anis(3,30,1,2);
	//doAnistropicI(src,ret,anis);
	TrilateralfilterI tril(1,3);
	doTrilateralfilterI(src,ret,tril);
	//Raw *ret=(Raw *)doTrilateralfilterI(src,tril);
	//BilateralFilterI bil(3,3,5);
	//Raw * ret=(Raw *)doBilateralI(src,bil);
	//Raw *ret=(Raw *)doAnistropicykfour_diff(src,anis);
	//Raw *ret =(Raw*)doAnistropicI(src,anis);
	//GuassFilterI gs(3,2);
	//Raw * ret = (Raw *)doGuassFilterI(src , gs);

	//Raw *ret=(Raw *)doTrilateralfilterI(src,tril);zzz
	//BilateralFilterI bil(3,3,10);
	//Raw * ret = (Raw *)doBilateralI(src,bil);
	//Raw *ret=(Raw *)doAnistropicykfour_diff(src,anis);
	//doAnistropicI(src,ret,anis);
	//GuassFilterI gs(3,15);
	//doGuassFilterI(src,ret,gs);

	//lowPassI lpass(5000.0);
	//Raw *ret = (Raw *)dolowPassI(src,lpass);
	test.writeImagesesmicarray(ret->Data ,ret->Width,ret->Height,ret->Depth);
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
//int  testv1()
//{
//	int l=281,m=481,n = 10;
//	RawImage test;
//	unsigned char * indata=new unsigned char [l*m*n];
//
//	test.readImage(indata,"F:\\lab\\VTKproj\\mig.raw",l*m*n*sizeof(unsigned char));
//
//	ImageVolume *imagevol=new ImageVolume(l,m,n,1,indata);
//	Raw  * ret = testinterface(*imagevol);
//	unsigned char* data = (unsigned char*)Raw2ImageVolume(*ret,1);
//	test.writeImagesesmicarray(data ,ret->getXsize(),ret->getYsize(),ret->getZsize());
//
//	unsigned char * outdata=new unsigned char [l*m*n];
//	test.readImage(indata,"G:\\geo\\data\\mig.vol",l*m*n*sizeof(unsigned char));
//	ImageVolume *in=new ImageVolume(l,m,n,1,indata);
//	ImageVolume *out=new ImageVolume(l,m,n,1,outdata);
//	ImageVolume *ret= testinterface(in,out);
//	unsigned char* data =(unsigned char *) ret->Data;
//	test.writeImagesesmicarray(data ,ret->Width,ret->Height,ret->Depth);
//
//	system("pause");
//	return 1;
//}



void testprocess()
{
	int l = 281, m = 481, n = 15;
	RawImage test;
	unsigned char * indata = new unsigned char [l*m*n];
	unsigned char  *result = indata; 
	test.readImage(indata,"G:\\geo\\data\\mig.vol",l*m*n*sizeof(unsigned char));
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
	ImageVolume *src = new ImageVolume(l,m,n,1,indata);
	ImageVolume *ret = new ImageVolume(l,m,n,1,outdata);
	//testinterface(src,ret);
	//unsigned char* data = (unsigned char*)Raw2ImageVolume(*ret,1);
	test.writeImagesesmicarray(ret->Data ,ret->Width,ret->Height,ret->Depth);
}
int main(int argc, char* argv[])
{
	testinterface();
	system("pause");
	return 1;

}
//sadasdasdasd/