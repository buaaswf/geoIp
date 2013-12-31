#include "vol_math_WipeNioisePde.h"


WipeNioisePde::WipeNioisePde(Raw &src,Raw &ret,int iter,int time,PIXTYPE value, int method)
{
	//raw=src;
	delt=time;
	val=value;
	way=method;
	if (way==1)
	{
		Perona_MalikSipl(src,ret,iter);
		//cout <<"aaa"<<endl;
	} 
	else
	{
		src=FourPDiff(src);
	}
}
WipeNioisePde::WipeNioisePde(Raw &src,int time,PIXTYPE value, int method)
{
	//raw=src;
	delt=time;
	val=value;
	way=method;
	if (way==1)
	{
		Perona_Malik(src);
		//cout <<"aaa"<<endl;
	} 
	else
	{
		src=FourPDiff(src);
	}
}

WipeNioisePde::~WipeNioisePde(void)
{
}

void  WipeNioisePde::Perona_Malik(Raw &src)
{
	//P-M function  2nd-order PDE denoise
	PIXTYPE a=1,sum;
	int z,y,x,K,j,i;
	//Raw *d=new Raw(src.getXsize(),src.getYsize(),src.getZsize(),src.getdata());
	Raw *d=new Raw(src.getXsize(),src.getYsize(),src.getZsize(),src.getdata());
	//Raw s=Raw(src);
	PIXTYPE *around=new PIXTYPE[6];
	for (i = 0 ;i < delt; i++)
	{
		for (z = 1; z < src.getZsize()-1;z++)
		{
			for ( y = 1; y < src.getYsize()-1; y++)
			{
				for ( x = 1; x < src.getXsize()-1; x++)
				{
					around[0]=d->get(x-1,y,z)-d->get(x,y,z);
					around[1]=d->get(x,y-1,z)-d->get(x,y,z);
					around[2]=d->get(x,y,z-1)-d->get(x,y,z);
					around[3]=d->get(x+1,y,z-1)-d->get(x,y,z);
					around[4]=d->get(x,y+1,z)-d->get(x,y,z);
					around[5]=d->get(x,y,z+1)-d->get(x,y,z);
					sum=0;
					for (int k=0; k < 6; k++)
					{
						//implementation sum(g(i)*f(i))
						sum+=around[k]/(1+around[k]*around[k]/(val*val));

					}
					src.put(x,y,z,d->get(x,y,z)+a*sum/double(6));
					
				}//for
			}//for
			//cout << "times = :" << i << endl;
		}//for
		//d += s*(-1);
		d=&src;
	}//for delte
	delete d;
	//src = s;
	//return  d;
}
void WipeNioisePde::Perona_MalikSipl( Raw &src,Raw & ret,int iter)
{
	PIXTYPE a=1,sum;
	int z,y,x,K,j,i;
	//Raw *d=new Raw(src.getXsize(),src.getYsize(),src.getZsize(),src.getdata());
	//Raw *d=new Raw(ret.getXsize(),ret.getYsize(),ret.getZsize(),ret.getdata());
	//Raw s=Raw(src);
	if (iter > 0 && ( iter+1 )*ret.getZsize() < src.getZsize())
	{
		Raw *s = new Raw(ret.getXsize(), ret.getYsize(), ret.getZsize()+2, src.getdata()+iter*ret.getXsize()*ret.getYsize()*(ret.getZsize()-1));


		PIXTYPE *around =new PIXTYPE[6];
		for (i = 0 ;i < delt; i++)
		{
			for (z = 1; z < s->getZsize()-1;z++)
			{
				for ( y = 1; y < s->getYsize()-1; y++)
				{
					for ( x = 1; x < s->getXsize()-1; x++)
					{
						around[0] = s->get(x-1,y,z)-s->get(x,y,z);
						around[1] = s->get(x,y-1,z)-s->get(x,y,z);
						around[2] = s->get(x,y,z-1)-s->get(x,y,z);
						around[3] = s->get(x+1,y,z-1)-s->get(x,y,z);
						around[4] = s->get(x,y+1,z)-s->get(x,y,z);
						around[5] = s->get(x,y,z+1)-s->get(x,y,z);
						sum=0;
						for (int k=0; k < 6; k++)
						{
							//implementation sum(g(i)*f(i))
							sum += around[k]/(1+around[k]*around[k]/(val*val));

						}
						ret.put(x,y,z-1,s->get(x,y,z)+a*sum/double(6));

					}//for
				}//for
				//cout << "times = :" << i << endl;
			}//for
			s=&ret;
		}//for
		delete s;
	} 
	else
	{
		if (iter == 0 || (iter+1)*ret.getZsize() ==  src.getZsize())
		{
			Raw *s = new Raw(ret.getXsize(),ret.getYsize(),ret.getZsize()+1,src.getdata()+iter*ret.getXsize()*ret.getYsize()*(ret.getZsize()-1),true);
			PIXTYPE *around=new PIXTYPE[6];
			for (i = 0 ;i < delt; i++)
			{
				for (z = 1; z < s->getZsize()-1;z++)
				{
					for ( y = 1; y < s->getYsize()-1; y++)
					{
						for ( x = 1; x < s->getXsize()-1; x++)
						{
							around[0]=s->get(x-1,y,z)-s->get(x,y,z);
							around[1]=s->get(x,y-1,z)-s->get(x,y,z);
							around[2]=s->get(x,y,z-1)-s->get(x,y,z);
							around[3]=s->get(x+1,y,z-1)-s->get(x,y,z);
							around[4]=s->get(x,y+1,z)-s->get(x,y,z);
							around[5]=s->get(x,y,z+1)-s->get(x,y,z);
							sum=0;
							for (int k=0; k < 6; k++)
							{
								//implementation sum(g(i)*f(i))
								sum+=around[k]/(1+around[k]*around[k]/(val*val));

							}
							ret.put(x,y,z-1,s->get(x,y,z)+a*sum/double(6));

						}//for
					}//for
					
				}//for
				s=&ret;
			}//for
			delete s;
		}//if
		//if ((iter+1)*ret.getZsize() ==  src.getZsize())
		//{
		//	Raw *s = new Raw(ret.getXsize(),ret.getYsize(),ret.getZsize()+1,src.getdata()+iter*ret.getXsize()*ret.getYsize()*(ret.getZsize()-1),true);
		//	PIXTYPE *around=new PIXTYPE[6];
		//	for (i = 0 ;i < delt; i++)
		//	{
		//		for (z = 1; z < s->getZsize()-1;z++)
		//		{
		//			for ( y = 1; y < s->getYsize()-1; y++)
		//			{
		//				for ( x = 1; x < s->getXsize()-1; x++)
		//				{
		//					around[0]=s->get(x-1,y,z)-s->get(x,y,z);
		//					around[1]=s->get(x,y-1,z)-s->get(x,y,z);
		//					around[2]=s->get(x,y,z-1)-s->get(x,y,z);
		//					around[3]=s->get(x+1,y,z-1)-s->get(x,y,z);
		//					around[4]=s->get(x,y+1,z)-s->get(x,y,z);
		//					around[5]=s->get(x,y,z+1)-s->get(x,y,z);
		//					sum=0;
		//					for (int k=0; k < 6; k++)
		//					{
		//						//implementation sum(g(i)*f(i))
		//						sum+=around[k]/(1+around[k]*around[k]/(val*val));

		//					}
		//					ret.put(x,y,z-1,s->get(x-1,y-1,z-1)+a*sum/double(6));

		//				}//for
		//			}//for

		//		}//for

		//	}//for

		//}

	}
}
Raw gradientlaplace(Raw &src)
{
	Raw val(src);
	for (int i=2;i<src.getXsize()-2;i++)
	{
		for (int j=2;j<src.getYsize()-2;j++)
		{
			for (int k=2;k<src.getZsize()-2;k++)
			{
				val.put(i,j,k,(src.get(i+1,j,k)+src.get(i-1,k,k)+src.get(i,j+1,k)+src.get(i,j-1,k)+src.get(i,j,k+1)+src.get(i,j,k-1)-6*src.get(i,j,k)));

			}
		}
	}

	return val;
}

Raw WipeNioisePde::FourPDiff(Raw &src)			//based on Y-K model
{
	PIXTYPE sum;
	int x,y,z,j;
	Raw s;
	Raw d(src);
	for (int j=0;j<delt;j++)
	{
		s=gradientlaplace(src);
		s=s/(s*s+1);
		s=gradientlaplace(s);
		s=d-s/double(6);
	}
	return s;

}




