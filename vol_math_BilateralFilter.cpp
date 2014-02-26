#include "vol_math_BilateralFilter.h"
//qym
#include <math.h>
#include "vol_math_RawImage.h"


void BilateralFilter_Struct_Oriented(Raw *raw,float sigmaC, float sigmaR, int filterSize)
{
	

}
int max(int num1, int num2){
	if(num1>num2)
		return num1;
	else
		return num2;
}
//Bilateralfilter::BilateralFilter(RawImage* img)
//{
//	this->img=img;
//	this->raw=rawarray(img);
//}
BilateralFilter::BilateralFilter(Raw2D *image,double sigmaD, double sigmaR){



	this->src=*image;
	int sigmaMax = max(sigmaD, sigmaR);
	this->kernelRadius = 6;//ceil((double)2 * sigmaMax);


	double twoSigmaRSquared = 2 * sigmaR * sigmaR;

	int kernelSize = this->kernelRadius * 2 + 1;

	kernelD = new double*[kernelSize];
	for(int i=0; i<kernelSize;i++){
		kernelD[i] = new double[kernelSize];
	}

	int center = (kernelSize - 1) / 2;

	for (int x = -center; x < -center + kernelSize; x++) {
		for (int y = -center; y < -center + kernelSize; y++) {
			kernelD[x + center][y + center] = this->gauss(sigmaD, x, y);//sigmaD
		}
	}


	gaussSimilarity = new double[256];
	for (int i = 0; i < 256; i++) {
		gaussSimilarity[i] = exp((double)-((i) /twoSigmaRSquared));
	}

	src = image;
}
//Raw2D & BilateralFilter::runFilter(Raw2D &src){
//
//
//	//for(int i=0;i<src.getXsize();i++){
//	//	for (int j=0;j<src.getYsize();j++){
//
//			apply(src);
//
//	//	}
//	//}
//
//	return src;
//
//}

double BilateralFilter::getSpatialWeight(int m, int n,int i,int j){
	return kernelD[(int)(i-m + kernelRadius)][(int)(j-n + kernelRadius)];
}


void BilateralFilter::apply(Raw2D &src) {// ~i=y j=x
	Raw2D temp(src);
	for (int i=0;i<src.getXsize();i++)
	{
		for (int j=0;j<src.getYsize();j++)
		{
			if(i>0 && j>0 && i<src.getXsize() && j< src.getYsize()){
				double sum = 0;
				double totalWeight = 0;
				int intensityCenter =src.get(i,j);


				int mMax = i + kernelRadius;
				int nMax = j + kernelRadius;
				double weight;

				for (int m = i-kernelRadius; m < mMax; m++) 
				{
					for (int n = j-kernelRadius; n < nMax; n++) 
					{

						if (this->isInsideBoundaries(m, n)) 
						{
							int intensityKernelPos = src.get(m,n);
							weight = getSpatialWeight(m,n,i,j) * similarity(intensityKernelPos,intensityCenter);
							//if (weight!=0)
							//{
							//	cout<<weight<<endl;
							//}
							totalWeight += weight;
							sum += (weight * intensityKernelPos);
						}
					}
				}
				//int newvalue=(int)floor(sum / totalWeight);
				if (sum!=0)
				{
					int newvalue=(int)(sum / totalWeight);
					temp.put(i,j,newvalue);
				
				
				//if (newvalue!=src.get(i,j))
				//{
				//	if (newvalue-src.get(i,j)!=-1)
				//	{
				//		cout<<newvalue-src.get(i,j)<<endl;
				//	}

				//}
				}
				//RawArray pixel[3];
				//pixel[0]= newvalue;
				//pixel[1]=newvalue;
				//pixel[2]=newvalue;
				//src.put(i,j,pixel);
				
				
			}
		}
		
		}
	
	src=temp;
	

}

double BilateralFilter::similarity(int p, int s) {
	// this equals: Math.exp(-(( Math.abs(p-s)) /  2 * this->sigmaR * this->sigmaR));
	// but is precomputed to improve performance
	return this->gaussSimilarity[abs(p-s)];
}


double BilateralFilter::gauss (double sigma, int x, int y) {
	return exp(-((x * x + y * y) / (2 * sigma * sigma)));
}


bool BilateralFilter::isInsideBoundaries(int m,int n){
	if(m>-1 && n>-1 && m<src.getXsize() && n <src.getYsize())
		return true;
	else 
		return false;
}

BilateralFilter::~BilateralFilter( void )
{

	
	
	delete [] gaussSimilarity;
	gaussSimilarity = NULL;
}
