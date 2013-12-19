#include <stdio.h>
#include <iostream>
#include <complex>
#include <bitset>
#include <fstream>
#include <math.h>
#include "FourierFilter2.h"
#include "FourierFilter3.h"
#define pi 3.1415926535
#define DELTA 5.0
using std::iostream;
using std::bitset;
using std::complex;
using namespace std;

struct dimTuple
{
	int width;
	int height;
	int depth;
};
FourierFilter2::FourierFilter2(Raw2D & raw2)
{
	x = raw2.getXsize();
	y = raw2.getYsize();
	int length = x*y;
	buf = new float[length];
	for (int i = 0; i < length; i++)
	{
		buf[i] = (float)raw2.getXY(i)/255.0;
	}
}

FourierFilter2::~FourierFilter2()
{
	delete buf;
}

FourierFilter3::FourierFilter3(Raw & raw)
{
	x = raw.getXsize();
	y = raw.getYsize();
	z = raw.getZsize();
	int length = x*y*z;
	buf = new float[length];
	for (int i = 0; i < length; i++)
	{
		buf[i] = (float)raw.getXYZ(i)/255.0;
	}
}

FourierFilter3::~FourierFilter3()
{
	delete buf;
}

/*******为自底向上的迭代重排序列计算位置************/
int rev(int k,int n)
{
	 bitset<32> tmp(k);
	 bitset<32> dist;
	 for(int m = 0;m<n;m++)
	 {
		if(tmp.test(m))
		{
			dist.set(n-1-m);
		}
	 }
	 int revk = (int)dist.to_ulong();
	 return revk;
}

complex<float>* FFT(const complex<float> *srcimg,int n)
{
	float flag = log10((float)n)/log10(2.0);
	int N = n;
	if(flag - (int)flag != 0){   //判断是否为2的指数次
		cout <<"the length of srcimg is wrong"<< endl;
		/*填充成2的指数项*/
		cout <<"need padding"<<endl;
		N = pow(2,(float)((int)flag+1));
		flag = log10((float)N)/log10(2.0);
	}
	/*改变成奇偶顺序*/
	complex<float> *arr= new complex<float>[N];
	int sub;
	for(int k =0;k<N;k++)
	{
		sub =rev(k,(int)flag); 
		if(sub <= n-1){
			arr[k] = *(srcimg + sub);
		}else{
			complex<float> t = complex<float>(0,0);
			arr[k] = t;
		}
	}
 	for(int s =1;s <= flag;s++)
	{
		int m = pow(2,(float)s);
		complex<float> wm = complex<float>(cos(2*pi/m),sin(2*pi/m));//wm1：从W21开始,周期变换
		for(int p = 0;p<N;p+=m)
		{
			complex<float> w(1,0);
			for(int j = 0;j<=m/2-1;j++)
			{
				complex<float> t = w * arr[p+j+m/2];
				complex<float> u = arr[p+j];
				arr[p+j] = u+t;
				arr[p+j+m/2] = u-t;   
				w = w*wm;
			}
		}
	}
//	cout<<"visit array times: "<<countimes<<endl;
	return arr;
}
//重载形式
complex<float>* FFT(const complex<float> *srcimg,int n,int *v)
{
	int countimes = 0;
	float flag = log10((float)n)/log10(2.0);
	int N = n;
	if(flag - (int)flag != 0){   //判断是否为2的指数次
		cout <<"the length of srcimg is wrong"<< endl;
		/*填充成2的指数项*/
		cout <<"need padding"<<endl;
		N = pow(2,(float)((int)flag+1));
		flag = log10((float)N)/log10(2.0);
	}
	/*改变成奇偶顺序*/
	complex<float> *arr= new complex<float>[N];
	int sub;
	for(int k =0;k<N;k++)
	{
		sub =rev(k,(int)flag); 
		if(sub <= n-1){
			arr[k] = *(srcimg + sub);
		}else{
			complex<float> t = complex<float>(0,0);
			arr[k] = t;
		}
	}
 	for(int s =1;s <= flag;s++)
	{
		int m = pow(2,(float)s);
		complex<float> wm = complex<float>(cos(2*pi/m),sin(2*pi/m));//wm1：从W21开始,周期变换
		for(int p = 0;p<N;p+=m)
		{
			complex<float> w(1,0);
			for(int j = 0;j<=m/2-1;j++)
			{
				complex<float> t = w * arr[p+j+m/2];
				complex<float> u = arr[p+j];
				arr[p+j] = u+t;
				arr[p+j+m/2] = u-t;   
				w = w*wm;
				countimes++;
			}
		}
	}
//	cout<<"visit array times: "<<countimes<<endl;
	*v+=countimes;
	return arr;
}
/***********一维快速傅立叶变换********************
*srcimg : 原始一维序列                          *
*n     ：一维序列的长度
*************************************************/
complex<float>* FFT(const float *srcimg,int n)
{
	float flag = log10((float)n)/log10(2.0);
	int N = n;
	if(flag - (int)flag != 0){   //判断是否为2的指数次
		N = pow(2,(float)((int)flag+1));
		flag = log10((float)N)/log10(2.0);
	}
	/*改变成奇偶顺序*/
	complex<float> *arr= new complex<float>[N];
	int sub;
	for(int k =0;k<N;k++)
	{
		sub =rev(k,(int)flag); 
		if(sub <= n-1){
			arr[k] = complex<float>(*(srcimg + sub),0);
		}else{
			complex<float> t = complex<float>(0,0);
			arr[k] = t;
		}
	}
	/*基于迭代的蝶形快速傅立叶变换，自底向上*/
 	for(int s =1;s <= flag;s++)
	{
		int m = pow(2,(float)s);
		complex<float> wm = complex<float>(cos(2*pi/m),sin(2*pi/m));//wm1：从W21开始,周期变换
		for(int p = 0;p<N;p+=m)
		{
			complex<float> w(1,0);
			for(int j = 0;j<=m/2-1;j++)
			{
				complex<float> t = w * arr[p+j+m/2];
				complex<float> u = arr[p+j];
				arr[p+j] = u+t;
				arr[p+j+m/2] = u-t;
				w = w*wm;
			}
		}
	}
	return arr;
}

