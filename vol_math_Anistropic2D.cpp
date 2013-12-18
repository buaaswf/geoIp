#include "vol_math_Anistropic2D.h"

Anistropic2D::Anistropic2D(Raw2D &src,int time,PIXTYPE value, int method)
{
	this->src=src;
	this->time=time;
	val=value;
	this->method=method;
	if (method==0)
	{
		Perona_Malik(src);
	} 
	else
	{
		FourPDiff(src);
	}
}


Anistropic2D::~Anistropic2D(void)
{
}

Raw2D  Anistropic2D::Perona_Malik(Raw2D &src)
{
	//P-M function  2nd-order PDE denoise
	PIXTYPE a=1,sum;
	int z,y,x,K,j,i;
	Raw2D d(src);
	Raw2D s(src);
	PIXTYPE *around=new PIXTYPE[6];
	for (i=0;i<time;i++)
	{

			for (y=1;y<src.getYsize()-1;y++)
			{
				for (x=1;x<src.getXsize()-1;x++)
				{
					around[0]=d.get(x-1,y)-d.get(x,y);
					around[1]=d.get(x,y-1)-d.get(x,y);
					around[2]=d.get(x,y+1)-d.get(x,y);
					around[3]=d.get(x+1,y)-d.get(x,y);

					sum=0;
					for (i=0;i<4;i++)
					{
						//implementation sum(g(i)*f(i))
						sum+=around[i]/(1+around[i]*around[i]/(val*val));

					}
					s.put(x,y,d.get(x,y)+a*sum/double(4));
				}
			}
	
		d=s;
	}
	return  d;
}
Raw2D gradientlaplace(Raw2D &src)
{
	Raw2D val(src);
	for (int i=2;i<src.getXsize()-2;i++)
	{
		for (int j=2;j<src.getYsize()-2;j++)
		{

				val.put(i,j,(src.get(i+1,j)+src.get(i-1,j)+src.get(i,j+1)+src.get(i,j-1)-4*src.get(i,j)));

		}
	}

	return val;
}

Raw2D Anistropic2D::FourPDiff(Raw2D &src)			//based on Y-K model
{
	PIXTYPE sum;
	int x,y,z,j;
	Raw2D s;
	Raw2D d(src);
	for (int j=0;j<time;j++)
	{
		s=gradientlaplace(src);
		s=s/(s*s+1);
		s=gradientlaplace(s);
		s=d-s/double(4);
	}
	return s;

}