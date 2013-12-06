#include "BilateralFilter.h"
#include <complex>
#include <iostream>
#include <math.h>
using namespace std;
#define min( a, b ) ( (a) < (b) ? (a) : (b) )
#define max( a, b ) ( (a) > (b) ? (a) : (b) )

/*******************************************************************************
*˫���˲�����
*���룺����ͼ��src�����򴰿ڴ�СΪneighbor x neighbor��neighborΪż��ʱ��
*	   �Զ�����Ϊ�������ռ���Ȩ�ز���sigma_s��������Ȩ�ز���sigma_r
*����ֵ���˲���ͼ��IplImageָ��
********************************************************************************/
R *BilaFlt( IplImage *src, int neighbor, double sigma_s, double sigma_r )
{
	IplImage *dst = cvCreateImage( cvGetSize( src ), IPL_DEPTH_8U, src->nChannels );
	int half_neigh = neighbor / 2;
	int neighbor2 = 2 * half_neigh + 1; //��neighbor��ż��ʱ�������滻��neighbor2 
	//����������ƫ�����ռ�
	long int **deltas = new long int*[neighbor2]; 
	//����ռ����˹Ȩ�ؿռ�
	double **GauS = new double*[neighbor2]; 
	int ii, jj;
	double temp;
	for ( ii = 0; ii <neighbor2; ii++ )
	{
		deltas[ii] = new long int[neighbor2];
		GauS[ii] = new double[neighbor2];
	}

	//���������ڵĿռ���ĸ�˹Ȩ��
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

	//����һЩ����
	int x, y;
	long int tempxy;
	int dgray, dL, da, db;
	double GauR;
	double weight,Wb, Ib, ILb, Iab, Ibb;
	int iMin, iMax, jMin, jMax;
	int indexi, indexj;
	int indexiMin, indexiMax, indexjMin, indexjMax;

	if ( 1 == src->nChannels )		//�������ͼ��Ϊ�ڰ�ͼ��
	{
		for ( x = 0;x < src->height; x++ )
		{
			for ( y = 0; y < src->width; y++ )
			{
				//��ʼ��
				weight = 0;
				Wb = 0, Ib = 0;
				tempxy = x * src->widthStep + y * src->nChannels;

				//�������Ȥ����߽�
				iMin = max( x - half_neigh, 0 );
				iMax = min( x + half_neigh, src->height - 1 );
				jMin = max( y - half_neigh, 0 );
				jMax = min( y + half_neigh, src->width - 1 );

				indexiMin = iMin - x + half_neigh;
				indexiMax = iMax - x + half_neigh;
				indexjMin = jMin - y + half_neigh;
				indexjMax = jMax - y + half_neigh;

				//ɨ������
				for ( indexi = indexiMin; indexi <= indexiMax; indexi++ )
				{
					for ( indexj = indexjMin; indexj <= indexjMax; indexj++ )
					{
						dgray = ( uchar )src->imageData[tempxy] - 
							( uchar )src->imageData[tempxy + deltas[indexi][indexj]];
						temp = dgray * dgray / ( 2 * sigma_r * sigma_r );
						temp *= -1;
						GauR = exp( temp );			//�õ�������Ȩ��
						weight = GauS[indexi][indexj] * GauR;	//����Ȩ��
						Wb += weight;				//����Ȩ��֮��
						Ib += weight * 
							( uchar )src->imageData[tempxy + deltas[indexi][indexj]]; 
					}
				}
				dst->imageData[tempxy] = ( uchar )( Ib / Wb );	//��һ����ȡ��
			}
		}
	}
	else
	{
		cvCvtColor( src, src, CV_RGB2Lab );		//ת��ΪLab��ɫ�ռ�ͼ��
		sigma_r *= 1.732;						//����sigma_r
		for ( x = 0;x < src->height; x++ )
		{
			for ( y = 0; y < src->width; y++ )
			{
				//��ʼ��
				weight = 0;
				Wb = 0, ILb = 0, Iab = 0, Ibb = 0;
				tempxy = x * src->widthStep + y * src->nChannels;

				//�������Ȥ����߽�
				iMin = max( x - half_neigh, 0 );
				iMax = min( x + half_neigh, src->height - 1 );
				jMin = max( y - half_neigh, 0 );
				jMax = min( y + half_neigh, src->width - 1 );

				indexiMin = iMin - x + half_neigh;
				indexiMax = iMax - x + half_neigh;
				indexjMin = jMin - y + half_neigh;
				indexjMax = jMax - y + half_neigh;

				//ɨ������
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
						GauR = exp( temp );			//�õ�������Ȩ��
						weight = GauS[indexi][indexj] * GauR;	//����Ȩ��
						Wb += weight;				//����Ȩ��֮��
						ILb += weight * ( uchar )src->imageData[tempxy + deltas[indexi][indexj] + 0]; 
						Iab += weight * ( uchar )src->imageData[tempxy + deltas[indexi][indexj] + 1]; 
						Ibb += weight * ( uchar )src->imageData[tempxy + deltas[indexi][indexj] + 2]; 
					}
				}
				dst->imageData[tempxy + 0] = ( uchar )( ILb / Wb );	//��һ����ȡ��
				dst->imageData[tempxy + 1] = ( uchar )( Iab / Wb );	//��һ����ȡ��
				dst->imageData[tempxy + 2] = ( uchar )( Ibb / Wb );	//��һ����ȡ��
			}
		}
		cvCvtColor( src, src ,CV_Lab2RGB );			//ת��ΪRGB��ɫ�ռ�ͼ��
		cvCvtColor( dst, dst, CV_Lab2RGB );			//ת��ΪRGB��ɫ�ռ�ͼ��

	}

	//�ͷſռ�
	for ( ii = 0; ii < neighbor; ii++ )
	{
		delete[] deltas[ii];
		delete[] GauS[ii];
	}
	delete[] deltas;
	delete[] GauS;

	return dst;
}

