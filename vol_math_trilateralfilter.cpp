#include "vol_math_trilateralfilter.h"

static float lgtt=log10(2.0f);
//=====================================================================================================
Trilateralfilter::Trilateralfilter(RawImage* img)
{
	this->img=img;
	this->raw=rawarray(img);
}
Trilateralfilter::~Trilateralfilter(void)
{
	img=NULL;
	delete raw;
	//delete rawarray;
}
void Trilateralfilter::TrilateralFilter(PIXTYPE sigmaC)
	//=====================================================================================================
{	
	Raw* pSrcImg=this->raw;
	Raw destImg; 			
	Raw fTheta; 			//stores Adaptive neighborhood size for each pixel
	RawArray minGradientStack;	
	RawArray maxGradientStack; 
	Raw xGradient, yGradient,zGradient; 	//X and Y gradients of the input RawImage
	Raw xSmoothGradient, ySmoothGradient,zSmoothGradient; 	//Bilaterally filtered X and Y gradients of the input RawImage
	int levX, levY,levZ, levelMax, filterSize;  //level = log2(xsize) or log2(ysize)
	float sigmaR, sigmaCTheta, sigmaRTheta,beta;
	PIXTYPE  R;
	//domain variance for the two filters: sigmaC, sigmaCTheta
	//range variance of the two filters: sigmaR, sigmaRTheta
	//R -- threshold to compute adaptive region (see paper for details)

	//Default internal Parameters
	sigmaCTheta = sigmaC; //Variance of the Domain Filter, the only user set parameter
	beta = (float)0.15; //Always set between 0.1 and 0.2
	filterSize = (int) sigmaC; 

	//Compute the RawImage stack height
	/*levX=(int) (log10(float (pSrcImg->getXsize()))/log10(2.0f));
	levY=(int) (log10(float (pSrcImg->getYsize()))/log10(2.0f));
	*/

	levX=(int) (log10(float (pSrcImg->getXsize()))/lgtt);
	levY=(int) (log10(float (pSrcImg->getYsize()))/lgtt);
	levZ=(int) (log10(float (pSrcImg->getZsize()))/lgtt);
	if(levX < levY)
		levelMax = levX+1;
	else
		levelMax = levY+1;

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
	ComputeGradients(&xGradient,&yGradient,&zGradient);

	/**
	Builds the Min-Max Image Stack consisting of Image Gradients (Step 2).
	Also computes sigmaR, the range variance, (see equation 11 in the paper for further details).
	min and max gradients are stored in two separate stacks.
	Range variance sigmaR is calculated from equation 11 in the paper.
	Large ssq improves noise reduction, but also reduces outlier
	rejection, and may blur weaker boundaries of slight intensity
	changes.
	**/
	sigmaR = buildMinMaxImageStack(&xGradient,&yGradient,&zGradient,&minGradientStack,&maxGradientStack,levelMax,beta);

	//Set the remaining internal parameters required for trilateral filter
	R = sigmaR;
	sigmaRTheta =sigmaR;//from  sigmaRTheta= R = sigmaR
	/**
	Bilaterally filter the X and Y gradients of the input RawImage (Step 3, equation 4 and 5)
	to produce xSmoothGradient and ySmoothGradient.
	**/
	BilateralGradientFilter(&xGradient, &yGradient,&zGradient, &xSmoothGradient, &ySmoothGradient, &zSmoothGradient,sigmaC, sigmaR, filterSize);

	/**
	Find the adaptive neighborhood fTheta for each pixel location (Step 4). fTheta size is
	given by stack level. The min-max gradient stacks and range threshold "R" are used for this calculation
	(see equation 10 in the paper for details).
	**/
	findAdaptiveRegion(&minGradientStack, &maxGradientStack, R, levelMax);

	/**
	Performs bilateral filter on the detail signal (Step 5).
	See Equation 6, 7, 8 and 9.
	Output is stored in destimg (end result of equation 8, Section 3.1)
	**/
	DetailBilateralFilter(pSrcImg, &xSmoothGradient, &ySmoothGradient,&zSmoothGradient,&fTheta, sigmaCTheta, sigmaRTheta);

	//Copying the result to the output RawImage
	//wipecopy(&destImg);

	//FILE *p=fopen("F:\\3D.raw","ab+");
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

	}
