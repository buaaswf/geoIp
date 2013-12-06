#include "BilateralFilter.h"
#include <complex>
#include <iostream>
#include <math.h>
using namespace std;
#define min( a, b ) ( (a) < (b) ? (a) : (b) )
#define max( a, b ) ( (a) > (b) ? (a) : (b) )

/*******************************************************************************
*双边滤波函数
*输入：输入图像src，领域窗口大小为neighbor x neighbor（neighbor为偶数时，
*	   自动调整为奇数）空间域权重参数sigma_s，幅度域权重参数sigma_r
*返回值：滤波后图像IplImage指针
********************************************************************************/
R *BilaFlt( IplImage *src, int neighbor, double sigma_s, double sigma_r )
{
	IplImage *dst = cvCreateImage( cvGetSize( src ), IPL_DEPTH_8U, src->nChannels );
	int half_neigh = neighbor / 2;
	int neighbor2 = 2 * half_neigh + 1; //当neighbor是偶数时，邻域替换成neighbor2 
	//申请邻域内偏移量空间
	long int **deltas = new long int*[neighbor2]; 
	//申请空间域高斯权重空间
	double **GauS = new double*[neighbor2]; 
	int ii, jj;
	double temp;
	for ( ii = 0; ii <neighbor2; ii++ )
	{
		deltas[ii] = new long int[neighbor2];
		GauS[ii] = new double[neighbor2];
	}

	//计算邻域内的空间域的高斯权重
	for ( ii = 0; ii < neighbor2; ii++ )
	{
		for ( jj=0; jj < neighbor2; jj++ )
		{
			deltas[ii][jj] = ( ii - half_neigh ) * src->widthStep + ( jj - half_neigh ) 
				* src->nChannels;
			/*cout << deltas[ii][jj] << ' ';*/
			temp = ( half_neigh - ii ) * ( half_neigh - ii ) + ( half_neigh - jj) 
				* ( half_neigh - jj );
			temp = -1 * temp / ( 2 * sigma_s * sigma_s );
			GauS[ii][jj] = exp( temp );
		}
		/*cout << endl;*/
	}	

	//定义一些变量
	int x, y;
	long int tempxy;
	int dgray, dL, da, db;
	double GauR;
	double weight,Wb, Ib, ILb, Iab, Ibb;
	int iMin, iMax, jMin, jMax;
	int indexi, indexj;
	int indexiMin, indexiMax, indexjMin, indexjMax;

	if ( 1 == src->nChannels )		//如果输入图像为黑白图像
	{
		for ( x = 0;x < src->height; x++ )
		{
			for ( y = 0; y < src->width; y++ )
			{
				//初始化
				weight = 0;
				Wb = 0, Ib = 0;
				tempxy = x * src->widthStep + y * src->nChannels;

				//计算感兴趣区域边界
				iMin = max( x - half_neigh, 0 );
				iMax = min( x + half_neigh, src->height - 1 );
				jMin = max( y - half_neigh, 0 );
				jMax = min( y + half_neigh, src->width - 1 );

				indexiMin = iMin - x + half_neigh;
				indexiMax = iMax - x + half_neigh;
				indexjMin = jMin - y + half_neigh;
				indexjMax = jMax - y + half_neigh;

				//扫描领域
				for ( indexi = indexiMin; indexi <= indexiMax; indexi++ )
				{
					for ( indexj = indexjMin; indexj <= indexjMax; indexj++ )
					{
						dgray = ( uchar )src->imageData[tempxy] - 
							( uchar )src->imageData[tempxy + deltas[indexi][indexj]];
						temp = dgray * dgray / ( 2 * sigma_r * sigma_r );
						temp *= -1;
						GauR = exp( temp );			//得到幅度域权重
						weight = GauS[indexi][indexj] * GauR;	//总体权重
						Wb += weight;				//计算权重之和
						Ib += weight * 
							( uchar )src->imageData[tempxy + deltas[indexi][indexj]]; 
					}
				}
				dst->imageData[tempxy] = ( uchar )( Ib / Wb );	//归一化，取整
			}
		}
	}
	else
	{
		cvCvtColor( src, src, CV_RGB2Lab );		//转化为Lab颜色空间图像
		sigma_r *= 1.732;						//调整sigma_r
		for ( x = 0;x < src->height; x++ )
		{
			for ( y = 0; y < src->width; y++ )
			{
				//初始化
				weight = 0;
				Wb = 0, ILb = 0, Iab = 0, Ibb = 0;
				tempxy = x * src->widthStep + y * src->nChannels;

				//计算感兴趣区域边界
				iMin = max( x - half_neigh, 0 );
				iMax = min( x + half_neigh, src->height - 1 );
				jMin = max( y - half_neigh, 0 );
				jMax = min( y + half_neigh, src->width - 1 );

				indexiMin = iMin - x + half_neigh;
				indexiMax = iMax - x + half_neigh;
				indexjMin = jMin - y + half_neigh;
				indexjMax = jMax - y + half_neigh;

				//扫描邻域
				for ( indexi = indexiMin; indexi <= indexiMax; indexi++ )
				{
					for ( indexj = indexjMin; indexj <= indexjMax; indexj++ )
					{
						dL = ( uchar )src->imageData[tempxy + 0] - 
							( uchar )src->imageData[tempxy + deltas[indexi][indexj] + 0];
						da = ( uchar )src->imageData[tempxy + 1] - 
							( uchar )src->imageData[tempxy + deltas[indexi][indexj] + 1];
						db = ( uchar )src->imageData[tempxy + 2] - 
							( uchar )src->imageData[tempxy + deltas[indexi][indexj] + 2];
						temp = ( dL * dL + da * da + db * db ) / ( 2 * sigma_r * sigma_r );
						temp *= -1;
						GauR = exp( temp );			//得到幅度域权重
						weight = GauS[indexi][indexj] * GauR;	//总体权重
						Wb += weight;				//计算权重之和
						ILb += weight * ( uchar )src->imageData[tempxy + deltas[indexi][indexj] + 0]; 
						Iab += weight * ( uchar )src->imageData[tempxy + deltas[indexi][indexj] + 1]; 
						Ibb += weight * ( uchar )src->imageData[tempxy + deltas[indexi][indexj] + 2]; 
					}
				}
				dst->imageData[tempxy + 0] = ( uchar )( ILb / Wb );	//归一化，取整
				dst->imageData[tempxy + 1] = ( uchar )( Iab / Wb );	//归一化，取整
				dst->imageData[tempxy + 2] = ( uchar )( Ibb / Wb );	//归一化，取整
			}
		}
		cvCvtColor( src, src ,CV_Lab2RGB );			//转化为RGB颜色空间图像
		cvCvtColor( dst, dst, CV_Lab2RGB );			//转化为RGB颜色空间图像

	}

	//释放空间
	for ( ii = 0; ii < neighbor; ii++ )
	{
		delete[] deltas[ii];
		delete[] GauS[ii];
	}
	delete[] deltas;
	delete[] GauS;

	return dst;
}

