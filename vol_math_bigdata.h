#ifndef VOL_MATH_BIGDATA
#define VOL_MATH_BIGDATA
#include"vol_math_filter_Interface.h"
PIXTYPE ** readslices(int x,int y, int z,char *dir)
{
	int l=281,m=481,n = 100;
	RawImage test;
	unsigned char ** indata=new unsigned char [n][l*m];
	test.readImage(indata,"F:\\lab\\VTKproj\\mig.raw",l*m*n*sizeof(unsigned char));
	ImageVolume *imagevol=new ImageVolume(l,m,n,1,indata);
	

}



#endif