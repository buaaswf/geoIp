#ifndef VOL_MATH_BIGDATA
#define VOL_MATH_BIGDATA
#include"vol_math_filter_Interface.h"
PIXTYPE ** readslices(int x,int y, int z,char *dir)
{
	int l = 281, m = 481, n  = 15;
	RawImage test;
	unsigned char ** indata=new unsigned char [n][l*m];
	test.readImage(indata,"F:\\lab\\VTKproj\\mig.raw",l*m*n*sizeof(unsigned char));
	//ImageVolume *imagevol = new ImageVolume(l,m,n,1,indata);
	PIXTYPE *res = new PIXTYPE [l*m];
	Process para(1,1,l,m,3,indata,res,3.0,3.0,3.0,3.0,3.0, 5);
	Raw  * ret = testinterface(*imagevol);
	doAnistropicI(para);
	//unsigned char* data = (unsigned char*)Raw2ImageVolume(*ret,1);
	test.writeImagesesmicarray(data ,ret->getXsize(),ret->getYsize(),ret->getZsize());
	system("pause");
	return 1;

}



#endif