int countPadding(int n);

/*****************一维快速傅立叶逆变换********************/
/*fftimg:原始一维傅立叶序列
  n     : 序列长度
*******************************************************/
complex<float>* IFFT(const complex<float> *fftimg,int n)
{
	n = countPadding(n);
	float flag = log10((float)n)/log10(2.0);
	int N = n;
	if(flag - (int)flag != 0){   //判断是否为2的指数次
		cout <<"the length of srcimg is wrong"<< endl;
		/*填充成2的指数项*/
		cout <<"need padding"<<endl;
		N = pow(2,(float)((int)flag+1));
	    flag = log10((float)N)/log10(2.0);
	}
	/*改变成奇偶顺序*/
	complex<float> * spit = new complex<float>[N];
	int sub=0;
	for(int k =0;k<N;k++)
	{
		sub =rev(k,(int)flag); 
		if(sub < n){
			spit[k] = complex<float>(*(fftimg + sub));
		}else{
			spit[k] = complex<float>(0,0);
		}
	}

	for(int s =1;s <= flag;s++)
	{
		int m = pow(2,(float)s);
		complex<float> wm = complex<float>(cos(-2*pi/m),sin(-2*pi/m));//wm1：从W2(-1)开始
		for(int p = 0;p<N;p+=m)
		{
			complex<float> w(1,0);
			for(int j = 0;j<=m/2-1;j++)
			{
				complex<float> t = w * spit[p+j+m/2];
				complex<float> u = spit[p+j];
				spit[p+j] = u+t;
				spit[p+j+m/2] = u-t;
				w = w*wm;
			}
		}
	}

	for(size_t p =0;p<n;p++)
	{
		spit[p] = spit[p]/complex<float>(N,0);
	}
	return spit;
}
  
