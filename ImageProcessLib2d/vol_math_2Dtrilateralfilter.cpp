#include "vol_math_2Dtrilateralfilter.h"
#include <math.h>
#include <iostream>
#include <stdlib.h>
#define u_char unsigned char
using namespace std;
Trilateralfilter2D::Trilateralfilter2D(void)
{
	
}
Trilateralfilter2D::~Trilateralfilter2D()
{

}
 static float lgtt=log10(2.0f);

//FUNCTION CALL THAT IMPLEMENTS TRILATERAL FILTER
//=====================================================================================================
Raw2D * Trilateralfilter2D::TrilateralFilter2D(Raw2D* pSrcImg, PIXTYPE sigmaC)
//=====================================================================================================
{
Raw2D destImg; 			
Raw2D fTheta; 			//stores Adaptive neighborhood size for each pixel
Raw2DArray minGradientStack, maxGradientStack; 
Raw2D xGradient, yGradient; 	//X and Y gradients of the input image
Raw2D xSmoothGradient, ySmoothGradient; 	//Bilaterally filtered X and Y gradients of the input image
int levX, levY, levelMax, filterSize;  //level = log2(xsize) or log2(ysize)
PIXTYPE sigmaR, sigmaCTheta, sigmaRTheta, R, beta; //domain variance for the two filters: sigmaC, sigmaCTheta
						//range variance of the two filters: sigmaR, sigmaRTheta
						//R -- threshold to compute adaptive region (see paper for details)
	
	//Default internal Parameters
	sigmaCTheta = sigmaC; //Variance of the Domain Filter, the only user set parameter
	beta = (PIXTYPE) 0.15; //Always set between 0.1 and 0.2
	filterSize = (int) sigmaC; 

	//Compute the image stack height
	/*levX=(int) (log10(float (pSrcImg->getXsize()))/log10(2.0f));
	levY=(int) (log10(float (pSrcImg->getYsize()))/log10(2.0f));
	*/
	
	levX=(int) (log10(float (pSrcImg->getXsize()))/lgtt);
	levY=(int) (log10(float (pSrcImg->getYsize()))/lgtt);
	if(levX < levY)
		levelMax = levX+1;
	else
		levelMax = levY+1;

	//Allocate memory for the Min-Max Image Stack
	minGradientStack.sizer(pSrcImg->getXsize(),pSrcImg->getYsize(),levelMax);
	maxGradientStack.sizer(pSrcImg->getXsize(),pSrcImg->getYsize(),levelMax);

	//Allocate memory for the gradient vectors and output image
	xGradient.sizer(pSrcImg); 
	yGradient.sizer(pSrcImg);
	xSmoothGradient.sizer(pSrcImg);
	ySmoothGradient.sizer(pSrcImg); 
	fTheta.sizer(pSrcImg);
	destImg.sizer(pSrcImg);

	/**
		Compute Gradients using Forward Difference (Step 1)
		X gradient is stored in xGradient
		Y gradient is stored in yGradient
	**/
	pSrcImg->ComputeGradients(&xGradient,&yGradient);

	/**
		Builds the Min-Max Image Stack consisting of Image Gradients (Step 2).
		Also computes sigmaR, the range variance, (see equation 11 in the paper for further details).
		min and max gradients are stored in two separate stacks.
		Range variance sigmaR is calculated from equation 11 in the paper.
Large ssq improves noise reduction, but also reduces outlier
rejection, and may blur weaker boundaries of slight intensity
changes.
	**/
	sigmaR = pSrcImg->buildMinMaxImageStack(&xGradient,&yGradient,&minGradientStack,&maxGradientStack,levelMax,beta);

	//Set the remaining internal parameters required for trilateral filter
	sigmaRTheta = R = sigmaR;
	/**
		Bilaterally filter the X and Y gradients of the input image (Step 3, equation 4 and 5)
		to produce xSmoothGradient and ySmoothGradient.
	**/

	/**
		Find the adaptive neighborhood fTheta for each pixel location (Step 4). fTheta size is
		given by stack level. The min-max gradient stacks and range threshold "R" are used for this calculation
		(see equation 10 in the paper for details).
	**/
	fTheta.findAdaptiveRegion(&minGradientStack, &maxGradientStack, R, levelMax);

	/**
		Performs bilateral filter on the detail signal (Step 5).
		See Equation 6, 7, 8 and 9.
		Output is stored in destimg (end result of equation 8, Section 3.1)
	**/
	destImg.DetailBilateralFilter(pSrcImg, &xSmoothGradient, &ySmoothGradient, &fTheta, sigmaCTheta, sigmaRTheta);
	
	//Copying the result to the output image
	pSrcImg->wipecopy(&destImg);
	return pSrcImg;


}



