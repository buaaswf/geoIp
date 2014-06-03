#ifndef VOL_MATH_IMAGEPROC_H
#define VOL_MATH_IMAGEPROC_H
#include "vol_math_RawImage.h"
//定义几个平滑滤波的常量
#define CV_BLUR_NO_SCALE  0 //普通的平滑
#define CV_GAUSSIAN 1 //高斯滤波
#define CV_MEDIAN 2  //中值滤波
#define CV_BLUR  3  //对每个象素param1×param2邻域 求和并做尺度变换 
#define CV_BILATERAL 4 //双向滤波
struct POINT3D{
    long long  x;//
	long long  y;
	long long z;
};
typedef struct POINT2D
{
	int  x;
	int  y;
} ;
//two Dimensions
//gray image smooth function
void Smooth(Raw2D &image,int type);
void Smooth(Raw2D *image,int type);
//Noise process gray image
void NoiseProcess(Raw2D &image);
//Extremum value
void MaxValue(Raw2D &image);//极大值函数
void MinValue(Raw2D &image);//极小值函数

//three Dimensions
//gray image smooth function
void Smooth3D_7(Raw &image,int type);
void Smooth3D_27(Raw &image,int type);
void Smooth3D(Raw *image,int type);
//Noise process gray image
void NoiseProcess(Raw &image);
//Extremum value
void MaxValue(Raw &image,int smoothsize,int threshold);//极大值函数
void MinValue(Raw &image,int smoothsize,int threshold);//极小值函数

#endif