/*******使用共轭的快速傅立叶逆变换**************/
/*
complex<float>* IFFT2(const complex<float> *fftimg,int n)
{
	n = countPadding(n);
	complex<float> *gfftimg = new complex<float>[n];
	for(size_t i = 0;i<n;i++){
		gfftimg[i] = complex<float>(fftimg[i].real(),-fftimg[i].imag());
	}
	complex<float> *ifft = FFT(gfftimg,n); 
	for(size_t j = 0;j<n;j++)
	{
		ifft[j] = ifft[j]/complex<float>(n,0);
	}
	delete gfftimg;
	return ifft;
}
*/
/*************二维快速傅立叶变换**************************
*srcimg: 用一维表示的二维原始序列
*width ：宽度
*height：高度
********************************************************/
complex<float>* twoDFFT(const float *srcimg, int width, int height)
{
	int w = countPadding(width);
	int h = countPadding(height);
	int pixes = w * h;
	int vistimes = 0;
	complex<float> *hdirection = new complex<float>[w];
	complex<float> *vdirection = new complex<float>[h];
	complex<float> *fourier = new complex<float>[pixes];
	/*二维水平方向*/
	for(size_t i = 0;i<h;i++){
		for(size_t j = 0;j<w;j++){
			if(i>=height || j >=width){
				hdirection[j] = complex<float>(0,0);
			}else{
				hdirection[j] = complex<float>(srcimg[i*width + j],0);
			}
	
		}
	
		complex<float> *hfourier = FFT(hdirection,w,&vistimes);
		for(size_t m = 0;m<w;m++){
			fourier[i*w+m] = hfourier[m];
		}
		delete hfourier;
	}
	/*二维垂直方向*/
	for(size_t ii = 0;ii<w;ii++){
		for(size_t jj = 0;jj<h;jj++){
			vdirection[jj] = fourier[jj*w + ii];
		}
		complex<float> *vfourier = FFT(vdirection,h,&vistimes);
		for(size_t mm = 0;mm < h;mm++){
			fourier[mm*w +ii] = vfourier[mm];
		}
		delete vfourier;
	}
	delete hdirection;
	delete vdirection;
	return fourier;

}
/**************二维快速傅立叶逆变换*************************
*fourier : 一维表示的二维傅立叶变换序列                     *
*width   :宽                                             *
*height  :高                                             *
***********************************************************/
complex<float>* twoDIFFT(const complex<float> *fourier,int width,int height)
{
	width = countPadding(width);
	height = countPadding(height);
	int fpoints = width * height;
	complex<float> *hdirection = new complex<float>[width];
	complex<float> *vdirection = new complex<float>[height];
	complex<float> *ifourier = new complex<float>[fpoints];

	for(size_t ii = 0;ii<height;ii++)
	{
		for(size_t jj = 0;jj<width;jj++){
			hdirection[jj] = fourier[ii*width+jj];
		}
		complex<float> *hifour = IFFT(hdirection,width);//临时变量
		for(size_t mm = 0;mm<width;mm++){
			ifourier[ii*width+mm] = hifour[mm];
		}
		delete hifour;
	}
	for(size_t i = 0;i<width;i++){
		for(size_t j = 0;j<height;j++){
			vdirection[j] = ifourier[j*width+i];
		}
		complex<float> *vifour = IFFT(vdirection,height);
		for(size_t m = 0;m<height;m++){
			ifourier[m*width+i] = vifour[m];
		}
		delete vifour;
	}	
	delete hdirection;
	delete vdirection;
	return ifourier;
}

complex<float>* threeDFFT(const float* src,int width,int height,int depth){
	int p_width = countPadding(width);
	int p_height = countPadding(height);
	int p_depth = countPadding(depth);
	float *pad_plane = new float[p_width*p_height];
	for (size_t h = 0; h < height; h++)
	{
		for (size_t w = 0; w < width; w++)
		{
			pad_plane[h*width + w] = 0;
		}
	}
	complex<float> *threed_fourier = new complex<float>[p_width*p_height*p_depth];
	complex<float>* oneplane;
	for(int i = 0;i<p_depth;i++){
		if(i<depth){
			oneplane = twoDFFT((src+(width*height)*i),width,height);
			
		}else{
			oneplane = 	twoDFFT(pad_plane,p_width,p_height);
		}
		for(size_t x = 0;x<p_height;x++){
			for(size_t y = 0;y<p_width;y++){
				threed_fourier[i*p_width*p_height+x*p_width+y] = oneplane[x*p_width+y];
			}     
		}
	}

	delete oneplane;
	complex<float> *z_fourier;
	complex<float> *z_direct_extra = new complex<float>[p_depth];
	for(size_t x = 0;x<p_height;x++){
		for(size_t y = 0;y<p_width;y++){
			for(size_t z = 0;z<p_depth;z++){
				z_direct_extra[z] = threed_fourier[z*p_width*p_height+x*p_width+y];
			}
			z_fourier = FFT(z_direct_extra,p_depth);
			for(size_t z2 = 0;z2<p_depth;z2++){
				threed_fourier[z2*p_width*p_height+x*p_width+y] = z_fourier[z2];
			}
		}
	}

	return threed_fourier;
}

