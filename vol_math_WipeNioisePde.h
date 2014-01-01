#pragma once
#include"vol_math_RawImage.h"
//#include "Wipe.h"
class WipeNioisePde
{
private:
	Raw raw;
	int delt;
	PIXTYPE val;
	int way;
public:
	WipeNioisePde(Raw &raw ,Raw &ret,int iter,int time,PIXTYPE value,int method);
	WipeNioisePde(Raw &raw ,int time,PIXTYPE value,int method);
	~WipeNioisePde(void);
	void  Perona_MalikSipl( Raw &src,Raw & ret,int iter);
	void  Perona_Malik( Raw &src);
	Raw FourPDiff(Raw &src);
	Raw FourPDiffSipl(Raw &src,Raw &ret,int iter);	
	double funcg(PIXTYPE *array,int dz,int dy,int dx);

	
};

