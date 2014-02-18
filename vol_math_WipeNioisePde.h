#ifndef SWF_WIPE_NOISE_PDE_H
#define SWF_WIPE_NOISE_PDE_H
#include"vol_math_filter_Interface.h"
#include"vol_math_RawImage.h"
//#include "Wipe.h"
class WipeNioisePde
{
private:
	Raw raw;
	int delt;
	PIXTYPE val;
	int way;
	int datatype;
	
public:
	void (*ProgressChanged)(int ,int ,int ,bool &);
	WipeNioisePde(Raw &raw ,Raw &ret,int iter,int time,PIXTYPE value,int method,void(*ProgressChanged)(int,int,int,bool &));
	WipeNioisePde(Raw *raw ,Raw *ret,int iter,int time,PIXTYPE value,int method,void(*ProgressChanged)(int,int,int,bool &),int datatype);

	WipeNioisePde();
	WipeNioisePde(Raw &raw ,int time,PIXTYPE value,int method);
	~WipeNioisePde(void);
	void  Perona_MalikSipl( Raw &src,Raw & ret,int iter);
	void  Perona_MalikSipl( Raw *src,Raw * ret,int iter);
	void  Perona_Malik( Raw &src);
	Raw FourPDiff(Raw &src);
	void FourPDiff(Raw &src,Raw &ret);
	void FourPDiffSipl(Raw &src,Raw &ret,int iter);	
	double funcg(PIXTYPE *array,int dz,int dy,int dx);

	
};

#endif