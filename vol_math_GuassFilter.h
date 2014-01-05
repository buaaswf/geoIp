#ifndef _FILTER_H_
#define _FILTER_H_

#include "vol_math_Raw3D_Independt.h"
#include "vol_math_RawImage.h"
//qym
#include <math.h>
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

	static void guass3DFilter(Raw* src, int halfsize)
	{
		int i=0,j=0,k=0,m=0,n=0,l=0,width=0,depth=0,length=0,index=0;
		float sum = 0;
		int delta=1;
		width=src->getXsize();
		length=src->getYsize();
		depth = src->getZsize();
		Raw *guass = new Raw(*src);	///< The result of guass filter. 

		for (i = 0; i < depth; i++)
		{
			for (j = 0; j < length; j++)
			{
				for (k =0; k < width; k++)
				{
					sum=0;
					float weight=0, total=0;
					for( l=i-halfsize; l<=i+halfsize; l++)
					{
						for(n=j-halfsize; n<=j+halfsize; n++)
						{
							for (int m = k - halfsize; m <= k + halfsize; m++ )
							{
								if(m >= 0 && m < width && n>=0 && n < length && l >= 0 && l < depth) 
								{
									//weight=1.0f/((m-i)*(m-i)+(n-i)*(n-i)+1);
									weight=1.0f/exp((float)((m-k)*(m-k)+(n-j)*(n-j) + (l-i)*(l-i) ));
									sum += weight*(guass->get(m, n, l));
									total += weight;
								}
							}
						}
					}

					if(total!=0)
					{	
						sum /= total;//total is 1,regulation
						src->put(k, j,i , (PIXTYPE)sum);		
					}
					else  //should never come here
					{
						//cout << "total==0" << endl;
					}
				}
				
			}
		}

		//return guass;
	}
	static void guass3DFilterSipl(Raw* src, Raw *ret,int iter, int halfsize)
	{
		int i=0,j=0,k=0,m=0,n=0,l=0,width=0,depth=0,length=0,index=0;
		float sum = 0;
		int delta=1;
		Raw *temp;
		if (iter !=0 && (iter+1)*ret->getZsize() < src->getZsize())
		{
			temp = new Raw(src->getXsize(),src->getYsize(),ret->getZsize() + 2*halfsize,src->getdata()+ ret->getXsize()*ret->getYsize()*ret->getZsize()*(iter-1*halfsize),true);
		} 
		else if ( (iter == 0 && (iter+1)*ret->getZsize() !=  src->getZsize())|| ((iter+1)*ret->getZsize() ==  src->getZsize()&& iter !=0 ))
		{
			temp = new Raw(ret->getXsize(),ret->getYsize(),ret->getZsize()+1*halfsize,src->getdata()+iter*ret->getXsize()*ret->getYsize()*(ret->getZsize()-1*halfsize),true);
		}
		else 
		{
			temp = new Raw(ret->getXsize(),ret->getYsize(),ret->getZsize(),src->getdata(),true);
		}
		//Raw * _ret =new Raw(*temp);
		width=temp->getXsize();
		length=temp->getYsize();
		depth = temp->getZsize();
		Raw *guass = new Raw(*temp);	///< The result of guass filter. 

		for (i = 0; i < depth; i++)
		{
			for (j = 0; j < length; j++)
			{
				for (k =0; k < width; k++)
				{
					sum=0;
					float weight=0, total=0;
					for( l=i-halfsize; l<=i+halfsize; l++)
					{
						for(n=j-halfsize; n<=j+halfsize; n++)
						{
							for (int m = k - halfsize; m <= k + halfsize; m++ )
							{
								if(m >= 0 && m < width && n>=0 && n < length && l >= 0 && l < depth) 
								{
									//weight=1.0f/((m-i)*(m-i)+(n-i)*(n-i)+1);
									weight=1.0f/exp((float)((m-k)*(m-k)+(n-j)*(n-j) + (l-i)*(l-i) ));
									sum += weight*(temp->get(m, n, l));
									total += weight;
								}
							}
						}
					}

					if(total!=0)
					{	
						sum /= total;//total is 1,regulation
						guass->put(k, j,i , (PIXTYPE)sum);
						
								
					}
					else  //should never come here
					{
						//cout << "total==0" << endl;
					}
				}//k...

			}//j...
		}//i...
		if (iter !=0 && (iter+1)*ret->getZsize() < src->getZsize())
		{
			for (int i = 0; i < ret->size();i++ )
			{
				ret->putXYZ( i, guass->getXYZ(i + ret->getXsize()*ret->getYsize()) );
			}

		} 
		else if ((iter == 0 && (iter+1)*ret->getZsize() !=  src->getZsize())|| ((iter+1)*ret->getZsize() ==  src->getZsize()&& iter !=0 ))
		{
			for (int i =0; i < ret->size(); i++)
			{
				ret->putXYZ(i ,  guass->getXYZ(i + ret->getXsize()*ret->getYsize()));
			}
			
		} 
		else
		{
			for ( int i = 0; i < ret->size(); i ++)
			{
				ret->putXYZ(i , guass->getXYZ(ret->getXsize()*ret->getYsize()));
			}
			
		}

		//return guass;
	}
};

#endif