complex<float>* threeDIFFT(const complex<float>* src,int p_width,int p_height,int p_depth){
	complex<float> *threed_fourier = new complex<float>[p_width*p_height*p_depth];
	complex<float>* oneplane;
	for(int i = 0;i<p_depth;i++){
		oneplane = twoDIFFT((src+(p_width*p_height)*i),p_width,p_height);
	
		for(size_t x = 0;x<p_height;x++){
			for(size_t y = 0;y<p_width;y++){
				threed_fourier[i*p_width*p_height+x*p_width+y] = oneplane[x*p_width+y];
			}     
		}
	}
	delete oneplane;
	complex<float> *z_fourier;
	complex<float> *z_direct_extra = new complex<float>[p_depth];
	for(size_t x = 0;x<p_height;x++){
		for(size_t y = 0;y<p_width;y++){
			for(size_t z = 0;z<p_depth;z++){
				z_direct_extra[z] = threed_fourier[z*p_width*p_height+x*p_width+y];
			}
			z_fourier = IFFT(z_direct_extra,p_depth);
			for(size_t z2 = 0;z2<p_depth;z2++){
				threed_fourier[z2*p_width*p_height+x*p_width+y] = z_fourier[z2];
			}
		}
	}
	float total = p_width*p_height*p_depth;
	return threed_fourier;
}
/******************计算填充数********************************************
*蝶形傅立叶变换算法只计算2的指数次的离散序列，对于非2的指数次的序列，使用零填充*
***********************************************************************/
inline int countPadding(int n)
{
	float lg = log10((float)n)/log10(2.0);
	if((lg - (int)lg) == 0){
		return n;
	}
	int N = pow(2.0,((int)lg+1));
	return N;
}

/*****高斯低通滤波函数*************************
*src：   输入频谱
*width： 输入频谱宽度
*height：输入频谱高度
*D：     高斯函数方差，即滤波阈值
*只对于移位居中后的傅立叶频谱进行高斯低通滤波
*********************************************/
void FourierFilter2::guass_low_pass2(complex<float> *src,int width,int height,float D)
{
	//find centre point
	int orgx = floor(width/2.0);
	int orgy = floor(height/2.0);
	float distence = 0;
	for(size_t i = 0;i<height;i++)
	{
		for(size_t j = 0;j<width;j++)
		{
			distence = sqrt(pow(abs((int)(i-orgy)),2.0)+pow(abs((int)(j-orgx)),2.0));
			src[i*width+j] = src[i*width+j] * (float)exp(-distence/(2*pow(D,2.0)));
			
		}	
	}
}
void FourierFilter3::low_Pass3(complex<float>*src,int width,int height,int depth,float thresold)
{
	//wdith height,height都被填充成2的幂；
	int orgx = width/2;
	int orgy = height/2;
	int orgz = depth/2;
	float distance = 0;
	for(int i = 0;i<depth;i++)
	{
		for(int j = 0;j<height;j++)
		{
			for(int k = 0;k<width;i++)
			{
				distance = sqrt(pow(abs((int)(i-orgz)),2.0)+pow(abs((int)(j-orgy)),2.0)+pow(abs((int)(k-orgx)),2.0));
				if(distance > thresold){
					src[i*width*height+j*width+k] = 0;
				}
			}
		}
	}
}
/************复数傅立叶频谱数组转换成256级灰度数组*****************/
void toGray(complex<float> *twodfourier,int pwid,int phei,char* pixval)
{
		float *vals = new float[pwid*phei];
		float max = 0;
		float min = 255;
		for(size_t p = 0;p<pwid*phei;p++){
			vals[p]=log(1+sqrt(pow(twodfourier[p].real(),2.0)+pow(twodfourier[p].imag(),2.0)));//对数级的幅度铺
			if(vals[p] > max){
				max = vals[p];
			}
			if(vals[p] < min){
				min = vals[p];
			}
		}
		cout<<min<< " "<<max<<endl;
		cout<<pwid<<" "<<phei<<endl;
		for(size_t s = 0;s<pwid*phei;s++){
			pixval[s] = (char)((vals[s]-min)/(max-min)*255);
		}
		delete vals;
}
/*
*srcInt:接收unsigned char类型数组，转换为float后做三维傅立叶变换；
*width,height,depth:数据的x,y,z长度；
*dist:指向3DFFT变换后的数组；
*返回值：FFT变换后的数据维度（填充模式)；
*/

