#ifndef LOW_PASS_FILTER
#define LOW_PASS_FILTER

#include "vol_math_RawImage.h"
#include<complex>
#include "vol_math_filter_Interface.h"
class FourierFilter3

{
public:
	int x, y,z;
	float* buf;
	Raw * fraw;
	FourierFilter3(Raw & raw);
	void (*ProgressChanged)(int ,int ,int ,bool &);
	~FourierFilter3();
public:
	void lowpass_trans(double thresold, void (*ProgressChanged)(int ,int ,int ,bool &));
	void low_Pass3(complex<float>*src, int width, int height, int depth, float thresold);
};

#endif