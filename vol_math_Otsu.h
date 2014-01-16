#ifndef VOL_MATH_OTSU_H
#define VOL_MATH_OTSU_H
#include <vector>
#include <queue>
#include "vol_math_RawImage.h"
#include "vol_math_Raw3D_Independt.h"
#include "vol_math_filter_Interface.h"
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
	vector<int>arrays;//������ֵ

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
    int Otsu(Raw2D image_2D,int Min_pix,int Max_pix);//�򵥾ֲ�ͼ����ֵOTSU�㷨 
    int Otsu(Raw2D image_2D);//�Ż���������ֵOTSU�㷨 

	// ���涨�����ֵOTSU�㷨
	void Otsu_MultiVal(Raw2D image_2D,int t_number);//����ֵOTSU�㷨һ
	void Otsu_MultiVal(Raw2D image_2D);//����ֵOTSU�㷨��
	void SaveImage(Raw2D image_2D);//�����ά����
//	Points** output(Raw2D image_2D);

	//��ά���ݴ���
    int Otsu(Raw image,int Min_pix,int Max_pix);//�򵥾ֲ�ͼ����ֵOTSU�㷨 
    int Otsu(Raw image);//�Ż���������ֵOTSU�㷨 

	/*  ���涨�����ֵOTSU�㷨*/
	void Otsu_MultiVal(Raw image,int t_number);//����ֵOTSU�㷨һ
	void Otsu_MultiVal(Raw image);//����ֵOTSU�㷨��
	void SaveImage(Raw image);//������ά����
//	Points** output(Raw image);


	//��������
	~OTSU(){
	  arrays.~vector<int>(); 
	}
};
#endif