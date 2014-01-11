#include "vol_math_RawImage.h"
#include<complex>
class FourierFilter3
{
private:
	int x, y,z;
	float* buf;
	Raw fraw;
public:
	FourierFilter3(Raw & raw);
	~FourierFilter3();
public:

	void lowpass_trans(double thresold);
	void low_Pass3(complex<float>*src, int width, int height, int depth, float thresold);
};