struct dimTuple threeDFourierT(const unsigned char *srcInt,complex<float>* dist,int width,int height,int depth)
{
	cout << "begin char to float" << endl;
	long length = width * height * depth;
	float * tranfSrc = new float[length];
	for(size_t i = 0;i<depth;i++)
	{
		for(size_t j = 0;j<height;j++)
		{
			for(size_t k = 0;k<width;k++)
			{
				tranfSrc[i*height*width+j*height+width] = srcInt[i*height*width+j*height+width]*pow(-1,(float)(i+j+k))/255.0;
			}
		
		}
	}
	cout << "after char to float" << endl;
	dist = threeDFFT(tranfSrc,width,height,depth); 
	cout << "after FFT" << endl;
	struct dimTuple dim;
	dim.width = countPadding(width);
	dim.height = countPadding(height);
	dim.depth = countPadding(depth);
	delete tranfSrc;
	return dim;
}

void threeDInverseFFT(const complex<float> *invSrc,complex<float>* idist,int width,int height,int depth)
{
	idist= threeDIFFT(invSrc,width,height,depth);
}

void FourierFilter2::lowpass_trans(double thresold)
{
	int p_width = countPadding(x);
	int p_height = countPadding(y);
	complex<float> * tmp = twoDFFT(buf, x, y);
	guass_low_pass2(tmp, p_width, p_height, thresold);
	tmp = twoDIFFT(tmp, p_width, p_height);

	for (int i = 0; i < y; i++){
		for (int j = 0; j < x; j++)
		{
			buf[i*x + j] = tmp[i*x + j].real*255.0;
		}
	}
	Raw2D fr(p_width, p_height, buf);
	fraw2d = fr;
}

void FourierFilter3::lowpass_trans(double thresold)
{
	int p_width = countPadding(x);
	int p_height = countPadding(y);
	int p_depth = countPadding(z);
	complex<float> * tmp = threeDFFT(buf, x, y, z);
	low_Pass3(tmp, p_width, p_height, p_depth, thresold);
	tmp = threeDIFFT(tmp, p_width, p_height, p_depth);
	for (int i = 0; i < z; i++){
		for (int j = 0; j < y;j++)
		{
			for (int k = 0; k < x;k++)
			{
				buf[i*y*z + j*y + k] = tmp[i*y*z + j*y + k].real*255.0;
			}
		}
	}
	Raw fr(x, y, z, buf);
	fraw = fr;
}

/******************opencv 读取图像与效果***********************/
/*
int main(int argc,char **argv)
{
	IplImage *img;
	if((img = cvLoadImage("F:\\Fig0222(a)(face).tif",0))!=0){
		int dim = img->imageSize;
		//从图像矩阵复制出单维数组，并做频谱居中操作，对应改写接口，返回单维数组；
		float * src = new float[dim];
		size_t j =0;
		for(int y =0;y<img->height;y++)
		{
			uchar * ptr = (uchar*)(img->imageData + y * img->widthStep);
			for(int x =0;x<img->width;x++){
			    /***输入函数乘以（-1）的（x+y）次方，频谱将居中*//*
				src[j] = (float)ptr[x]*pow(-1,(float)(x+y))/256;
				j++;
			}
		}
		int w = img->width;
		int h = img->height;
		int pwid = countPadding(w);
		int phei = countPadding(h);

		complex<float> *twodfourier = twoDFFT(src,w,h);
		char * pixval = new char[pwid*phei];
		CvMat freq;
		toGray(twodfourier,pwid,phei,pixval);//复数频谱转256灰度
		cvInitMatHeader(&freq,pwid,phei,CV_8UC1,pixval);
		IplImage *ipl = cvCreateImage(cvGetSize(&freq),8,1);
		cvGetImage(&freq,ipl);//获取频谱图像

		guass(twodfourier,pwid,phei,DELTA);//方差DELTA的高斯平滑（高斯低通滤波）
		CvMat gaufre;
		char *pixvals = new char[pwid*phei];
		toGray(twodfourier,pwid,phei,pixvals);
		cvInitMatHeader(&gaufre,pwid,phei,CV_8UC1,pixvals);
		IplImage *gausf = cvCreateImage(cvGetSize(&gaufre),8,1);
		cvGetImage(&gaufre,gausf);//高斯平滑后的频谱图像

		complex<float> *twodifourier = twoDIFFT(twodfourier,w,h);
		float ipix = 0;
		size_t po = 0;
		for(int y =0;y<img->height;y++)
		{
			uchar * ptr = (uchar*)(img->imageData + y * img->widthStep);
			for(int x =0;x<img->width;x++){
				ipix = twodifourier[po*pwid+x].real();
				ptr[x] = (uchar)(ipix * 256)*pow(-1,(float)(x+y));
			}
			po++;
		}
		cvNamedWindow("frequence_domain",CV_WINDOW_AUTOSIZE);
		cvShowImage("frequence_domain",ipl);
		cvNamedWindow("gauss_fre",CV_WINDOW_AUTOSIZE);
		cvShowImage("gauss_fre",gausf);
		cvNamedWindow("fourier_t",CV_WINDOW_AUTOSIZE);
		cvShowImage("fourier_t",img);
		cvWaitKey(0);
		cvReleaseImage(&ipl);
		cvReleaseImage(&gausf);
		cvReleaseImage(&img);
		cvDestroyWindow("fourier_t");
		cvDestroyWindow("frequence_domain");
		cvDestroyWindow("gauss_fre");
		delete pixval;
		delete pixvals;
		delete src;
		delete twodfourier;
		delete twodifourier;
		return 1;
	}
	return 0;
}*/

