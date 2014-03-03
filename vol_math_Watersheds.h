//watersheds.h
#ifndef VOL_MATH_WATERSHEDS_H
#define VOL_MATH_WATERSHEDS_H
#include"vol_math_Morph_Opera.h"
#include"vol_math_ImageProc.h"

void Zero(int *p,int number);//����
void Zero_2D(int **p,int row,int col);//����
void Zero_3D(int ***p,int row,int col,int height);//����

//two Dimensions
/*====================================================================
��������            Watershed
���ܣ�              �ñ��-��ˮ���㷨������ͼ����зָ�
�㷨ʵ�֣�          ��
�������˵����      OriginalImage   --����ͼ�񣨻Ҷ�ͼ��0~255��
                    SeedImage       --���ͼ�񣨶�ֵͼ��0-�Ǳ�ǣ�1-��ǣ�
                    LabelImage      --���ͼ��1-��һ���ָ�����2-�ڶ����ָ�����...��
                    row             --ͼ������
                    col             --ͼ������
����ֵ˵����        ��        
====================================================================*/

void Watersheds( Raw2D &OriginalImage, Raw2D &SeedImage, Raw2D &LabelImage,int *classnum);
void Watersheds( Raw2D &OriginalImage, Raw2D &LabelImage);
void Gradient( Raw2D &Image,Raw2D &dest);//��ά���ݶ� 

void WatershedsProcess(Raw2D &OriginalImage);
void WatershedIterface(Raw2D *src,Raw2D *ret);
void Watersheds( Raw2D *OriginalImage, Raw2D *SeedImage, Raw2D *LabelImage);
//void Watersheds(const Raw2D *OriginalImage, Raw2D *LabelImage);
void Gradient( Raw2D *Image,Raw2D *dest);//��ά���ݶ� 

void Watersheds(unsigned char **OriginalImage, char** SeedImage, int **LabelImage, int row, int col);
void Gradient(unsigned char **Image,unsigned char **GradientImage,int row,int col);//��ά���ݶ� 
void Watershed_2D_test(unsigned char **OriginalImage, char** SeedImage, int **LabelImage, int row, int col);

//three  Dimensions
/*====================================================================
��������            Watershed
���ܣ�              �ñ��-��ˮ���㷨������ͼ����зָ�
�㷨ʵ�֣�          ��
�������˵����      OriginalImage   --����ͼ�񣨻Ҷ�ͼ��0~255��
                    SeedImage       --���ͼ�񣨶�ֵͼ��0-�Ǳ�ǣ�1-��ǣ�
                    LabelImage      --���ͼ��1-��һ���ָ�����2-�ڶ����ָ�����...��
                    row             --ͼ������
                    col             --ͼ������
����ֵ˵����        ��        
====================================================================*/


//void Watersheds(const Raw &OriginalImage, Raw &SeedImage, Raw &LabelImage);
void Watersheds(const Raw &OriginalImage, Raw &LabelImage);
void Gradient(const Raw &Image,Raw &dest);//��ά���ݶ� 

//void Watersheds(const Raw *OriginalImage, Raw *SeedImage, Raw *LabelImage);
//void Watersheds(const Raw *OriginalImage, Raw *LabelImage);
//void Gradient(const Raw *Image,Raw *dest);//��ά���ݶ� 


#endif