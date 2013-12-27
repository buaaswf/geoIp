#include <iostream>
#include <math.h>
#include "Otsu.h"

using namespace std;

int Max_number(struct Point_3 *p,int n){
	struct Point_3 *Max_s=new struct Point_3 [1];
	Max_s->s=p->s;
	int m=0,number=0;
	//寻找最大方差
	while(n--){
		if( Max_s->s< p->s){
			Max_s->s= p->s;
			number=m;
		}
		p++;
		m++;
	}
	delete(Max_s);
    return number;
}

void order(int *p,int n){
     int i,j,k;
	 for(i=0;i<n-1;i++){
	    for(j=i+1;j<n;j++)
			if(*(p+i)>*(p+j)){
				k=*(p+i);
				*(p+i)=*(p+j);
				*(p+j)=k;
			}
	 }
}

//OTSU类的实现
OTSU::OTSU(Raw2D newImage):image_2D(newImage){}
OTSU::OTSU(Raw2D newImage,int newT_number):image_2D(newImage){
	t_number=newT_number;
}
OTSU::OTSU(Raw newImage):image(newImage){}
OTSU::OTSU(Raw newImage,int newT_number):image(newImage){
	t_number=newT_number;
}

void OTSU::setData(Raw2D newImage){
	image_2D=newImage;
}
void OTSU::setData(Raw2D newImage,int newT_number){
	image_2D=newImage;
	t_number=newT_number;
}
void OTSU::setData(Raw newImage){
	image=newImage;
}
void OTSU::setData(Raw newImage,int newT_number){
	image=newImage;
	t_number=newT_number;
}

