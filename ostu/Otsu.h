#ifndef FUNCTION_H
#define FUNCTION_H
#include <vector>
#include "vol_math_RawImage.h"
#include "vol_math_Raw3D_Independt.h"
#define SINGLE_OTSU 0 //单阈值分割
#define MULTIPLE_OTSU 1 //多阈值分割
struct Point_3{
     int Min_val;//最小值
	 int Max_val;//最大值
	 double s;   //方差
};
void order(int *p,int n);//数组大小排序：从小到大
 
int Max_number(struct Point_3 *p,int n);//寻找最大方差


//定义一个类OTSU
class OTSU{
public:
	Raw2D image_2D;//二维数据
	Raw image;//三维数据
	vector<int>array;//储存阈值
	int t_number;//指定阈值的个数
	//定义构造函数
	OTSU(){}//空的构造函数
	//二维构造函数
	OTSU(Raw2D newImage);
	OTSU(Raw2D newImage,int newT_number);
	//三维构造函数
	OTSU(Raw newImage);
	OTSU(Raw newImage,int newT_number);

	//成员函数：赋值
	//二维成员函数
	void setData(Raw2D newImage);
	void setData(Raw2D newImage,int newT_number);
    //三维成员函数
	void setData(Raw newImage);
	void setData(Raw newImage,int newT_number);

	//二维数据处理
	//int Otsu(Raw2D image_2D);//简单整幅图像单阈值OTSU算法 
 //   int Otsu1(Raw2D image_2D,int Min_pix,int Max_pix);//简单局部图像单阈值OTSU算法 
 //   int Otsu_Step(Raw2D image_2D);//优化步长单阈值OTSU算法 
	//int Otsu_Recursion(Raw2D image_2D);//递归调用简单单阈值OTSU算法
	////  下面定义二维OTSU算法
 //   int Otsu_2D(Raw2D image_2D);//二维单阈值OTSU算法 
 //   int Otsu_2D_Step(Raw2D image_2D);//改进的二维单阈值OTSU算法一

	//// 下面定义多阈值OTSU算法
	//void Otsu_MultiVal(Raw2D image_2D,int t_number);//多阈值OTSU算法一
	//void Otsu_MultiVal(Raw2D image_2D);//多阈值OTSU算法二
	//void SaveImage(Raw2D image_2D);//保存二维数据
	//二维算法整合
	//void Otsu(Raw2D image_2D,int label,int number);//label: 0/1 表示单/多阈值处理  number:表示指定分类的个数

	//三维数据处理
	int Otsu(Raw image);//简单整幅图像单阈值OTSU算法 
    int Otsu1(Raw image,int Min_pix,int Max_pix);//简单局部图像单阈值OTSU算法 
    int Otsu_Step(Raw image);//优化步长单阈值OTSU算法 
	int Otsu_Recursion(Raw image);//递归调用简单单阈值OTSU算法
	/*  下面定义二维OTSU算法*/
    int Otsu_2D(Raw image);//二维单阈值OTSU算法 
    int Otsu_2D_Step(Raw image);//改进的二维单阈值OTSU算法一

	/*  下面定义多阈值OTSU算法*/
	void Otsu_MultiVal(Raw image,int t_number);//多阈值OTSU算法一
	void Otsu_MultiVal(Raw image);//多阈值OTSU算法二
	void SaveImage(Raw image);//保存三维数据
	//三维算法整合
	//void Otsu(Raw image,int label,int number);//label: 0/1 表示单/多阈值处理  number:表示指定分类的个数

	//析构函数
	~OTSU(){
	   
	}
};
#endif