#include "vol_math_trilateralfilter.h"
static float lgtt=log10(2.0f);
//=====================================================================================================
Trilateralfilter::Trilateralfilter(Raw* img,Raw * ret, int iter)
{
	this->src = img;
	this->ret = ret;
	this->iter = iter;
	//if ()
	//{
	//} 
	//else if ()
	//{
	//}
	//else
	//{
	//}

	//this->src=rawarray(img);
}
Trilateralfilter::Trilateralfilter(Raw* img)
{
	this->src = img;
	//this->ret=ret;
	//this->src=rawarray(img);
}
Trilateralfilter::~Trilateralfilter(void)
{
	img=NULL;
	//delete this->src;
	//delete rawarray;
}
//void * TrilateralFilter_single(void * para)
//{
//	Tri_para *p=(Tri_para *)para; 
//	Raw *ret = p->src;
//	PIXTYPE sigmaC= p->sigmaC;
//	Trilateralfilter *tf = new Trilateralfilter(ret);
//		tf->TrilateralFilter(sigmaC);
//		delete tf;
//	return NULL;
//}

//void Trilateralfilter::TrilateralFilter_Multi(PIXTYPE sigmaC,int threadcount)
//{
//	vector <Raw *>  src;
//	std::vector<pthread_t> threads;threads.resize(threadcount);
//	std::vector<int> pids;pids.resize(threadcount);
//	std::vector<int> res;res.resize(threadcount);
//	std::vector<Tri_para> parms;parms.resize(threadcount);
//	for (int i = 0; i <= this->src->getZsize()/threadcount; i++ )
//	{
//		
//		src.push_back(new Raw(this->src->getXsize(),this->src->getYsize(),this->src->getZsize()/threadcount));
//		memcpy(src[i]->getdata(),this->src->getdata()+i*this->src->getZsize(),sizeof(PIXTYPE)*src[i]->size());
//		int ret;
//		parms[i]=Tri_para(*src[i],sigmaC);
//		pthread_attr_t *attr;
//		ret=pthread_create(&threads[i],NULL,TrilateralFilter_single,&parms[i]);
//		cout <<i<<endl;
//		
//
//	}
//	for(int i=0;i<threadcount;i++)
//	{
//		pthread_join(threads[i], NULL);   
//	}
//
//} 

