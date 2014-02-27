#include "vol_math_GuassFilter.h"
#include "vol_math_filter_Interface.h"

void puregauss(Raw *temp,int rs,Raw *src,Raw *ret,int halfsize,int sum,int globalProgressChanged,int progressStep,void (*ProgressChanged)(int,int ,int,bool&))
{
	size_t interval = globalProgressChanged/1000 == 0? 1:globalProgressChanged/1000;
	//int rs = 0;
	bool flag = false;
	//Raw * _ret =new Raw(*temp);
	int width = temp->getXsize();
	int length = temp->getYsize();
	int depth  = temp->getZsize();
	Raw *guass = new Raw(*temp);	///< The result of guass filter. 

	for (int i = 0; i < depth; i++)
	{
		for (int j = 0; j < length; j++)
		{
			for (int k =0; k < width; k++)
			{
				rs++;
				if ( rs == interval && ProgressChanged != NULL )
				{
					rs = 0;
					progressStep += interval;
					ProgressChanged (1, 100,(long long)( progressStep)*100/(globalProgressChanged + 
						(src->getZsize()/ret->getZsize()-1)*halfsize*2*ret->getXsize() * ret->getYsize()),flag);
				}

				sum=0;
				float weight=0, total=0;
				for( int l=i-halfsize; l<=i+halfsize; l++)
				{
					for(int n=j-halfsize; n<=j+halfsize; n++)
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
	for (int i =0; i < ret->size(); i++)
	{
		rs++;

		ret->putXYZ(i ,  guass->getXYZ(i));
	}


	delete guass;
	delete temp;
}


void  Filter::guass3DFilterSipl(Raw* src, Raw *ret,int iter, int halfsize,void (*ProgressChanged)(int,int ,int,bool&))
{
	
	globalProgressChanged = src->size();
	bool flag = false;
	int rs = 0 ;
	if (  ProgressChanged != NULL )
	{
		ProgressChanged (0, 100,0,flag);
	}
	progressStep =0;
	ProgressChanged = ProgressChanged;

	int i=0,j=0,k=0,m=0,n=0,l=0,width=0,depth=0,length=0;
	float sum = 0;
	int delta=1;
	Raw *temp;
	bool MULTITHREADMIDLESILCES = iter !=0 && (iter+1)*ret->getZsize() < src->getZsize();
	bool MULTITHREADFIRST = (iter == 0 && (iter+1)*ret->getZsize() !=  src->getZsize());
	bool MULTITHREADLAST =   ((iter+1)*ret->getZsize() >=  src->getZsize()&& iter !=0 );
	bool SINGLETHREAD = true ;
	//int rs = 0;
	if (MULTITHREADMIDLESILCES)
	{
		//Raw *s = new Raw(ret->getXsize(), ret->getYsize(), ret->getZsize() + 2, src->getdata()+iter*ret->size()-ret->getXsize()*ret->getYsize());

		temp = new Raw(src->getXsize(),src->getYsize(),ret->getZsize() + 2*halfsize,src->getdata()+ 
			ret->size()*iter-halfsize*ret->getXsize()*ret->getYsize(),true);
		//puregauss(temp,rs,src,ret,halfsize,sum,globalProgressChanged,progressStep,ProgressChanged);

		//globalProgressChanged = src->size();
		size_t interval = globalProgressChanged/1000 == 0? 1:globalProgressChanged/1000;
		
		bool flag = false;
		//Raw * _ret =new Raw(*temp);
		width = temp->getXsize();
		length = temp->getYsize();
		depth = temp->getZsize();
		Raw *guass = new Raw(*temp);	///< The result of guass filter. 

		for (i = 0; i < depth; i++)
		{
			for (j = 0; j < length; j++)
			{
				for (k =0; k < width; k++)
				{
					rs++;
					if ( rs == interval && ProgressChanged != NULL )
					{
						rs = 0;
						progressStep += interval;
						ProgressChanged (1, 100, (long long)( progressStep)*100/(globalProgressChanged + 
							(src->getZsize()/ret->getZsize()-1)*halfsize*2*ret->getXsize() * ret->getYsize()),flag);
					}

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
									if (i == 250 && j == 407 && k == 0)
									{
										cout <<temp->get(i,j,k)<<endl;
									}
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
		for (int i = 0; i < ret->size();i++ )
		{

			ret->putXYZ( i, guass->getXYZ(i + ret->getXsize()*ret->getYsize()) );
		}
		delete guass;
		delete temp;
	} 

	else if ( MULTITHREADFIRST|| MULTITHREADLAST )
	{
		if ( iter ==0 )
		{
			temp = new Raw(ret->getXsize(),ret->getYsize(),ret->getZsize()+ halfsize,src->getdata(),true);
			//puregauss(temp,rs,src,ret,halfsize,sum,globalProgressChanged,progressStep,ProgressChanged);
			globalProgressChanged = src->size();
			size_t interval = globalProgressChanged/1000 == 0? 1:globalProgressChanged/1000;
			//int rs = 0;
			bool flag = false;
			//Raw * _ret =new Raw(*temp);
			width = temp->getXsize();
			length = temp->getYsize();
			depth  = temp->getZsize();
			Raw *guass = new Raw(*temp);	///< The result of guass filter. 

			for (i = 0; i < depth; i++)
			{
				for (j = 0; j < length; j++)
				{
					for (k =0; k < width; k++)
					{
						rs++;
						if ( rs == interval && ProgressChanged != NULL )
						{
							rs = 0;
							progressStep += interval;
							ProgressChanged (1, 100,(long long)( progressStep)*100/(globalProgressChanged + 
								(src->getZsize()/ret->getZsize()-1)*halfsize*2*ret->getXsize() * ret->getYsize()),flag);
						}

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
			for (int i =0; i < ret->size(); i++)
			{
				rs++;

				ret->putXYZ(i ,  guass->getXYZ(i));
			}


			delete guass;
			delete temp;
		} //...if first multi thread 
		else//last slice
		{
			temp = new Raw(ret->getXsize(),ret->getYsize(), ret->getZsize()+halfsize, 
				src->getdata() + iter*ret->getXsize()*ret->getYsize()*(src->getZsize()/(iter+1))-ret->getXsize()*ret->getYsize()*halfsize,true);
			//puregauss(temp,rs,src,ret,halfsize,sum,globalProgressChanged,progressStep,ProgressChanged);
			globalProgressChanged = src->size();
			size_t interval = globalProgressChanged/1000 == 0? 1:globalProgressChanged/1000;
			//int rs = 0;
			bool flag = false;
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
						rs++;
						if ( rs == interval && ProgressChanged != NULL )
						{
							rs = 0;
							progressStep += interval;
							ProgressChanged (1, 100,(long long)( progressStep)*100/(globalProgressChanged + 
								(src->getZsize()/ret->getZsize()-1)*halfsize*2*ret->getXsize() * ret->getYsize()),flag);
						}

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
			for (int i =0; i < ret->size(); i++)
			{
				rs++;

				ret->putXYZ(i ,  guass->getXYZ(i + ret->getXsize()*ret->getYsize()));//?????
			}
			delete guass;
			delete temp;
		}
		



	}
	else 
	{
		temp = new Raw(ret->getXsize(),ret->getYsize(),ret->getZsize(),src->getdata(),true);
		//puregauss(temp,rs,src,ret,halfsize,sum,globalProgressChanged,progressStep,ProgressChanged);
		globalProgressChanged = src->size();
		size_t interval = globalProgressChanged/1000 == 0? 1:globalProgressChanged/1000;
		//int rs = 0;
		bool flag = false;
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
					rs++;
					if ( rs == interval && ProgressChanged != NULL )
					{
						rs = 0;
						progressStep += interval;
						ProgressChanged (1, 100,(long long)( progressStep)*100/(globalProgressChanged + 
							(src->getZsize()/ret->getZsize()-1)*halfsize*2*ret->getXsize() * ret->getYsize()),flag);
					}

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
		for ( int i = 0; i < ret->size(); i ++)
		{

			ret->putXYZ(i , guass->getXYZ(i));
		}
		delete guass;
		delete temp;
	}
	

}