//三维数据处理
int OTSU::Otsu(Raw image){
	double w1;//前景的像素点数占整幅图像的比例
	double u1;//前景的平均灰度
	double w2;//背景的像素点数占整幅图像的比例
	double u2;//背景的平均灰度
	double s=0.0,M_s=0.0;//类间方差
	int t;//阀值

	int i,j,k,pix,size,Min_pix,Max_pix;
	size=image.size();//获得图像的大小
	Min_pix=255;//像素最小值初始化
	Max_pix=0;//像素最大值初始化

	int Pix_array[256]={0};
	double pix_number;//像素点统计
	double Pix_val;//像素值统计
	// 生成直方图，并统计出最大值、最小值
	for(i=0;i<size;i++){    
		pix=image.getXYZ(i);
		Pix_array[pix]++;
		Min_pix=Min_pix<pix?Min_pix:pix;
		Max_pix=Max_pix>pix?Max_pix:pix;
	}
	t=Min_pix;
	//直方图平滑
	for(i=Min_pix;i<=Max_pix;i++){
        Pix_val=0.0;
		for(j=-2;j<=2;j++){
            k=i+j;
			k=k<Min_pix?Min_pix:k;//下界越界处理
			k=k>Max_pix?Max_pix:k;//上界越界处理
			Pix_val+=Pix_array[k];
        }
		Pix_array[i]=(int)(Pix_val/5.0+0.5);
	}
	//cout<<Min_pix<<"  "<<Max_pix<<endl;//输出最大值、最小值
	for(k=Min_pix;k<Max_pix;k++){
		//统计前景像素点数占整幅图像的比例
		pix_number=0.0;
		Pix_val=0.0;
		for(i=Min_pix;i<=k;i++){
			pix_number+=Pix_array[i];
			Pix_val+=i*Pix_array[i];
		}
		w1=pix_number/size;//前景像素点数占整幅图像的比例
		//计算前景的平均灰度
		if(pix_number){
			u1=Pix_val/pix_number;
		}
		else u1=Min_pix;
		w2=1-w1;//背景素点数占整幅图像的比例
		pix_number=0.0;
		Pix_val=0.0;
		for(;i<=Max_pix;i++){
			pix_number+=Pix_array[i];
			Pix_val+=i*Pix_array[i];
		}
		//计算背景的平均灰度
		if(pix_number){
		  u2=Pix_val/pix_number;
		}
		else u2=Max_pix;
        s=w1*w2*(u1-u2)*(u1-u2);
		if(M_s<s){
			M_s=s;
			t=k;
			cout<<t;
		}
	}
  return t;
}
int OTSU::Otsu1(Raw image,int Min_pix,int Max_pix){
	double w1;//前景的像素点数占整幅图像的比例
	double u1;//前景的平均灰度
	double w2;//背景的像素点数占整幅图像的比例
	double u2;//背景的平均灰度
	double s=0.0,M_s=0.0;//类间方差
	int t;//阀值

	int i,j,k,pix,size;
	size=image.size();//获得图像的大小
	int Pix_array[256]={0};
	double pix_number1,pix_number2,pixs;//像素点统计
	double Pix_val;//像素值统计
	// 生成直方图
	for(i=0;i<size;i++){
		pix=image.getXYZ(i);
		Pix_array[pix]++;
	}
	//直方图平滑
	for(i=Min_pix;i<=Max_pix;i++){
        Pix_val=0.0;
		for(j=-2;j<=2;j++){
            k=i+j;
			k=k<Min_pix?Min_pix:k;//下界越界处理
			k=k>Max_pix?Max_pix:k;//上界越界处理
			Pix_val+=Pix_array[k];
        }
		Pix_array[i]=(int)(Pix_val/5.0+0.5);
	}
	for(k=Min_pix;k<Max_pix;k++){
		    //统计前景像素点数占整幅图像的比例
		    pix_number1=0.0;
		    Pix_val=0.0;
		    for(i=Min_pix;i<=k;i++){
			    pix_number1+=Pix_array[i];
			    Pix_val+=i*Pix_array[i];
		    }		    
		    u1=Pix_val/pix_number1;//计算前景的平均灰度	    
		    pix_number2=0.0;
		    Pix_val=0.0;
		    for(;i<=Max_pix;i++){
			    pix_number2+=Pix_array[i];
			    Pix_val+=i*Pix_array[i];
		    }
			pixs=pix_number1+pix_number2;//总的像素点数
			w1=pix_number1/pixs;//前景像素点数占整幅图像的比例
			w2=1-w1;//背景素点数占整幅图像的比例
		    u2=Pix_val/pix_number2;//计算背景的平均灰度
            s=w1*w2*(u1-u2)*(u1-u2); 
		    if(M_s<s){
			   M_s=s;
			   t=k;
		    }
	    }//计算出分割阀值t
  return t;
}
int OTSU::Otsu_Step(Raw image){
	int step;//步长
	double w1;//前景的像素点数占整幅图像的比例
	double u1;//前景的平均灰度
	double w2;//背景的像素点数占整幅图像的比例
	double u2;//背景的平均灰度
	double s=0.0,M_s=0.0;//类间方差
	int t;//阀值

	int i,j,k,pix,size,Min_pix,Max_pix;
	size=image.size();//获得图像的大小
	Min_pix=255;//像素最小值初始化
	Max_pix=0;//像素最大值初始化

	int Pix_array[256]={0};
	double pix_number;//像素点统计
	double Pix_val;//像素值统计
	// 生成直方图,并统计出最大值、最小值
	for(i=0;i<size;i++){
		pix=image.getXYZ(i);
		Pix_array[pix]++;
		Min_pix=Min_pix<pix?Min_pix:pix;
		Max_pix=Max_pix>pix?Max_pix:pix;
	}
	//直方图平滑
	for(i=Min_pix;i<=Max_pix;i++){
        Pix_val=0.0;
		for(j=-2;j<=2;j++){
            k=i+j;
			k=k<Min_pix?Min_pix:k;//下界越界处理
			k=k>Max_pix?Max_pix:k;//上界越界处理
			Pix_val+=Pix_array[k];
        }
		Pix_array[i]=(int)(Pix_val/5.0+0.5);
	}
	step=(Max_pix-Min_pix)/20;//计算步长
	for(k=0;k<20;k++){
		//统计前景像素点数占整幅图像的比例
		pix_number=0.0;
		Pix_val=0.0;
		for(i=Min_pix;i<=(k*step+Min_pix);i++){
			pix_number+=Pix_array[i];
			Pix_val+=i*Pix_array[i];
		}
		w1=pix_number/size;//前景像素点数占整幅图像的比例
		u1=Pix_val/pix_number;//计算前景的平均灰度
		w2=1-w1;//背景素点数占整幅图像的比例
		pix_number=0.0;
		Pix_val=0.0;
		for(;i<=Max_pix;i++){
			pix_number+=Pix_array[i];
			Pix_val+=i*Pix_array[i];
		}
		u2=Pix_val/pix_number;//计算背景的平均灰度
        s=w1*w2*(u1-u2)*(u1-u2);
		if(M_s<=s){
			M_s=s;
			t=k*step+Min_pix;
		}
		else break; 
	}
	if(t<Min_pix+20*step){
		Max_pix=k*step+Min_pix;
		Min_pix=((k-2)*step+Min_pix)>Min_pix?((k-2)*step+Min_pix):Min_pix;
	}
	else Min_pix=k*step+Min_pix;
    for(k=Min_pix;k<Max_pix;k++){
		    //统计前景像素点数占整幅图像的比例
		    pix_number=0.0;
		    Pix_val=0.0;
		    for(i=Min_pix;i<=k;i++){
			    pix_number+=Pix_array[i];
			    Pix_val+=i*Pix_array[i];
		    }
		    w1=pix_number/size;//前景像素点数占整幅图像的比例
		    u1=Pix_val/pix_number;//计算前景的平均灰度
		    w2=1-w1;//背景素点数占整幅图像的比例
		    pix_number=0.0;
		    Pix_val=0.0;
		    for(;i<=Max_pix;i++){
			    pix_number+=Pix_array[i];
			    Pix_val+=i*Pix_array[i];
		    }
		    u2=Pix_val/pix_number;//计算背景的平均灰度
            s=w1*w2*(u1-u2)*(u1-u2);
		    if(M_s<s){
			  M_s=s;
			  t=k;
		    }
	    }
  return t;
}
int OTSU::Otsu_Recursion(Raw image){
	double u1;//前景的平均灰度
	double pix_number1;//前景像素点统计
	double u2;//并背景的平均灰度
	double pix_number2;//背景像素点统计
	int t;//阀值
	double s=0,S=0;//特征值
	double Pix_val;//像素值统计
	int i,j,k,m,pix,size,Min_pix,Max_pix;
	size=image.size();//获得图像的大小
	Min_pix=255;
	Max_pix=0;
	int Pix_array[256]={0};
	//生成直方图
	for(i=0;i<size;i++){	    
		pix=image.getXYZ(i);	
		if(pix>0){ 		   
			Pix_array[pix]++;		  
			Min_pix=Min_pix<pix?Min_pix:pix;		   
			Max_pix=Max_pix>pix?Max_pix:pix;
		}
	}
	//直方图平滑
	for(i=Min_pix;i<=Max_pix;i++){
        Pix_val=0.0;
		for(j=-2;j<=2;j++){
            k=i+j;
			k=k<Min_pix?Min_pix:k;//下界越界处理
			k=k>Max_pix?Max_pix:k;//上界越界处理
			Pix_val+=Pix_array[k];
        }
		Pix_array[i]=(int)(Pix_val/5.0+0.5);
	}
	k=0;
	do{
		t=Otsu1(image,Min_pix,Max_pix);//求得阀值	
		pix_number1=0.0;
		Pix_val=0.0;
		for(m=Min_pix;m<=t;m++){
			pix_number1+=Pix_array[m];//前景像素点统计
			Pix_val+=m*Pix_array[m];
		}
		u1=Pix_val/pix_number1;//计算前景的平均灰度
		pix_number2=0.0;
		Pix_val=0.0;
		for(;m<=Max_pix;m++){
			pix_number2+=Pix_array[m];//背景像素点统计
			Pix_val+=m*Pix_array[m];
		}
		u2=Pix_val/pix_number2;//计算背景的平均灰度
		S=(pix_number1+pix_number2)*fabs(u1-u2)/fabs(pix_number1-pix_number2);
		if(S<s) break;
		else s=S;
		Min_pix=t;				
	}while(k++<10);//结束条件
	return t;
}
int OTSU::Otsu_2D(Raw image){//二维OTSU算法
	double w1;//前景的像素点数占整幅图像的比例
	double w1_k;//前景的k邻域像素点数占整幅图像的比例
	double u1;//前景的平均灰度
	double u1_k;//前景的k邻域平均灰度
	double w2;//背景的像素点数占整幅图像的比例
	double w2_k;//背景的k邻域像素点数占整幅图像的比例
	double u2;//背景的平均灰度
	double u2_k;//背景的k邻域平均灰度
	double s=0.0,M_s=0.0;//类间方差
	int t;//阀值

	int i,j,k,l,m,n,x,y,z,X_VALUE,Y_VALUE,Z_VALUE,size,pix,pix_k,Min_pix,Max_pix;
	X_VALUE=image.getXsize();//获得x轴方向的大小
	Y_VALUE=image.getYsize();//获得y方向的大小
	Z_VALUE=image.getZsize();//获得z方向的大小
	size=X_VALUE*Y_VALUE*Z_VALUE;//获得图像的大小
	Min_pix=255;//像素最小值初始化
	Max_pix=0;//像素最大值初始化
	
	int Pix_Array[256][256]={{0,0}};//像素直方图矩阵
	double pix_number;//像素点统计,pix_number_k
	double Pix_val,Pix_val_k;//像素值统计
	// 生成直方图，并统计出最大值、最小值
	for(z=0;z<Z_VALUE;z++){//遍历Z轴
		for(y=0;y<Y_VALUE;y++){//遍历Y轴
			for(x=0;x<X_VALUE;x++){//遍历X轴
			    Pix_val_k=0.0;
		        pix=image.getXYZ(z*X_VALUE*Y_VALUE+y*X_VALUE+x);
		        Min_pix=Min_pix<pix?Min_pix:pix;//统计最小值
		        Max_pix=Max_pix>pix?Max_pix:pix;//统计最大值
				for(i=-1;i<2;i++){
				    l=z+i;
					l=l<0?0:l;
					l=l>(Z_VALUE-1)?(Z_VALUE-1):l;
					for(j=-1;j<2;j++){
						m=y+j;
						m=m<0?0:m;
						m=m>(Y_VALUE-1)?(Y_VALUE-1):m;
						for(k=-1;k<2;k++){
							n=x+k;
							n=n<0?0:n;
							n=n>(X_VALUE-1)?(X_VALUE-1):n;
						    pix_k=image.getXYZ(l*X_VALUE*Y_VALUE+m*X_VALUE+n);
						}
					}
				}
				pix_k=(int)(Pix_val_k/27.0+0.5);//k邻域像素平均值
		        Pix_Array[pix+128][pix_k+128]++;//由于输入的数据最小值为-128
			}
		}
	}
	Min_pix+=128;//由于输入的数据最小值为-128
	Max_pix+=128;//由于输入的数据最小值为-128
	t=Min_pix;
	//计算矩阵的迹
	for(k=Min_pix;k<Max_pix;k++){
		for(l=Min_pix;l<=Max_pix;l++){
		    pix_number=0.0;
			//pix_number_k=0.0;
		    Pix_val=0.0;
			Pix_val_k=0.0;
			//统计前景像素点数占整幅图像的比例
			for(i=Min_pix;i<=k;i++){
				for(j=Min_pix;j<=l;j++){
				   pix_number+=Pix_Array[i][j];
				   Pix_val+=i*Pix_Array[i][j];
				   Pix_val_k+=j*Pix_Array[i][j];
				}
			}
			w1=pix_number/size;//前景像素点数占整幅图像的比例
			w1_k=w1;
			u1=Pix_val/pix_number;//计算前景的平均灰度
			u1_k=Pix_val_k/pix_number;//计算前景k邻域的平均灰度

			pix_number=0.0;
			//pix_number_k=0.0;
		    Pix_val=0.0;
			Pix_val_k=0.0;
			//统计背景像素点数占整幅图像的比例
			for(;i<=Max_pix;i++){
				for(;j<=Max_pix;j++){
				   pix_number+=Pix_Array[i][j];
				   Pix_val+=i*Pix_Array[i][j];
				   Pix_val_k+=j*Pix_Array[i][j];
				}
			}
			//w2=pix_number/SIZE;//背景像素点数占整幅图像的比例
			//w2_k=pix_number/SIZE;
			w2=1-w1;
			w2_k=w2;
			u2=Pix_val/pix_number;//计算背景的平均灰度
			u2_k=Pix_val_k/pix_number;//计算背景k邻域的平均灰度
		    s=w1*w2*(u1-u2)*(u1-u2)+w1_k*w2_k*(u1_k-u2_k)*(u1_k-u2_k);
		    if(M_s<s){
		       M_s=s;
			   t=(int)(w2*k+w1*l);
		    }
		}
	 }
     return t-128;
}
int OTSU::Otsu_2D_Step(Raw image){//改进的二维OTSU算法
	double w1;//前景的像素点数占整幅图像的比例
	double w1_k;//前景的k邻域像素点数占整幅图像的比例
	double u1;//前景的平均灰度
	double u1_k;//前景的k邻域平均灰度
	double w2;//背景的像素点数占整幅图像的比例
	double w2_k;//背景的k邻域像素点数占整幅图像的比例
	double u2;//背景的平均灰度
	double u2_k;//背景的k邻域平均灰度
	double s=0.0,M_s=0.0;//类间方差
	int t;//阀值
	int step;//步长
	int i,j,k,l,x,y,z,m,n,X_VALUE,Y_VALUE,Z_VALUE,size,pix,pix_k,Min_pix,Max_pix;
	X_VALUE=image.getXsize();//获得x轴方向的大小
	Y_VALUE=image.getYsize();//获得y方向的大小
	Z_VALUE=image.getZsize();//获得z方向的大小
	size=X_VALUE*Y_VALUE*Z_VALUE;//获得图像的大小
	Min_pix=255;//像素最小值初始化
	Max_pix=0;//像素最大值初始化

	int Pix_Array[256][256]={{0,0}};//像素直方图矩阵
	double pix_number;//像素点统计,pix_number_k
	double Pix_val,Pix_val_k;//像素值统计
	// 生成直方图，并统计出最大值、最小值
	for(z=0;z<Z_VALUE;z++){//遍历Z轴
		for(y=0;y<Y_VALUE;y++){//遍历Y轴
			for(x=0;x<X_VALUE;x++){//遍历X轴
			    Pix_val_k=0.0;
		        pix=image.getXYZ(z*X_VALUE*Y_VALUE+y*X_VALUE+x);
		        Min_pix=Min_pix<pix?Min_pix:pix;//统计最小值
		        Max_pix=Max_pix>pix?Max_pix:pix;//统计最大值
				for(i=-1;i<2;i++){
				    l=z+i;
					l=l<0?0:l;
					l=l>(Z_VALUE-1)?(Z_VALUE-1):l;
					for(j=-1;j<2;j++){
						m=y+j;
						m=m<0?0:m;
						m=m>(Y_VALUE-1)?(Y_VALUE-1):m;
						for(k=-1;k<2;k++){
							n=x+k;
							n=n<0?0:n;
							n=n>(X_VALUE-1)?(X_VALUE-1):n;
						    pix_k=image.getXYZ(l*X_VALUE*Y_VALUE+m*X_VALUE+n);
						}
					}
				}
				pix_k=(int)(Pix_val_k/27.0+0.5);//k邻域像素平均值
		        Pix_Array[pix+128][pix_k+128]++;//由于输入的数据最小值为-128
			}
		}
	}
	Min_pix+=128;//由于输入的数据最小值为-128
	Max_pix+=128;//由于输入的数据最小值为-128
	t=Min_pix;
	step=(Max_pix-Min_pix)/10;//计算步长
	//寻找最优阀值区域
	for(k=0;k<=20;k++){
		l=k*step+Min_pix;
		pix_number=0.0;
		Pix_val=0.0;
		Pix_val_k=0.0;
		for(i=Min_pix;i<=l;i++){
			for(j=l-i;j>=Min_pix;j--){
				if(Min_pix<=i && i<=Max_pix && Min_pix<=j && j<=Max_pix && (i-l/2)<j && j<(i+l/2)){
				   pix_number+=Pix_Array[i][j];
				   Pix_val+=i*Pix_Array[i][j];
				   Pix_val_k+=j*Pix_Array[i][j];
				}
			}
		}
		w1=pix_number/size;//前景像素点数占整幅图像的比例
		w1_k=w1;
		u1=Pix_val/pix_number;//计算前景的平均灰度
		u1_k=Pix_val_k/pix_number;//计算前景k邻域的平均灰度
		pix_number=0.0;
		Pix_val=0.0;
		Pix_val_k=0.0;
		for(i=Min_pix;i<=Max_pix;i++){
			for(j=Min_pix;j<=Max_pix;j++){
				if(i+j>l && j<=Max_pix && (i-l/2)<j && j<(i+l/2)){
				   pix_number+=Pix_Array[i][j];
				   Pix_val+=i*Pix_Array[i][j];
				   Pix_val_k+=j*Pix_Array[i][j];
				}
			}
		}
		w2=pix_number/size;//背景像素点数占整幅图像的比例
		w2_k=w2;
		if(pix_number){
		   u2=Pix_val/pix_number;//计算背景的平均灰度
		   u2_k=Pix_val_k/pix_number;//计算背景k邻域的平均灰度
		}
		else {
		   u2=Max_pix;
		   u2_k=Max_pix;
		}
		s=w1*w2*(u1-u2)*(u1-u2)+w1_k*w2_k*(u1_k-u2_k)*(u1_k-u2_k);
		if(M_s<s){
		   M_s=s;
		   t=l/2;
		}
	}
	m=t-step;
	n=t+2*step;
	Min_pix=Min_pix<m?Min_pix:m;//重新划分下线
	Max_pix=Max_pix>n?n:Max_pix;//重新划分上线
	//再次计算矩阵的迹
	M_s=0.0;
	for(k=Min_pix;k<=Max_pix;k++){
		for(l=Min_pix;l<=Max_pix;l++){
		    pix_number=0.0;
		    Pix_val=0.0;
			Pix_val_k=0.0;
			//统计前景像素点数占整幅图像的比例
			for(i=Min_pix;i<=k;i++){
				for(j=Min_pix;j<=l;j++){
				   pix_number+=Pix_Array[i][j];
				   Pix_val+=i*Pix_Array[i][j];
				   Pix_val_k+=j*Pix_Array[i][j];
				}
			}
			w1=pix_number/size;//前景像素点数占整幅图像的比例
			w1_k=w1;
			u1=Pix_val/pix_number;//计算前景的平均灰度
			u1_k=Pix_val_k/pix_number;//计算前景k邻域的平均灰度

			pix_number=0.0;
		    Pix_val=0.0;
			Pix_val_k=0.0;
			//统计背景像素点数占整幅图像的比例
			for(;i<=Max_pix;i++){
				for(;j<=Max_pix;j++){
				   pix_number+=Pix_Array[i][j];
				   Pix_val+=i*Pix_Array[i][j];
				   Pix_val_k+=j*Pix_Array[i][j];
				}
			}
			w2=1-w1;
			w2_k=w2;
			u2=Pix_val/pix_number;//计算背景的平均灰度
			u2_k=Pix_val_k/pix_number;//计算背景k邻域的平均灰度
		    s=w1*w2*(u1-u2)*(u1-u2)+w1_k*w2_k*(u1_k-u2_k)*(u1_k-u2_k);
		    if(M_s<s){
		       M_s=s;
			   t=(int)(w2*k+w1*l);
		    }
		}
	 }
    return t-128;
}