/**
	Computes the X and Y gradients using forward difference
	X gradient is stored in pX 
	Y gradient is stored in pY
**/

void Raw2D::ComputeGradients(Raw2D* pX, Raw2D *pY)
{

	int i, j, imax, jmax, jN, iE;
	PIXTYPE Cval, Eval, Nval, gE, gN;

	imax = getXsize();		// get image size,
	jmax = getYsize();

	for(j=0; j<jmax; j++)			// for each scanline,
	{
		jN = j+1;					// addr. of North neighbor;
		if(jN>jmax-1) jN=jmax-1;
		for(i=0;i<imax;i++)			// and each pixel on a scanline,
		{
			iE = i+1;
			if(iE>imax-1) iE=imax-1;	// addr. of East neighbor
			Cval = get(i,j);
			Eval = get(iE,j);
			Nval = get(i,jN);
			gE = (PIXTYPE) (Eval-Cval); //gradient computation with forward difference
			gN = (PIXTYPE) (Nval-Cval);
			
			pX->put(i,j,gE);		//copy the gradient values to pX and pY
			pY->put(i,j,gN);
		}// for(i...
	}// for(j...


}

/***
	Building the Min-Max Stack of Image Gradients
	Input:
	pX and pY -- pointers to X and Y gradients of original image
	levelMax -- height of the image stack
	beta -- user specified parameter used to compute the range variance
	Output:
	pMinStack and pMaxStack -- pointers to min and max stack of image gradients
	Return Value;
	Range variance (sigmaR), equation 11.
***/

PIXTYPE Raw2D::buildMinMaxImageStack(Raw2D* pX, Raw2D* pY, Raw2DArray* pMinStack,
									 Raw2DArray* pMaxStack , int levelMax, float beta)
{
	int imax, jmax, i, j, lev, m, n;
	PIXTYPE min, max, minGrad = 1000000.0, maxGrad = -1000000.0, tmp, tmpMin, tmpMax, rangeVariance;

	imax=getXsize();	//get image size
	jmax=getYsize();
	
	for(lev=0; lev < levelMax; lev++) {
		for(j=0; j < jmax; j++) {
			for(i=0; i < imax; i++) {
				if( lev == 0) { //stack level 0 is the magnitude of the gradients of original image
					tmp = (PIXTYPE) sqrt((float) pX->get(i,j)*pX->get(i,j) + pY->get(i,j)*pY->get(i,j) );
					if(maxGrad < tmp)
						maxGrad = tmp;
					if(minGrad > tmp)
						minGrad = tmp;
					max = min = tmp;
					pMinStack->put(i,j,0,min);
					pMaxStack->put(i,j,0,max);
				}
				if( lev > 0) { //Gradients at each level of the image stack is computed from the level below
					min = pMinStack->get(i,j,lev-1);
					max = pMaxStack->get(i,j,lev-1);
					
					for(m=-1; m <= 1; m++) {
						for(n=-1; n <= 1; n++) {	
							//Computes the maximum and minimum gradient value in the neighborhood
							if((i+m) >=0 && (i+m) < imax && (j+n) >=0 && (j+n) < jmax ) {
								tmpMin = (PIXTYPE) pMinStack->get(i+m,j+n,lev-1);
								tmpMax = (PIXTYPE) pMaxStack->get(i+m,j+n,lev-1);
								if(min > tmpMin)
									min = tmpMin;
								if(max < tmpMax)
									max = tmpMax;
							}
						}
					}
					
					pMinStack->put(i,j,lev,min);
					pMaxStack->put(i,j,lev,max);
				} 
				
			}	// for(i...
		}	// for(j...
	}	// for(lev...
	
	//range variance is computed as a ratio of difference between max and min gradient
	rangeVariance = beta*(maxGrad - minGrad); 
	return rangeVariance;

}