/*测试一维填充形式傅立叶变换*/
/*
int main()
{
	ifstream infile("D:\\ftest.txt");
	if(!infile.is_open())
	{
		cout<<"file read error"<<endl;
		return -1;
	}
	istream_iterator<float> df(infile);
	istream_iterator<float> eof;
	int N = *df++;
	float *src = new float[N];
	size_t i = 0;
	while(df != eof)
	{
		src[i] = *df++;
		i++;
	}
//	complex<float> *fourier = new complex<float>[N];
//	complex<float> *spit = new complex<float>[N];
/*	complex<float> *fourier = FFT(src,N);
	cout <<"--------fourier sieres----------"<<endl;
	for(size_t x = 0;x <16;x++)
	{
		cout << (*(fourier +x)).real()<<"  "<<(*(fourier+x)).imag() << endl;
	}
	cout <<"----------------inverse fourier -----------------------"<<endl;
	complex<float> *spit = IFFT(fourier,N);
	for(size_t y = 0;y <16;y++)
	{
		cout << spit[y].real()<<"  "<<spit[y].imag() << endl;
	}
	delete src;
//	delete fourier;
//	delete spit;
	return 1;
}*/
/*测试二维零填充傅立叶正负变换*/


/*
int main()
{
	ifstream infile("E:\\largeTest.txt");
	if(!infile.is_open())
	{
		cout<<"file read error"<<endl;
		return -1;
	}
	istream_iterator<float> df(infile);
	istream_iterator<float> eof;
//	int N = *df++;
	float *src = new float[1024*1024];                                     
	size_t i = 0;
	while(df != eof)
	{
		src[i] = *df++;
		i++;
	}
	clock_t start,finish;
	start = clock();
	complex<float> *fourier = twoDFFT(src,1024,1024);
	finish = clock();
	float duration = (finish - start);
	cout<<"fourier transform time:"<< duration<<endl;
	cout <<"--------fourier sieres sample 64----------"<<endl;
/*	for(size_t x = 0;x <64;x++)
	{
		cout << fourier[x*64+x].real()<<"  "<< fourier[x*64+x].imag() << endl;
	}*//*
	cout <<"----------------inverse fourier sample 64 -----------------------"<<endl;
	start = clock();
	complex<float> *spit = twoDIFFT(fourier,1024,1024);
	finish = clock();
	duration = (finish - start);
	cout<<"inverse fourier transform time:"<< duration<<endl;
/*	for(size_t y = 0;y <64;y++)
	{
		cout << spit[y*64+y].real()<<"  "<<spit[y*64+y].imag() << endl;
	}*/
