#include "vol_math_filter_Interface.h"

//qym
#include "vol_math_RawImage.h"
using namespace std;
extern size_t  globalProgressChanged = 0;
extern size_t volatile progressStep = 0;



 void *doAnistropicI2D (Image2D &src,AnistropicI & para)
{
    //qym non-const must be an lvalue
	//Raw2D &indata=Image2D2Raw2D(src);//to be test
	Raw2D indata=Image2D2Raw2D(src);//to be test
	Anistropic2D * pde=new Anistropic2D(indata,para.time,para.val,para.method);//to be ask
	delete pde;
	return Raw2D2Image2D(indata,src.PixelType);;
}
extern void *doBilateralI2D (Image2D & src, BilateralFilterI &para)
{
    //qym non-const must be an lvalue
	//Raw2D &indata=Image2D2Raw2D(src);
	Raw2D indata=Image2D2Raw2D(src);
	BilateralFilter *b=new BilateralFilter(&indata,6,3);
	b->apply(indata);
	return Raw2D2Image2D(indata,src.PixelType);
}
extern void * doGuassFilterI2D (Image2D &src, GuassFilterI &para)
{
    //qym non-const must be an lvalue
	//Raw2D &indata = Image2D2Raw2D(src);
	Raw2D indata = Image2D2Raw2D(src);
	Filter *guass = new Filter(); 
	Raw2D *ret=guass->guassFilter(&indata,para.halfsize);
	return Raw2D2Image2D(*ret,src.PixelType);
}
extern void * doTrilateralfilterI2D ( Image2D &src, TrilateralfilterI &para)
{
    //qym non-const must be an lvalue
	//Raw2D &indata = Image2D2Raw2D(src);
	Raw2D indata = Image2D2Raw2D(src);
	Trilateralfilter2D * tf=new Trilateralfilter2D();
	Raw2D *ret=tf->TrilateralFilter2D(&indata,para.sigmaC);
	return Raw2D2Image2D(*ret,src.PixelType);
}
