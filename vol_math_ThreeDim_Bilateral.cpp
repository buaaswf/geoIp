#include "vol_math_ThreeDim_Bilateral.h"

//qym
#include <math.h>
#include <string.h>
#include <limits>

ThreeDim_Bilateral::ThreeDim_Bilateral(Raw *image,Raw &ret,double sigmaD, double sigmaR,void(*ProgressChanged)(int, int, int, bool&))
{
	this->src=image;
	this->ret = &ret;
	int sigmaMax = max(sigmaD, sigmaR);
	this->ProgressChanged = ProgressChanged;



	this->kernelRadius = 2;//ceil((double)2 * sigmaMax);
	double twoSigmaRSquared = 2 * sigmaR * sigmaR;

	int kernelSize = this->kernelRadius * 2 + 1;
	kernelD = new double**[kernelSize];
	for(int i=0; i<kernelSize;i++)
	{
		kernelD[i] = new double*[kernelSize];
		for(int j=0; j<kernelSize;j++)
		{
			kernelD[i][j] = new double[kernelSize];
		}
	}

	int center = (kernelSize - 1) / 2;

	for (int x = -center; x < -center + kernelSize; x++) {
		for (int y = -center; y < -center + kernelSize; y++) {
			for (int z = -center; z < -center + kernelSize; z++)
			{
				kernelD[x + center][y + center][z + center] = this->gauss(sigmaD, x, y,z);//sigmaD
			}

		}
	}

	//delete kernelD;
	//kernelD = NULL;
	gaussSimilarity = new double[256];
	for (int i = 0; i < 256; i++) {
		gaussSimilarity[i] = exp((double)-((i) /twoSigmaRSquared));
	}


}
ThreeDim_Bilateral::ThreeDim_Bilateral(Raw *image,double sigmaD, double sigmaR)
{
	this->src=image;

	int sigmaMax = max(sigmaD, sigmaR);




	this->kernelRadius = 2;//ceil((double)2 * sigmaMax);//or 6
	double twoSigmaRSquared = 2 * sigmaR * sigmaR;

	int kernelSize = this->kernelRadius * 2 + 1;
	kernelD = new double**[kernelSize];
	for(int i=0; i<kernelSize;i++)
	{
		kernelD[i] = new double*[kernelSize];
		for(int j=0; j<kernelSize;j++)
		{
			kernelD[i][j] = new double[kernelSize];
		}
	}

	int center = (kernelSize - 1) / 2;

	for (int x = -center; x < -center + kernelSize; x++) {
		for (int y = -center; y < -center + kernelSize; y++) {
			for (int z = -center; z < -center + kernelSize; z++)
			{
				kernelD[x + center][y + center][z + center] = this->gauss(sigmaD, x, y,z);//sigmaD
			}

		}
	}


	gaussSimilarity = new double[256];
	for (int i = 0; i < 256; i++) {
		gaussSimilarity[i] = exp((double)-((i) /twoSigmaRSquared));
	}


}
double ThreeDim_Bilateral::getSpatialWeight(int m, int n,int l, int i, int j, int k)
{
	return kernelD[(int)(i-m + kernelRadius)][(int)(j-n + kernelRadius)][(int)(k-l + kernelRadius)];
}