void OTSU::Otsu_MultiVal(Raw image,int t_number){
	struct Point_3 *ss=new struct Point_3 [t_number];
    double s=0.0,M_s=0.0;//类间方差
	double u1;//前景的平均灰度
	double s1;//前景的类内方差
	double u2;//背景的平均灰度
	double s2;//背景的类内方差
	int t;//阀值

	int i,j,k,l,size,pix,Min_pix,Max_pix;
	size=image.size();//获得图像的大小
	Min_pix=255;//像素最小值初始化
	Max_pix=0;//像素最大值初始化
	l=t_number;//记录阈值个数

	int Pix_array[256]={0};//直方图
	double pix_number1,pix_number2;//像素点统计
	double Pix_val;//像素值统计
	// 生成直方图，并统计最大值、最小值
	for(i=0;i<size;i++){
		pix=image.getXYZ(i);	
		Pix_array[pix]++;
		Min_pix=Min_pix<pix?Min_pix:pix;
		Max_pix=Max_pix>pix?Max_pix:pix;
	}
	t=Min_pix;
	//直方图平滑
	for(i=Min_pix;i<=Max_pix;i++){
        Pix_val=0.0;
		for(j=-2;j<=2;j++){
            k=i+j;
			k=k<Min_pix?Min_pix:k;//下界越界处理
			k=k>Max_pix?Max_pix:k;//上界越界处理
			Pix_val+=Pix_array[k];
        }
		Pix_array[i]=(int)(Pix_val/5.0+0.5);
	}
	ss->Min_val=Min_pix;//赋初值下界
	ss->Max_val=Max_pix;//赋初值上界
	for(i=0;i<t_number;i++){//对方差赋初值零
		(ss+i)->s=0;
	}
	array.clear();
	do{//多阀值OTSU分割算法
		//单阀值Otsu算法步骤
		//cout<<"Min_pix="<< Min_pix<<"  Max_pix="<<Max_pix<<endl;
		t=Otsu1(image,Min_pix,Max_pix);//计算出分割阀值t
        cout<<"t_number="<<l<<"  t="<<t<<endl;
		l--;
		array.push_back(t);//提取阀值
		if(l){
		   //计算类内方差
		   pix_number1=0.0;//像素个数
		   Pix_val=0.0;//像素值
		   for(i=Min_pix;i<=t;i++){
			   pix_number1+=Pix_array[i];
			   Pix_val+=i*Pix_array[i];
		   }
		   u1=Pix_val/pix_number1;//计算前景的平均灰度
		   s1=0.0;
		   for(i=Min_pix;i<=t;i++){
               s1+=Pix_array[i]*(i-u1)*(i-u1)/pix_number1;//计算前景方差
		   }

		   pix_number2=0.0;
		   Pix_val=0.0;
		   for(;i<=Max_pix;i++){
			   pix_number2+=Pix_array[i];
			   Pix_val+=i*Pix_array[i];
		   }
		   u2=Pix_val/pix_number2;//计算背景的平均灰度
		   s2=0.0;
		   for(i=t+1;i<=Max_pix;i++){
               s2+=Pix_array[i]*(i-u2)*(i-u2)/pix_number2;//计算背景方差
		   }
		   //储存方差与上下界
		   cout<<"s1="<<s1<<" s2="<<s2<<endl;
		   for(i=0;i<t_number;i++,ss++){
			    if(ss->Min_val==Min_pix && ss->Max_val==Max_pix){
			       ss->Max_val=t;//改变上界
			       ss->s=s1;//改变方差大小
				   break;
		        }
		   }
		   ss-=i;//返回到指针头位置
		   k=0;
		   while(ss->s){
			   ss++;//查找方差为零的指针
			   k++;
		   }
		   //赋值
		   cout<<"ss->s="<<ss->s<<endl;
		   ss->Min_val=t;//
		   ss->Max_val=Max_pix;
		   ss->s=s2;
		   cout<<"ss->s="<<ss->s<<endl;
		   ss-=k;//返回到指针头位置
		   k=Max_number(ss,t_number);//计算方差最大的位置
		   cout<<"k="<<k<<endl;
		   Min_pix=(ss+k)->Min_val;
		   Max_pix=(ss+k)->Max_val;
		}
	}while(l);
	delete(ss);
}
void OTSU::Otsu_MultiVal(Raw image){
	int t;//阀值
	int i,j,k,size,Min_val,t_number,pix,Min_pix,Max_pix;
	size=image.size();//获得图像的大小
	t_number=0;//记录阀值的个数
	Min_pix=255;//像素最小值初始化
	Max_pix=0;//像素最大值初始化
	Min_val=(int)(size/250);//波峰的最小值:略大于平均值 
	int temp_tvalue[256]={-129};//储存波峰
	int Pix_array[256]={0};//直方图
	double Pix_val;//像素值统计
	// 生成直方图
	for(i=0;i<size;i++){  
		pix=image.getXYZ(i);
		Pix_array[pix]++;
		Min_pix=Min_pix<pix?Min_pix:pix;
		Max_pix=Max_pix>pix?Max_pix:pix;
	}
	t=Min_pix;//赋初值
	for(i=Min_pix;i<=Max_pix;i++){
        Pix_val=0.0;
		for(j=-3;j<=3;j++){
            k=i+j;
			k=k<Min_pix?Min_pix:k;//下界越界处理
			k=k>Max_pix?Max_pix:k;//上界越界处理
			Pix_val+=Pix_array[k];
        }
		Pix_array[i]=(int)(Pix_val/5.0+0.5);
	}
	//查找波峰
	k=0;//记录波峰的个数
	for(i=Min_pix+1;i<Max_pix;i++){
		if(i==(Min_pix+1)){
			if(Pix_array[i-1] > Pix_array[i] && Pix_array[i-1]>Min_val){	
			   temp_tvalue[k]=i-1;
			   k++;
			}
		}
		else if(i==(Max_pix-1)){
		    if(Pix_array[i] < Pix_array[i+1] && Pix_array[i+1]>Min_val){
			   temp_tvalue[k]=i+1;
			   k++;
			}
		}
		else {
		    if(Pix_array[i-1] < Pix_array[i] && Pix_array[i] > Pix_array[i+1] && Pix_array[i]>Min_val){
			   temp_tvalue[k]=i;
			   k++;
			}
		}
	}
	cout<<"**********************k="<<k<<endl;
	cout<<"t=";
	for(i=0;i<k;i++){
		cout<<temp_tvalue[i]<<"  ";
	}
	cout<<endl;
	//除去伪峰
	for(i=0;i<k-1;i++){
		if(abs(temp_tvalue[i]-temp_tvalue[i+1]) < 10){//合并伪峰
			if(i+2<k){
			  if(abs(temp_tvalue[i]-temp_tvalue[i+1])<=abs(temp_tvalue[i+1]-temp_tvalue[i+2])){
				 if(temp_tvalue[i]<temp_tvalue[i+1]){
				    temp_tvalue[i]=Min_pix-1;
				 }
				 else temp_tvalue[i+1]=Min_pix-1;
			  }
			  else {
			     if(temp_tvalue[i+1]<temp_tvalue[i+2]){
				    temp_tvalue[i+1]=Min_pix-1;
				 }
				 else temp_tvalue[i+2]=Min_pix-1;
			  }
		    }
			else {
				if(temp_tvalue[i]<temp_tvalue[i+1]){
				    temp_tvalue[i]=Min_pix-1;
				 }
				 else temp_tvalue[i+1]=Min_pix-1;
			} 		
		}
	}
	cout<<"t=";
	for(i=0;i<k;i++){
		cout<<temp_tvalue[i]<<"  ";
	}
	cout<<endl;
	//在相邻的两个波峰间进行阀值分割
	array.clear();
	for(i=0;i<k-1;i++){
		if(temp_tvalue[i]>=Min_pix){
		  for(j=i+1;i<k;j++){
			  if(temp_tvalue[j]>=Min_pix){
                 t=Otsu1(image,temp_tvalue[i],temp_tvalue[j]);
				 array.push_back(t);
				 break;
			  }
		  }
		}
	}
}
void OTSU::SaveImage(Raw image){
	int size=image.size();
	int i,j,k,step;
	FILE *f=NULL;
	float val;
	k=array.size();//阈值个数
	if(k>1){
		step=(int)(250/k);//步长
		for( i=0;i<size;i++){
			val=image.getXYZ(i);
			if(val<array.at(0)) image.putXYZ(i,-128);
			else if(val>=array.at(k-1)) image.putXYZ(i,127);	
			else{
				for(j=1;j<k;j++){
					if(val>=array.at(j-1) && val<array.at(j)) image.putXYZ(i,-128+step*j);	
				}
			}
		}	
	}
	else{
		for( i=0;i<size;i++){
			if(val<array.at(0)) image.putXYZ(i,-128);
			else  image.putXYZ(i,127);
		}
	}
	f=fopen("img\\Image.raw","wb");
	fwrite(image.getdata(),sizeof(unsigned char),size,f);
	fclose(f);//关闭文件
}

