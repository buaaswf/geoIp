#include <ccomplex>
#include "vol_math_Raw3D_Independt.h"
class FourierFilter2    
{
private:
	int x, y;
	float* buf;
	Raw2D fraw2d;
public:
	FourierFilter2(Raw2D & raw2);
	~FourierFilter2();
public:
	void lowpass_trans(double thresold);
	void guass_low_pass2(complex<float> *src, int width, int height, float D);

};
