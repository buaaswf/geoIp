#ifndef FUNCTION_H
#define FUNCTION_H
#include <vector>
#include "vol_math_RawImage.h"
#include "vol_math_Raw3D_Independt.h"
#define SINGLE_OTSU 0 //����ֵ�ָ�
#define MULTIPLE_OTSU 1 //����ֵ�ָ�
struct Point_3{
     int Min_val;//��Сֵ
	 int Max_val;//���ֵ
	 double s;   //����
};
void order(int *p,int n);//�����С���򣺴�С����
 
int Max_number(struct Point_3 *p,int n);//Ѱ����󷽲�


//����һ����OTSU
class OTSU{
public:
	Raw2D image_2D;//��ά����
	Raw image;//��ά����
	vector<int>array;//������ֵ
	int t_number;//ָ����ֵ�ĸ���
	//���幹�캯��
	OTSU(){}//�յĹ��캯��
	//��ά���캯��
	OTSU(Raw2D newImage);
	OTSU(Raw2D newImage,int newT_number);
	//��ά���캯��
	OTSU(Raw newImage);
	OTSU(Raw newImage,int newT_number);

	//��Ա��������ֵ
	//��ά��Ա����
	void setData(Raw2D newImage);
	void setData(Raw2D newImage,int newT_number);
    //��ά��Ա����
	void setData(Raw newImage);
	void setData(Raw newImage,int newT_number);

	//��ά���ݴ���
	//int Otsu(Raw2D image_2D);//������ͼ����ֵOTSU�㷨 
 //   int Otsu1(Raw2D image_2D,int Min_pix,int Max_pix);//�򵥾ֲ�ͼ����ֵOTSU�㷨 
 //   int Otsu_Step(Raw2D image_2D);//�Ż���������ֵOTSU�㷨 
	//int Otsu_Recursion(Raw2D image_2D);//�ݹ���ü򵥵���ֵOTSU�㷨
	////  ���涨���άOTSU�㷨
 //   int Otsu_2D(Raw2D image_2D);//��ά����ֵOTSU�㷨 
 //   int Otsu_2D_Step(Raw2D image_2D);//�Ľ��Ķ�ά����ֵOTSU�㷨һ

	//// ���涨�����ֵOTSU�㷨
	//void Otsu_MultiVal(Raw2D image_2D,int t_number);//����ֵOTSU�㷨һ
	//void Otsu_MultiVal(Raw2D image_2D);//����ֵOTSU�㷨��
	//void SaveImage(Raw2D image_2D);//�����ά����
	//��ά�㷨����
	//void Otsu(Raw2D image_2D,int label,int number);//label: 0/1 ��ʾ��/����ֵ����  number:��ʾָ������ĸ���

	//��ά���ݴ���
	int Otsu(Raw image);//������ͼ����ֵOTSU�㷨 
    int Otsu1(Raw image,int Min_pix,int Max_pix);//�򵥾ֲ�ͼ����ֵOTSU�㷨 
    int Otsu_Step(Raw image);//�Ż���������ֵOTSU�㷨 
	int Otsu_Recursion(Raw image);//�ݹ���ü򵥵���ֵOTSU�㷨
	/*  ���涨���άOTSU�㷨*/
    int Otsu_2D(Raw image);//��ά����ֵOTSU�㷨 
    int Otsu_2D_Step(Raw image);//�Ľ��Ķ�ά����ֵOTSU�㷨һ

	/*  ���涨�����ֵOTSU�㷨*/
	void Otsu_MultiVal(Raw image,int t_number);//����ֵOTSU�㷨һ
	void Otsu_MultiVal(Raw image);//����ֵOTSU�㷨��
	void SaveImage(Raw image);//������ά����
	//��ά�㷨����
	//void Otsu(Raw image,int label,int number);//label: 0/1 ��ʾ��/����ֵ����  number:��ʾָ������ĸ���

	//��������
	~OTSU(){
	   
	}
};
#endif