//二维数据处理
/*
int OTSU::Otsu(Raw2D image){
	double w1;//前景的像素点数占整幅图像的比例
	double u1;//前景的平均灰度
	double w2;//背景的像素点数占整幅图像的比例
	double u2;//背景的平均灰度
	double s=0.0,M_s=0.0;//类间方差
	int t;//阀值

	int i,j,k,pix,size,Min_pix,Max_pix;
	size=image.size();//获得图像的大小
	Min_pix=255;//像素最小值初始化
	Max_pix=0;//像素最大值初始化

	int Pix_array[256]={0};
	double pix_number;//像素点统计
	double Pix_val;//像素值统计
	// 生成直方图，并统计出最大值、最小值
	for(i=0;i<size;i++){    
		pix=image.getXY(i);
		Pix_array[pix]++;
		Min_pix=Min_pix<pix?Min_pix:pix;
		Max_pix=Max_pix>pix?Max_pix:pix;
	}
	cout<<Min_pix<<"  "<<Max_pix<<endl;//输出最大值、最小值
	t=Min_pix;
	//直方图平滑
	for(i=Min_pix;i<=Max_pix;i++){
        Pix_val=0.0;
		for(j=-2;j<=2;j++){
            k=i+j;
			k=k<Min_pix?Min_pix:k;//下界越界处理
			k=k>Max_pix?Max_pix:k;//上界越界处理
			Pix_val+=Pix_array[k];
        }
		Pix_array[i]=(int)(Pix_val/5.0+0.5);
	}
	cout<<Min_pix<<"  "<<Max_pix<<endl;//再次输出最大值、最小值
	for(k=Min_pix;k<Max_pix;k++){
		//统计前景像素点数占整幅图像的比例
		pix_number=0.0;
		Pix_val=0.0;
		for(i=Min_pix;i<=k;i++){
			pix_number+=Pix_array[i];
			Pix_val+=i*Pix_array[i];
		}
		w1=pix_number/size;//前景像素点数占整幅图像的比例
		//计算前景的平均灰度
		if(pix_number){
			u1=Pix_val/pix_number;
		}
		else u1=Min_pix;
		w2=1-w1;//背景素点数占整幅图像的比例
		pix_number=0.0;
		Pix_val=0.0;
		for(;i<=Max_pix;i++){
			pix_number+=Pix_array[i];
			Pix_val+=i*Pix_array[i];
		}
		//计算背景的平均灰度
		if(pix_number){
		  u2=Pix_val/pix_number;
		}
		else u2=Max_pix;
        s=w1*w2*(u1-u2)*(u1-u2);
		if(M_s<s){
			M_s=s;
			t=k;
			cout<<t;
		}
	}
  return t;
}
int OTSU::Otsu1(Raw2D image,int Min_pix,int Max_pix){
	double w1;//前景的像素点数占整幅图像的比例
	double u1;//前景的平均灰度
	double w2;//背景的像素点数占整幅图像的比例
	double u2;//背景的平均灰度
	double s=0.0,M_s=0.0;//类间方差
	int t;//阀值

	int i,j,k,pix,size;
	size=image.size();//获得图像的大小
	int Pix_array[256]={0};
	double pix_number1,pix_number2,pixs;//像素点统计
	double Pix_val;//像素值统计
	// 生成直方图
	for(i=0;i<size;i++){
		pix=image.getXY(i);
		Pix_array[pix]++;
	}
	//直方图平滑
	for(i=Min_pix;i<=Max_pix;i++){
        Pix_val=0.0;
		for(j=-2;j<=2;j++){
            k=i+j;
			k=k<Min_pix?Min_pix:k;//下界越界处理
			k=k>Max_pix?Max_pix:k;//上界越界处理
			Pix_val+=Pix_array[k];
        }
		Pix_array[i]=(int)(Pix_val/5.0+0.5);
	}
	for(k=Min_pix;k<Max_pix;k++){
		    //统计前景像素点数占整幅图像的比例
		    pix_number1=0.0;
		    Pix_val=0.0;
		    for(i=Min_pix;i<=k;i++){
			    pix_number1+=Pix_array[i];
			    Pix_val+=i*Pix_array[i];
		    }		    
		    u1=Pix_val/pix_number1;//计算前景的平均灰度	    
		    pix_number2=0.0;
		    Pix_val=0.0;
		    for(;i<=Max_pix;i++){
			    pix_number2+=Pix_array[i];
			    Pix_val+=i*Pix_array[i];
		    }
			pixs=pix_number1+pix_number2;//总的像素点数
			w1=pix_number1/pixs;//前景像素点数占整幅图像的比例
			w2=1-w1;//背景素点数占整幅图像的比例
		    u2=Pix_val/pix_number2;//计算背景的平均灰度
            s=w1*w2*(u1-u2)*(u1-u2); 
		    if(M_s<s){
			   M_s=s;
			   t=k;
		    }
	    }//计算出分割阀值t
  return t;
}
int OTSU::Otsu_Step(Raw2D image){
	int step;//步长
	double w1;//前景的像素点数占整幅图像的比例
	double u1;//前景的平均灰度
	double w2;//背景的像素点数占整幅图像的比例
	double u2;//背景的平均灰度
	double s=0.0,M_s=0.0;//类间方差
	int t;//阀值

	int i,j,k,pix,size,Min_pix,Max_pix;
	size=image.size();//获得图像的大小
	Min_pix=255;//像素最小值初始化
	Max_pix=0;//像素最大值初始化

	int Pix_array[256]={0};
	double pix_number;//像素点统计
	double Pix_val;//像素值统计
	// 生成直方图,并统计出最大值、最小值
	for(i=0;i<size;i++){
		pix=image.getXY(i);
		Pix_array[pix]++;
		Min_pix=Min_pix<pix?Min_pix:pix;
		Max_pix=Max_pix>pix?Max_pix:pix;
	}
	//直方图平滑
	for(i=Min_pix;i<=Max_pix;i++){
        Pix_val=0.0;
		for(j=-2;j<=2;j++){
            k=i+j;
			k=k<Min_pix?Min_pix:k;//下界越界处理
			k=k>Max_pix?Max_pix:k;//上界越界处理
			Pix_val+=Pix_array[k];
        }
		Pix_array[i]=(int)(Pix_val/5.0+0.5);
	}
	step=(Max_pix-Min_pix)/20;//计算步长
	for(k=0;k<20;k++){
		//统计前景像素点数占整幅图像的比例
		pix_number=0.0;
		Pix_val=0.0;
		for(i=Min_pix;i<=(k*step+Min_pix);i++){
			pix_number+=Pix_array[i];
			Pix_val+=i*Pix_array[i];
		}
		w1=pix_number/size;//前景像素点数占整幅图像的比例
		u1=Pix_val/pix_number;//计算前景的平均灰度
		w2=1-w1;//背景素点数占整幅图像的比例
		pix_number=0.0;
		Pix_val=0.0;
		for(;i<=Max_pix;i++){
			pix_number+=Pix_array[i];
			Pix_val+=i*Pix_array[i];
		}
		u2=Pix_val/pix_number;//计算背景的平均灰度
        s=w1*w2*(u1-u2)*(u1-u2);
		if(M_s<=s){
			M_s=s;
			t=k*step+Min_pix;
		}
		else break; 
	}
	if(t<Min_pix+20*step){
		Max_pix=k*step+Min_pix;
		Min_pix=((k-2)*step+Min_pix)>Min_pix?((k-2)*step+Min_pix):Min_pix;	
	}
	else Min_pix=k*step+Min_pix;
    for(k=Min_pix;k<Max_pix;k++){
		    //统计前景像素点数占整幅图像的比例
		    pix_number=0.0;
		    Pix_val=0.0;
		    for(i=Min_pix;i<=k;i++){
			    pix_number+=Pix_array[i];
			    Pix_val+=i*Pix_array[i];
		    }
		    w1=pix_number/size;//前景像素点数占整幅图像的比例
		    u1=Pix_val/pix_number;//计算前景的平均灰度
		    w2=1-w1;//背景素点数占整幅图像的比例
		    pix_number=0.0;
		    Pix_val=0.0;
		    for(;i<=Max_pix;i++){
			    pix_number+=Pix_array[i];
			    Pix_val+=i*Pix_array[i];
		    }
		    u2=Pix_val/pix_number;//计算背景的平均灰度
            s=w1*w2*(u1-u2)*(u1-u2);
		    if(M_s<s){
			  M_s=s;
			  t=k;
		    }
	    }
  return t;
}
int OTSU::Otsu_Recursion(Raw2D image){
	double u1;//前景的平均灰度
	double pix_number1;//前景像素点统计
	double u2;//并背景的平均灰度
	double pix_number2;//背景像素点统计
	int t;//阀值
	double s=0,S=0;//特征值
	double Pix_val;//像素值统计
	int i,j,k,m,pix,size,Min_pix,Max_pix;
	size=image.size();//获得图像的大小
	Min_pix=255;
	Max_pix=0;
	int Pix_array[256]={0};
	//生成直方图
	for(i=0;i<size;i++){	    
		pix=image.getXY(i);	
		if(pix>0){ 		   
			Pix_array[pix]++;		  
			Min_pix=Min_pix<pix?Min_pix:pix;		   
			Max_pix=Max_pix>pix?Max_pix:pix;
		}
	}
	//直方图平滑
	for(i=Min_pix;i<=Max_pix;i++){
        Pix_val=0.0;
		for(j=-2;j<=2;j++){
            k=i+j;
			k=k<Min_pix?Min_pix:k;//下界越界处理
			k=k>Max_pix?Max_pix:k;//上界越界处理
			Pix_val+=Pix_array[k];
        }
		Pix_array[i]=(int)(Pix_val/5.0+0.5);
	}
	k=0;
	do{
		t=Otsu1(image,Min_pix,Max_pix);//求得阀值	
		pix_number1=0.0;
		Pix_val=0.0;
		for(m=Min_pix;m<=t;m++){
			pix_number1+=Pix_array[m];//前景像素点统计
			Pix_val+=m*Pix_array[m];
		}
		u1=Pix_val/pix_number1;//计算前景的平均灰度
		pix_number2=0.0;
		Pix_val=0.0;
		for(;m<=Max_pix;m++){
			pix_number2+=Pix_array[m];//背景像素点统计
			Pix_val+=m*Pix_array[m];
		}
		u2=Pix_val/pix_number2;//计算背景的平均灰度
		S=(pix_number1+pix_number2)*fabs(u1-u2)/fabs(pix_number1-pix_number2);
		if(S<s) break;
		else s=S;
		Min_pix=t;				
	}while(k++<10);//结束条件
	return t;
}
int OTSU::Otsu_2D(Raw2D image){//二维OTSU算法
	double w1;//前景的像素点数占整幅图像的比例
	double w1_k;//前景的k邻域像素点数占整幅图像的比例
	double u1;//前景的平均灰度
	double u1_k;//前景的k邻域平均灰度
	double w2;//背景的像素点数占整幅图像的比例
	double w2_k;//背景的k邻域像素点数占整幅图像的比例
	double u2;//背景的平均灰度
	double u2_k;//背景的k邻域平均灰度
	double s=0.0,M_s=0.0;//类间方差
	int t;//阀值

	int i,j,k,l,x,y,z,X_VALUE,Y_VALUE,size,pix,pix_k,Min_pix,Max_pix;
	X_VALUE=image.getXsize();//获得x轴方向的大小
	Y_VALUE=image.getYsize();//获得y方向的大小
	size=X_VALUE*Y_VALUE;//获得图像的大小
	Min_pix=255;//像素最小值初始化
	Max_pix=0;//像素最大值初始化
	
	int Pix_Array[256][256]={{0,0}};//像素直方图矩阵
	double pix_number;//像素点统计,pix_number_k
	double Pix_val,Pix_val_k;//像素值统计
	// 生成直方图，并统计出最大值、最小值
	for(i=0;i<size;i++){
		Pix_val_k=0.0;
		pix=image.getXY(i);
		Min_pix=Min_pix<pix?Min_pix:pix;//统计最小值
		Max_pix=Max_pix>pix?Max_pix:pix;//统计最大值
		for(j=-1;j<2;j++){//在Y轴上平滑	
			y=i/X_VALUE+j;//Y轴	
			y=y<0?0:y;
			y=y>(Y_VALUE-1)?(Y_VALUE-1):y;
			for(k=-1;k<2;k++){//在X轴上平滑   
				x=i%X_VALUE+k;//X轴
				x=x<0?0:x;
				x=x>(X_VALUE-1)?(X_VALUE-1):x;
				Pix_val_k+=image.getXY(y*X_VALUE+x);//计算9个像素的值
			}
		}
		pix_k=(int)(Pix_val_k/9.0+0.5);//k邻域像素平均值
		Pix_Array[pix+128][pix_k+128]++;//由于输入的数据最小值为-128
	}
	Min_pix+=128;//由于输入的数据最小值为-128
	Max_pix+=128;//由于输入的数据最小值为-128
	t=Min_pix;
	//计算矩阵的迹
	for(k=Min_pix;k<Max_pix;k++){
		for(l=Min_pix;l<=Max_pix;l++){
		    pix_number=0.0;
			//pix_number_k=0.0;
		    Pix_val=0.0;
			Pix_val_k=0.0;
			//统计前景像素点数占整幅图像的比例
			for(i=Min_pix;i<=k;i++){
				for(j=Min_pix;j<=l;j++){
				   pix_number+=Pix_Array[i][j];
				   Pix_val+=i*Pix_Array[i][j];
				   Pix_val_k+=j*Pix_Array[i][j];
				}
			}
			w1=pix_number/size;//前景像素点数占整幅图像的比例
			w1_k=w1;
			u1=Pix_val/pix_number;//计算前景的平均灰度
			u1_k=Pix_val_k/pix_number;//计算前景k邻域的平均灰度

			pix_number=0.0;
			//pix_number_k=0.0;
		    Pix_val=0.0;
			Pix_val_k=0.0;
			//统计背景像素点数占整幅图像的比例
			for(;i<=Max_pix;i++){
				for(;j<=Max_pix;j++){
				   pix_number+=Pix_Array[i][j];
				   Pix_val+=i*Pix_Array[i][j];
				   Pix_val_k+=j*Pix_Array[i][j];
				}
			}
			//w2=pix_number/SIZE;//背景像素点数占整幅图像的比例
			//w2_k=pix_number/SIZE;
			w2=1-w1;
			w2_k=w2;
			u2=Pix_val/pix_number;//计算背景的平均灰度
			u2_k=Pix_val_k/pix_number;//计算背景k邻域的平均灰度
		    s=w1*w2*(u1-u2)*(u1-u2)+w1_k*w2_k*(u1_k-u2_k)*(u1_k-u2_k);
		    if(M_s<s){
		       M_s=s;
			   t=(int)(w2*k+w1*l);
		    }
		}
	 }
     return t-128;
}
int OTSU::Otsu_2D_Step(Raw2D image){//改进的二维OTSU算法
	double w1;//前景的像素点数占整幅图像的比例
	double w1_k;//前景的k邻域像素点数占整幅图像的比例
	double u1;//前景的平均灰度
	double u1_k;//前景的k邻域平均灰度
	double w2;//背景的像素点数占整幅图像的比例
	double w2_k;//背景的k邻域像素点数占整幅图像的比例
	double u2;//背景的平均灰度
	double u2_k;//背景的k邻域平均灰度
	double s=0.0,M_s=0.0;//类间方差
	int t;//阀值
	int step;//步长
	int i,j,k,l,x,y,z,m,n,X_VALUE,Y_VALUE,size,pix,pix_k,Min_pix,Max_pix;
	X_VALUE=image.getXsize();//获得x轴方向的大小
	Y_VALUE=image.getYsize();//获得y方向的大小
	size=X_VALUE*Y_VALUE;//获得图像的大小
	Min_pix=255;//像素最小值初始化
	Max_pix=0;//像素最大值初始化

	int Pix_Array[256][256]={{0,0}};//像素直方图矩阵
	double pix_number;//像素点统计,pix_number_k
	double Pix_val,Pix_val_k;//像素值统计
	// 生成直方图，并统计出最大值、最小值
	for(i=0;i<size;i++){
		Pix_val_k=0.0;
		pix=image.getXY(i);
		Min_pix=Min_pix<pix?Min_pix:pix;//统计最小值
		Max_pix=Max_pix>pix?Max_pix:pix;//统计最大值
		for(j=-1;j<2;j++){//在Y轴上平滑	
			y=i/X_VALUE+j;//Y轴	
			y=y<0?0:y;
			y=y>(Y_VALUE-1)?(Y_VALUE-1):y;
			for(k=-1;k<2;k++){//在X轴上平滑   
				x=i%X_VALUE+k;//X轴
				x=x<0?0:x;
				x=x>(X_VALUE-1)?(X_VALUE-1):x;
				Pix_val_k+=image.getXY(y*X_VALUE+x);//计算9个像素的值
			}
		}
		pix_k=(int)(Pix_val_k/9.0+0.5);//k邻域像素平均值
		Pix_Array[pix+128][pix_k+128]++;//由于输入的数据最小值为-128
	}
	Min_pix+=128;//由于输入的数据最小值为-128
	Max_pix+=128;//由于输入的数据最小值为-128
	t=Min_pix;
	step=(Max_pix-Min_pix)/10;//计算步长
	//寻找最优阀值区域
	for(k=0;k<=20;k++){
		l=k*step+Min_pix;
		pix_number=0.0;
		Pix_val=0.0;
		Pix_val_k=0.0;
		for(i=Min_pix;i<=l;i++){
			for(j=l-i;j>=Min_pix;j--){
				if(Min_pix<=i && i<=Max_pix && Min_pix<=j && j<=Max_pix && (i-l/2)<j && j<(i+l/2)){
				   pix_number+=Pix_Array[i][j];
				   Pix_val+=i*Pix_Array[i][j];
				   Pix_val_k+=j*Pix_Array[i][j];
				}
			}
		}
		w1=pix_number/size;//前景像素点数占整幅图像的比例
		w1_k=w1;
		u1=Pix_val/pix_number;//计算前景的平均灰度
		u1_k=Pix_val_k/pix_number;//计算前景k邻域的平均灰度
		pix_number=0.0;
		Pix_val=0.0;
		Pix_val_k=0.0;
		for(i=Min_pix;i<=Max_pix;i++){
			for(j=Min_pix;j<=Max_pix;j++){
				if(i+j>l && j<=Max_pix && (i-l/2)<j && j<(i+l/2)){
				   pix_number+=Pix_Array[i][j];
				   Pix_val+=i*Pix_Array[i][j];
				   Pix_val_k+=j*Pix_Array[i][j];
				}
			}
		}
		w2=pix_number/size;//背景像素点数占整幅图像的比例
		w2_k=w2;
		if(pix_number){
		   u2=Pix_val/pix_number;//计算背景的平均灰度
		   u2_k=Pix_val_k/pix_number;//计算背景k邻域的平均灰度
		}
		else {
		   u2=Max_pix;
		   u2_k=Max_pix;
		}
		s=w1*w2*(u1-u2)*(u1-u2)+w1_k*w2_k*(u1_k-u2_k)*(u1_k-u2_k);
		if(M_s<s){
		   M_s=s;
		   t=l/2;
		}
	}
	m=t-step;
	n=t+2*step;
	Min_pix=Min_pix<m?Min_pix:m;//重新划分下线
	Max_pix=Max_pix>n?n:Max_pix;//重新划分上线
	//再次计算矩阵的迹
	M_s=0.0;
	for(k=Min_pix;k<=Max_pix;k++){
		for(l=Min_pix;l<=Max_pix;l++){
		    pix_number=0.0;
		    Pix_val=0.0;
			Pix_val_k=0.0;
			//统计前景像素点数占整幅图像的比例
			for(i=Min_pix;i<=k;i++){
				for(j=Min_pix;j<=l;j++){
				   pix_number+=Pix_Array[i][j];
				   Pix_val+=i*Pix_Array[i][j];
				   Pix_val_k+=j*Pix_Array[i][j];
				}
			}
			w1=pix_number/size;//前景像素点数占整幅图像的比例
			w1_k=w1;
			u1=Pix_val/pix_number;//计算前景的平均灰度
			u1_k=Pix_val_k/pix_number;//计算前景k邻域的平均灰度

			pix_number=0.0;
		    Pix_val=0.0;
			Pix_val_k=0.0;
			//统计背景像素点数占整幅图像的比例
			for(;i<=Max_pix;i++){
				for(;j<=Max_pix;j++){
				   pix_number+=Pix_Array[i][j];
				   Pix_val+=i*Pix_Array[i][j];
				   Pix_val_k+=j*Pix_Array[i][j];
				}
			}
			w2=1-w1;
			w2_k=w2;
			u2=Pix_val/pix_number;//计算背景的平均灰度
			u2_k=Pix_val_k/pix_number;//计算背景k邻域的平均灰度
		    s=w1*w2*(u1-u2)*(u1-u2)+w1_k*w2_k*(u1_k-u2_k)*(u1_k-u2_k);
		    if(M_s<s){
		       M_s=s;
			   t=(int)(w2*k+w1*l);
		    }
		}
	 }
    return t-128;
}

void OTSU::Otsu_MultiVal(Raw2D image,int t_number){
	struct Point_3 *ss=new struct Point_3 [t_number];
    double s=0.0,M_s=0.0;//类间方差
	double u1;//前景的平均灰度
	double s1;//前景的类内方差
	double u2;//背景的平均灰度
	double s2;//背景的类内方差
	int t;//阀值

	int i,j,k,l,size,pix,Min_pix,Max_pix;
	size=image.size();//获得图像的大小
	Min_pix=255;//像素最小值初始化
	Max_pix=0;//像素最大值初始化
	l=t_number;//记录阈值个数

	int Pix_array[256]={0};//直方图
	double pix_number1,pix_number2;//像素点统计
	double Pix_val;//像素值统计
	// 生成直方图，并统计最大值、最小值
	for(i=0;i<size;i++){
		pix=image.getXY(i);	
		Pix_array[pix]++;
		Min_pix=Min_pix<pix?Min_pix:pix;
		Max_pix=Max_pix>pix?Max_pix:pix;
	}
	t=Min_pix;
	//直方图平滑
	for(i=Min_pix;i<=Max_pix;i++){
        Pix_val=0.0;
		for(j=-2;j<=2;j++){
            k=i+j;
			k=k<Min_pix?Min_pix:k;//下界越界处理
			k=k>Max_pix?Max_pix:k;//上界越界处理
			Pix_val+=Pix_array[k];
        }
		Pix_array[i]=(int)(Pix_val/5.0+0.5);
	}
	ss->Min_val=Min_pix;//赋初值下界
	ss->Max_val=Max_pix;//赋初值上界
	for(i=0;i<t_number;i++){//对方差赋初值零
		(ss+i)->s=0;
	}
	do{//多阀值OTSU分割算法
		//单阀值Otsu算法步骤
		//cout<<"Min_pix="<< Min_pix<<"  Max_pix="<<Max_pix<<endl;
		t=Otsu1(image,Min_pix,Max_pix);//计算出分割阀值t
        cout<<"t_number="<<l<<"  t="<<t<<endl;
		l--;
		array.push_back(t);//提取阀值
		if(l){
		   //计算类内方差
		   pix_number1=0.0;//像素个数
		   Pix_val=0.0;//像素值
		   for(i=Min_pix;i<=t;i++){
			   pix_number1+=Pix_array[i];
			   Pix_val+=i*Pix_array[i];
		   }
		   u1=Pix_val/pix_number1;//计算前景的平均灰度
		   s1=0.0;
		   for(i=Min_pix;i<=t;i++){
               s1+=Pix_array[i]*(i-u1)*(i-u1)/pix_number1;//计算前景方差
		   }

		   pix_number2=0.0;
		   Pix_val=0.0;
		   for(;i<=Max_pix;i++){
			   pix_number2+=Pix_array[i];
			   Pix_val+=i*Pix_array[i];
		   }
		   u2=Pix_val/pix_number2;//计算背景的平均灰度
		   s2=0.0;
		   for(i=t+1;i<=Max_pix;i++){
               s2+=Pix_array[i]*(i-u2)*(i-u2)/pix_number2;//计算背景方差
		   }
		   //储存方差与上下界
		   cout<<"s1="<<s1<<" s2="<<s2<<endl;
		   for(i=0;i<t_number;i++,ss++){
			    if(ss->Min_val==Min_pix && ss->Max_val==Max_pix){
			       ss->Max_val=t;//改变上界
			       ss->s=s1;//改变方差大小
				   break;
		        }
		   }
		   ss-=i;//返回到指针头位置
		   k=0;
		   while(ss->s){
			   ss++;//查找方差为零的指针
			   k++;
		   }
		   //赋值
		   cout<<"ss->s="<<ss->s<<endl;
		   ss->Min_val=t;//
		   ss->Max_val=Max_pix;
		   ss->s=s2;
		   cout<<"ss->s="<<ss->s<<endl;
		   ss-=k;//返回到指针头位置
		   k=Max_number(ss,t_number);//计算方差最大的位置
		   cout<<"k="<<k<<endl;
		   Min_pix=(ss+k)->Min_val;
		   Max_pix=(ss+k)->Max_val;
		}
	}while(l);
	delete(ss);
}
void OTSU::Otsu_MultiVal(Raw2D image){
	int t;//阀值
	int i,j,k,size,Min_val,t_number,pix,Min_pix,Max_pix;
	size=image.size();//获得图像的大小
	t_number=0;//记录阀值的个数
	Min_pix=255;//像素最小值初始化
	Max_pix=0;//像素最大值初始化
	Min_val=(int)(size/250);//波峰的最小值:略大于平均值 
	int temp_tvalue[256]={-129};//储存波峰
	int Pix_array[256]={0};//直方图
	double Pix_val;//像素值统计
	// 生成直方图
	for(i=0;i<size;i++){  
		pix=image.getXY(i);
		Pix_array[pix]++;
		Min_pix=Min_pix<pix?Min_pix:pix;
		Max_pix=Max_pix>pix?Max_pix:pix;
	}
	t=Min_pix;//赋初值
	for(i=Min_pix;i<=Max_pix;i++){
        Pix_val=0.0;
		for(j=-3;j<=3;j++){
            k=i+j;
			k=k<Min_pix?Min_pix:k;//下界越界处理
			k=k>Max_pix?Max_pix:k;//上界越界处理
			Pix_val+=Pix_array[k];
        }
		Pix_array[i]=(int)(Pix_val/5.0+0.5);
	}
	//查找波峰
	k=0;//记录波峰的个数
	for(i=Min_pix+1;i<Max_pix;i++){
		if(i==(Min_pix+1)){
			if(Pix_array[i-1] > Pix_array[i] && Pix_array[i-1]>Min_val){	
			   temp_tvalue[k]=i-1;
			   k++;
			}
		}
		else if(i==(Max_pix-1)){
		    if(Pix_array[i] < Pix_array[i+1] && Pix_array[i+1]>Min_val){
			   temp_tvalue[k]=i+1;
			   k++;
			}
		}
		else {
		    if(Pix_array[i-1] < Pix_array[i] && Pix_array[i] > Pix_array[i+1] && Pix_array[i]>Min_val){
			   temp_tvalue[k]=i;
			   k++;
			}
		}
	}
	cout<<"**********************k="<<k<<endl;
	cout<<"t=";
	for(i=0;i<k;i++){
		cout<<temp_tvalue[i]<<"  ";
	}
	cout<<endl;
	//除去伪峰
	for(i=0;i<k-1;i++){
		if(abs(temp_tvalue[i]-temp_tvalue[i+1]) < 10){//合并伪峰
			if(i+2<k){
			  if(abs(temp_tvalue[i]-temp_tvalue[i+1])<=abs(temp_tvalue[i+1]-temp_tvalue[i+2])){
				 if(temp_tvalue[i]<temp_tvalue[i+1]){
				    temp_tvalue[i]=Min_pix-1;
				 }
				 else temp_tvalue[i+1]=Min_pix-1;
			  }
			  else {
			     if(temp_tvalue[i+1]<temp_tvalue[i+2]){
				    temp_tvalue[i+1]=Min_pix-1;
				 }
				 else temp_tvalue[i+2]=Min_pix-1;
			  }
		    }
			else {
				if(temp_tvalue[i]<temp_tvalue[i+1]){
				    temp_tvalue[i]=Min_pix-1;
				 }
				 else temp_tvalue[i+1]=Min_pix-1;
			} 		
		}
	}
	cout<<"t=";
	for(i=0;i<k;i++){
		cout<<temp_tvalue[i]<<"  ";
	}
	cout<<endl;
	//在相邻的两个波峰间进行阀值分割
	array.clear();//清空处理
	for(i=0;i<k-1;i++){
		if(temp_tvalue[i]>=Min_pix){
		  for(j=i+1;i<k;j++){
			  if(temp_tvalue[j]>=Min_pix){
                 t=Otsu1(image,temp_tvalue[i],temp_tvalue[j]);
				 array.push_back(t);
				 break;
			  }
		  }
		}
	}
}
void OTSU::SaveImage(Raw2D image){
	int size=image.size();//图像大小
	int step;//步长
	int i,j,k;
	FILE *f=NULL;
	float val;
	k=array.size();//阈值个数
	if(k>1){
		step=(int)(250/k);
		for( i=0;i<size;i++){
			val=image.getXY(i);
			if(val<array.at(0)) image.putXY(i,-128);
			else if(val>=array.at(k-1)) image.putXY(i,127);	
			else{
				for(j=1;j<k;j++){
					if(val>=array.at(j-1) && val<array.at(j)) image.putXY(i,-128+step*j);	
				}
			}
		}	
	}
	else{
		for( i=0;i<size;i++){
			if(val<array.at(0)) image.putXY(i,-128);
			else  image.putXY(i,127);
		}
	}
	f=fopen("img\\Image.raw","wb");//打开文件
	fwrite(image.getdata(),sizeof(unsigned char),size,f);
	fclose(f);//关闭文件
}
*/