void Trilateralfilter::ComputeGradients(Raw* pX, Raw *pY,Raw* pZ)
{

	int i, j,k, imax, jmax,kmax, jN, iE,kU;
	PIXTYPE Cval, Eval, Nval,Uval, gE, gN,gU;

	imax = raw->getXsize();		// get RawImage size,
	jmax =  raw->getYsize();
	kmax =  raw->getZsize();
	for(k=0; k<kmax; k++)
	{
		kU = k+1;
		if(kU>kmax-1) kU=kmax-1;
	for(j=0; j<jmax; j++)			// for each scanline,
	{
		jN = j+1;					// addr. of North neighbor;
		if(jN>jmax-1) jN=jmax-1;
		for(i=0;i<imax;i++)			// and each pixel on a scanline,
		{
			iE = i+1;
			if(iE>imax-1) iE=imax-1;	// addr. of East neighbor
			Cval =  raw->get(i,j,k);
			Eval =  raw->get(iE,j,k);
			Nval =  raw->get(i,jN,k);
			Uval =  raw->get(i,j,kU);
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

	imax= raw->getXsize();	//get RawImage size
	jmax= raw->getYsize();
	kmax= raw->getZsize();


	for(lev=0; lev < levelMax; lev++) 
	{
		for(k=0;k<kmax;k++)
		{
			for(j=0; j < jmax; j++) 
			{
				for(i=0; i < imax; i++) 
				{
					if( lev == 0) { //stack level 0 is the magnitude of the gradients of original RawImage
						tmp = (PIXTYPE) sqrt((float) pX->get(i,j,k)*pX->get(i,j,k) + pY->get(i,j,k)*pY->get(i,j,k) +pZ->get(i,j,k)*pZ->get(i,j,k));
						if(maxGrad < tmp)
							maxGrad = tmp;
						if(minGrad > tmp)
							minGrad = tmp;
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
void Trilateralfilter::BilateralGradientFilter(Raw* pX, Raw* pY,Raw*pZ, Raw* pSmoothX, Raw* pSmoothY,Raw* pSmoothZ, 
	float sigmaC, float sigmaR, int filterSize)
{
	int i,j,k,m,n,l,imax,jmax,kmax,halfSize;
	PIXTYPE tmpX, tmpY, tmpZ,posDiff, gradDiff, domainWeight, rangeWeight,g1, g2;
	float  normFactor;
	imax = raw-> getXsize(); //get RawImage size
	jmax =  raw->getYsize(); 
	kmax =  raw->getZsize();
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
							domainWeight = (PIXTYPE) pow(M_EXP, (double) (-posDiff/(2*sigmaC*sigmaC*sigmaC)));
							if( (i+m) >= 0 && (i+m) <imax && (j+n) >=0 &&(j+n) < jmax &&(k+l)>=0&&(k+l)<kmax) {
								g1 = (PIXTYPE) (pow( float (pX->get(i+m,j+n,k+l)),2.0f) + pow(float(pY->get(i+m,j+n,k+l)),2.0f) );
								g2 = (PIXTYPE) (pow( float(pX->get(i,j,k)),2.0f) + pow(float(pY->get(i,j,k)),2.0f)+pow(float(pZ->get(i,j,k)),2.0f) );
								//g3 = (PIXTYPE) (pow( float(pX->get(i,j)),2.0f) + pow(float(pY->get(i,j)),2.0f) );
								//Compute the gradient difference between a pixel and its neighborhood pixel 
								gradDiff = (PIXTYPE) (sqrt(double(g1)) - sqrt(double(g2)));
								//Compute the weight for the range filter (rangeWeight). The range filter
								//is a Gaussian filter defined by the difference in gradient magnitude.
								if(sigmaR==0){sigmaR=0.1;}
								rangeWeight = (PIXTYPE) pow(M_EXP, (double) (-(gradDiff*gradDiff)/(2*sigmaR*sigmaR*sigmaR)));	
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
void Trilateralfilter::findAdaptiveRegion(RawArray* pMinStack, RawArray* pMaxStack, PIXTYPE R, int levelMax)
{
	int imax, jmax,kmax,i,j,k,lev;
	imax= raw->getXsize();	//get RawImage size
	jmax= raw->getYsize();
	kmax= raw->getZsize();
	for(k=0; k<kmax; k++)
	{
	for(j=0; j<jmax; j++) {
		for(i=0; i<imax;i++) {
			for(lev=0; lev < levelMax; lev++) {
				//Compute the adaptive neighboirhood based on the similarity of
				//the neighborhood gradients, equation 10, Section 3.2.
				if ( pMaxStack->get(i,j,k,lev) > (pMaxStack->get(i,j,k,0) + R) ||
					pMinStack->get(i,j,k,lev) < (pMaxStack->get(i,j,k,0) - R) )
					break;
			}
			 raw->put(i,j,k,(PIXTYPE) (lev-1) );
		}	// for(i...
	}	// for(j...
	}//for(k...
}
void Trilateralfilter::DetailBilateralFilter(Raw* srcImg, Raw* pSmoothX, Raw* pSmoothY, Raw* pSmoothZ,
	Raw* fTheta, float sigmaCTheta, float sigmaRTheta)
{

	int i,j,k,m,n,l,imax,jmax,kmax,halfSize;
	int countvar=0;
	PIXTYPE tmp, diff, detail;
	float  domainWeight, rangeWeight, normFactor;
	PIXTYPE coeffA, coeffB, coeffC,coeffD; //Plane Equation is z = coeffA.x + coeffB.y + coeffC.z+coeffD
	//coeffA = dI/dx, coeffB = dI/dy, coeffC = I at center pixel of the filter kernel

	imax= raw->getXsize();	//get RawImage size
	jmax= raw->getYsize();	
	kmax= raw->getZsize();
	for(i=0; i<imax; i++) { //X scankline...
		for(j=0; j<jmax; j++) {	//Y scanline...
			for(k=0;k<kmax;k++){ //zscanline..
			normFactor=0.0;
			tmp=0.0;
			//filter window width is calculated from fTheta
			//halfsize is half of the filter window width
			halfSize=(int) fTheta->get(i,j,k); 
			halfSize = (int) (pow(2.0f,halfSize)/2);
			//halfSize=halfSize*halfSize;
			//halfSize=1.5;
			if(halfSize>2){halfSize=2;}//halfsize=5
			//Coefficients defining the centerplane (equation6, section 3.1) is calculated
			//from the smoothed RawImage gradients
			coeffA=pSmoothX->get(i,j,k); 
			assert(coeffA==pSmoothX->get(i,j,k));
			coeffB=pSmoothY->get(i,j,k);
			coeffC=pSmoothZ->get(i,j,k);
			coeffD=srcImg->get(i,j,k);
			
			for(m = -halfSize; m<=halfSize; m++) {
				for (n = -halfSize; n<=halfSize; n++) {
					for(l= -halfSize; l<=halfSize; l++)
					diff = (PIXTYPE) (m*m+n*n+l*l);
					//Compute the weight for the domain filter (domainWeight). The domain filter
					//is a Gaussian lowpass filter
					domainWeight = (PIXTYPE) pow(M_EXP, (double) (-diff/(2*sigmaCTheta*sigmaCTheta)));		
					if( (i+m) >= 0 && (i+m) <imax && (j+n) >=0 && (j+n) < jmax && (k+l) >=0&& (k+l)< kmax) {
						//Compute the detail signal (detail) based on the difference between a 
						//neighborhood pixel and the centerplane passing through the center-pixel 
						//of the filter window. See equation 7, section 3.1 for details
						detail=(PIXTYPE) (srcImg->get(i+m,j+n,k+l) - coeffA*m - coeffB*n - coeffC*l-coeffD);	
						if(detail!=0)//printf("detail!=0%d",detail); 
						//printf("detail====>>%d ",detail);
						countvar++;
						//Compute the weight for the range filter (rangeWeight). The range filter
						//is a Gaussian filter defined by the detail signal.
						if(sigmaRTheta==0){sigmaRTheta=0.1;}// 1===>0.1
						rangeWeight = (PIXTYPE) pow(M_EXP, (double) (-(detail*detail)/(2*sigmaRTheta*sigmaRTheta)));	
						if(rangeWeight==0)rangeWeight=0.1;
						if(domainWeight==0)domainWeight=0.1;
						//tmp+=detail*domainWeight*rangeWeight;
						tmp+=detail*domainWeight*rangeWeight;
						
						//Detail Bilateral filter normalized by normFactor (eq. 9, Section 3.1)
						normFactor+= domainWeight*rangeWeight;
					}
				}
			}
			if(normFactor==0) normFactor=0.1;
			tmp = tmp/normFactor;
			if(tmp!=0)
			{
				//printf("tmp=%d\n",tmp);
				tmp+=coeffD;
				if(tmp==coeffD)
					printf("tmp==get(i,j)");
			

			}
			else tmp= coeffD;
			//if(tmp!=0){if(tmp==coeffC)printf("tmp==get(i,j)");}
			 raw->put(i,j,k,tmp);//copy to the output

		}	
		// for(j...
	}	// for(i...
	} //for(k..
//	printf("i=%d,j=%d\n",i,j);
	printf("countvar=%d\n",countvar);
}
