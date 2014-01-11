#include <stdio.h>
#include <stdlib.h>
//#include "CImg.h"
#include "test.h"
#include <assert.h>
#include "vol_math_Interface.h"
#include <stdio.h>
#include <tchar.h>

Raw * testinterface(ImageVolume &src)
{
	 AnistropicI anis(2,10,1,4);
	//TrilateralfilterI tril(5,3);
	//Raw *ret=(Raw *)doTrilateralfilterI(src,tril);
	//BilateralFilterI bil(3,3,5);
	//Raw * ret=(Raw *)doBilateralI(src,bil);
	Raw *ret=(Raw *)doAnistropicI(src,anis);
	//GuassFilterI gs(3,1);
	//Raw * ret = (Raw *)doGuassFilterI(src , gs);
	
	return ret;
	

}
int main(int argc, char* argv[])
{
	int l=281,m=481,n=100;
	RawImage test;
	unsigned char * indata=new unsigned char [l*m*n];
	test.readImage(indata,"F:\\lab\\VTKproj\\mig.raw",l*m*n*sizeof(unsigned char));
	ImageVolume *imagevol=new ImageVolume(l,m,n,1,indata);
	Raw  * ret = testinterface(*imagevol);
	unsigned char* data = (unsigned char*)Raw2ImageVolume(*ret,1);
	test.writeImagesesmicarray(data ,ret->getXsize(),ret->getYsize(),ret->getZsize());
	system("pause");
	return 1;
}