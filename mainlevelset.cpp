#include "vol_math_LevelSet.h"

#include "test.h"
#include <iostream>
#include <crtdbg.h> 
#include "CImg.h" 
#include "ThreeDim_LevelSet.h"
#include "Filter.h"
#include "WipeNioisePde.h"

using namespace cimg_library;
using namespace std;

void testcolon(int argc,char **argv)
{
	
	char *pt="single_well";
	int l=501,m=1741,n=1,l1=0,l2=0,iter_outer=5;
	//cin>> l >> m >> n;
	//int LX=0,LY=0,LZ=0;
	
	RawImage test;
	//unsigned char * indata=new unsigned char [l*m*n];
	short * indata=new short [l*m*n];
	//Raw *inputeasy=new Raw(l,m,n);
	
	//for (int i=0;i<l;i++)
	//{
	//	for (int j=0;j<m;j++)
	//	{
	//		for (int k=0;k<n;k++)
	//		{
	//			if (i>l1 && j>l1 && k>l1 && i<l2 && j<l2 && k<l2)
	//			{
	//				inputeasy->put(i,j,k,20.0);

	//			}
	//			else inputeasy->put(i,j,k,0);

	//		}
	//	}
	//}
	//test.readImage(indata,"E:\\geo\\lobster.raw",301*324*56);

	//test.readImage(indata,"E:\\volume\\cthead-8bit\\cthead-8bit.raw",256*256*99);
	//test.readImage(indata,"E:\\volume\\little\\little.raw",256*256*26);

	test.readStream(indata,"E:\\volume\\clean\\WI_3032_P_iso_clean.raw",l*m*n);

	Raw *initial=new Raw(l,m,n);
	float *inputo=new float[l*m*n];
	for (int i = 0; i < l*m*n; i++)
	{
		inputo[i]=(float) indata[i];		
	}
	Raw *input=new Raw(l,m,n,inputo);
	
	int width=10;
	//for (int i=0;i<l;i++)
	//{
	//	for (int j=0;j<m;j++)
	//	{
	//		for (int k=0;k<n;k++)
	//		{

	//			if (i>l1+width && j>l1+width && k>l1+width && i<l2-width && j<l2-width && k<l2-width)
	//			{
	//				initial->put(i,j,k,-2.0);
	//			} 
	//			else if (i<l1+width || j<l1+width || k < l1+width || i>l2-width || j>l2-width || k> l2-width)
	//			{
	//				initial->put(i,j,k,2.0);
	//			}
	//			else {
	//				initial->put(i,j,k,-2.0);
	//			}

	//		}
	//	}
	//}
	//int x1=84,x2=123,y1=118,y2=180,z1=19,z2=24; //test for little data

	//int x1=257,x2=299,y1=345,y2=358,z1=6,z2=16; //test for colon data inside 
	//int x1=225,x2=346,y1=301,y2=398,z1=6,z2=16; //test for colon data outside 
	//	int x1=140,x2=405,y1=269,y2=372,z1=1,z2=30; //test for colon data outside 
	//	int x12,x22,y12,y22,z12,z22;
	//	int x13,x23,y13,y23,z13,z23;
	//	int x14,x24,y14,y24,z14,z24;
	//for (int i=0;i<l;i++)
	//{
	//	for (int j=0;j<m;j++)
	//	{
	//		for (int k=0;k<n;k++)
	//		{
	//			//region 1
	//			if (i>x1 && j>y1 && k>z1 && i<x2 && j<y2 && k<z2)
	//			{
	//				initial->put(i,j,k,-2.0);
	//			} 
	//			else if (i<x1|| j<y1 || k < z1 || i>x2 || j>y2 || k> z2)
	//			{
	//				initial->put(i,j,k,2.0);
	//			}
	//			else {
	//				initial->put(i,j,k,-2.0);
	//			}
	//			//region 2
	//			if (i>x12 && j>y12 && k>z12 && i<x22 && j<y22 && k<z22)
	//			{
	//				initial->put(i,j,k,-2.0);
	//			} 
	//			else if (i<x12|| j<y12 || k < z12 || i>x22 || j>y22 || k> z22)
	//			{
	//				initial->put(i,j,k,2.0);
	//			}
	//			else {
	//				initial->put(i,j,k,-2.0);
	//			}
	//			//region 3
	//			if (i>x13 && j>y13 && k>z13 && i<x23 && j<y23 && k<z23)
	//			{
	//				initial->put(i,j,k,-2.0);
	//			} 
	//			else if (i<x14|| j<y14 || k < z14 || i>x24 || j>y24 || k> z24)
	//			{
	//				initial->put(i,j,k,2.0);
	//			}
	//			else {
	//				initial->put(i,j,k,-2.0);
	//			}

	//		}
	//	}
	//}
	//IShowraw(*initial,argc,argv);
	RawImage *write=new RawImage();
	IShowraw(*input,1,argv);
	Raw bak(*input);
	ThreeDim_LevelSet *ls=new ThreeDim_LevelSet();
	//IShowraw(*input,argc,argv);
	ls->initialg(*input);
	for (int i=0; i<input->getXsize(); i++)
	{
		for (int j=0; j<input->getYsize(); j++)
		{
			for (int k=0; k<input->getZsize(); k++)
			{
				if (input->get(i,j,k)>=1)
				{
					initial->put(i,j,k,-2);
				}
				else initial->put(i,j,k,2);

			}
		}

	}
	//IShowraw(*input,argc,argv);
	//IShowraw(*initial,argc,argv);
	ls->minimal_surface(*initial,*input,5.0,0.1,-3,1.5,1,iter_outer,pt);
	//for (int i=0;i<initial->getXsize();i++)
	//{
	//	for (int j=0;j<initial->getYsize();j++)
	//	{
	//		for (int k=0;k<initial->getZsize();k++)
	//		{
	//			if (initial->get(i,j,k)>=0)
	//			{
	//				initial->put(i,j,k,1);
	//			} 
	//			else
	//			{
	//				initial->put(i,j,k,0);
	//			}

	//		}
	//	}
	//}
	//*initial*=bak;
	//ls->outerwall(*initial,*input,5.0,0.1,-3,1.5,1,iter_outer,pt);
	IShowraw(*initial,argc,argv);
	//test.writeImage(*initial);
	//delete [] inputo;


}
void testlittle(int argc,char **argv)
{
	char *pt="single_well";
	int l=256,m=256,n=26,l1=0,l2=0,iter_outer=10;
	RawImage test;
	unsigned char * indata=new unsigned char [l*m*n];
	//Raw *inputeasy=new Raw(l,m,n);
	Raw *initial=new Raw(l,m,n);
	
	test.readImage(indata,"E:\\volume\\little\\little.raw",256*256*26);

	float *inputo=new float[l*m*n];
	for (int i = 0; i < l*m*n; i++)
	{
		inputo[i]=(float) indata[i];
	}

	Raw *input=new Raw(l,m,n,inputo);
	int width=10;

	int x1=84,x2=123,y1=118,y2=180,z1=19,z2=24; //test for little data

	for (int i=0;i<l;i++)
	{
		for (int j=0;j<m;j++)
		{
			for (int k=0;k<n;k++)
			{

				if (i>x1 && j>y1 && k>z1 && i<x2 && j<y2 && k<z2)
				{
					initial->put(i,j,k,-2.0);
				} 
				else if (i<x1|| j<y1 || k < z1 || i>x2 || j>y2 || k> z2)
				{
					initial->put(i,j,k,2.0);
				}
				else {
					initial->put(i,j,k,-2.0);
				}

			}
		}
	}
	RawImage *write=new RawImage();

	ThreeDim_LevelSet *ls=new ThreeDim_LevelSet();
	//IShowraw(*input,argc,argv);
	ls->initialg(*input);	
	ls->minimal_surface(*initial,*input,5.0,0.1,-3,1.5,1,iter_outer,pt);
	IShowraw(*initial,argc,argv);
	test.writeImage(*initial);


}
void testlobster(int argc,char **argv)
{
	char *pt="single_well";
	int l=301,m=324,n=56,l1=0,l2=0,iter_outer=50;
	RawImage test;
	unsigned char * indata=new unsigned char [l*m*n];
	//Raw *inputeasy=new Raw(l,m,n);
	Raw *initial=new Raw(l,m,n);

	test.readImage(indata,"E:\\geo\\lobster.raw",301*324*56);

	float *inputo=new float[l*m*n];
	for (int i = 0; i < l*m*n; i++)
	{
		inputo[i]=(float) indata[i];
	}

	Raw *input=new Raw(l,m,n,inputo);
	int width=10;

	int x1=138,x2=144,y1=137,y2=155,z1=30,z2=35; //test for little data

	for (int i=0;i<l;i++)
	{
		for (int j=0;j<m;j++)
		{
			for (int k=0;k<n;k++)
			{

				if (i>x1 && j>y1 && k>z1 && i<x2 && j<y2 && k<z2)
				{
					initial->put(i,j,k,-2.0);
				} 
				else if (i<x1|| j<y1 || k < z1 || i>x2 || j>y2 || k> z2)
				{
					initial->put(i,j,k,2.0);
				}
				else {
					initial->put(i,j,k,-2.0);
				}

			}
		}
	}
	RawImage *write=new RawImage();

	ThreeDim_LevelSet *ls=new ThreeDim_LevelSet();
	//IShowraw(*input,argc,argv);
	ls->initialg(*input);	
	ls->minimal_surface(*initial,*input,5.0,0.1,-3,1.5,1,iter_outer,pt);
	IShowraw(*initial,argc,argv);
	test.writeImage(*initial);


}
void testeasy(int argc,char **argv)
{
	char *pt="single_well";
	int l=100,m=100,n=100,l1=30,l2=40,iter_outer=50;
	//int LX=0,LY=0,LZ=0;

	RawImage test;
	//unsigned char * indata=new unsigned char [l*m*n];
	//short * indata=new short  [l*m*n];
	Raw *inputeasy=new Raw(l,m,n);

	for (int i=0;i<l;i++)
	{
		for (int j=0;j<m;j++)
		{
			for (int k=0;k<n;k++)
			{
				if (i>l1 && j>l1 && k>l1 && i<l2 && j<l2 && k<l2)
				{
					inputeasy->put(i,j,k,20.0);

				}
				else inputeasy->put(i,j,k,0);

			}
		}
	}

	Raw *initial=new Raw(l,m,n);

	int width=-10;
	for (int i=0;i<l;i++)
	{
		for (int j=0;j<m;j++)
		{
			for (int k=0;k<n;k++)
			{

				if (i>l1+width && j>l1+width && k>l1+width && i<l2-width && j<l2-width && k<l2-width)
				{
					initial->put(i,j,k,-2.0);
				} 
				else if (i<l1+width || j<l1+width || k < l1+width || i>l2-width || j>l2-width || k> l2-width)
				{
					initial->put(i,j,k,2.0);
				}
				else {
					initial->put(i,j,k,-2.0);
				}

			}
		}
	}
	IShowraw(*initial,argc,argv);
	RawImage *write=new RawImage();
	ThreeDim_LevelSet *ls=new ThreeDim_LevelSet();
	IShowraw(*inputeasy,argc,argv);
	ls->initialg(*inputeasy);	
	//IShowraw(*input,argc,argv);
	ls->minimal_surface(*initial,*inputeasy,5.0,0.1,3,1.5,1,iter_outer,pt);
	IShowraw(*initial,argc,argv);
	test.writeImage(*inputeasy);



}
void testanistropic(int argc,char **argv)
{//sqrt change 
	//datatype write data	test.writeImage(*output);
	char *pt="single_well";
	int l=281,m=481,n=2501,l1=0,l2=0,iter_outer=30;
	//int LX=0,LY=0,LZ=0;

	RawImage test;
	unsigned char * indata=new unsigned char [l*m*n];
	//short * indata=new short  [l*m*n];


	test.readImage(indata,"F:\\lab\\VTKproj\\mig.raw",l*m*n);
	Raw *initial=new Raw(l,m,n);
	float *inputo=new PIXTYPE[l*m*n];
	for (int i = 0; i < l*m*n; i++)
	{
		inputo[i]=(unsigned char) indata[i];
	}
	delete [] indata;
	
	Raw *input=new Raw(l,m,n,inputo);

	IShowraw(*input,1,argv);
	WipeNioisePde *pm=new WipeNioisePde(*input,1,1.0,0);
	Raw  * output=new Raw (pm->Perona_Malik(*input));
	//test.writeImage();
	IShowraw(*output,1,argv);
	test.writeImage(*output);
	
}
int main(int argc,char **argv)
{
	//testlittle(argc,argv);
	testcolon(argc,argv);
	//testanistropic(argc,argv);
	//testeasy(argc,argv);
	system("pause");
	return 0;
}