/**
	Bilaterally filters the X and Y gradients of the input image.
	Input:
	pX -- X gradient of the input image
	pY -- Y gradient of the input image
	sigmaC -- domain variance of the bilateral filter
	sigmaR -- range variance of the bilateral filter
	filterSize -- size of the filter kernel
	Output:
	pSmoothX -- bilaterally filtered X gradient
	pSmoothY -- bilaterally filtered Y gradient
	
**/
void Raw2D::BilateralGradientFilter(Raw2D* pX, Raw2D* pY, Raw2D* pSmoothX, Raw2D* pSmoothY, 
				float sigmaC, float sigmaR, int filterSize)
{
	int i,j,m,n,imax,jmax,halfSize;
	PIXTYPE tmpX, tmpY, posDiff, gradDiff, domainWeight, rangeWeight, normFactor, g1, g2;
	
	imax = getXsize(); //get image size
	jmax = getYsize(); 
	halfSize = (int) (filterSize-1)/2; //size of the filter kernel

	
	for(i=0; i<imax; i++) {		//X scanline
		for(j=0; j<jmax; j++) { //Y scaline
			normFactor=0.0;
			tmpX=0.0;
			tmpY=0.0;
			for(m = -halfSize; m<=halfSize; m++) {
				for (n = -halfSize; n<=halfSize; n++) {
					posDiff=(PIXTYPE) (m*m+n*n); 
					//Compute the weight for the domain filter (domainWeight). The domain filter
					//is a Gaussian low pass filter
					domainWeight = (PIXTYPE) powf(M_EXP, (double) (-posDiff/(2*sigmaC*sigmaC)));
					if( (i+m) >= 0 && (i+m) <imax && (j+n) >=0 &&(j+n) < jmax) {
						g1 = (PIXTYPE) (powf( float (pX->get(i+m,j+n)),2.0f) + powf(float(pY->get(i+m,j+n)),2.0f) );
						g2 = (PIXTYPE) (powf( float(pX->get(i,j)),2.0f) + powf(float(pY->get(i,j)),2.0f) );
						//Compute the gradient difference between a pixel and its neighborhood pixel 
						gradDiff = (PIXTYPE) (sqrt(double(g1)) - sqrt(double(g2)));
						//Compute the weight for the range filter (rangeWeight). The range filter
						//is a Gaussian filter defined by the difference in gradient magnitude.
						if(sigmaR==0){sigmaR=1;}
						rangeWeight = (PIXTYPE) powf(M_EXP, (double) (-(gradDiff*gradDiff)/(2*sigmaR*sigmaR)));	
						tmpX += pX->get(i+m,j+n)*domainWeight*rangeWeight;
						tmpY += pY->get(i+m,j+n)*domainWeight*rangeWeight;
						//Bilateral filter normalized by normFactor (eq. 5, Section 3.1) 
						normFactor += domainWeight*rangeWeight;
					}
				}
			}	
			if(normFactor==0)normFactor=1;
			tmpX = tmpX/normFactor;
			tmpY = tmpY/normFactor;
			pSmoothX->put(i,j,tmpX);	//copy smooth gradients to pSmoothX and pSmoothY
			pSmoothY->put(i,j,tmpY);
		}	// for(j...
	}	// for(i...

}

