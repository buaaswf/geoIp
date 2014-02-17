#include <iostream>
#include "F:\\vol_math_RawImage.h"
#include "F:\\vol_math_ImageVolume.h"
#include "F:\\vol_math_FourierFilter3.h"
#include "F:\\vol_math_filter_Interface.h"
using namespace std;

void testFftinterface(){
    int wid = 4338,hei=3353,dep = 1;
    RawImage rawtest;
    long length = wid*hei*dep;
    unsigned char * indata = new unsigned char[length];
    rawtest.readImage(indata,"",length*sizeof(unsigned char));
    unsigned char * outdata = new unsigned char[length];
    ImageVolume * src = new ImageVolume(wid,hei,dep,1,indata);
 //   ImageVolume *¡¡ret = new ImageVolume(wid,hei,dep,1,outdata);
    lowPassI p(250);
    Raw * ret = (Raw *)dolowPassI(src,p);
    rawtest.writeImagesesmicarray(ret,wid,hei,dep);


}

int main()
{

}
