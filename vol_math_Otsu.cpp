#include <iostream>
#include <math.h>
#include "vol_math_Otsu.h"

using namespace std;

int Max_number(struct Point_3 *p,int n){
    double Maxs=p->s;
	int m=0,number=0;
	//寻找最大方差
	while(n--){
		if( Maxs<(p+m)->s){
			Maxs=(p+m)->s;
			number=m;
		}
		m++;
	}
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

//three Dimensions data process
int OTSU::Otsu(Raw &image,int Min_pix,int Max_pix){
	double w1;//前景的像素点数所占比例
	double u1;//前景的平均灰度
	double w2;//背景的像素点数所占比例
	double u2;//背景的平均灰度
	double s=0.0,M_s=0.0;//类间方差
	int t;//阈值
	int i,j,k,pix,size;//几个中间变量
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
		if(pix_number1) u1=Pix_val/pix_number1;//计算前景的平均灰度	 
		else  u1=Min_pix; 
		pix_number2=0.0;
		Pix_val=0.0;
		for(;i<=Max_pix;i++){
			pix_number2+=Pix_array[i];
			Pix_val+=i*Pix_array[i];
		}
		pixs=pix_number1+pix_number2;//总的像素点数
		w1=pix_number1/pixs;//前景像素点数所占比例
		w2=1-w1;//背景素点数所占比例
		if(pix_number2) u2=Pix_val/pix_number2;//计算背景的平均灰度
		else u2=Max_pix;
		s=w1*w2*(u1-u2)*(u1-u2); //类间方差
		if(M_s<s){  
			M_s=s;
			t=k;
		}
	}//计算出分割阀值t
	return t;
}
int OTSU::Otsu(Raw &image){
	int step;//步长
	double w1;//前景的像素点数占整幅图像的比例
	double u1;//前景的平均灰度
	double w2;//背景的像素点数占整幅图像的比例
	double u2;//背景的平均灰度
	double s=0.0,M_s=0.0;//类间方差
	int t;//阀值
	int i,j,k,pix,size,Min_pix,Max_pix,Min_val,Max_val;
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
	for(k=1;k<=20;k++){
		//统计前景像素点数占整幅图像的比例
		pix_number=0.0;
		Pix_val=0.0;
		for(i=Min_pix;i<(k*step+Min_pix);i++){
			pix_number+=Pix_array[i];
			Pix_val+=i*Pix_array[i];
		}
		w1=pix_number/size;//前景像素点数占整幅图像的比例
		if(pix_number) u1=Pix_val/pix_number;//计算前景的平均灰度
		else u1=Min_pix;
		w2=1-w1;//背景素点数占整幅图像的比例
		pix_number=0.0;
		Pix_val=0.0;
		for(;i<=Max_pix;i++){
			pix_number+=Pix_array[i];
			Pix_val+=i*Pix_array[i];
		}
		if(pix_number) u2=Pix_val/pix_number;//计算背景的平均灰度
		else u2=Max_pix;
        s=w1*w2*(u1-u2)*(u1-u2);//类间方差
		if(M_s<=s){
			M_s=s;
			t=k*step+Min_pix;
		}
	}	
	//提取以t为中间的且以两个step步长为长度的区间
	i=t+step;
	Max_val=i>Max_pix?Max_pix:i;//上界越界处理
	j=t-step;
	Min_val=j<Min_pix?Min_pix:j;//下界越界处理
	
    for(k=Min_val;k<Max_val;k++){   
		//统计前景像素点数占整幅图像的比例
		pix_number=0.0;
		Pix_val=0.0;
		for(i=Min_pix;i<=k;i++){  
			pix_number+=Pix_array[i];  
			Pix_val+=i*Pix_array[i];
		}
		w1=pix_number/size;//前景像素点数占整幅图像的比例 
		if(pix_number) u1=Pix_val/pix_number;//计算前景的平均灰度  
		else u1=Min_pix;
		w2=1-w1;//背景素点数占整幅图像的比例 
		pix_number=0.0;  
		Pix_val=0.0; 
		for(;i<=Max_pix;i++){    
			pix_number+=Pix_array[i];    
			Pix_val+=i*Pix_array[i];
		}
		if(pix_number) u2=Pix_val/pix_number;//计算背景的平均灰度    
		else u2=Max_pix;
		s=w1*w2*(u1-u2)*(u1-u2);
		if(M_s<s){
			M_s=s;
			t=k;
		}
	}
	return t;
}
void OTSU::Otsu_MultiVal(Raw &image,int t_number){
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
	arrays.clear();//清零
	do{//多阀值OTSU分割算法
		//单阀值Otsu算法步骤
		t=Otsu(image,Min_pix,Max_pix);//计算出分割阀值t
		arrays.push_back((unsigned char)t);//储存阈值
		l--;
		if(l){
		   //计算类间方差
		   pix_number1=0.0;//像素个数
		   Pix_val=0.0;//像素值
		   for(i=Min_pix;i<=t;i++){
			   pix_number1+=Pix_array[i];
			   Pix_val+=i*Pix_array[i];
		   } 
		   s1=0.0;
		   if(pix_number1){
			   u1=Pix_val/pix_number1;//计算前景的平均灰度
			   for(i=Min_pix;i<=t;i++){
				   s1+=Pix_array[i]*(i-u1)*(i-u1)/pix_number1;//计算前景方差
			   }
		   } 
		   else u1=Min_pix;
		   pix_number2=0.0;
		   Pix_val=0.0;
		   for(;i<=Max_pix;i++){
			   pix_number2+=Pix_array[i];
			   Pix_val+=i*Pix_array[i];
		   }
		   s2=0.0;
		   if(pix_number2){
			   u2=Pix_val/pix_number2;//计算背景的平均灰度
			   for(i=t+1;i<=Max_pix;i++){
				   s2+=Pix_array[i]*(i-u2)*(i-u2)/pix_number2;//计算背景方差
			   }
		   }
		   else u2=Max_pix;

		   //储存方差与上下界
		   cout<<"s1="<<s1<<" s2="<<s2<<endl;
		   for(i=0;i<t_number;i++){
			    if((ss+i)->Min_val==Min_pix && (ss+i)->Max_val==Max_pix){
					if(s1<s2){
						(ss+i)->Min_val=t;//改变下界
						(ss+i)->s=s2;
					}
					else {
						(ss+i)->Max_val=t;//改变上界
						(ss+i)->s=s1;//改变方差大小
					}
				   break;
		        }
		   }  
		   i=0;
		   while((ss+(i++))->s){//查找方差为零的指针
		   }
		   //赋值
		   j=i-1;
		   cout<<"j="<<j<<endl;
		   if(s1<s2){		
			   (ss+j)->Max_val=t;//改变上界	
			   (ss+j)->Min_val=Min_pix;//改变下界
			   (ss+j)->s=s1;//改变方差大小
		   }		
		   else {		
			   (ss+j)->Max_val=Max_pix;//改变上界	
			   (ss+j)->Min_val=t;//改变下界	
			   (ss+j)->s=s2;//改变方差大小	
		   }
		   k=Max_number(ss,t_number);//计算方差最大的位置
		   Min_pix=(ss+k)->Min_val;
		   Max_pix=(ss+k)->Max_val;
		}
	}while(l);
	delete []ss;
}
void OTSU::Otsu_MultiVal(Raw &image){
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
	arrays.clear();
	if(k>1){
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
		cout<<"k="<<k<<endl;
		//在相邻的两个波峰间进行阀值分割
		
		for(i=0;i<k-1;i++){
			if(temp_tvalue[i]>=Min_pix){
				for(j=i+1;i<k;j++){
					if(temp_tvalue[j]>=Min_pix){
						t=Otsu(image,temp_tvalue[i],temp_tvalue[j]);
						arrays.push_back(t);
						break;
					}
				}
			}
		}
	}
	else arrays.push_back(Otsu(image));
}
void OTSU::SaveImage(Raw &image){
	int size=image.size();
	int i,j,k,step;
	FILE *f=NULL;
	unsigned char val;
	k=arrays.size();//阈值个数
	if(k==0)return;
	else if(k==1){
		for( i=0;i<size;i++){
			val=image.getXYZ(i);
			if(val<arrays.at(0)) image.putXYZ(i,0);
			else  image.putXYZ(i,255);
		}
	}
	else{
		step=(int)(250/k);//步长
		for( i=0;i<size;i++){
			val=image.getXYZ(i);
			if(val<arrays.at(0)) image.putXYZ(i,0);
			else if(val>=arrays.at(k-1)) image.putXYZ(i,255);	
			else{
				for(j=1;j<k;j++){
					if(val>=arrays.at(j-1) && val<arrays.at(j)) image.putXYZ(i,step*j);	
				}
			}
		}	
	}
	f=fopen("img\\Image.raw","wb");
	fwrite(image.getdata(),sizeof(unsigned char),size,f);
	fclose(f);//关闭文件
}
void OTSU::Output(Raw &image,Raw &dest){
	int k,m,n,size;
	float val;
	//图像大小
	size=image.size();
	m=arrays.size();//阈值个数
	if(m>1){//多阈值处理
		//遍历
		for(k=0;k<size;k++){
			val=image.getXYZ(k);//获得图像的像素值的大小		
			if(val<arrays.at(0)) {	
				dest.putXYZ(k,1);	
			}	
			else if(val>=arrays.at(m-1)){		
				dest.putXYZ(k,m+1);	
			}	
			else{//中间的类	
				for(n=1;n<m;n++){		
					if(val>=arrays.at(n-1) && val<arrays.at(n)){			
						dest.putXYZ(k,n+1);	
					}		
				}//for	
			}//else
		}//for	
	}
	else{//单阈值处理
		//遍历
		for(k=0;k<size;k++){		
			val=image.getXYZ(k);//获得图像的像素值的大小		
			if(val<arrays.at(0)) dest.putXYZ(k,1);	
			else  dest.putXYZ(k,255);	
		}//for
	}
}


