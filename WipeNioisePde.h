#pragma once
#include"vol_math_RawImage.h"

class WipeNioisePde
{
private:
	Raw raw;
	int delt;
	PIXTYPE val;
	int way;
public:
	WipeNioisePde(Raw &raw ,int time,PIXTYPE value,int method);
	~WipeNioisePde(void);
	Raw Perona_Malik( Raw &src);
	Raw FourPDiff(Raw &src);
	double funcg(PIXTYPE *array,int dz,int dy,int dx);

	
};