//void Trilateralfilter::TrilateralFilter(float sigmaC)
//	//=====================================================================================================
//{	
//	Raw* pSrcImg=new Raw(*src);
//	Raw destImg; 			
//	Raw fTheta; 			//stores Adaptive neighborhood size for each pixel
//	RawArray minGradientStack;	
//	RawArray maxGradientStack; 
//	Raw xGradient, yGradient,zGradient; 	//X and Y gradients of the input RawImage
//	Raw xSmoothGradient, ySmoothGradient, zSmoothGradient; 	//Bilaterally filtered X and Y gradients of the input RawImage
//	int levX, levY,levZ, levelMax, filterSize;  //level = log2(xsize) or log2(ysize)
//	float sigmaR, sigmaCTheta, sigmaRTheta,beta;
//	float  R;
//	//domain variance for the two filters: sigmaC, sigmaCTheta
//	//range variance of the two filters: sigmaR, sigmaRTheta
//	//R -- threshold to compute adaptive region 
//
//	//Default internal Parameters
//	sigmaCTheta = sigmaC; //Variance of the Domain Filter, the only user set parameter
//	beta = (float)0.15; //Always set between 0.1 and 0.2
//	filterSize = (int) sigmaC; 
//
//	//Compute the RawImage stack height
//
//
//	levX=(int) (log10(float (pSrcImg->getXsize()))/lgtt);
//	levY=(int) (log10(float (pSrcImg->getYsize()))/lgtt);
//	levZ=(int) (log10(float (pSrcImg->getZsize()))/lgtt);
//	if(levX < levY)
//		levelMax = levX+1;
//	else
//		levelMax = levY+1;
//
//	//Allocate memory for the Min-Max Image Stack
//	minGradientStack.sizer(pSrcImg->getXsize(),pSrcImg->getYsize(),pSrcImg->getZsize(),levelMax);
//	maxGradientStack.sizer(pSrcImg->getXsize(),pSrcImg->getYsize(),pSrcImg->getZsize(),levelMax);
//
//	//Allocate memory for the gradient vectors and output RawImage
//	xGradient.sizer(pSrcImg); 
//	yGradient.sizer(pSrcImg);
//	zGradient.sizer(pSrcImg);
//	xSmoothGradient.sizer(pSrcImg);
//	ySmoothGradient.sizer(pSrcImg); 
//	zSmoothGradient.sizer(pSrcImg); 
//	fTheta.sizer(pSrcImg);
//	destImg.sizer(pSrcImg);
//
//	/**
//	Compute Gradients using Forward Difference (Step 1)
//	X gradient is stored in xGradient
//	Y gradient is stored in yGradient
//	**/
//	ComputeGradients(&xGradient,&yGradient,&zGradient);
//
//	/**
//	Builds the Min-Max Image Stack consisting of Image Gradients (Step 2).
//
//	**/
//	sigmaR = buildMinMaxImageStack(&xGradient,&yGradient,&zGradient,&minGradientStack,&maxGradientStack,levelMax,beta);
//
//	//Set the remaining internal parameters required for trilateral filter
//	R = sigmaR;
//	sigmaRTheta =sigmaR;//from  sigmaRTheta= R = sigmaR
//	/**
//	Bilaterally filter the X and Y gradients of the input RawImage (Step 3, equation 4 and 5)
//	to produce xSmoothGradient and ySmoothGradient.
//	**/
//	BilateralGradientFilter(&xGradient, &yGradient,&zGradient, &xSmoothGradient, &ySmoothGradient, &zSmoothGradient,sigmaC, sigmaR, filterSize);
//
//	/**
//	Find the adaptive neighborhood fTheta for each pixel location (Step 4). fTheta size is
//	given by stack level. The min-max gradient stacks and range threshold "R" are used for this calculation
//	
//	**/
//	findAdaptiveRegion(&minGradientStack, &maxGradientStack, R, levelMax);
//
//	/**
//	Performs bilateral filter on the detail signal (Step 5).
//
//	**/
//	DetailBilateralFilter(pSrcImg, &xSmoothGradient, &ySmoothGradient,&zSmoothGradient,&fTheta, sigmaCTheta, sigmaRTheta);
//
//	//Copying the result to the output RawImage
//	//wipecopy(&destImg);
//
//	//FILE *p=fopen("F:\\3D.src","ab+");
//	//fwrite(destImg.getdata(),sizeof(PIXTYPE),281*481*100,p);
//	//fclose(p);
//	//fflush(stdout);
//
//	//printf("write is ok");
//	/*delete [] xGradient.y;
//	delete [] yGradient.y;
//	delete [] xSmoothGradient.y;
//	delete [] ySmoothGradient.y;
//	delete [] minGradientStack.z;
//	delete [] maxGradientStack.z;
//	delete [] fTheta.y;
//	delete [] destImg.y;*/
//
//
//	/*
//	fwrite(pointer,sizeof(T),length,nfile);
//	fclose(nfile);*/
//
//	}
void Trilateralfilter::TrilateralFilter(Raw & src,Raw & ret,float sigmaC)
	//=====================================================================================================
{	
	this->src=&src;
	this->ret=&ret;
	Raw *pSrcImg;
	if (iter !=0 && (iter+1)*ret.getZsize() < src.getZsize())
	{
		temp = new Raw(src.getXsize(),src.getYsize(),ret.getZsize() + 2,src.getdata()+ ret.getXsize()*ret.getYsize()*ret.getZsize()*(iter-1),true);
	} 
	else if ( (iter == 0 && (iter+1)*ret.getZsize() !=  src.getZsize())|| ((iter+1)*ret.getZsize() ==  src.getZsize()&& iter !=0 ))
	{
		temp = new Raw(ret.getXsize(),ret.getYsize(),ret.getZsize()+1,src.getdata()+iter*ret.getXsize()*ret.getYsize()*(ret.getZsize()-1),true);
	}
	else 
	{
		temp = new Raw(ret.getXsize(),ret.getYsize(),ret.getZsize(),src.getdata(),true);
	}
	pSrcImg = temp;
	Raw destImg;//= new Raw(ret,true); 			
	Raw fTheta; 			//stores Adaptive neighborhood size for each pixel
	RawArray minGradientStack;	
	RawArray maxGradientStack; 
	Raw xGradient, yGradient,zGradient; 	//X and Y gradients of the input RawImage
	Raw xSmoothGradient, ySmoothGradient, zSmoothGradient; 	//Bilaterally filtered X and Y gradients of the input RawImage
	int levX, levY,levZ, levelMax, filterSize;  //level = log2(xsize) or log2(ysize)
	float sigmaR, sigmaCTheta, sigmaRTheta,beta;
	float  R;
	//domain variance for the two filters: sigmaC, sigmaCTheta
	//range variance of the two filters: sigmaR, sigmaRTheta
	//R -- threshold to compute adaptive region 

	//Default internal Parameters
	sigmaCTheta = sigmaC; //Variance of the Domain Filter, the only user set parameter
	beta = (float)0.15; //Always set between 0.1 and 0.2
	filterSize = (int) sigmaC; 

	//Compute the RawImage stack height


	levX=(int) (log10(float (pSrcImg->getXsize()))/lgtt);
	levY=(int) (log10(float (pSrcImg->getYsize()))/lgtt);
	levZ=(int) (log10(float (pSrcImg->getZsize()))/lgtt);
	if(levX < levY)
		levelMax = levX+1;
	else
		levelMax = levY+1;
	if (levelMax > levZ +1 )
	{
		levelMax = levZ + 1;
	}
	//Allocate memory for the Min-Max Image Stack
	minGradientStack.sizer(pSrcImg->getXsize(),pSrcImg->getYsize(),pSrcImg->getZsize(),levelMax);
	maxGradientStack.sizer(pSrcImg->getXsize(),pSrcImg->getYsize(),pSrcImg->getZsize(),levelMax);

	//Allocate memory for the gradient vectors and output RawImage
	xGradient.sizer(pSrcImg); 
	yGradient.sizer(pSrcImg);
	zGradient.sizer(pSrcImg);
	xSmoothGradient.sizer(pSrcImg);
	ySmoothGradient.sizer(pSrcImg); 
	zSmoothGradient.sizer(pSrcImg); 
	fTheta.sizer(pSrcImg);
	destImg.sizer(pSrcImg);

	/**
	Compute Gradients using Forward Difference (Step 1)
	X gradient is stored in xGradient
	Y gradient is stored in yGradient
	**/
	ComputeGradientsSipl(&xGradient,&yGradient,&zGradient);

	/**
	Builds the Min-Max Image Stack consisting of Image Gradients (Step 2).//compute the six neighbors M-m
	**/
	sigmaR = buildMinMaxImageStack(&xGradient,&yGradient,&zGradient,&minGradientStack,&maxGradientStack,levelMax,beta);
	
	//Set the remaining internal parameters required for trilateral filter
	R = sigmaR;
	sigmaRTheta =sigmaR;//from  sigmaRTheta= R = sigmaR
	/**
	Bilaterally filter the X,Y and Z gradients of the input RawImage (Step 3, equation 4 and 5)
	to produce xSmoothGradient and ySmoothGradient.
	**/
	//Trilateralfilter ps = new Trilateralfilter(temp);
	BilateralGradientFilter(&xGradient, &yGradient,&zGradient, &xSmoothGradient, &ySmoothGradient, &zSmoothGradient,sigmaC, sigmaR, filterSize);

	/**
	Find the adaptive neighborhood fTheta for each pixel location (Step 4). fTheta size is
	given by stack level. The min-max gradient stacks and range threshold "R" are used for this calculation
	
	**/
	//Trilateralfilter *f = new Trilateralfilter(temp);
	findAdaptiveRegion(&minGradientStack, &maxGradientStack, &fTheta, R, levelMax );
	//fTheta=new Raw(*(f->src),false);
	
	/**
	Performs bilateral filter on the detail signal (Step 5).

	**/
	//Trilateralfilter *d = new Trilateralfilter(temp);
	DetailBilateralFilter(pSrcImg, &xSmoothGradient, &ySmoothGradient,&zSmoothGradient,&fTheta,&destImg, sigmaCTheta, sigmaRTheta);
	//destImg =new Raw (*(d->src),false);
	//Copying the result to the output RawImage
	//wipecopy(&destImg);

	//FILE *p=fopen("F:\\3D.src","ab+");
	//fwrite(destImg.getdata(),sizeof(PIXTYPE),281*481*100,p);
	//fclose(p);
	//fflush(stdout);

	//printf("write is ok");
	/*delete [] xGradient.y;
	delete [] yGradient.y;
	delete [] xSmoothGradient.y;
	delete [] ySmoothGradient.y;
	delete [] minGradientStack.z;
	delete [] maxGradientStack.z;
	delete [] fTheta.y;
	delete [] destImg.y;*/



	/*
	fwrite(pointer,sizeof(T),length,nfile);
	fclose(nfile);*/
	if (iter !=0 && (iter+1)*ret.getZsize() < src.getZsize())
	{
		for ( int i = 0; i < ret.size(); i++)
		{
			ret.putXYZ( i, destImg.getXYZ(i + ret.getXsize()*ret.getYsize()) );

		}
	} 
	else if ( (iter == 0 && (iter+1)*ret.getZsize() !=  src.getZsize())|| ((iter+1)*ret.getZsize() ==  src.getZsize()&& iter !=0 ) )
	{
		for ( int i = 0; i < ret.size(); i++)
		{
			ret.putXYZ( i, destImg.getXYZ(i + ret.getXsize()*ret.getYsize()) );

		}
	}
	else 
	{
		for ( int i = 0; i < ret.size(); i++)
		{
			ret.putXYZ( i, destImg.getXYZ(i) );

		}
		
	}

}
void Trilateralfilter::ComputeGradientsSipl(Raw* pX, Raw *pY, Raw* pZ)
{
	//Raw *temp;
	//if (iter > 0 && ret->getZsize() *(iter+1) < src->getZsize())
	//{
	//	temp =new Raw (src->getXsize(), src->getYsize(), ret->getZsize() + 2, 
	//		src->getdata()+ ret->getXsize()*ret->getYsize()*ret->getZsize()*(iter-1));
	//}
	//else if ( (iter == 0 && (iter+1)*ret->getZsize() !=  src->getZsize())|| ((iter+1)*ret->getZsize() ==  src->getZsize()&& iter !=0 )  )
	//{
	//	temp =new Raw (src->getXsize(), src->getYsize(), ret->getZsize() + 1, 
	//		src->getdata()+ ret->getXsize()*ret->getYsize()*ret->getZsize()*(iter-1));

	//} 
	//else
	//{
	//	temp =new Raw (src->getXsize(), src->getYsize(), ret->getZsize(), 
	//		src->getdata());
	//}
	int i, j, k, imax, jmax, kmax, jN, iE, kU;
	PIXTYPE Cval, Eval, Nval,Uval, gE, gN, gU;

	imax =  temp->getXsize();		// get RawImage size,
	jmax =  temp->getYsize();
	kmax =  temp->getZsize();
	for(k=0; k<kmax; k++)
	{
		kU = k+1;
		if(kU > kmax -1) 
			kU = kmax-1;
	for(j = 0; j < jmax; j++)			// for each scanline,
	{
		jN = j+1;					// addr. of North neighbor;
		if( jN > jmax -1) 
			jN = jmax -1 ;
		for( i=0; i < imax; i++)			// and each pixel on a scanline,
		{
			iE = i+1;
			if( iE > imax -1)
				iE = imax -1;	// addr. of East neighbor
			Cval =  temp->get( i, j, k);
			Eval =  temp->get( iE, j, k);
			Nval =  temp->get( i, jN, k);
			Uval =  temp->get( i, j, kU);
			gE = (PIXTYPE) (Eval-Cval); //gradient computation with forward difference
			gN = (PIXTYPE) (Nval-Cval);
			gU = (PIXTYPE) (Uval-Cval);
			//	if(gE!=0)printf("gE");
			pX->put(i,j,k,gE);		//copy the gradient values to pX and pY
			pY->put(i,j,k,gN);
			pZ->put(i,j,k,gU);
		}// for(i...
	}// for(j...
	}//for(k...

}
void Trilateralfilter::ComputeGradients(Raw* pX, Raw *pY, Raw* pZ)
{
	//Raw *temp;
	//if (iter > 0 && ret->getZsize() *(iter+1) < src->getZsize())
	//{
	//	temp =new Raw (src->getXsize(), src->getYsize(), ret->getZsize() + 1, 
	//		src->getdata()+iter*ret->size(),true);
	//}
	int i, j, k, imax, jmax, kmax, jN, iE, kU;
	PIXTYPE Cval, Eval, Nval,Uval, gE, gN, gU;

	imax =  pX->getXsize();		// get RawImage size,
	jmax =  pX->getYsize();
	kmax =  pX->getZsize();
	for(k=0; k<kmax; k++)
	{
		kU = k+1;
		if(kU > kmax- 1) 
			kU = kmax-1;
		for(j = 0; j < jmax; j++)			// for each scanline,
		{
			jN = j+1;					// addr. of North neighbor;
			if( jN > jmax -1 ) 
				jN = jmax-1;
			for( i=0; i < imax; i++)			// and each pixel on a scanline,
			{
				iE = i+1;
				if( iE > imax -1)
					iE = imax -1;	// addr. of East neighbor
				Cval =  src->get( i, j, k);
				Eval =  src->get( iE, j, k);
				Nval =  src->get( i, jN, k);
				Uval =  src->get( i, j, kU);
				gE = (PIXTYPE) (Eval-Cval); //gradient computation with forward difference
				gN = (PIXTYPE) (Nval-Cval);
				gU = (PIXTYPE) (Uval-Cval);
				//	if(gE!=0)printf("gE");
				pX->put(i,j,k,gE);		//copy the gradient values to pX and pY
				pY->put(i,j,k,gN);
				pZ->put(i,j,k,gU);
			}// for(i...
		}// for(j...
	}//for(k...

}
PIXTYPE Trilateralfilter::buildMinMaxImageStack(Raw* pX, Raw* pY,Raw *pZ, RawArray* pMinStack,
RawArray* pMaxStack , int levelMax, float beta)
{
	int imax, jmax,kmax, i, j,k, lev, m, n,l;
	PIXTYPE min, max, minGrad = 1000000.0, maxGrad = -1000000.0, tmp, tmpMin, tmpMax, rangeVariance;

	imax= temp->getXsize();	//get RawImage size
	jmax= temp->getYsize();
	kmax= temp->getZsize();


	for(lev=0; lev < levelMax; lev++) 
	{
		for(k=0;k<kmax;k++)
		{
			for(j=0; j < jmax; j++) 
			{
				for(i=0; i < imax; i++) 
				{
					if( lev == 0) { //stack level 0 is the magnitude of the gradients of original RawImage
						tmp = (float) sqrt((float) pX->get(i,j,k)*pX->get(i,j,k) + pY->get(i,j,k)*pY->get(i,j,k) +pZ->get(i,j,k)*pZ->get(i,j,k));
						if(maxGrad < tmp)
							maxGrad = tmp;
						if(minGrad > tmp)
							minGrad = tmp;
						/*use to compute the variance*/
						max = min = tmp;
						pMinStack->put(i,j,k,0,min);
						pMaxStack->put(i,j,k,0,max);
					}
					if( lev > 0) { //Gradients at each level of the RawImage stack is computed from the level below 
						min = pMinStack->get(i,j,k,lev-1);
						max = pMaxStack->get(i,j,k,lev-1);

						for(m=-1; m <= 1; m++) 
						{
							for(n=-1; n <= 1; n++) 
							{	
								for(l=-1;l<=1;l++)
								{
									//Computes the maximum and minimum gradient value in the neighborhood
									if((i+m) >=0 && (i+m) < imax && (j+n) >=0 && (j+n) < jmax &&(k+l)>=0&&(k+l)<kmax)
									{
										tmpMin = (PIXTYPE) pMinStack->get(i+m,j+n,k+l,lev-1);
										tmpMax = (PIXTYPE) pMaxStack->get(i+m,j+n,k+l,lev-1);
										if(min > tmpMin)
											min = tmpMin;
										if(max < tmpMax)
											max = tmpMax;
									}
								}
							}
						}

						pMinStack->put(i,j,k,lev,min);
						pMaxStack->put(i,j,k,lev,max);
					} 

				}	// for(i...
			}	// for(j...
		} //for (k...
	}	// for(lev...

	//range variance is computed as a ratio of difference between max and min gradient
	rangeVariance = beta*(maxGrad - minGrad); 
	return rangeVariance;

}