//	delete src;
//	delete fourier;
//	delete spit;
/*	delete src;
	return 1;
}*/
/*三维填充傅立叶变换测试*/
//int main()
//{
//	ifstream infile("E:\\test2.txt");
//	if(!infile.is_open())
//	{
//		cout<<"file read error"<<endl;
//		return -1;
//	}
//	istream_iterator<float> df(infile);
//	istream_iterator<float> eof;
////	int N = *df++;
//	int n = 16;
//	float *src = new float[n*n*15];                                     
//	size_t i = 0;
//	while(df != eof)
//	{
//		src[i] = *df++;
//		i++;
//	}
//	clock_t start,finish;
//	start = clock();
//	complex<float> *fourier = threeDFFT(src,n,n,15);
//	finish = clock();
//	float duration = (finish - start);
//	cout<<"fourier transform time:"<< duration<<endl;
//	cout <<"---------------fourier sieres sample 64-------------------------"<<endl;
//	//for(size_t x = 0;x <16*16*16;x++)
//	//{
//	//	cout << fourier[x].real()<<"  "<< fourier[x].imag() << endl;
//	//}
//	cout <<"----------------inverse fourier sample 64 -----------------------"<<endl;
//	start = clock();
//	complex<float> *spit = threeDIFFT(fourier,n,n,n);
//	finish = clock();
//	duration = (finish - start);
//	cout<<"inverse fourier transform time:"<< duration<<endl;
//	for(size_t y = 0;y <16*16*15;y++)
//	{
//		cout << spit[y].real()<<"  "<<spit[y].imag() << endl;
//	}
//	delete src;
//	delete fourier;
//	delete spit;
////	delete src;
//	return 1;
//}

int main(void)
{
	ifstream infile("E:\\graytext.text");
	int width = 281; int height = 481; int depth = 2501;
	int xy = width*height;
	const long length = width*height*depth;
	unsigned char *srcdata = new unsigned char[length];
	unsigned char gray;
	for (size_t i = 0; i < length; i++)
	{
		gray = infile.get();
		srcdata[i] = gray;
	}
	cout << "read end";
	complex<float> *distT = NULL;
	struct dimTuple dd;
	dd = threeDFourierT(srcdata, distT, width, height, depth);
	delete srcdata;
	delete distT;
	return 0;
}
/*
int main(int argc,char ** argv)
{
	IplImage *img;
	img = cvLoadImage("F:\\Fig0222(a)(face).tif",0);
	int dim = img->imageSize;
	//从图像矩阵复制出单维数组；
	float * src = new float[dim];
	size_t j =0;
	for(int y =0;y<img->height;y++)
	{
		uchar * ptr = (uchar*)(img->imageData + y * img->widthStep);
		for(int x =0;x<img->width;x++){
			src[j] = (float)ptr[x]/256;
			j++;
		}
	}
	//一维数组做傅立叶变换
	complex<float>* fourier = FFT(src,dim);
	//计算填充后傅氏数组大小
	float lg = log10((float)dim)/log10(2.0);
	int n = pow(2,(float)((int)lg+1));
	//傅立叶逆变换
	complex<float> *ifourier = IFFT(fourier,n);
	float ipix = 0;
	size_t po = 0;
	//重填充图像
	for(int y =0;y<img->height;y++)
	{
		uchar * ptr = (uchar*)(img->imageData + y * img->widthStep);
		for(int x =0;x<img->width;x++){
			ipix = ifourier[po].real();
			ptr[x] = (uchar)(ipix * 256);
			po++;
		}
	}
	cvNamedWindow("fourier_t",CV_WINDOW_AUTOSIZE);
	cvShowImage("fourier_t",img);
	cvWaitKey(0);
	cvReleaseImage(&img);
	cvDestroyWindow("fourier_t");

	return 1;
/*	ifstream infile("F:\\Fig0222(a)(face).tif",ios::binary);
	vector<float> collection;
	istream_iterator<byte> in_iter(infile);
	istream_iterator<byte> eof;
	float temp = 0;
	unsigned char c = 0;
	while(in_iter != eof)
	{
		c = *in_iter++;
		temp = (float)c/256;
		collection.push_back(temp);
	}
	int amount = collection.size();
	float * src = new float[amount];  
	size_t j = 0;
	for(vector<float>::iterator iter = collection.begin();iter != collection.end();++iter)
	{
		src[j] = *iter;
		j++;
	}*/
/*	delete src;
	delete fourier;
	delete ifourier;
} 
*/