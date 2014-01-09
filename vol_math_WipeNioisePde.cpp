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
		FourPDiffSipl(src,ret,iter);
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
	 Raw * temp;
	if (iter > 0 && ( iter+1 )*ret.getZsize() < src.getZsize())
	{
		Raw *s = new Raw(ret.getXsize(), ret.getYsize(), ret.getZsize() + 2, src.getdata()+iter*ret.getXsize()*ret.getYsize()*(ret.getZsize())-ret.getXsize()*ret.getYsize());
		temp = new Raw(*s); 

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
						//swf 2014-1-9 for divide by zero
						if(val == 0)
							sum = 0;
						for (int k=0; k < 6; k++)
						{
							//implementation sum(g(i)*f(i))
							sum += around[k]/(1+around[k]*around[k]/(val*val));

						}
						temp->put(x,y,z,s->get(x,y,z)+a*sum/double(6));
						//ret.put(x,y,z-1,s->get(x,y,z));

					}//for
				}//for
				//cout << "times = :" << i << endl;
			}//for
			s = temp;
		}//for
		for (int i = 0; i < ret.size(); i++ )
		{
			ret.putXYZ( i, temp->getXYZ(i + ret.getXsize() * ret.getYsize()) );
		}
		delete s;
	} 
	else
	{
		if ((iter == 0 && (iter+1)*ret.getZsize() !=  src.getZsize())|| ((iter+1)*ret.getZsize() ==  src.getZsize() && iter !=0 ))
		{
			Raw *s;
			if (iter != 0)
			{
				 s= new Raw(ret.getXsize(),ret.getYsize(),ret.getZsize() + 1,
					src.getdata()+ iter * ret.getXsize() * ret.getYsize() * ret.getZsize()-ret.getXsize()*ret.getYsize());
			} 
			else
			{
				s = new Raw(ret.getXsize(),ret.getYsize(),ret.getZsize() + 1,src.getdata());
			}
			
			temp = new Raw (*s);
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

                            //qym 2014-1-8 for divide by zero
                            if(val == 0)
                                sum = 0;

							for (int k=0; k < 6; k++)
							{
								//implementation sum(g(i)*f(i))
								sum+=around[k]/(1+around[k]*around[k]/(val*val));

							}
							temp->put(x,y,z,s->get(x,y,z)+a*sum/double(6));

						}//for
					}//for
					
				}//for
				s = temp;
			}//for
			if (iter == 0)
			{
				for (int i =0; i < ret.size(); i++)
				{
					ret.putXYZ(i ,temp->getXYZ(i));
				}
			} 
			else
			{
				for (int i =0; i < ret.size(); i++)
				{
					ret.putXYZ(i ,temp->getXYZ(i + ret.getXsize()*ret.getYsize() ));
				}
			}

			delete s;
		}//if
		else
		{
			//P-M function  2nd-order PDE denoise
			PIXTYPE a=1,sum;
			int z,y,x,K,j,i;
			//Raw *d=new Raw(src.getXsize(),src.getYsize(),src.getZsize(),src.getdata());
			Raw *d=new Raw(src.getXsize(), src.getYsize(), src.getZsize(), src.getdata());
			temp = new Raw(*d);
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
							//swf 2014-1-9 for divide by zero
							if(val == 0)
								sum = 0;
							for (int k=0; k < 6; k++)
							{
								//implementation sum(g(i)*f(i))
								sum += around[k]/(1 + around[k] * around[k]/(val * val));

							}
							temp->put(x,y,z,d->get(x,y,z)+a*sum/double(6));

						}//for
					}//for
					//cout << "times = :" << i << endl;
				}//for
				//d += s*(-1);
				d = temp;
			}//for delte
			for ( int i = 0; i < ret.size(); i ++)
			{
				ret.putXYZ(i , temp->getXYZ(i) );
			}
			delete d;
			//src = s;
			//return  d;
		}


	}
	//if (iter !=0 && (iter+1)*ret.getZsize() < src.getZsize())
	//{


	//} 
	//else if ((iter == 0 && (iter+1)*ret.getZsize() !=  src.getZsize())|| ((iter+1)*ret.getZsize() ==  src.getZsize()&& iter !=0 ))
	//{


	//} 
	//else
	//{


	//}

	
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
void  gradientlaplaceSipl(Raw &src,Raw &_ret,int iter)
{
	if (iter > 0 && (iter+1) *_ret.getZsize() != src.getZsize())
	{
		Raw *temp = new Raw (src.getXsize(),src.getYsize(),_ret.getZsize() + 2, src.getdata() + src.getXsize()*src.getYsize()*(_ret.getZsize()-1) );
		for (int i = 1; i < temp->getXsize()-1; i++)
		{
			for (int j = 1; j < temp->getYsize()-1; j++)
			{
				for (int k = 1; k < temp->getZsize()-1; k++)
				{
					_ret.put(i, j, k-1, (temp->get(i+1,j,k)+temp->get(i-1,k,k)+temp->get(i,j+1,k)+temp->get(i,j-1,k)+temp->get(i,j,k+1)+temp->get(i,j,k-1)-6*temp->get(i,j,k)));

				}
			}
		}

	}
	else if ( iter == 0 && (iter+1) *_ret.getZsize() != src.getZsize() || iter != 0 && (iter+1) *_ret.getZsize() == src.getZsize())
	{
		Raw *temp = new Raw(_ret.getXsize(),_ret.getYsize(),_ret.getZsize()+1,src.getdata()+iter*_ret.getXsize()*_ret.getYsize()*(_ret.getZsize()-1));
		for (int i = 1; i < temp->getXsize()-1; i++)
		{
			for (int j = 1; j < temp->getYsize()-1; j++)
			{
				for (int k = 1; k < temp->getZsize()-1; k++)
				{
					_ret.put(i, j, k-1, (temp->get(i+1,j,k) + temp->get(i-1,k,k) + temp->get(i,j+1,k) + temp->get(i,j-1,k)+temp->get(i,j,k+1)+temp->get(i,j,k-1)-6*temp->get(i,j,k)));

				}
			}
		}
	}
	else 
	{
		Raw *temp = new Raw(_ret.getXsize(),_ret.getYsize(),_ret.getZsize(),src.getdata());
		for (int i = 1; i < temp->getXsize()-1; i++)
		{
			for (int j = 1; j < temp->getYsize()-1; j++)
			{
				for (int k = 1; k < temp->getZsize()-1; k++)
				{
					_ret.put(i, j, k-1, (temp->get(i+1,j,k) + temp->get(i-1,k,k) + temp->get(i,j+1,k) + temp->get(i,j-1,k)+temp->get(i,j,k+1)+temp->get(i,j,k-1)-6*temp->get(i,j,k)));

				}
			}
		}
		delete temp;
	}
	
	
	
}
Raw WipeNioisePde::FourPDiff(Raw &src)			//based on Y-K model
{
	
	PIXTYPE sum;
	int x,y,z,j;
	Raw s;
	Raw d(src);
	for (int j=0;j<delt;j++)
	{
		s = gradientlaplace(src);
		s = s/(s*s+1);
		s = gradientlaplace(s);
		s=d-s/double(6);
	}
	return s;

}

