#include <stdio.h>
#include <stdlib.h>
#include "vol_math_RawImage.h"
#include "vol_math_trilateralfilter.h"
#include "BilateralFilter.h"
#include "ThreeDim_Bilateral.h"
#include "CImg.h"
#include "test.h"
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
	int l=256,m=256,n=26,l1=0,l2=0,iter_outer=10;
	RawImage test;
	unsigned char * indata=new unsigned char [l*m*n];
	Raw *initial=new Raw(l,m,n);

	test.readImage(indata,"E:\\volume\\little\\little.raw",256*256*26*sizeof(unsigned char));

	float *inputo=new float[l*m*n];
	for (int i = 0; i < l*m*n; i++)
	{
		inputo[i]=(float) indata[i];
	}

	Raw *input=new Raw(l,m,n,inputo);
	RawImage *write=new RawImage();
	ThreeDim_Bilateral *b=new ThreeDim_Bilateral(input,6,3);
	b->apply(*input);
	IShowraw(*input,argc,argv);
}
int main(int argc,char **argv)
{
	//RawImage *img=new RawImage(281,481,2501);
	//img->readImage(img->buf,"F:\\lab\\VTKproj\\mig.raw",img->getlength());
	//Trilateralfilter f(img);
	//f.TrilateralFilter(1);
	test3dbilateralfilter(argc,argv);
	system("pause");
	return 0;


}