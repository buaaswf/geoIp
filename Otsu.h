#ifndef FUNCTION_H
#define FUNCTION_H

#define X_VALUE 281  //z�����ֵ
#define Y_VALUE 481  //y�����ֵ
#define Z_VALUE 2501 //z�����ֵ
#define SIZE X_VALUE*Y_VALUE*Z_VALUE  //һά����Ĵ�С

int Otsu_2(int arrays[100][100]);

int Otsu_3(int arrays[60][60][60]);

struct Point_3{
     int Min_val;//��Сֵ
	 int Max_val;//���ֵ
	 double s;   //����
};
void order(int *p,int n);//�����С���򣺴�С����
 
int Max_number(struct Point_3 *p,int n);//Ѱ����󷽲�


/*  ���涨�嵥��ֵOTSU�㷨*/
int Otsu(char *image);//������ͼ����ֵOTSU�㷨 
int Otsu1(char *image,int Min_pix,int Max_pix);//�򵥾ֲ�ͼ����ֵOTSU�㷨 
int Otsu_Step(char *image);//�Ż���������ֵOTSU�㷨 

int Cfc(char *image);//���ڷ�����С��
int Otsu_test(char *image);//�򵥵�ֵOTSU�㷨 + ���������ֵ��Ӱ��

int Otsu_Recursion(char *image);//�ݹ���ü򵥵���ֵOTSU�㷨


/*  ���涨���άOTSU�㷨*/
int Otsu_2D(char *image);//��ά����ֵOTSU�㷨 
int Otsu_2D_Step(char *image);//�Ľ��Ķ�ά����ֵOTSU�㷨һ


/*  ���涨�����ֵOTSU�㷨*/
void Otsu_MultiVal(char *image,int *t_array,int t_number);//����ֵOTSU�㷨һ
void Otsu_MultiVal(char *image,int *t_array);//����ֵOTSU�㷨��
int Otsu_Step_MultiVal(char *image);//�Ż���������ֵOTSU�㷨

#endif