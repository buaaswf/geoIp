#include "vol_math_GuassFilter.h"
#include "vol_math_filter_Interface.h"
void  Filter::guass3DFilterSipl(Raw* src, Raw *ret,int iter, int halfsize,void (*ProgressChanged)(int,int ,int,bool&))
{
	globalProgressChanged = src->size();
	progressStep =0;
	ProgressChanged = ProgressChanged;

	int i=0,j=0,k=0,m=0,n=0,l=0,width=0,depth=0,length=0;
	float sum = 0;
	int delta=1;
	Raw *temp;
	if (iter !=0 && (iter+1)*ret->getZsize() < src->getZsize())
	{
		temp = new Raw(src->getXsize(),src->getYsize(),ret->getZsize() + 2*halfsize,src->getdata()+ 
			ret->getXsize()*ret->getYsize()*ret->getZsize()*iter-halfsize*ret->getXsize()*ret->getYsize(),true);
	} 
	else if ( (iter == 0 && (iter+1)*ret->getZsize() !=  src->getZsize())|| ((iter+1)*ret->getZsize() ==  src->getZsize()&& iter !=0 ))
	{
		if ( iter ==0 )
		{
			temp = new Raw(ret->getXsize(),ret->getYsize(),ret->getZsize()+halfsize,src->getdata(),true);
		} 
		else
		{
			temp = new Raw(ret->getXsize(),ret->getYsize(), ret->getZsize()+halfsize, src->getdata() + iter*ret->getXsize()*ret->getYsize()*ret->getZsize()-ret->getXsize()*ret->getYsize() * halfsize,true);
		}
	}
	else 
	{
		temp = new Raw(ret->getXsize(),ret->getYsize(),ret->getZsize(),src->getdata(),true);
	}
	//globalProgressChanged = src->size();
	size_t interval = globalProgressChanged/1000 == 0? 1:globalProgressChanged/1000;
	int rs = 0;
	bool flag = false;
	//Raw * _ret =new Raw(*temp);
	width=temp->getXsize();
	length=temp->getYsize();
	depth = ret->getZsize();
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
					ProgressChanged (1, 100,(int) (long long)( progressStep)*100/(globalProgressChanged + 
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
			rs++;

			ret->putXYZ(i ,  guass->getXYZ(i + ret->getXsize()*ret->getYsize()));//?????
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