void  WipeNioisePde::FourPDiffSipl(Raw &src,Raw &ret,int iter)			//based on Y-K model
{
	PIXTYPE sum;
	int x,y,z,j;
	Raw *s = new Raw (ret.getXsize(), ret.getYsize(), ret.getZsize(), ret.getdata()) ;
		//s2(ret);
	//Raw d(src);
	//if (iter > 0 && (iter+1) *ret.getZsize() != src.getZsize())
	//{
		for (int j=0;j < delt; j++)
		{
			gradientlaplaceSipl(src, *s, iter);
			*s = *s/(*s**s+1);
			gradientlaplaceSipl(src, *s, iter);
			*s = *s - *s/double(6);
		}
//} 
	//else if (iter == 0 && (iter+1) *ret.getZsize() == src.getZsize() || iter != 0 && (iter+1) *ret.getZsize() == src.getZsize())
	//{
		//for (int j=0;j < delt; j++)
		//{
		//	ret=gradientlaplaceSipl(src, ret, iter);
		//	ret=ret/(ret*ret+1);
		//	ret=gradientlaplaceSipl(src, ret, iter);
		//	ret=src-ret/double(6);
		//}
	//}

		for ( int i =0; i < s->size() ;i++)
		{
			ret.putXYZ(i,s->getXYZ(i));
		}
		//ret = *s;
		delete s;
	
	//return ret;

}