void ThreeDim_Bilateral::apply(Raw &ret) {// ~i=y j=x 
	Raw * temp = new Raw(*src);
	//Raw *s =new Raw(ret);
	for (int i=0;i<src->getZsize();i++)
	{
		for (int j=0;j<src->getYsize();j++)
		{
			for (int k=0; k < src->getXsize(); k++)
			{
				if(i > 0 && j > 0 && k > 0 && i < src->getXsize() && j < src->getYsize() && k < src->getZsize()){
					double sum = 0;
					double totalWeight = 0;
					int intensityCenter =temp->get(i,j,k);


					int mMax = i + kernelRadius;
					int nMax = j + kernelRadius;
					int lMax = k + kernelRadius;
					double weight;

					for (int m = i-kernelRadius; m < mMax; m++) 
					{
						for (int n = j-kernelRadius; n < nMax; n++) 
						{
							for (int l = k-kernelRadius; l < lMax; l++)
							{
								if (this->isInsideBoundaries(m, n, l)) 
								{
									int intensityKernelPos = temp->get(m,n,l);
									weight = getSpatialWeight(m,n,l,i,j,k) * similarity(intensityKernelPos,intensityCenter);
									totalWeight += weight;
									sum += (weight * intensityKernelPos);
								}
							}

						}
					}
					int newvalue=(int)floor(sum / totalWeight);
					if (sum!=0)
					{
						int newvalue=(int)(sum / totalWeight);
						src->put(i,j,k,newvalue);

					}

				}//if..
			}//i..

		}//k..

	}//j..
	//src = *temp;
	//return *temp;


}
void ThreeDim_Bilateral::applySipl(int iter)
{

	globalProgressChanged = src->size();


	//middle slices
	if (iter !=0 && (iter+1)*ret->getZsize() < src->getZsize())
	{
		temp = new Raw(src->getXsize(),src->getYsize(),ret->getZsize() + 2*kernelRadius,
			src->getdata()+ ret->getXsize()*ret->getYsize()*ret->getZsize()*iter
			-(ret->getXsize()*ret->getYsize()*(int)kernelRadius),false);
		Raw *res=new Raw(*temp);
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
		//Raw *s =new Raw(*ret);
		int interval = globalProgressChanged/1000 == 0 ? 1:globalProgressChanged /1000 ;//first call diygieshi0 houmianshi 1
		int rs = 0 ;
		bool flag = false;
		for (int i = 0;i < temp->getZsize();i++)
		{
			for (int j = 0; j < temp->getYsize();j++)
			{
				for (int k=0; k < temp->getXsize(); k++)
				{

					rs ++;
					if ( rs == interval && ProgressChanged != NULL )
					{
						progressStep += interval;
						rs = 0;
						ProgressChanged (1, 100,(int) (long long)( progressStep)*100/(globalProgressChanged ),flag);
					}
					if(i>0 && j>0 && k>0 && i<ret->getZsize() && j< ret->getYsize() && k < ret->getXsize())
					{
						double sum = 0;
						double totalWeight = 0;
						int intensityCenter = temp->get(k,j,i);


						int mMax = i + kernelRadius;
						int nMax = j + kernelRadius;
						int lMax = k + kernelRadius;

						double weight;

						for (int m = i-kernelRadius; m < mMax; m++) 
						{
							for (int n = j-kernelRadius; n < nMax; n++) 
							{
								for (int l = k-kernelRadius; l < lMax; l++)
								{
									if (this->isInsideBoundaries(l, n, m)) 
									{
										int intensityKernelPos = temp->get(l,n,m);
										weight = getSpatialWeight(l,n,m,k,j,i) * similarity(intensityKernelPos,intensityCenter);
										totalWeight += weight;
										sum += (weight * intensityKernelPos);
									}
								}

							}
						}
						float newvalue=( float)floor(sum / totalWeight);
						if ( newvalue <= Maxvar)
						{
							res->put(k,j,i,newvalue);
						} 
						else
						{
							res->put(i,j,k,temp->get(i,j,k));
						}

					}//if..
				}//i..
				 
			}//k..

		}//j..
		//src = *temp;
		//return *temp;
		for (int i = 0; i < ret->size();i++ )
		{

			ret->putXYZ( i, res->getXYZ(i + ret->getXsize()*ret->getYsize()) );
		}

	} 
	//first and last slices 
	else if ( (iter == 0 && (iter+1)*ret->getZsize() !=  src->getZsize())|| 
		((iter+1)*ret->getZsize() >=  src->getZsize() && iter !=0 )) 
	{
		
		if ( iter ==0 )
		{//first
			temp = new Raw(ret->getXsize(),ret->getYsize(),ret->getZsize()+kernelRadius,src->getdata(),false);
			Raw *res=new Raw(*temp);
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
			//Raw *s =new Raw(*ret);
			int interval = globalProgressChanged/1000 == 0 ? 1:globalProgressChanged /1000 ;//first call diygieshi0 houmianshi 1
			int rs = 0 ;
			bool flag = false;
			for (int i = 0;i < temp->getZsize();i++)
			{
				for (int j = 0; j < temp->getYsize();j++)
				{
					for (int k=0; k < temp->getXsize(); k++)
					{

						rs ++;
						if ( rs == interval && ProgressChanged != NULL )
						{
							progressStep += interval;
							rs = 0;
							ProgressChanged (1, 100,(int) (long long)( progressStep)*100/(globalProgressChanged ),flag);
						}
						if(i>0 && j>0 && k>0 && i<ret->getZsize() && j< ret->getYsize() && k < ret->getXsize())
						{
							double sum = 0;
							double totalWeight = 0;
							int intensityCenter = temp->get(k,j,i);


							int mMax = i + kernelRadius;
							int nMax = j + kernelRadius;
							int lMax = k + kernelRadius;

							double weight;

							for (int m = i-kernelRadius; m < mMax; m++) 
							{
								for (int n = j-kernelRadius; n < nMax; n++) 
								{
									for (int l = k-kernelRadius; l < lMax; l++)
									{
										if (this->isInsideBoundaries(l, n, m)) 
										{
											int intensityKernelPos = temp->get(l,n,m);
											weight = getSpatialWeight(l,n,m,k,j,i) * similarity(intensityKernelPos,intensityCenter);
											totalWeight += weight;
											sum += (weight * intensityKernelPos);
										}
									}

								}
							}
							float newvalue=( float)floor(sum / totalWeight);
							if ( newvalue <= Maxvar)
							{
								res->put(k,j,i,newvalue);
							} 
							else
							{
								res->put(i,j,k,temp->get(i,j,k));
							}

						}//if..
					}//i..

				}//k..

			}//j..
			//src = *temp;
			//return *temp;
			for (int i =0; i < ret->size(); i++)
			{
				rs++;

				ret->putXYZ(i ,  res->getXYZ(i));
			}
		} 
		//last slices
		else //if(((iter+1)*ret->getZsize() ==  src->getZsize()&& iter !=0 ))
		{
			temp = new Raw(ret->getXsize(),ret->getYsize(),ret->getZsize()+kernelRadius,
				src->getdata()+iter*ret->getXsize()*ret->getYsize()*(src->getZsize()/(iter+1))-ret->getXsize()*ret->getYsize()*(int)kernelRadius,false);
			Raw *res=new Raw(*temp);
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
			//Raw *s =new Raw(*ret);
			int interval = globalProgressChanged/1000 == 0 ? 1:globalProgressChanged /1000 ;//first call diygieshi0 houmianshi 1
			int rs = 0 ;
			bool flag = false;
			for (int i = 0;i < temp->getZsize();i++)
			{
				for (int j = 0; j < temp->getYsize();j++)
				{
					for (int k=0; k < temp->getXsize(); k++)
					{

						rs ++;
						if ( rs == interval && ProgressChanged != NULL )
						{
							progressStep += interval;
							rs = 0;
							ProgressChanged (1, 100,(int) (long long)( progressStep)*100/(globalProgressChanged ),flag);
						}
						if(i>0 && j>0 && k>0 && i<ret->getZsize() && j< ret->getYsize() && k < ret->getXsize())
						{
							double sum = 0;
							double totalWeight = 0;
							int intensityCenter = temp->get(k,j,i);


							int mMax = i + kernelRadius;
							int nMax = j + kernelRadius;
							int lMax = k + kernelRadius;

							double weight;

							for (int m = i-kernelRadius; m < mMax; m++) 
							{
								for (int n = j-kernelRadius; n < nMax; n++) 
								{
									for (int l = k-kernelRadius; l < lMax; l++)
									{
										if (this->isInsideBoundaries(l, n, m)) 
										{
											int intensityKernelPos = temp->get(l,n,m);
											weight = getSpatialWeight(l,n,m,k,j,i) * similarity(intensityKernelPos,intensityCenter);
											totalWeight += weight;
											sum += (weight * intensityKernelPos);
										}
									}

								}
							}
							float newvalue=( float)floor(sum / totalWeight);
							if ( newvalue <= Maxvar)
							{
								res->put(k,j,i,newvalue);
							} 
							else
							{
								res->put(i,j,k,temp->get(i,j,k));
							}

						}//if..
					}//i..

				}//k..

			}//j..
			//src = *temp;
			//return *temp;
			for (int i =0; i < ret->size(); i++)
			{
				rs++;

				ret->putXYZ(i ,  res->getXYZ(i + ret->getXsize()*ret->getYsize()));//?????
			}
		}
		//else if((iter+1) * (src->getZsize()/(iter+1))  >=  src->getZsize())
		//{
		//	temp = new Raw(ret->getXsize(),ret->getYsize(),ret->getZsize()+1,src->getdata()+src->size()-ret->getXsize()*ret->getYsize()*(1+ret->getZsize()),true);
		//}
	}
	else 
	{
		temp = new Raw(ret->getXsize(),ret->getYsize(),ret->getZsize(),src->getdata(),false);
		Raw *res=new Raw(*temp);
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
		//Raw *s =new Raw(*ret);
		int interval = globalProgressChanged/1000 == 0 ? 1:globalProgressChanged /1000 ;//first call diygieshi0 houmianshi 1
		int rs = 0 ;
		bool flag = false;
		for (int i = 0;i < temp->getZsize();i++)
		{
			for (int j = 0; j < temp->getYsize();j++)
			{
				for (int k=0; k < temp->getXsize(); k++)
				{

					rs ++;
					if ( rs == interval && ProgressChanged != NULL )
					{
						progressStep += interval;
						rs = 0;
						ProgressChanged (1, 100,(int) (long long)( progressStep)*100/(globalProgressChanged ),flag);
					}
					if(i>0 && j>0 && k>0 && i<ret->getZsize() && j< ret->getYsize() && k < ret->getXsize())
					{
						double sum = 0;
						double totalWeight = 0;
						int intensityCenter = temp->get(k,j,i);


						int mMax = i + kernelRadius;
						int nMax = j + kernelRadius;
						int lMax = k + kernelRadius;

						double weight;

						for (int m = i-kernelRadius; m < mMax; m++) 
						{
							for (int n = j-kernelRadius; n < nMax; n++) 
							{
								for (int l = k-kernelRadius; l < lMax; l++)
								{
									if (this->isInsideBoundaries(l, n, m)) 
									{
										int intensityKernelPos = temp->get(l,n,m);
										weight = getSpatialWeight(l,n,m,k,j,i) * similarity(intensityKernelPos,intensityCenter);
										totalWeight += weight;
										sum += (weight * intensityKernelPos);
									}
								}

							}
						}
						float newvalue=( float)floor(sum / totalWeight);
						if ( newvalue <= Maxvar)
						{
							res->put(k,j,i,newvalue);
						} 
						else
						{
							res->put(i,j,k,temp->get(i,j,k));
						}

					}//if..
				}//i..

			}//k..

		}//j..
		//src = *temp;
		//return *temp;
		for ( int i = 0; i < ret->size(); i ++)
		{

			ret->putXYZ(i , res->getXYZ(i));
		}
	}


	delete temp;


}


double ThreeDim_Bilateral::similarity(int p, int s) {
	// this equals: Math.exp(-(( Math.abs(p-s)) /  2 * this->sigmaR * this->sigmaR));
	// but is precomputed to improve performance
	return this->gaussSimilarity[abs(p-s)];
}


double ThreeDim_Bilateral::gauss (double sigma, int x, int y,int z) {
	return exp(-((x * x + y * y + z*z) / (2 * sigma * sigma)));
}


bool ThreeDim_Bilateral::isInsideBoundaries(int m,int n, int l){
	if(m>-1 && n>-1 && l >-1 && m < temp->getXsize() && n < temp->getYsize() && l < temp->getZsize())
		return true;
	else 
		return false;
}

ThreeDim_Bilateral::~ThreeDim_Bilateral( void )
{

	int kernelSize = this->kernelRadius * 2 + 1;

	for(int i=0; i<kernelSize;i++)
	{

		for(int j=0; j<kernelSize;j++)
		{
			delete kernelD[i][j];
		}
		delete kernelD[i];

	}
	delete kernelD;
	kernelD =NULL;
	delete [] gaussSimilarity;

}
