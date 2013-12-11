#include <stdio.h>
#include <stdlib.h>
#include "vol_math_RawImage.h"
#include "vol_math_trilateralfilter.h"
#include "BilateralFilter.h"
#include "ThreeDim_Bilateral.h"
#include "WipeNioisePde.h"
#include "CImg.h"
#include "test.h"
#include "Filter.h"
using namespace cimg_library;
void testbilateralfilter()
{
	CImg<unsigned char> source;
	//source.load("E:\\geo\\geoIp\\einstein.bmp");E:\geo\testbmps
	source.load("E:\\geo\\testbmps\\0o.bmp");
	//	source.display();
	//int x=source->height;
	//int y=source->width;
	int x=source.width();
	int y=source.height();
	PIXTYPE *p=new PIXTYPE[x*y];
	//PIXTYPE *q=new PIXTYPE[x*y];
	for(int i=0;i<x;i++)
	{
		for (int j=0;j<y;j++)
		{
			p[i*y+j]=source.atXY(i,j);
		}
	}
	Raw2D *input=new Raw2D(x,y,p);
	IShowImg(*input);
	//for (int i=0;i<2;i++)
	//{
	BilateralFilter *b=new BilateralFilter(input,6,3);
	b->apply(*input);
	//}
	IShowImg(*input);
}
void test3dbilateralfilter(int argc,char **argv)
{

	char *pt="single_well";
	int l=281,m=481,n=10;
	RawImage test;
	unsigned char * indata=new unsigned char [l*m*n];
	Raw *initial=new Raw(l,m,n);
	//float *indata1=(float*)indata;
	test.readImage(indata,"F:\\lab\\VTKproj\\mig.raw",l*m*n*sizeof(unsigned char));

	float *inputo=new float[l*m*n];
	for (int i = 0; i < l*m*n; i++)
	{
		inputo[i]=(float) indata[i];
	}

	Raw *input=new Raw (l,m,n,inputo);
	RawImage *write=new RawImage();
	ThreeDim_Bilateral *b=new ThreeDim_Bilateral(input,6,3);
	Raw *ret=new Raw(b->apply(*input));
	//IShowraw(*input,argc,argv);
	write->writeImagesesmic(*ret);
	//delete  ret;
}
//void testanistropic()
//{
//	int l=281,m=481,n=10;
//	RawImage test;
//	unsigned char * indata=new unsigned char [l*m*n];
//	Raw *initial=new Raw(l,m,n);
//	test.readImage(indata,"F:\\lab\\VTKproj\\mig.raw",l*m*n*sizeof(unsigned char));
//	float *inputo=new float[l*m*n];
//	for (int i = 0; i < l*m*n; i++)
//	{
//		inputo[i]=(float) indata[i];
//	}
//
//	Raw *input=new Raw (l,m,n,inputo);
//	RawImage *write=new RawImage();
//	WipeNioisePde *pde=new WipeNioisePde(*input,1,1,1);
//	pde->Perona_Malik(*input);
//	write->writeImagesesmic(*input);
//}
void test3dguass()
{
	int l=281,m=481,n=10;
	RawImage test;
	unsigned char * indata=new unsigned char [l*m*n];
	Raw *initial=new Raw(l,m,n);
	test.readImage(indata,"F:\\lab\\VTKproj\\mig.raw",l*m*n*sizeof(unsigned char));
	float *inputo=new float[l*m*n];
	for (int i = 0; i < l*m*n; i++)
	{
		inputo[i]=(float) indata[i];
	}

	Raw *input=new Raw (l,m,n,inputo);
	RawImage *write=new RawImage();
	Filter f;
	Raw *output=new Raw(*(f.guass3DFilter(input,1)));
	//Raw *ret=new Raw(b->apply(*input));
	//IShowraw(*input,argc,argv);
	write->writeImagesesmic(*output);

}

void testanistropic(int argc,char **argv)
{
		char *pt="single_well";
		int l=281,m=481,n=10,l1=0,l2=0,iter_outer=30;
		//int LX=0,LY=0,LZ=0;
		RawImage test;
		unsigned char * indata=new unsigned char [l*m*n];
		//short * indata=new short  [l*m*n];
	
			test.readImage(indata,"F:\\lab\\VTKproj\\mig.raw",l*m*n*sizeof(unsigned char));
			Raw *initial=new Raw(l,m,n);
			PIXTYPE *inputo=new PIXTYPE[l*m*n];
			for (int i = 0; i < l*m*n; i++)
				{
						inputo[i]=(unsigned char) indata[i];
				}
			delete [] indata;
			
				Raw *input=new Raw(l,m,n,inputo);
		
			//	IShowraw(*input,1,argv);
		WipeNioisePde *pm=new WipeNioisePde(*input,1,1.0,0);
			Raw  * output=new Raw (pm->FourPDiff(*input));
			//test.writeImage();
			test.writeImagesesmic(*output);
}


int main(int argc,char **argv)
{
	//RawImage *img=new RawImage(281,481,2501);
	//img->readImage(img->buf,"F:\\lab\\VTKproj\\mig.raw",img->getlength());
	//Trilateralfilter f(img);
	//f.TrilateralFilter(1);
	//test3dbilateralfilter(argc,argv);
	//test3dguass();
	testanistropic(argc,argv);
	system("pause");
	return 0;


}