/**
	Finds adaptive neighborhood for every pixel. 
	Input:
	pMinStack -- pointer to the min gradient stack
	pmaxStack -- pointer to the max gradient stack
	R -- threshold value for computing fTheta
	levelmax -- maximum level of the image stack
	Output:
	fTheta -- stack level that satisfies equation 10
**/

void Raw2D::findAdaptiveRegion(Raw2DArray* pMinStack, Raw2DArray* pMaxStack, PIXTYPE R, int levelMax)
{
	int imax, jmax,i,j,lev;

	imax=getXsize();	//get image size
	jmax=getYsize();
	
	for(j=0; j<jmax; j++) {
		for(i=0; i<imax;i++) {
			for(lev=0; lev < levelMax; lev++) {
				//Compute the adaptive neighboirhood based on the similarity of
				//the neighborhood gradients, equation 10, Section 3.2.
				if ( pMaxStack->get(i,j,lev) > (pMaxStack->get(i,j,0) + R) ||
					pMinStack->get(i,j,lev) < (pMaxStack->get(i,j,0) - R) )
					break;
			}
			put(i,j,(PIXTYPE) (lev-1) );
		}	// for(i...
	}	// for(j...

}



/**
	Filters the detail signal and computes the output (2nd filtering pass for trilateral filter).
	Input:
	srcImg -- input image
	pSmoothX -- bilaterally filtered X gradient of srcImg
	pSmoothY -- bilaterally filtered Y gradient of srcImg
	fTheta -- Adaptive neighborhood for each pixel of srcImg
	sigmaCTheta -- domain variance of the bilateral filter
	sigmaRTheta -- range variance of the bilateral filter
	Output:
	Trilaterally filtered input.
**/


void Raw2D::DetailBilateralFilter(Raw2D* srcImg, Raw2D* pSmoothX, Raw2D* pSmoothY, 
				Raw2D* fTheta, PIXTYPE sigmaCTheta, PIXTYPE sigmaRTheta)
{

	int i,j,m,n,imax,jmax,halfSize;
	PIXTYPE tmp, diff, detail, domainWeight, rangeWeight, normFactor;
	PIXTYPE coeffA, coeffB, coeffC; //Plane Equation is z = coeffA.x + coeffB.y + coeffC
				//coeffA = dI/dx, coeffB = dI/dy, coeffC = I at center pixel of the filter kernel
	
	imax=getXsize();	//get image size
	jmax=getYsize();	

	for(i=0; i<imax; i++) { //X scankline...
		for(j=0; j<jmax; j++) {	//Y scanline...
			normFactor=0.0;
			tmp=0.0;
			//filter window width is calculated from fTheta
			//halfsize is half of the filter window width
			//halfSize=(int) fTheta->get(i,j); 
			//halfSize = (int) (powf(2.0f,halfSize)/2);
			halfSize=1.5;
			//Coefficients defining the centerplane (equation6, section 3.1) is calculated
			//from the smoothed image gradients
			coeffA=pSmoothX->get(i,j); 
			coeffB=pSmoothY->get(i,j);
			coeffC=srcImg->get(i,j);
			for(m = -halfSize; m<=halfSize; m++) {
				for (n = -halfSize; n<=halfSize; n++) {
					diff = (PIXTYPE) (m*m+n*n);
					//Compute the weight for the domain filter (domainWeight). The domain filter
					//is a Gaussian lowpass filter
					domainWeight = (PIXTYPE) powf(M_EXP, (double) (-diff/(2*sigmaCTheta*sigmaCTheta)));		
					if( (i+m) >= 0 && (i+m) <imax && (j+n) >=0 && (j+n) < jmax) {
						//Compute the detail signal (detail) based on the difference between a 
						//neighborhood pixel and the centerplane passing through the center-pixel 
						//of the filter window. See equation 7, section 3.1 for details
						detail=(PIXTYPE) (srcImg->get(i+m,j+n) - coeffA*m - coeffB*n - coeffC);	
						//Compute the weight for the range filter (rangeWeight). The range filter
						//is a Gaussian filter defined by the detail signal.
						if(sigmaRTheta==0){sigmaRTheta=1;}
						rangeWeight = (PIXTYPE) powf(M_EXP, (double) (-(detail*detail)/(2*sigmaRTheta*sigmaRTheta)));	
						tmp += detail*domainWeight*rangeWeight;
						//Detail Bilateral filter normalized by normFactor (eq. 9, Section 3.1)
						normFactor += domainWeight*rangeWeight;
					}
				}
			}
			if(normFactor==0) normFactor=1;
			tmp = tmp/normFactor;
			tmp += coeffC;
			put(i,j,tmp);//copy to the output
			
		}	
		// for(j...
	}	// for(i...
	printf("i=%d,j=%d\n",i,j);
}


