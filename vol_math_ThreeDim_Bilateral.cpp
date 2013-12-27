#include "vol_math_ThreeDim_Bilateral.h"

//qym
#include <math.h>
//int max(int num1, int num2){
//	if(num1>num2)
//		return num1;
//	else
//		return num2;
//}
//ThreeDim_Bilateral::ThreeDim_Bilateral(RawImage* img)
//{
//	this->img=img;
//	this->raw=rawarray(img);
//}
ThreeDim_Bilateral::ThreeDim_Bilateral(Raw *image,double sigmaD, double sigmaR){



	this->src=*image;
	int sigmaMax = max(sigmaD, sigmaR);
	this->kernelRadius = 6;//ceil((double)2 * sigmaMax);


	double twoSigmaRSquared = 2 * sigmaR * sigmaR;

	int kernelSize = this->kernelRadius * 2 + 1;

	kernelD = new double**[kernelSize];
	for(int i=0; i<kernelSize;i++){
		kernelD[i] = new double*[kernelSize];
		for(int j=0; j<kernelSize;j++){
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

double ThreeDim_Bilateral::getSpatialWeight(int m, int n,int l, int i, int j, int k){
	return kernelD[(int)(i-m + kernelRadius)][(int)(j-n + kernelRadius)][(int)(k-l + kernelRadius)];
}


Raw ThreeDim_Bilateral::apply(Raw &src) {// ~i=y j=x 
	Raw temp(src);
	for (int i=0;i<src.getZsize();i++)
	{
		for (int j=0;j<src.getYsize();j++)
		{
			for (int k=0; k < src.getXsize(); k++)
			{
				if(i>0 && j>0 && k>0 && i<src.getXsize() && j< src.getYsize() && k< src.getZsize()){
					double sum = 0;
					double totalWeight = 0;
					int intensityCenter =src.get(i,j,k);


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
									int intensityKernelPos = src.get(m,n,l);
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
						temp.put(i,j,k,newvalue);

					}

				}
			}

		}

	}

	return temp;


}

double ThreeDim_Bilateral::similarity(int p, int s) {
	// this equals: Math.exp(-(( Math.abs(p-s)) /  2 * this->sigmaR * this->sigmaR));
	// but is precomputed to improve performance
	return this->gaussSimilarity[abs(p-s)];
}


double ThreeDim_Bilateral::gauss (double sigma, int x, int y,int z) {
	return exp(-((x * x + y * y+z*z) / (2 * sigma * sigma)));
}


bool ThreeDim_Bilateral::isInsideBoundaries(int m,int n, int l){
	if(m>-1 && n>-1 && l >-1 && m<src.getXsize() && n <src.getYsize() && l<src.getZsize())
		return true;
	else 
		return false;
}
