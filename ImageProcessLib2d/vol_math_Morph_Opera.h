//Morph_Opera.h
#ifndef VOL_MATH_MORPH_OPERA_H
#define VOL_MATH_MORPH_OPERA_H
#include <string>
#include "vol_math_RawImage.h"
using namespace std;

double Cpow(int m,int n);//
//定义一个二维矩阵
struct Mat{
    int row;//row number
	int col;//col number
	int *data;//data
	Mat(){}
	Mat(int r,int c){
		this->col=c;
		this->row=r;
	}
	Mat(Mat &mat){
		this->col=mat.col;
		this->row=mat.row;
		this->data=new int[mat.col*mat.row];
		memcpy(this->data,mat.data,sizeof(int)*mat.col*mat.row);
	}
};
//Release memory
void CReleaseMat(Mat &mat);
//void CReleaseMat(Mat *mat);
//mat compare
int CompareMat( Mat &mat1, Mat &mat2);
//Copy mat
Mat Copy(Mat &mat);
//construction mat
Mat strel(string strings,int n);//设定特定的形状
Mat strel(int row,int col,int value);//矩阵
Mat strel_B(string strings,int n);//特定的形状

//two Dimensions
//Gray image compare
int CompareImage( Raw2D &src1, Raw2D &src2);
//Gray image copy
void Copy( Raw2D &src,Raw2D &dest);
//Gray image reversion
void Reversion(Raw2D &image);
//erode
void Erode_Gray( Raw2D &mask,Raw2D &dest,int n);//gray image
//dilate
void Dilate_Gray( Raw2D &mask,Raw2D &dest,int n);//gray image
//Morphological erode
void Morph_Erode_Gray( Raw2D &mask,Raw2D &dest,int n);//gray image
//Morphological dilate
void Morph_Dilate_Gray( Raw2D &mask,Raw2D &dest,int n);//gray image
//Morphological reconstruction
//strings : dilate or erode
void Morph_reconstuct_Gray( Raw2D &mask,Raw2D &dest,string strings,int n);//gray image reconstruction


//three Dimensions
//Gray image compare
int CompareImage(Raw &src1, Raw &src2);
//Gray image copy
void Copy( Raw &src,Raw &dest);
//Gray image reversion
void Reversion(Raw &image);
//erode
void Erode_Gray( Raw &mask,Raw &dest,int n);//gray image
//dilate
void Dilate_Gray( Raw &mask,Raw &dest,int n);//gray image
//Morphological erode
void Morph_Erode_Gray( Raw &mask,Raw &dest,int n);//gray image
//Morphological dilate
void Morph_Dilate_Gray( Raw &mask,Raw &dest,int n);//gray image
//Morphological reconstruction
//strings : dilate or erode
void Morph_reconstuct_Gray( Raw &mask,Raw &dest,string strings,int n);//gray image reconstruction

#endif