//================================================================================
//Some helper functions for Raw2D and Raw3D class.
//================================================================================

/**
	Memory allocator for a rectangular 2D object. Takes an 'empty' Raw2D
	object and reserves a rectangular field of ixsize by iysize pixels.
**/
//void Raw2D::sizer(int ixsize, int iysize) {
//	if(data!=NULL)
//		delete [] this->data;
//		data=NULL;
//		this->data = new PIXTYPE[ixsize*iysize];	// & allocate memory.
//    	xsize = ixsize;				// set new image size,
//    	ysize = iysize;
//}

/**
	Memory allocator for a 2D object.  Allocates space for an object of
	same size as 'src'.
**/
//void Raw2D::sizer(Raw2D* src) {
//    	int ix, iy;
//	
//    	ix = src->getXsize();
//    	iy = src->getYsize();
//    	sizer(ix,iy);
//}
//


/**
	Copy all the pixels from 'src'.  If size of 'src' is different from
	us, 'wipe' out our current pixels, and change our size to match 'src'.
	If a 'wipe' was necessary, return 'FALSE', else return 'TRUE'.
**/
//
// bool Raw2D::wipecopy(Raw2D* src) {
//	bool out=true;
//	int i,imax;	
//    
//	if(getYsize() != src->getYsize() || getXsize()!=src->getXsize()) { // resize to fit 'src'.
//		sizer(src);
//		out=FALSE;
//    	}
//    	imax = getXsize()*getYsize();
//    	for(i=0; i<imax; i++) {
//		putXY(i,src->getXY(i));
//    	}
//		
//		return(out);
//}
//


/**
	Sets the size and allocates memory. Discards any existing contents if
	the 3D object is not already 'empty'
**/
//void Raw3D::sizer(int ixsize, int iysize, int izsize) {
//	int i;
//		if(z!=NULL)
//			delete[]this->z;
//    	z = new Raw2D[izsize];			// make room for the 2D objects,
//    	zsize = izsize;
//    	for(i=0; i< zsize; i++) 
//		z[i].sizer(ixsize,iysize);	// use the Raw2D sizer.	
//}
//
//void Raw3D::sizer(Raw3D* src)
//{
//	z=src->z;
//	zsize=src->zsize;
//
//}
///**
//	Copy all the pixels from 'src', resizing as needed. Do not change
//	name of this object.
//**/ 
//void Raw3D::wipecopy(Raw3D& src) {
//int k,kmax;
//
//	if(&src==NULL)return;
//	if(src.zsize==0) return;		// ignore empty inputs.
//    	if(src.getZsize() != zsize || src.getYsize() != z[0].getYsize() ||
//		src.getXsize() != getXsize()) {
//		sizer(&src);
//	}
//	kmax = getZsize();
//    	for(k=0; k< kmax; k++) {		// copy each field;
//		z[k].wipecopy(&(src.z[k]));
//	}
//}