PIXTYPE Trilateralfilter::buildMinMaxImageStackSipl(Raw* pX, Raw* pY,Raw *pZ, RawArray* pMinStack,
	RawArray* pMaxStack , int levelMax, float beta)
{

	int imax, jmax,kmax, i, j,k, lev, m, n,l;
	PIXTYPE min, max, minGrad = 1000000.0, maxGrad = -1000000.0, tmp, tmpMin, tmpMax, rangeVariance;

	imax= src->getXsize();	//get RawImage size
	jmax= src->getYsize();
	kmax= src->getZsize();


	for(lev=0; lev < levelMax; lev++) 
	{
		for(k=0;k<kmax;k++)
		{
			for(j=0; j < jmax; j++) 
			{
				for(i=0; i < imax; i++) 
				{
					if( lev == 0) 
					{ //stack level 0 is the magnitude of the gradients of original RawImage
						tmp = (float) sqrt((float) pX->get(i,j,k)*pX->get(i,j,k) + 
							pY->get(i,j,k)*pY->get(i,j,k) +pZ->get(i,j,k)*pZ->get(i,j,k));
						if( maxGrad < tmp)
							maxGrad = tmp;
						if( minGrad > tmp)
							minGrad = tmp;
						max = min = tmp;
						pMinStack->put(i, j, k, 0, min);
						pMaxStack->put(i, j, k, 0, max);
					}
					if( lev > 0)
					{ //Gradients at each level of the RawImage stack is computed from the level below 
						min = pMinStack->get(i, j, k, lev-1);
						max = pMaxStack->get(i, j, k, lev-1);

						for(m=-1; m <= 1; m++) 
						{
							for(n=-1; n <= 1; n++) 
							{	
								for(l=-1; l<=1; l++)
								{
									//Computes the maximum and minimum gradient value in the neighborhood
									if((i+m) >= 0 && (i+m) < imax && (j+n) >= 0 && (j+n) < jmax && (k+l) >= 0 && (k+l) < kmax)
									{
										tmpMin = (PIXTYPE) pMinStack->get(i+m, j+n, k+l, lev-1);
										tmpMax = (PIXTYPE) pMaxStack->get(i+m, j+n, k+l, lev-1);
										if(min > tmpMin)
											min = tmpMin;
										if(max < tmpMax)
											max = tmpMax;
									}
								}
							}
						}

						pMinStack->put(i,j,k,lev,min);
						pMaxStack->put(i,j,k,lev,max);
					} 

				}	// for(i...
			}	// for(j...
		} //for (k...
	}	// for(lev...

	//range variance is computed as a ratio of difference between max and min gradient
	rangeVariance = beta*(maxGrad - minGrad); 
	return rangeVariance;

}
//pscImg
void Trilateralfilter::BilateralGradientFilter(Raw* pX, Raw* pY,Raw*pZ, Raw* pSmoothX, Raw* pSmoothY,Raw* pSmoothZ, 
	float sigmaC, float sigmaR, int filterSize)
{
	int i,j,k,m,n,l,imax,jmax,kmax,halfSize;
	float tmpX, tmpY, tmpZ, posDiff, gradDiff, domainWeight, rangeWeight, g1, g2;
	float  normFactor;
	imax =	temp->getXsize(); //get RawImage size
	jmax =  temp->getYsize(); 
	kmax =  temp->getZsize();
	halfSize = (int) (filterSize-1)/2; //size of the filter kernel


	for(i=0; i<imax; i++) //X scanline origion i=0
	{		
		for(j=0; j<jmax; j++) //Y scaline origion i=0
		{ 
			for(k=0;k<kmax;k++)//Z scanline origion k=0
			{
				normFactor=0.0;
				tmpX=0.0;
				tmpY=0.0;
				tmpZ=0.0;
				for(m = -halfSize; m<=halfSize; m++)
				{
					for (n = -halfSize; n<=halfSize; n++)
					{

						for(l = - halfSize; l<=halfSize; l++ )
						{
							posDiff=(PIXTYPE) (m*m+n*n+l*l); 
							//Compute the weight for the domain filter (domainWeight). The domain filter
							//is a Gaussian low pass filter
							domainWeight = (float) pow(M_EXP, (double) (-posDiff/(2*sigmaC*sigmaC)));
							if( (i+m) >= 0 && (i+m) < imax && (j+n) >=0 && (j+n) < jmax && (k+l) >= 0 && (k+l) < kmax ) {
								g1 = (float) (pow( float (pX->get(i+m,j+n,k+l)),2.0f) + pow(float(pY->get(i+m,j+n,k+l)),2.0f)+pow(float(pZ->get(i+m,j+n,k+l)),2.0f) );
								g2 = (float) (pow( float(pX->get(i,j,k)),2.0f) + pow(float(pY->get(i,j,k)),2.0f)+pow(float(pZ->get(i,j,k)),2.0f) );
								//g3 = (PIXTYPE) (pow( float(pX->get(i,j)),2.0f) + pow(float(pY->get(i,j)),2.0f) );
								//Compute the gradient difference between a pixel and its neighborhood pixel 
								gradDiff = (float) (sqrt(double(g1)) - sqrt(double(g2)));
								//if (gradDiff !=0)
								//{
								//	cout << gradDiff << endl;
								//}
								//Compute the weight for the range filter (rangeWeight). The range filter
								//is a Gaussian filter defined by the difference in gradient magnitude.
								if(sigmaR==0)
								{
									sigmaR=0.1;
								}
								rangeWeight = (float) pow(M_EXP, (double) (-(gradDiff*gradDiff)/(2*sigmaR*sigmaR)));	
								tmpX += pX->get(i+m,j+n,k+l)*domainWeight*rangeWeight;
								tmpY += pY->get(i+m,j+n,k+l)*domainWeight*rangeWeight;
								tmpZ += pZ->get(i+m,j+n,k+l)*domainWeight*rangeWeight;
								//Bilateral filter normalized by normFactor (eq. 5, Section 3.1) 
								normFactor += domainWeight*rangeWeight;
								//printf("nornfactor====>>%d",normFactor);
							}
						}
					}	
					if(normFactor==0)normFactor=0.1;
					tmpX = tmpX/normFactor;
					/*if(tmpX||tmpY)
					printf("not 0"); for test */
					tmpY = tmpY/normFactor;
					tmpZ = tmpZ/normFactor;
					pSmoothX->put(i,j,k,tmpX);	//copy smooth gradients to pSmoothX and pSmoothY

					/*if(pSmoothX->get(i,j)!=tmpX)
					printf("pSmoothX->get(i,j)!=tmpX");*/
					pSmoothY->put(i,j,k,tmpY);
					pSmoothZ->put(i,j,k,tmpZ);
				}  //for(k...
			}	// for(j...
		}	// for(i...
	}

}
//psrcImage
void Trilateralfilter::BilateralGradientFilterSipl(Raw* pX, Raw* pY,Raw*pZ, Raw* pSmoothX, Raw* pSmoothY,Raw* pSmoothZ, 
	float sigmaC, float sigmaR, int filterSize)
{
	Raw *temp;
	if (iter > 0 && ret->getZsize()*(iter+1) < src->getZsize())
	{
		temp = new Raw( ret->getXsize(), ret->getYsize(), ret->getZsize()+ 2 ,
			src->getdata()+	iter * src->getXsize() * src->getYsize()*(ret->getZsize()-1));
	} 
	else
	{
	}
	int i,j,k,m,n,l,imax,jmax,kmax,halfSize;
	float tmpX, tmpY, tmpZ, posDiff, gradDiff, domainWeight, rangeWeight, g1, g2;
	float  normFactor;
	imax =	temp->getXsize(); //get RawImage size
	jmax =  temp->getYsize(); 
	kmax =  temp->getZsize();
	halfSize = (int) (filterSize-1)/2; //size of the filter kernel


	for(i=0; i<imax; i++) //X scanline origion i=0
	{		
		for(j=0; j<jmax; j++) //Y scaline origion i=0
		{ 
			for(k=0;k<kmax;k++)//Z scanline origion k=0
			{
				normFactor=0.0;
				tmpX=0.0;
				tmpY=0.0;
				tmpZ=0.0;
				for(m = -halfSize; m<=halfSize; m++)
				{
					for (n = -halfSize; n<=halfSize; n++)
					{

						for(l = - halfSize; l<=halfSize; l++ )
						{
							posDiff=(PIXTYPE) (m*m+n*n+l*l); 
							//Compute the weight for the domain filter (domainWeight). The domain filter
							//is a Gaussian low pass filter
							domainWeight = (float) pow(M_EXP, (double) (-posDiff/(2*sigmaC*sigmaC)));
							if( (i+m) >= 0 && (i+m) < imax && (j+n) >=0 && (j+n) < jmax && (k+l) >= 0 && (k+l) < kmax ) {
								g1 = (float) (pow( float (pX->get(i+m,j+n,k+l)),2.0f) + pow(float(pY->get(i+m,j+n,k+l)),2.0f)+pow(float(pZ->get(i+m,j+n,k+l)),2.0f) );
								g2 = (float) (pow( float(pX->get(i,j,k)),2.0f) + pow(float(pY->get(i,j,k)),2.0f)+pow(float(pZ->get(i,j,k)),2.0f) );
								//g3 = (PIXTYPE) (pow( float(pX->get(i,j)),2.0f) + pow(float(pY->get(i,j)),2.0f) );
								//Compute the gradient difference between a pixel and its neighborhood pixel 
								gradDiff = (float) (sqrt(double(g1)) - sqrt(double(g2)));
								//if (gradDiff !=0)
								//{
								//	cout << gradDiff << endl;
								//}
								//Compute the weight for the range filter (rangeWeight). The range filter
								//is a Gaussian filter defined by the difference in gradient magnitude.
								if(sigmaR==0)
								{
									sigmaR=0.1;
								}
								rangeWeight = (float) pow(M_EXP, (double) (-(gradDiff*gradDiff)/(2*sigmaR*sigmaR)));	
								tmpX += pX->get(i+m,j+n,k+l)*domainWeight*rangeWeight;
								tmpY += pY->get(i+m,j+n,k+l)*domainWeight*rangeWeight;
								tmpZ += pZ->get(i+m,j+n,k+l)*domainWeight*rangeWeight;
								//Bilateral filter normalized by normFactor (eq. 5, Section 3.1) 
								normFactor += domainWeight*rangeWeight;
								//printf("nornfactor====>>%d",normFactor);
							}
						}
					}	
					if(normFactor==0)normFactor=0.1;
					tmpX = tmpX/normFactor;
					/*if(tmpX||tmpY)
					printf("not 0"); for test */
					tmpY = tmpY/normFactor;
					tmpZ = tmpZ/normFactor;
					pSmoothX->put(i,j,k,tmpX);	//copy smooth gradients to pSmoothX and pSmoothY

					/*if(pSmoothX->get(i,j)!=tmpX)
					printf("pSmoothX->get(i,j)!=tmpX");*/
					pSmoothY->put(i,j,k,tmpY);
					pSmoothZ->put(i,j,k,tmpZ);
				}  //for(k...
			}	// for(j...
		}	// for(i...
	}

}
//ftheta 
void Trilateralfilter::findAdaptiveRegion(RawArray* pMinStack, RawArray* pMaxStack, Raw *ftheta,float R, int levelMax)
{
	int imax, jmax,kmax,i,j,k,lev;
	imax = temp->getXsize();	//get RawImage size
	jmax = temp->getYsize();
	kmax = temp->getZsize();
	for(k = 0; k<kmax; k++)
	{
	for(j = 0; j < jmax; j++) {
		for(i = 0; i < imax;i++) {
			for(lev=0; lev < levelMax; lev++) {
				//Compute the adaptive neighboirhood based on the similarity of
				//the neighborhood gradients
				if ( pMaxStack->get(i,j,k,lev) > (pMaxStack->get(i,j,k,0) + R) ||
					pMinStack->get(i,j,k,lev) < (pMaxStack->get(i,j,k,0) - R) )
					break;
			}
			 ftheta->put(i,j,k,(PIXTYPE) (lev-1) );
		}	// for(i...
	}	// for(j...
	}//for(k...
}
void Trilateralfilter::findAdaptiveRegionSipl(RawArray* pMinStack, RawArray* pMaxStack, float R, int levelMax)
{
	int imax, jmax,kmax,i,j,k,lev;
	imax= src->getXsize();	//get RawImage size
	jmax= src->getYsize();
	kmax= src->getZsize();
	for(k=0; k<kmax; k++)
	{
		for(j=0; j<jmax; j++) {
			for(i=0; i<imax;i++) {
				for(lev=0; lev < levelMax; lev++) {
					//Compute the adaptive neighboirhood based on the similarity of
					//the neighborhood gradients
					if ( pMaxStack->get(i,j,k,lev) > (pMaxStack->get(i,j,k,0) + R) ||
						pMinStack->get(i,j,k,lev) < (pMaxStack->get(i,j,k,0) - R) )
						break;
				}
				temp->put(i,j,k,(PIXTYPE) (lev-1) );
			}	// for(i...
		}	// for(j...
	}//for(k...
}
//destImage...
void Trilateralfilter::DetailBilateralFilter(Raw* srcImg, Raw* pSmoothX, Raw* pSmoothY, Raw* pSmoothZ,
	Raw* fTheta, Raw *dest,float sigmaCTheta, float sigmaRTheta)
{

	int i, j, k, m, n, l, imax, jmax, kmax, halfSize;
	int countvar=0;
	float tmp, diff, detail;
	float  domainWeight, rangeWeight, normFactor;
	float coeffA, coeffB, coeffC,coeffD; //Plane Equation is z = coeffA.x + coeffB.y + coeffC.z+coeffD
	//coeffA = dI/dx, coeffB = dI/dy, coeffC = I at center pixel of the filter kernel

	imax= temp->getXsize();	//get RawImage size
	jmax= temp->getYsize();	
	kmax= temp->getZsize();
	for(i = 0; i < imax; i++)
	{ //X scankline...
		for(j = 0; j < jmax; j++ ) 
		{	//Y scanline...
			for(k = 0;k < kmax; k++ ) 
			{ //zscanline..
				normFactor=0.0;
				tmp=0.0;
				//filter window width is calculated from fTheta
				//halfsize is half of the filter window width
				halfSize=(int) fTheta->get(i,j,k); 
				//halfSize = (int) (pow(2.0f,halfSize)/2);
				//halfSize=halfSize*halfSize;
				//halfSize=1.5;
				if(halfSize>2){halfSize=2;}//halfsize=5
				//Coefficients defining the centerplane 
				//from the smoothed RawImage gradients
				coeffA=pSmoothX->get(i,j,k); 
				//assert(coeffA==pSmoothX->get(i,j,k));
				coeffB=pSmoothY->get(i,j,k);
				coeffC=pSmoothZ->get(i,j,k);
				coeffD=srcImg->get(i,j,k);

				for(m = -halfSize; m<= halfSize; m++) {
					for (n = -halfSize; n <= halfSize; n++) {
						for(l= -halfSize; l<=halfSize; l++)
						{


							diff = (float) (m*m+n*n+l*l);
							//Compute the weight for the domain filter (domainWeight). The domain filter
							//is a Gaussian lowpass filter
							domainWeight = (double) pow(M_EXP, (double) (-diff/(2*sigmaCTheta*sigmaCTheta)));		
							if( (i+m) >= 0 && (i+m) < imax && (j+n) >= 0 && (j+n) < jmax && (k+l) >= 0 && (k+l)  < kmax)
							{
								//Compute the detail signal (detail) based on the difference between a 
								//neighborhood pixel and the centerplane passing through the center-pixel 

								detail = (float) (srcImg->get(i+m,j+n,k+l) - coeffA*m - coeffB*n - coeffC*l-coeffD);	
								if(detail!=0)						
									countvar++;
								//Compute the weight for the range filter (rangeWeight). The range filter
								//is a Gaussian filter defined by the detail signal.
								if( sigmaRTheta == 0 )
								{
									sigmaRTheta=0.1;
								}// 1===>0.1
								rangeWeight = (double) pow(M_EXP, (double) (-(detail*detail)/(2*sigmaRTheta*sigmaRTheta)));	
								if(rangeWeight==0) 
									rangeWeight=0.1;


								if(domainWeight == 0) 

									domainWeight = 0.1;
								
								tmp += detail*domainWeight*rangeWeight;

								//Detail Bilateral filter normalized by normFactor 
								normFactor += domainWeight*rangeWeight;
							}//if
						}//l
					}//n
				}//m
				if(normFactor == 0)
					normFactor = 0.1;
				tmp = tmp / normFactor;
				if( tmp != 0)
				{
					//printf("tmp=%d\n",tmp);
					tmp += coeffD;
					if( tmp == coeffD)
						printf("tmp==get(i,j)");


				}
				else tmp = coeffD;//not changed
				//if(tmp!=0){if(tmp==coeffC)printf("tmp==get(i,j)");}
				dest->put(i,j,k,tmp);//copy to the output

			}	
			// for(j...
		}	// for(i...
	} //for(k..
	//	printf("i=%d,j=%d\n",i,j);
	
	printf("countvar=%d\n",countvar);
}
	void Trilateralfilter::DetailBilateralFilterSipl(Raw* srcImg, Raw* pSmoothX, Raw* pSmoothY, Raw* pSmoothZ,
		Raw* fTheta, float sigmaCTheta, float sigmaRTheta)
	{

		int i,j,k,m,n,l,imax,jmax,kmax,halfSize;
		int countvar=0;
		float tmp, diff, detail;
		float  domainWeight, rangeWeight, normFactor;
		float coeffA, coeffB, coeffC,coeffD; //Plane Equation is z = coeffA.x + coeffB.y + coeffC.z+coeffD
		//coeffA = dI/dx, coeffB = dI/dy, coeffC = I at center pixel of the filter kernel

		imax= src->getXsize();	//get RawImage size
		jmax= src->getYsize();	
		kmax= src->getZsize();
		for(i = 0; i < imax; i++)
		{ //X scankline...
			for(j = 0; j < jmax; j++ ) 
			{	//Y scanline...
				for(k = 0;k < kmax; k++ ) 
				{ //zscanline..
					normFactor=0.0;
					tmp=0.0;
					//filter window width is calculated from fTheta
					//halfsize is half of the filter window width
					halfSize=(int) fTheta->get(i,j,k); 
					//halfSize = (int) (pow(2.0f,halfSize)/2);
					//halfSize=halfSize*halfSize;
					//halfSize=1.5;
					if(halfSize>2){halfSize=2;}//halfsize=5
					//Coefficients defining the centerplane 
					//from the smoothed RawImage gradients
					coeffA=pSmoothX->get(i,j,k); 
					assert(coeffA==pSmoothX->get(i,j,k));
					coeffB=pSmoothY->get(i,j,k);
					coeffC=pSmoothZ->get(i,j,k);
					coeffD=srcImg->get(i,j,k);

					for(m = -halfSize; m<= halfSize; m++) {
						for (n = -halfSize; n <= halfSize; n++) {
							for(l= -halfSize; l<=halfSize; l++)
							{


								diff = (float) (m*m+n*n+l*l);
								//Compute the weight for the domain filter (domainWeight). The domain filter
								//is a Gaussian lowpass filter
								domainWeight = (double) pow(M_EXP, (double) (-diff/(2*sigmaCTheta*sigmaCTheta)));		
								if( (i+m) >= 0 && (i+m) < imax && (j+n) >= 0 && (j+n) < jmax && (k+l) >= 0 && (k+l)  < kmax)
								{
									//Compute the detail signal (detail) based on the difference between a 
									//neighborhood pixel and the centerplane passing through the center-pixel 

									detail = (float) (srcImg->get(i+m,j+n,k+l) - coeffA*m - coeffB*n - coeffC*l-coeffD);	
									if(detail!=0)						
										countvar++;
									//Compute the weight for the range filter (rangeWeight). The range filter
									//is a Gaussian filter defined by the detail signal.
									if( sigmaRTheta == 0 )
									{
										sigmaRTheta=0.1;
									}// 1===>0.1
									rangeWeight = (double) pow(M_EXP, (double) (-(detail*detail)/(2*sigmaRTheta*sigmaRTheta)));	
									if(rangeWeight==0) 
										rangeWeight=0.1;


									if(domainWeight == 0) 

										domainWeight = 0.1;

									tmp += detail*domainWeight*rangeWeight;

									//Detail Bilateral filter normalized by normFactor 
									normFactor += domainWeight*rangeWeight;
								}//if
							}//l
						}//n
					}//m
					if(normFactor == 0)
						normFactor = 0.1;
					tmp = tmp / normFactor;
					if( tmp != 0)
					{
						//printf("tmp=%d\n",tmp);
						tmp += coeffD;
						if( tmp == coeffD)
							printf("tmp==get(i,j)");


					}
					else tmp = coeffD;//not changed
					//if(tmp!=0){if(tmp==coeffC)printf("tmp==get(i,j)");}
					temp->put(i,j,k,tmp);//copy to the output

				}	
				// for(j...
			}	// for(i...
		} //for(k..
		//	printf("i=%d,j=%d\n",i,j);
		printf("countvar=%d\n",countvar);
	}