#ifndef VOL_MATH_IMAGEPROC_H
#define VOL_MATH_IMAGEPROC_H
#include "vol_math_RawImage.h"
//���弸��ƽ���˲��ĳ���
#define CV_BLUR_NO_SCALE  0 //��ͨ��ƽ��
#define CV_GAUSSIAN 1 //��˹�˲�
#define CV_MEDIAN 2  //��ֵ�˲�
#define CV_BLUR  3  //��ÿ������param1��param2���� ��Ͳ����߶ȱ任 
#define CV_BILATERAL 4 //˫���˲�
struct POINT3D{
    int x;//
	int y;
	int z;
};
typedef struct POINT2D
{
	int  x;
	int  y;
} POINT2D;
//two Dimensions
//gray image smooth function
void Smooth(Raw2D &image,int type);
void Smooth(Raw2D *image,int type);
//Noise process gray image
void NoiseProcess(Raw2D &image);
//Extremum value
void MaxValue(Raw2D &image);//����ֵ����
void MinValue(Raw2D &image);//��Сֵ����

//three Dimensions
//gray image smooth function
void Smooth3D(Raw &image,int type);
void Smooth3D(Raw *image,int type);
//Noise process gray image
void NoiseProcess(Raw &image);
//Extremum value
void MaxValue(Raw &image);//����ֵ����
void MinValue(Raw &image);//��Сֵ����

#endif