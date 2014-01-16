#ifndef VOL_MATH_OTSU_H
#define VOL_MATH_OTSU_H
#include <vector>
#include <queue>
#include "vol_math_RawImage.h"
#include "vol_math_Raw3D_Independt.h"
#include "vol_math_filter_Interface.h"
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
	vector<int>arrays;//储存阈值

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
    int Otsu(Raw2D image_2D,int Min_pix,int Max_pix);//简单局部图像单阈值OTSU算法 
    int Otsu(Raw2D image_2D);//优化步长单阈值OTSU算法 

	// 下面定义多阈值OTSU算法
	void Otsu_MultiVal(Raw2D image_2D,int t_number);//多阈值OTSU算法一
	void Otsu_MultiVal(Raw2D image_2D);//多阈值OTSU算法二
	void SaveImage(Raw2D image_2D);//保存二维数据
//	Points** output(Raw2D image_2D);

	//三维数据处理
    int Otsu(Raw image,int Min_pix,int Max_pix);//简单局部图像单阈值OTSU算法 
    int Otsu(Raw image);//优化步长单阈值OTSU算法 

	/*  下面定义多阈值OTSU算法*/
	void Otsu_MultiVal(Raw image,int t_number);//多阈值OTSU算法一
	void Otsu_MultiVal(Raw image);//多阈值OTSU算法二
	void SaveImage(Raw image);//保存三维数据
//	Points** output(Raw image);


	//析构函数
	~OTSU(){
	  arrays.~vector<int>(); 
	}
};
#endif