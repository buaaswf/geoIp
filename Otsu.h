#ifndef FUNCTION_H
#define FUNCTION_H

#define X_VALUE 281  //z轴最大值
#define Y_VALUE 481  //y轴最大值
#define Z_VALUE 2501 //z轴最大值
#define SIZE X_VALUE*Y_VALUE*Z_VALUE  //一维数组的大小

int Otsu_2(int arrays[100][100]);

int Otsu_3(int arrays[60][60][60]);

struct Point_3{
     int Min_val;//最小值
	 int Max_val;//最大值
	 double s;   //方差
};
void order(int *p,int n);//数组大小排序：从小到大
 
int Max_number(struct Point_3 *p,int n);//寻找最大方差


/*  下面定义单阈值OTSU算法*/
int Otsu(char *image);//简单整幅图像单阈值OTSU算法 
int Otsu1(char *image,int Min_pix,int Max_pix);//简单局部图像单阈值OTSU算法 
int Otsu_Step(char *image);//优化步长单阈值OTSU算法 

int Cfc(char *image);//类内方差最小化
int Otsu_test(char *image);//简单单值OTSU算法 + 考虑面积比值的影响

int Otsu_Recursion(char *image);//递归调用简单单阈值OTSU算法


/*  下面定义二维OTSU算法*/
int Otsu_2D(char *image);//二维单阈值OTSU算法 
int Otsu_2D_Step(char *image);//改进的二维单阈值OTSU算法一


/*  下面定义多阈值OTSU算法*/
void Otsu_MultiVal(char *image,int *t_array,int t_number);//多阈值OTSU算法一
void Otsu_MultiVal(char *image,int *t_array);//多阈值OTSU算法二
int Otsu_Step_MultiVal(char *image);//优化步长多阈值OTSU算法

#endif