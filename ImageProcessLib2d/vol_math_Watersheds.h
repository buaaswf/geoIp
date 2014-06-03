//watersheds.h
#ifndef VOL_MATH_WATERSHEDS_H
#define VOL_MATH_WATERSHEDS_H
#include"vol_math_Morph_Opera.h"
#include"vol_math_ImageProc.h"

void Zero(long long *p,long long number);//归零
void Zero(int *p,int number);//归零
void Zero_2D(int **p,int row,int col);//归零
void Zero_3D(int ***p,int row,int col,int height);//归零

//define one struct
struct WatershedsPara{
	int smoothsizes;
	int threshold;
	WatershedsPara(){}
	WatershedsPara(int sizes,int value){
		this->smoothsizes = sizes;
		this->threshold =value;
	}
};
//two Dimensions
/*====================================================================
函数名：            Watershed
功能：              用标记-分水岭算法对输入图像进行分割
算法实现：          无
输入参数说明：      OriginalImage   --输入图像（灰度图，0~255）
                    SeedImage       --标记图像（二值图，0-非标记，1-标记）
                    LabelImage      --输出图像（1-第一个分割区域，2-第二个分割区域，...）
                    row             --图像行数
                    col             --图像列数
返回值说明：        无        
====================================================================*/

void Watersheds( Raw2D &OriginalImage, Raw2D &SeedImage, Raw2D &LabelImage,int *classnum);
void Watersheds( Raw2D &OriginalImage, Raw2D &LabelImage);
void Gradient( Raw2D &Image,Raw2D &dest);//二维求梯度 

void WatershedsProcess(Raw2D &OriginalImage);
void WatershedIterface(Raw2D *src,Raw2D *ret);
void Watersheds( Raw2D *OriginalImage, Raw2D *SeedImage, Raw2D *LabelImage);
//void Watersheds(const Raw2D *OriginalImage, Raw2D *LabelImage);
void Gradient( Raw2D *Image,Raw2D *dest);//二维求梯度 

void Watersheds(unsigned char **OriginalImage, char** SeedImage, int **LabelImage, int row, int col);
void Gradient(unsigned char **Image,unsigned char **GradientImage,int row,int col);//二维求梯度 
void Watershed_2D_test(unsigned char **OriginalImage, char** SeedImage, int **LabelImage, int row, int col);

//three  Dimensions
/*====================================================================
函数名：            Watershed
功能：              用标记-分水岭算法对输入图像进行分割
算法实现：          无
输入参数说明：      OriginalImage   --输入图像（灰度图，0~255）
                    SeedImage       --标记图像（二值图，0-非标记，1-标记）
                    LabelImage      --输出图像（1-第一个分割区域，2-第二个分割区域，...）
					classnum        --输出图像最大值的指针 
                    row             --图像行数
                    col             --图像列数
返回值说明：        无        
====================================================================*/
void Watersheds( Raw &OriginalImage, Raw &SeedImage, Raw &LabelImage,int *classnum,void (*progresschanged)(int,int,int,bool &));
void Watersheds( Raw &OriginalImage, Raw &LabelImage);
void Gradient( Raw &src,Raw &dest);//三维求梯度 
void WatershedsProcess(Raw &OriginalImage,WatershedsPara &para,void (*progresschanged)(int,int,int,bool &));//test watersheds

//void Watersheds(const Raw *OriginalImage, Raw *SeedImage, Raw *LabelImage);
//void Watersheds(const Raw *OriginalImage, Raw *LabelImage);
//void Gradient(const Raw *Image,Raw *dest);//三维求梯度 
//void WatershedsProcess(Raw *OriginalImage);//test watersheds
#endif