//two Dimensions data process 
int OTSU::Otsu(Raw2D &image,int Min_pix,int Max_pix){
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
		if(pix_number1) u1=Pix_val/pix_number1;//计算前景的平均灰度	    
		else  u1=Min_pix; 
		pix_number2=0.0;   
		Pix_val=0.0;    
		for(;i<=Max_pix;i++){	  
			pix_number2+=Pix_array[i];	   
			Pix_val+=i*Pix_array[i];
		}
		pixs=pix_number1+pix_number2;//总的像素点数	
		w1=pix_number1/pixs;//前景像素点数占整幅图像的比例
		w2=1-w1;//背景素点数占整幅图像的比例 
		if(pix_number2) u2=Pix_val/pix_number2;//计算背景的平均灰度  
		else u2=Max_pix;
		s=w1*w2*(u1-u2)*(u1-u2); //计算类间方差 
		if(M_s<s){  
			M_s=s; 
			t=k;
		}
	}//计算出分割阀值t
	cout<<"t="<<t<<endl;
	return t;
}
int OTSU::Otsu(Raw2D &image){
	int step;//步长
	double w1;//前景的像素点数占整幅图像的比例
	double u1;//前景的平均灰度
	double w2;//背景的像素点数占整幅图像的比例
	double u2;//背景的平均灰度
	double s=0.0,M_s=0.0;//类间方差
	int t;//阀值
	int i,j,k,pix,size,Min_pix,Max_pix,Min_val,Max_val;
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
	for(k=1;k<=20;k++){
		//统计前景像素点数占整幅图像的比例
		pix_number=0.0;
		Pix_val=0.0;
		for(i=Min_pix;i<=(k*step+Min_pix);i++){
			pix_number+=Pix_array[i];
			Pix_val+=i*Pix_array[i];
		}
		w1=pix_number/size;//前景像素点数占整幅图像的比例
		if(pix_number) u1=Pix_val/pix_number;//计算前景的平均灰度
		else u1=Min_pix;
		w2=1-w1;//背景素点数占整幅图像的比例
		pix_number=0.0;
		Pix_val=0.0;
		for(;i<=Max_pix;i++){
			pix_number+=Pix_array[i];
			Pix_val+=i*Pix_array[i];
		}
		if(pix_number) u2=Pix_val/pix_number;//计算背景的平均灰度
		else u2=Max_pix;
        s=w1*w2*(u1-u2)*(u1-u2);
		if(M_s<=s){
			M_s=s;
			t=k*step+Min_pix;
		}
	}
	//提取以t为中间的且以两个step步长为长度的区间
	i=t+step;
	Max_val=i>Max_pix?Max_pix:i;//上界越界处理
	j=t-step;
	Min_val=j<Min_pix?Min_pix:j;//下界越界处理
    for(k=Min_pix;k<Max_pix;k++){ 
		//统计前景像素点数占整幅图像的比例
		pix_number=0.0;
		Pix_val=0.0;
		for(i=Min_pix;i<=k;i++){ 
			pix_number+=Pix_array[i]; 
			Pix_val+=i*Pix_array[i];
		}
		w1=pix_number/size;//前景像素点数占整幅图像的比例
		if(pix_number)  u1=Pix_val/pix_number;//计算前景的平均灰度
		else u1=Min_pix; 
		w2=1-w1;//背景素点数占整幅图像的比例
		pix_number=0.0;   
		Pix_val=0.0;   
		for(;i<=Max_pix;i++){  
			pix_number+=Pix_array[i]; 
			Pix_val+=i*Pix_array[i];
		}
		if(pix_number)  u2=Pix_val/pix_number;//计算背景的平均灰度
		else u2=Max_pix; 
		s=w1*w2*(u1-u2)*(u1-u2);//计算类间方差
		if(M_s<s){
			M_s=s;
			t=k;
		}
	}
	return t;
}
void OTSU::Otsu_MultiVal(Raw2D &image,int t_number){
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
	arrays.clear();//清零
	do{//多阀值OTSU分割算法
		//单阀值Otsu算法步骤
		//cout<<"Min_pix="<< Min_pix<<"  Max_pix="<<Max_pix<<endl;
		t=Otsu(image,Min_pix,Max_pix);//计算出分割阀值t
        cout<<"t_number="<<l<<"  t="<<t<<endl;
		l--;
		arrays.push_back(t);//提取阀值
		if(l){
		   //计算类内方差
		   pix_number1=0.0;//像素个数
		   Pix_val=0.0;//像素值
		   for(i=Min_pix;i<=t;i++){
			   pix_number1+=Pix_array[i];
			   Pix_val+=i*Pix_array[i];
		   }
		   s1=0.0;
		   if(pix_number1){
			   u1=Pix_val/pix_number1;//计算前景的平均灰度
			   for(i=Min_pix;i<=t;i++){
				   s1+=Pix_array[i]*(i-u1)*(i-u1)/pix_number1;//计算前景方差
			   }
		   }
		   else u1=Min_pix;

		   pix_number2=0.0;
		   Pix_val=0.0;
		   for(;i<=Max_pix;i++){
			   pix_number2+=Pix_array[i];
			   Pix_val+=i*Pix_array[i];
		   } 
		   s2=0.0;
		   if(pix_number2){
			   u2=Pix_val/pix_number2;//计算背景的平均灰度
			   for(i=t+1;i<=Max_pix;i++){
				   s2+=Pix_array[i]*(i-u2)*(i-u2)/pix_number2;//计算背景方差
			   }
		   }
		   else u2=Max_pix;
		  
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
	delete []ss;
}
void OTSU::Otsu_MultiVal(Raw2D &image){
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
	//cout<<"t=";
	//for(i=0;i<k;i++){
	//	cout<<temp_tvalue[i]<<"  ";
	//}
	//cout<<endl;
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
	//cout<<"t=";
	//for(i=0;i<k;i++){
	//	cout<<temp_tvalue[i]<<"  ";
	//}
	//cout<<endl;
	//在相邻的两个波峰间进行阀值分割
	arrays.clear();//清空处理
	for(i=0;i<k-1;i++){
		if(temp_tvalue[i]>=Min_pix){
		  for(j=i+1;i<k;j++){
			  if(temp_tvalue[j]>=Min_pix){
                 t=Otsu(image,temp_tvalue[i],temp_tvalue[j]);
				 arrays.push_back(t);
				 break;
			  }
		  }
		}
	}
}
/*
void Output(Raw2D &image_2D,Raw2D &dest){
	int k,m,n,size;
	float val;
	//图像大小
	size=image_2D.size();
	m=arrays.size();//阈值个数
	if(m>1){//多阈值处理
		//遍历
		for(k=0;k<size;k++){
			val=image_2D.getXY(k);//获得图像的像素值的大小		
			if(val<arrays.at(0)) {	
				dest.putXY(k,1);	
			}	
			else if(val>=arrays.at(m-1)){		
				dest.putXY(k,m+1);	
			}	
			else{//中间的类	
				for(n=1;n<m;n++){		
					if(val>=arrays.at(n-1) && val<arrays.at(n)){			
						dest.putXY(k,n+1);	
					}		
				}//for	
			}//else
		}//for	
	}
	else{//单阈值处理
		//遍历
		for(k=0;k<size;k++){		
			val=image_2D.getXYZ(k);//获得图像的像素值的大小		
			if(val<arrays.at(0)) dest.putXY(k,1);	
			else  dest.putXY(k,2);	
		}//for
	}
}
*/