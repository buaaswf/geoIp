#include "vol_math_WipeNioisePde.h"
#include <limits>



WipeNioisePde::WipeNioisePde(Raw &src,Raw &ret,int iter,int time,PIXTYPE value, int method, void(*ProgressChanged)(int,int,int,bool &))
{
	//raw=src;
	delt=time;
	val=value;
	way=method;
	this->ProgressChanged = ProgressChanged;
	if (way==1)
	{
		Perona_MalikSipl(src,ret,iter);
		//cout <<"aaa"<<endl;
	} 
	else
	{
		FourPDiff(src,ret);
	}
}
WipeNioisePde::WipeNioisePde(Raw *src,Raw *ret,int iter,int time,PIXTYPE value, int method, void(*ProgressChanged)(int,int,int,bool &))
{
	//raw=src;
	delt=time;
	val=value;
	way=method;
	this->ProgressChanged = ProgressChanged;
	if (way==1)
	{
		Perona_MalikSipl(src,ret,iter);
		//cout <<"aaa"<<endl;
	} 
	else
	{
		FourPDiff(*src,*ret);
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
	} 
	else
	{
		src=FourPDiff(src);
	}
}
WipeNioisePde::WipeNioisePde()
{

}
WipeNioisePde::~WipeNioisePde(void)
{
}
void WipeNioisePde::Perona_MalikSipl(Raw *src,Raw *ret,int iter)
{
	//size_t progresstotal = src->size()*this->delt;
	//size_t progressstep = .size();
	globalProgressChanged = src->size();//*this->delt;
	float a=1,sum;
	int z,y,x,K,j,i;
	//Raw *d=new Raw(src->getXsize(),src->getYsize(),src->getZsize(),src->getdata());
	//Raw *d=new Raw(ret->getXsize(),ret->getYsize(),ret->getZsize(),ret->getdata());
	//Raw s=Raw(src);
	 Raw * temp;
	 float Maxvar;
	 if ( sizeof (PIXTYPE) == 1)
	 {
		 Maxvar = 255;
	 } 
	 else if ( sizeof (PIXTYPE) == 2)
	 {
		 //qym 2014-1-10
		 //Maxvar = 65536;
		 Maxvar = 65535;
	 } 
	 else 
	 {
		 //qym 2014-1-10
		 //Maxvar = 10000000;
		 Maxvar = std::numeric_limits<float>::max();
	 }
	if (iter > 0 && ( iter+1 )*ret->getZsize() < src->getZsize())
	{
		Raw *s = new Raw(ret->getXsize(), ret->getYsize(), ret->getZsize() + 2, src->getdata()+iter*ret->getXsize()*ret->getYsize()*(ret->getZsize())-ret->getXsize()*ret->getYsize());
		temp = new Raw(*s); 

		PIXTYPE *around =new PIXTYPE[6];
		//size_t threadtotal = s->size();
		size_t step = iter *ret->size();
		int call = 10000;
		bool flag = false;
	
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

						else 
						{
							for (int k=0; k < 6; k++)
							{
								//implementation sum(g(i)*f(i))
								sum += around[k]/(1 + around[k]*around[k]/(val*val));

							}
							float tempval = s->get(x,y,z) + a*sum/double(6);

							if (tempval <= Maxvar)
							{
								temp->put(x, y, z, tempval);
							} 
							else 
							{
								temp->put(x, y, z, s->get(x,y,z));
							}

						}//else val !=0...

					}//forx...
				}//fory...
				//cout << "times = :" << i << endl;
			}//for z...
			s = temp;
		}//for delta .....
		int interval = globalProgressChanged/1000 == 0 ? 1:globalProgressChanged /1000 ;//first call diygieshi0 houmianshi 1
		int rs = 0 ;
		//bool flag = false;
		for ( int i = 0; i < ret->size(); i ++)
		{

			rs ++;
			if ( rs == interval && ProgressChanged != NULL )
			{
				progressStep += interval;
				rs = 0;
				ProgressChanged (1, 100,(int) (long long)( progressStep)*100/(globalProgressChanged ),flag);
			}

			ret->putXYZ( i, temp->getXYZ(i + ret->getXsize() * ret->getYsize()) );
		}
		delete s;
	} 
	else
	{
		if ((iter == 0 && (iter+1)*ret->getZsize() !=  src->getZsize()) || ((iter+1)*ret->getZsize() ==  src->getZsize() && iter !=0 ))
		{
			/*
				two cases:
				one:number one time
				two:the last block of the multi thread 
			*/

			Raw *s;
			if (iter != 0)
			{
				 s= new Raw(ret->getXsize(),ret->getYsize(),ret->getZsize() + 1,
					src->getdata()+ iter * ret->getXsize() * ret->getYsize() * ret->getZsize()-ret->getXsize()*ret->getYsize());
			} 
			else
			{
				s = new Raw(ret->getXsize(),ret->getYsize(),ret->getZsize() + 1,src->getdata());
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

							else
							{
								for (int k=0; k < 6; k++)
								{
									//implementation sum(g(i)*f(i))
									sum+=around[k]/(1+around[k]*around[k]/(val*val));

								}
								float tempval = s->get(x,y,z) + a*sum/double(6);
								if (tempval <= Maxvar)
								{
									temp->put(x, y, z, tempval);
								} 
								else 
								{
									temp->put(x, y, z, s->get(x,y,z));
								}
							}
						}//for x..

					}//for  y..

				}//forz..
				s = temp;
			}//for delta
			//size_t step = iter *ret->size();
			//int call = 10000;
			bool flag = false;
			int interval = globalProgressChanged/1000 == 0 ? 1:globalProgressChanged /1000 ;//first call diygieshi0 houmianshi 1
			int rs = 0 ;
			//bool flag = false;
			if (iter == 0)
			{
				int firstvar;
				if ( progressStep == 0 )
				{
					firstvar = 0;
				}
				else firstvar = 1;
				for ( int i = 0; i < ret->size(); i ++)
				{
					rs ++;
					
					if ( rs == interval && ProgressChanged != NULL )
					{

						progressStep += interval;
						rs = 0;
						ProgressChanged (firstvar, 100, (int) (long long)( progressStep)*100/(globalProgressChanged ), flag);
					}
					ret->putXYZ(i ,temp->getXYZ(i));
				}
			} 
			else//..iter!=0
			{
				int interval = globalProgressChanged/1000 == 0 ? 1:globalProgressChanged /1000 ;//first call diygieshi0 houmianshi 1
				int rs = 0 ;
				bool flag = false;

				for (int i = 0; i < ret->size(); i++)
				{
					rs++;
					if ( rs == interval && ProgressChanged != NULL )
					{
						progressStep += interval;
						rs = 0;
						ProgressChanged (1, 100,(int) (long long)( progressStep)*100/(globalProgressChanged ),flag);
					}
					ret->putXYZ(i ,temp->getXYZ(i + ret->getXsize()*ret->getYsize() ));
				}
			}//else ..._if iter =0

			delete s;
		}//if
		else
		{

			//P-M function  2nd-order PDE denoise
			PIXTYPE a=1,sum;
			int z,y,x,K,j,i;
			//Raw *d=new Raw(src->getXsize(),src->getYsize(),src->getZsize(),src->getdata());
			Raw *d=new Raw(src->getXsize(), src->getYsize(), src->getZsize(), src->getdata());
			temp = new Raw(*d);
			//Raw s=Raw(src);
			PIXTYPE *around=new PIXTYPE[6];

			for (i = 0 ;i < delt; i++)
			{
				for (z = 1; z < src->getZsize()-1;z++)
				{
					for ( y = 1; y < src->getYsize()-1; y++)
					{
						for ( x = 1; x < src->getXsize()-1; x++)
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
							else
							{
									for (int k=0; k < 6; k++)
									{
										//implementation sum(g(i)*f(i))
										sum += around[k]/(1 + around[k] * around[k]/(val * val));

									}
									float tempval = d->get(x,y,z) + a*sum/double(6);
									if ( tempval <= Maxvar )
									{
										temp->put(x, y, z, tempval);
									} 
									else 
									{
										temp->put(x, y, z, d->get(x,y,z));
									}
							}
						}//for
					}//for
					//cout << "times = :" << i << endl;
				}//for
				//d += s*(-1);
				d = temp;
			}//for delte
			//size_t step = iter *ret->size();
			int interval = globalProgressChanged/1000 == 0 ? 1:globalProgressChanged /1000 ;//first call diygieshi0 houmianshi 1
			int rs = 0 ;
			bool flag = false;
			//recover  data order from  x,z,y
			// 


			for ( int i = 0; i < ret->size(); i ++)
			{
				
				rs ++;
				if ( rs == interval && ProgressChanged != NULL )
				{
					progressStep += interval;
					rs = 0;
					ProgressChanged (1, 100,(int) (long long)( progressStep)*100/(globalProgressChanged ),flag);
				}
				ret->putXYZ(i , temp->getXYZ(i) );
			}
			delete d;
			//src = s;
			//return  d;
		}


	}
	//if (iter !=0 && (iter+1)*ret->getZsize() < src->getZsize())
	//{


	//} 
	//else if ((iter == 0 && (iter+1)*ret->getZsize() !=  src->getZsize())|| ((iter+1)*ret->getZsize() ==  src->getZsize()&& iter !=0 ))
	//{


	//} 
	//else
	//{


	//}


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
void  WipeNioisePde::Perona_MalikSipl( Raw &src,Raw & ret,int iter)
{
	//size_t progresstotal = src.size()*this->delt;
	//size_t progressstep = .size();
	globalProgressChanged = src.size();//*this->delt;
	float a=1,sum;
	int z,y,x,K,j,i;
	//Raw *d=new Raw(src.getXsize(),src.getYsize(),src.getZsize(),src.getdata());
	//Raw *d=new Raw(ret.getXsize(),ret.getYsize(),ret.getZsize(),ret.getdata());
	//Raw s=Raw(src);
	 Raw * temp;
	 float Maxvar;
	 if ( sizeof (PIXTYPE) == 1)
	 {
		 Maxvar = 255;
	 } 
	 else if ( sizeof (PIXTYPE) == 2)
	 {
		 //qym 2014-1-10
		 //Maxvar = 65536;
		 Maxvar = 65535;
	 } 
	 else 
	 {
		 //qym 2014-1-10
		 //Maxvar = 10000000;
		 Maxvar = std::numeric_limits<float>::max();
	 }
	if (iter > 0 && ( iter+1 )*ret.getZsize() < src.getZsize())
	{
		Raw *s = new Raw(ret.getXsize(), ret.getYsize(), ret.getZsize() + 2, src.getdata()+iter*ret.getXsize()*ret.getYsize()*(ret.getZsize())-ret.getXsize()*ret.getYsize());
		temp = new Raw(*s); 

		PIXTYPE *around =new PIXTYPE[6];
		//size_t threadtotal = s->size();
		size_t step = iter *ret.size();
		int call = 10000;
		bool flag = false;
	
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

						else 
						{
							for (int k=0; k < 6; k++)
							{
								//implementation sum(g(i)*f(i))
								sum += around[k]/(1 + around[k]*around[k]/(val*val));

							}
							float tempval = s->get(x,y,z) + a*sum/double(6);

							if (tempval <= Maxvar)
							{
								temp->put(x, y, z, tempval);
							} 
							else 
							{
								temp->put(x, y, z, s->get(x,y,z));
							}

						}//else val !=0...

					}//forx...
				}//fory...
				//cout << "times = :" << i << endl;
			}//for z...
			s = temp;
		}//for delta .....
		int interval = globalProgressChanged/1000 == 0 ? 1:globalProgressChanged /1000 ;//first call diygieshi0 houmianshi 1
		int rs = 0 ;
		//bool flag = false;
		for ( int i = 0; i < ret.size(); i ++)
		{

			rs ++;
			if ( rs == interval && ProgressChanged != NULL )
			{
				progressStep += interval;
				rs = 0;
				ProgressChanged (1, 100,(int) (long long)( progressStep)*100/(globalProgressChanged ),flag);
			}

			ret.putXYZ( i, temp->getXYZ(i + ret.getXsize() * ret.getYsize()) );
		}
		delete s;
	} 
	else
	{
		if ((iter == 0 && (iter+1)*ret.getZsize() !=  src.getZsize()) || ((iter+1)*ret.getZsize() >=  src.getZsize() && iter !=0 ))
		{
			/*
				two cases:
				one:number one time
				two:the last block of the multi thread 
			*/

			Raw *s;
			if (iter != 0)
			{
				 s= new Raw(ret.getXsize(),ret.getYsize(),ret.getZsize() + 1,
					src.getdata()+ iter*ret.getXsize()*ret.getYsize()*(src.getZsize()/(iter+1))-ret.getXsize()*ret.getYsize());
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

							else
							{
								for (int k=0; k < 6; k++)
								{
									//implementation sum(g(i)*f(i))
									sum+=around[k]/(1+around[k]*around[k]/(val*val));

								}
								float tempval = s->get(x,y,z) + a*sum/double(6);
								if (tempval <= Maxvar)
								{
									temp->put(x, y, z, tempval);
								} 
								else 
								{
									temp->put(x, y, z, s->get(x,y,z));
								}
							}
						}//for x..

					}//for  y..

				}//forz..
				s = temp;
			}//for delta
			//size_t step = iter *ret.size();
			//int call = 10000;
			bool flag = false;
			int interval = globalProgressChanged/1000 == 0 ? 1:globalProgressChanged /1000 ;//first call diygieshi0 houmianshi 1
			int rs = 0 ;
			//bool flag = false;
			if (iter == 0)
			{
				int firstvar;
				if ( progressStep == 0 )
				{
					firstvar = 0;
				}
				else firstvar = 1;
				for ( int i = 0; i < ret.size(); i ++)
				{
					rs ++;
					
					if ( rs == interval && ProgressChanged != NULL )
					{

						progressStep += interval;
						rs = 0;
						ProgressChanged (firstvar, 100, (int) (long long)( progressStep)*100/(globalProgressChanged ), flag);
					}
					ret.putXYZ(i ,temp->getXYZ(i));
				}
			} 
			else//..iter!=0
			{
				int interval = globalProgressChanged/1000 == 0 ? 1:globalProgressChanged /1000 ;//first call diygieshi0 houmianshi 1
				int rs = 0 ;
				bool flag = false;

				for (int i = 0; i < ret.size(); i++)
				{
					rs++;
					if ( rs == interval && ProgressChanged != NULL )
					{
						progressStep += interval;
						rs = 0;
						ProgressChanged (1, 100,(int) (long long)( progressStep)*100/(globalProgressChanged ),flag);
					}
					ret.putXYZ(i ,temp->getXYZ(i + ret.getXsize()*ret.getYsize() ));
				}
			}//else ..._if iter =0

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
							else
							{
									for (int k=0; k < 6; k++)
									{
										//implementation sum(g(i)*f(i))
										sum += around[k]/(1 + around[k] * around[k]/(val * val));

									}
									float tempval = d->get(x,y,z) + a*sum/double(6);
									if ( tempval <= Maxvar )
									{
										temp->put(x, y, z, tempval);
									} 
									else 
									{
										temp->put(x, y, z, d->get(x,y,z));
									}
							}
						}//for
					}//for
					//cout << "times = :" << i << endl;
				}//for
				//d += s*(-1);
				d = temp;
			}//for delte
			//size_t step = iter *ret.size();
			int interval = globalProgressChanged/1000 == 0 ? 1:globalProgressChanged /1000 ;//first call diygieshi0 houmianshi 1
			int rs = 0 ;
			bool flag = false;
			for ( int i = 0; i < ret.size(); i ++)
			{
				
				rs ++;
				if ( rs == interval && ProgressChanged != NULL )
				{
					progressStep += interval;
					rs = 0;
					ProgressChanged (1, 100,(int) (long long)( progressStep)*100/(globalProgressChanged ),flag);
				}
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
void gradientlaplace(Raw &src,Raw &ret)
{
	//Raw val(src);
	for (int i=2;i<src.getXsize()-2;i++)
	{
		for (int j=2;j<src.getYsize()-2;j++)
		{
			for (int k=2;k<src.getZsize()-2;k++)
			{
				ret.put(i,j,k,(src.get(i+1,j,k)+src.get(i-1,k,k)+src.get(i,j+1,k)+src.get(i,j-1,k)+src.get(i,j,k+1)+src.get(i,j,k-1)-6*src.get(i,j,k)));

			}
		}
	}

	//return val;
}
void  gradientlaplaceSipl(Raw &src,Raw &_ret,int iter)
{
	if (iter > 0 && (iter+1) *(_ret.getZsize()) != src.getZsize())
	{
		Raw *temp = new Raw (src.getXsize(),src.getYsize(),(_ret.getZsize() + 2), 
			src.getdata() + iter*src.getXsize()*src.getYsize()*_ret.getZsize()-_ret.getXsize()*_ret.getYsize() );
		Raw *s= new Raw(*temp);
		for (int i = 1; i < temp->getXsize()-1; i++)
		{
			for (int j = 1; j < temp->getYsize()-1; j++)
			{
				for (int k = 1; k < temp->getZsize()-1; k++)
				{
					temp->put(i, j, k-1, (s->get(i+1,j,k)+s->get(i-1,j,k)+s->get(i,j+1,k)+s->get(i,j-1,k)+s->get(i,j,k+1)+s->get(i,j,k-1)-6*s->get(i,j,k)));

				}
			}
		}

	}
	else if ( iter == 0 && (iter+1) *_ret.getZsize() != src.getZsize() || iter != 0 && (iter+1) *_ret.getZsize() == src.getZsize())
	{
		Raw  *temp;
		if (iter != 0)
		{
			temp = new Raw(_ret.getXsize(),_ret.getYsize(),_ret.getZsize() + 1,
				src.getdata()+ iter * _ret.getXsize() * _ret.getYsize() * _ret.getZsize()- _ret.getXsize()*_ret.getYsize());
		} 
		else
		{
			temp = new Raw(_ret.getXsize(),_ret.getYsize(),_ret.getZsize() + 1,src.getdata());
		}
		Raw *s = new Raw(*temp);
		//Raw *temp = new Raw(_ret.getXsize(),_ret.getYsize(),_ret.getZsize()+1,src.getdata()+iter*_ret.getXsize()*_ret.getYsize()*(_ret.getZsize()-1));
		if ( iter == 0 )
		{
			for (int i = 1; i < temp->getXsize()-1; i++)
			{
				for (int j = 1; j < temp->getYsize()-1; j++)
				{
					for (int k = 1; k < temp->getZsize()-1; k++)
					{
						temp->put(i, j, k-1, (s->get(i+1,j,k) + s->get(i-1,j,k) + s->get(i,j+1,k) + s->get(i,j-1,k) + s->get(i,j,k+1) + s->get(i,j,k-1)-6*s->get(i,j,k)));

					}
				}
			}
		} 
		else
		{
			for (int i = 1; i < temp->getXsize()-1; i++)
			{
				for (int j = 1; j < temp->getYsize()-1; j++)
				{
					for (int k = 1; k < temp->getZsize()-1; k++)
					{
						temp->put(i, j, k, (s->get(i+1,j,k) + s->get(i-1,k,k) + s->get(i,j+1,k) +
							s->get(i,j-1,k) + s->get(i,j,k+1) + s->get(i,j,k-1)-6*s->get(i,j,k)));

					}
				}
			}
		}//else
		if ( iter == 0 )
		{
			for ( int i = 0; i < temp->size(); i++)
			{

				_ret.putXYZ(i,temp->getXYZ(i));
			}
		} 
		else
		{
			for ( int i = 0;i < _ret.size(); i++ )
			{
				_ret.putXYZ(i,temp->getXYZ(i + temp->getXsize()*temp->getYsize()));
			}
		}
		
		
	}//if first and last
	else 
	{
		Raw *temp = new Raw(_ret.getXsize(),_ret.getYsize(),_ret.getZsize(),src.getdata());
		for (int i = 1; i < temp->getXsize()-1; i++)
		{
			for (int j = 1; j < temp->getYsize()-1; j++)
			{
				for (int k = 1; k < temp->getZsize()-1; k++)
				{
					_ret.put(i, j, k-1, (temp->get(i+1,j,k) + temp->get(i-1,j,k) + temp->get(i,j+1,k) + temp->get(i,j-1,k)+temp->get(i,j,k+1)+temp->get(i,j,k-1)-6*temp->get(i,j,k)));

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
void  WipeNioisePde::FourPDiff(Raw &src,Raw &ret)			//based on Y-K model
{

	PIXTYPE sum;
	int x,y,z,j;
	//Raw s;
	Raw d(src);
	for (int j=0;j<delt;j++)
	{
		ret = gradientlaplace(d);
		ret = ret/(ret*ret+1);
		ret = gradientlaplace(ret);
			for (int i=0; i < ret.size() ; i++)
			{
				float t=0;
				if ( (t = d.getXYZ(i) - ret.getXYZ(i)/6.0) <= 0 )
				{
					ret.putXYZ(i,0);// =  *d - *dd/double(6);
					//*s = 0;
				} 
				else
				{
					ret.putXYZ(i,t);
				}
			}
		//ret = d-ret/double(6);
		d = ret; 
	}
	//return s;

}
void  WipeNioisePde::FourPDiffSipl(Raw &src,Raw &ret,int iter)			//based on Y-K model
{
	PIXTYPE sum;
	int x,y,z,j;
	Raw *s = new Raw (ret.getXsize(), ret.getYsize(), src.getZsize(), src.getdata()) ;

	Raw *d=new Raw(ret);

		for (int j=0;j < delt; j++)
		{
			gradientlaplaceSipl(*s, *d, iter);
			ret =*d;
			*d *=*d;
			*d += 1;
			*d = ret / *d;
			//*d = *d/((*d) * (*d) + 1);
			Raw * dd = new Raw(*d);
			gradientlaplace(*d,*dd);//d must the same size as src
			*d = *s;
			

				
			
			*d = *s;
			//delete dd;
		}

		//delete d;
		for ( int i =0; i < s->size() ;i++)
		{
			ret.putXYZ(i,s->getXYZ(i));
		}

		delete s;


}


