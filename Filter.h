#ifndef _FILTER_H_
#define _FILTER_H_

#include "vol_math_Raw3D_Independt.h"

class Filter
{
	Raw2D *src;
public:
	static Raw2D* guassFilter(Raw2D* src, int halfsize)
	{
		int i=0,j=0,m=0,n=0,width=0,length=0,index=0;
		float sum = 0;
		int delta=1;
		width=src->getXsize();
		length=src->getYsize();

		Raw2D *guass = new Raw2D(width, length);	///< The result of guass filter. 

		for (i=0;i<width;i++)
		{
			for (j=0;j<length;j++)
			{
				sum=0;
				float weight=0, total=0;
				for( m=i-halfsize; m<=i+halfsize; m++)
				{
					for(n=j-halfsize; n<=j+halfsize; n++)
					{
						if(m >= 0 && m < width && n>=0 && n < length) 
						{
							//weight=1.0f/((m-i)*(m-i)+(n-i)*(n-i)+1);
							weight=1.0f/exp((float)((m-i)*(m-i)+(n-i)*(n-i)));
							sum += weight*(src->get(m, n));
							total += weight;
						}
					}
				}

				if(total!=0)
				{	
					sum /= total;//total is 1,regulation
					guass->put(i, j, (PIXTYPE)sum);		
				}
				else  //should never come here
				{
					//cout << "total==0" << endl;
				}
			}
		}
		
		return guass;
	}
	//static Raw *guassFilter(Raw *src,int halfsize)
	//{
	//	int i=0,j=0,k=0,m=0,n=0,l=0,width=0,length=0,height=0,index=0;
	//	float sum = 0;
	//	int delta=1;
	//	width=src->getXsize();
	//	length=src->getYsize();
	//	height=src->getZsize();

	//	Raw *guass = new Raw(width, length,height);	///< The result of guass filter. 

	//	for (i=0;i<width;i++)
	//	{
	//		for (j=0;j<length;j++)
	//		{
	//			for (k=0;k<height;k++)
	//			{
	//				sum=0;
	//				float weight=0, total=0;
	//				for( m=i-halfsize; m<=i+halfsize; m++)
	//				{
	//					for(n=j-halfsize; n<=j+halfsize; n++)
	//					{
	//						for (l=k-halfsize;l<=l+halfsize;l++)
	//						{
	//							if(m >= 0 && m < width && n>=0 && n < length && l>=0&& l <height ) 
	//							{
	//								//weight=1.0f/((m-i)*(m-i)+(n-i)*(n-i)+1);
	//								weight=1.0f/exp((float)((m-i)*(m-i)+(n-j)*(n-j)+(l-k)*(l-k)));
	//								sum += weight*(src->get(m, n,l));
	//								total += weight;
	//							}
	//						}
	//					}
	//				}

	//				if(total!=0)
	//				{	
	//					sum /= total;//total is 1,regulation
	//					guass->put(i, j,k, (PIXTYPE)sum);		
	//				}
	//				else  //should never come here
	//				{
	//					//cout << "total==0" << endl;
	//				}
	//			}
	//			
	//		}
	//	}

	//	return guass;

	//}
	
	
};

#endif