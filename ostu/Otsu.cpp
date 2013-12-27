#include <iostream>
#include <math.h>
#include "Otsu.h"

using namespace std;

int Max_number(struct Point_3 *p,int n){
	struct Point_3 *Max_s=new struct Point_3 [1];
	Max_s->s=p->s;
	int m=0,number=0;
	//Ѱ����󷽲�
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

//OTSU���ʵ��
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

//��ά���ݴ���
int OTSU::Otsu(Raw image){
	double w1;//ǰ�������ص���ռ����ͼ��ı���
	double u1;//ǰ����ƽ���Ҷ�
	double w2;//���������ص���ռ����ͼ��ı���
	double u2;//������ƽ���Ҷ�
	double s=0.0,M_s=0.0;//��䷽��
	int t;//��ֵ

	int i,j,k,pix,size,Min_pix,Max_pix;
	size=image.size();//���ͼ��Ĵ�С
	Min_pix=255;//������Сֵ��ʼ��
	Max_pix=0;//�������ֵ��ʼ��

	int Pix_array[256]={0};
	double pix_number;//���ص�ͳ��
	double Pix_val;//����ֵͳ��
	// ����ֱ��ͼ����ͳ�Ƴ����ֵ����Сֵ
	for(i=0;i<size;i++){    
		pix=image.getXYZ(i);
		Pix_array[pix]++;
		Min_pix=Min_pix<pix?Min_pix:pix;
		Max_pix=Max_pix>pix?Max_pix:pix;
	}
	t=Min_pix;
	//ֱ��ͼƽ��
	for(i=Min_pix;i<=Max_pix;i++){
        Pix_val=0.0;
		for(j=-2;j<=2;j++){
            k=i+j;
			k=k<Min_pix?Min_pix:k;//�½�Խ�紦��
			k=k>Max_pix?Max_pix:k;//�Ͻ�Խ�紦��
			Pix_val+=Pix_array[k];
        }
		Pix_array[i]=(int)(Pix_val/5.0+0.5);
	}
	//cout<<Min_pix<<"  "<<Max_pix<<endl;//������ֵ����Сֵ
	for(k=Min_pix;k<Max_pix;k++){
		//ͳ��ǰ�����ص���ռ����ͼ��ı���
		pix_number=0.0;
		Pix_val=0.0;
		for(i=Min_pix;i<=k;i++){
			pix_number+=Pix_array[i];
			Pix_val+=i*Pix_array[i];
		}
		w1=pix_number/size;//ǰ�����ص���ռ����ͼ��ı���
		//����ǰ����ƽ���Ҷ�
		if(pix_number){
			u1=Pix_val/pix_number;
		}
		else u1=Min_pix;
		w2=1-w1;//�����ص���ռ����ͼ��ı���
		pix_number=0.0;
		Pix_val=0.0;
		for(;i<=Max_pix;i++){
			pix_number+=Pix_array[i];
			Pix_val+=i*Pix_array[i];
		}
		//���㱳����ƽ���Ҷ�
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
	double w1;//ǰ�������ص���ռ����ͼ��ı���
	double u1;//ǰ����ƽ���Ҷ�
	double w2;//���������ص���ռ����ͼ��ı���
	double u2;//������ƽ���Ҷ�
	double s=0.0,M_s=0.0;//��䷽��
	int t;//��ֵ

	int i,j,k,pix,size;
	size=image.size();//���ͼ��Ĵ�С
	int Pix_array[256]={0};
	double pix_number1,pix_number2,pixs;//���ص�ͳ��
	double Pix_val;//����ֵͳ��
	// ����ֱ��ͼ
	for(i=0;i<size;i++){
		pix=image.getXYZ(i);
		Pix_array[pix]++;
	}
	//ֱ��ͼƽ��
	for(i=Min_pix;i<=Max_pix;i++){
        Pix_val=0.0;
		for(j=-2;j<=2;j++){
            k=i+j;
			k=k<Min_pix?Min_pix:k;//�½�Խ�紦��
			k=k>Max_pix?Max_pix:k;//�Ͻ�Խ�紦��
			Pix_val+=Pix_array[k];
        }
		Pix_array[i]=(int)(Pix_val/5.0+0.5);
	}
	for(k=Min_pix;k<Max_pix;k++){
		    //ͳ��ǰ�����ص���ռ����ͼ��ı���
		    pix_number1=0.0;
		    Pix_val=0.0;
		    for(i=Min_pix;i<=k;i++){
			    pix_number1+=Pix_array[i];
			    Pix_val+=i*Pix_array[i];
		    }		    
		    u1=Pix_val/pix_number1;//����ǰ����ƽ���Ҷ�	    
		    pix_number2=0.0;
		    Pix_val=0.0;
		    for(;i<=Max_pix;i++){
			    pix_number2+=Pix_array[i];
			    Pix_val+=i*Pix_array[i];
		    }
			pixs=pix_number1+pix_number2;//�ܵ����ص���
			w1=pix_number1/pixs;//ǰ�����ص���ռ����ͼ��ı���
			w2=1-w1;//�����ص���ռ����ͼ��ı���
		    u2=Pix_val/pix_number2;//���㱳����ƽ���Ҷ�
            s=w1*w2*(u1-u2)*(u1-u2); 
		    if(M_s<s){
			   M_s=s;
			   t=k;
		    }
	    }//������ָֵt
  return t;
}
int OTSU::Otsu_Step(Raw image){
	int step;//����
	double w1;//ǰ�������ص���ռ����ͼ��ı���
	double u1;//ǰ����ƽ���Ҷ�
	double w2;//���������ص���ռ����ͼ��ı���
	double u2;//������ƽ���Ҷ�
	double s=0.0,M_s=0.0;//��䷽��
	int t;//��ֵ

	int i,j,k,pix,size,Min_pix,Max_pix;
	size=image.size();//���ͼ��Ĵ�С
	Min_pix=255;//������Сֵ��ʼ��
	Max_pix=0;//�������ֵ��ʼ��

	int Pix_array[256]={0};
	double pix_number;//���ص�ͳ��
	double Pix_val;//����ֵͳ��
	// ����ֱ��ͼ,��ͳ�Ƴ����ֵ����Сֵ
	for(i=0;i<size;i++){
		pix=image.getXYZ(i);
		Pix_array[pix]++;
		Min_pix=Min_pix<pix?Min_pix:pix;
		Max_pix=Max_pix>pix?Max_pix:pix;
	}
	//ֱ��ͼƽ��
	for(i=Min_pix;i<=Max_pix;i++){
        Pix_val=0.0;
		for(j=-2;j<=2;j++){
            k=i+j;
			k=k<Min_pix?Min_pix:k;//�½�Խ�紦��
			k=k>Max_pix?Max_pix:k;//�Ͻ�Խ�紦��
			Pix_val+=Pix_array[k];
        }
		Pix_array[i]=(int)(Pix_val/5.0+0.5);
	}
	step=(Max_pix-Min_pix)/20;//���㲽��
	for(k=0;k<20;k++){
		//ͳ��ǰ�����ص���ռ����ͼ��ı���
		pix_number=0.0;
		Pix_val=0.0;
		for(i=Min_pix;i<=(k*step+Min_pix);i++){
			pix_number+=Pix_array[i];
			Pix_val+=i*Pix_array[i];
		}
		w1=pix_number/size;//ǰ�����ص���ռ����ͼ��ı���
		u1=Pix_val/pix_number;//����ǰ����ƽ���Ҷ�
		w2=1-w1;//�����ص���ռ����ͼ��ı���
		pix_number=0.0;
		Pix_val=0.0;
		for(;i<=Max_pix;i++){
			pix_number+=Pix_array[i];
			Pix_val+=i*Pix_array[i];
		}
		u2=Pix_val/pix_number;//���㱳����ƽ���Ҷ�
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
		    //ͳ��ǰ�����ص���ռ����ͼ��ı���
		    pix_number=0.0;
		    Pix_val=0.0;
		    for(i=Min_pix;i<=k;i++){
			    pix_number+=Pix_array[i];
			    Pix_val+=i*Pix_array[i];
		    }
		    w1=pix_number/size;//ǰ�����ص���ռ����ͼ��ı���
		    u1=Pix_val/pix_number;//����ǰ����ƽ���Ҷ�
		    w2=1-w1;//�����ص���ռ����ͼ��ı���
		    pix_number=0.0;
		    Pix_val=0.0;
		    for(;i<=Max_pix;i++){
			    pix_number+=Pix_array[i];
			    Pix_val+=i*Pix_array[i];
		    }
		    u2=Pix_val/pix_number;//���㱳����ƽ���Ҷ�
            s=w1*w2*(u1-u2)*(u1-u2);
		    if(M_s<s){
			  M_s=s;
			  t=k;
		    }
	    }
  return t;
}
int OTSU::Otsu_Recursion(Raw image){
	double u1;//ǰ����ƽ���Ҷ�
	double pix_number1;//ǰ�����ص�ͳ��
	double u2;//��������ƽ���Ҷ�
	double pix_number2;//�������ص�ͳ��
	int t;//��ֵ
	double s=0,S=0;//����ֵ
	double Pix_val;//����ֵͳ��
	int i,j,k,m,pix,size,Min_pix,Max_pix;
	size=image.size();//���ͼ��Ĵ�С
	Min_pix=255;
	Max_pix=0;
	int Pix_array[256]={0};
	//����ֱ��ͼ
	for(i=0;i<size;i++){	    
		pix=image.getXYZ(i);	
		if(pix>0){ 		   
			Pix_array[pix]++;		  
			Min_pix=Min_pix<pix?Min_pix:pix;		   
			Max_pix=Max_pix>pix?Max_pix:pix;
		}
	}
	//ֱ��ͼƽ��
	for(i=Min_pix;i<=Max_pix;i++){
        Pix_val=0.0;
		for(j=-2;j<=2;j++){
            k=i+j;
			k=k<Min_pix?Min_pix:k;//�½�Խ�紦��
			k=k>Max_pix?Max_pix:k;//�Ͻ�Խ�紦��
			Pix_val+=Pix_array[k];
        }
		Pix_array[i]=(int)(Pix_val/5.0+0.5);
	}
	k=0;
	do{
		t=Otsu1(image,Min_pix,Max_pix);//��÷�ֵ	
		pix_number1=0.0;
		Pix_val=0.0;
		for(m=Min_pix;m<=t;m++){
			pix_number1+=Pix_array[m];//ǰ�����ص�ͳ��
			Pix_val+=m*Pix_array[m];
		}
		u1=Pix_val/pix_number1;//����ǰ����ƽ���Ҷ�
		pix_number2=0.0;
		Pix_val=0.0;
		for(;m<=Max_pix;m++){
			pix_number2+=Pix_array[m];//�������ص�ͳ��
			Pix_val+=m*Pix_array[m];
		}
		u2=Pix_val/pix_number2;//���㱳����ƽ���Ҷ�
		S=(pix_number1+pix_number2)*fabs(u1-u2)/fabs(pix_number1-pix_number2);
		if(S<s) break;
		else s=S;
		Min_pix=t;				
	}while(k++<10);//��������
	return t;
}
int OTSU::Otsu_2D(Raw image){//��άOTSU�㷨
	double w1;//ǰ�������ص���ռ����ͼ��ı���
	double w1_k;//ǰ����k�������ص���ռ����ͼ��ı���
	double u1;//ǰ����ƽ���Ҷ�
	double u1_k;//ǰ����k����ƽ���Ҷ�
	double w2;//���������ص���ռ����ͼ��ı���
	double w2_k;//������k�������ص���ռ����ͼ��ı���
	double u2;//������ƽ���Ҷ�
	double u2_k;//������k����ƽ���Ҷ�
	double s=0.0,M_s=0.0;//��䷽��
	int t;//��ֵ

	int i,j,k,l,m,n,x,y,z,X_VALUE,Y_VALUE,Z_VALUE,size,pix,pix_k,Min_pix,Max_pix;
	X_VALUE=image.getXsize();//���x�᷽��Ĵ�С
	Y_VALUE=image.getYsize();//���y����Ĵ�С
	Z_VALUE=image.getZsize();//���z����Ĵ�С
	size=X_VALUE*Y_VALUE*Z_VALUE;//���ͼ��Ĵ�С
	Min_pix=255;//������Сֵ��ʼ��
	Max_pix=0;//�������ֵ��ʼ��
	
	int Pix_Array[256][256]={{0,0}};//����ֱ��ͼ����
	double pix_number;//���ص�ͳ��,pix_number_k
	double Pix_val,Pix_val_k;//����ֵͳ��
	// ����ֱ��ͼ����ͳ�Ƴ����ֵ����Сֵ
	for(z=0;z<Z_VALUE;z++){//����Z��
		for(y=0;y<Y_VALUE;y++){//����Y��
			for(x=0;x<X_VALUE;x++){//����X��
			    Pix_val_k=0.0;
		        pix=image.getXYZ(z*X_VALUE*Y_VALUE+y*X_VALUE+x);
		        Min_pix=Min_pix<pix?Min_pix:pix;//ͳ����Сֵ
		        Max_pix=Max_pix>pix?Max_pix:pix;//ͳ�����ֵ
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
				pix_k=(int)(Pix_val_k/27.0+0.5);//k��������ƽ��ֵ
		        Pix_Array[pix+128][pix_k+128]++;//���������������СֵΪ-128
			}
		}
	}
	Min_pix+=128;//���������������СֵΪ-128
	Max_pix+=128;//���������������СֵΪ-128
	t=Min_pix;
	//�������ļ�
	for(k=Min_pix;k<Max_pix;k++){
		for(l=Min_pix;l<=Max_pix;l++){
		    pix_number=0.0;
			//pix_number_k=0.0;
		    Pix_val=0.0;
			Pix_val_k=0.0;
			//ͳ��ǰ�����ص���ռ����ͼ��ı���
			for(i=Min_pix;i<=k;i++){
				for(j=Min_pix;j<=l;j++){
				   pix_number+=Pix_Array[i][j];
				   Pix_val+=i*Pix_Array[i][j];
				   Pix_val_k+=j*Pix_Array[i][j];
				}
			}
			w1=pix_number/size;//ǰ�����ص���ռ����ͼ��ı���
			w1_k=w1;
			u1=Pix_val/pix_number;//����ǰ����ƽ���Ҷ�
			u1_k=Pix_val_k/pix_number;//����ǰ��k�����ƽ���Ҷ�

			pix_number=0.0;
			//pix_number_k=0.0;
		    Pix_val=0.0;
			Pix_val_k=0.0;
			//ͳ�Ʊ������ص���ռ����ͼ��ı���
			for(;i<=Max_pix;i++){
				for(;j<=Max_pix;j++){
				   pix_number+=Pix_Array[i][j];
				   Pix_val+=i*Pix_Array[i][j];
				   Pix_val_k+=j*Pix_Array[i][j];
				}
			}
			//w2=pix_number/SIZE;//�������ص���ռ����ͼ��ı���
			//w2_k=pix_number/SIZE;
			w2=1-w1;
			w2_k=w2;
			u2=Pix_val/pix_number;//���㱳����ƽ���Ҷ�
			u2_k=Pix_val_k/pix_number;//���㱳��k�����ƽ���Ҷ�
		    s=w1*w2*(u1-u2)*(u1-u2)+w1_k*w2_k*(u1_k-u2_k)*(u1_k-u2_k);
		    if(M_s<s){
		       M_s=s;
			   t=(int)(w2*k+w1*l);
		    }
		}
	 }
     return t-128;
}
int OTSU::Otsu_2D_Step(Raw image){//�Ľ��Ķ�άOTSU�㷨
	double w1;//ǰ�������ص���ռ����ͼ��ı���
	double w1_k;//ǰ����k�������ص���ռ����ͼ��ı���
	double u1;//ǰ����ƽ���Ҷ�
	double u1_k;//ǰ����k����ƽ���Ҷ�
	double w2;//���������ص���ռ����ͼ��ı���
	double w2_k;//������k�������ص���ռ����ͼ��ı���
	double u2;//������ƽ���Ҷ�
	double u2_k;//������k����ƽ���Ҷ�
	double s=0.0,M_s=0.0;//��䷽��
	int t;//��ֵ
	int step;//����
	int i,j,k,l,x,y,z,m,n,X_VALUE,Y_VALUE,Z_VALUE,size,pix,pix_k,Min_pix,Max_pix;
	X_VALUE=image.getXsize();//���x�᷽��Ĵ�С
	Y_VALUE=image.getYsize();//���y����Ĵ�С
	Z_VALUE=image.getZsize();//���z����Ĵ�С
	size=X_VALUE*Y_VALUE*Z_VALUE;//���ͼ��Ĵ�С
	Min_pix=255;//������Сֵ��ʼ��
	Max_pix=0;//�������ֵ��ʼ��

	int Pix_Array[256][256]={{0,0}};//����ֱ��ͼ����
	double pix_number;//���ص�ͳ��,pix_number_k
	double Pix_val,Pix_val_k;//����ֵͳ��
	// ����ֱ��ͼ����ͳ�Ƴ����ֵ����Сֵ
	for(z=0;z<Z_VALUE;z++){//����Z��
		for(y=0;y<Y_VALUE;y++){//����Y��
			for(x=0;x<X_VALUE;x++){//����X��
			    Pix_val_k=0.0;
		        pix=image.getXYZ(z*X_VALUE*Y_VALUE+y*X_VALUE+x);
		        Min_pix=Min_pix<pix?Min_pix:pix;//ͳ����Сֵ
		        Max_pix=Max_pix>pix?Max_pix:pix;//ͳ�����ֵ
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
				pix_k=(int)(Pix_val_k/27.0+0.5);//k��������ƽ��ֵ
		        Pix_Array[pix+128][pix_k+128]++;//���������������СֵΪ-128
			}
		}
	}
	Min_pix+=128;//���������������СֵΪ-128
	Max_pix+=128;//���������������СֵΪ-128
	t=Min_pix;
	step=(Max_pix-Min_pix)/10;//���㲽��
	//Ѱ�����ŷ�ֵ����
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
		w1=pix_number/size;//ǰ�����ص���ռ����ͼ��ı���
		w1_k=w1;
		u1=Pix_val/pix_number;//����ǰ����ƽ���Ҷ�
		u1_k=Pix_val_k/pix_number;//����ǰ��k�����ƽ���Ҷ�
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
		w2=pix_number/size;//�������ص���ռ����ͼ��ı���
		w2_k=w2;
		if(pix_number){
		   u2=Pix_val/pix_number;//���㱳����ƽ���Ҷ�
		   u2_k=Pix_val_k/pix_number;//���㱳��k�����ƽ���Ҷ�
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
	Min_pix=Min_pix<m?Min_pix:m;//���»�������
	Max_pix=Max_pix>n?n:Max_pix;//���»�������
	//�ٴμ������ļ�
	M_s=0.0;
	for(k=Min_pix;k<=Max_pix;k++){
		for(l=Min_pix;l<=Max_pix;l++){
		    pix_number=0.0;
		    Pix_val=0.0;
			Pix_val_k=0.0;
			//ͳ��ǰ�����ص���ռ����ͼ��ı���
			for(i=Min_pix;i<=k;i++){
				for(j=Min_pix;j<=l;j++){
				   pix_number+=Pix_Array[i][j];
				   Pix_val+=i*Pix_Array[i][j];
				   Pix_val_k+=j*Pix_Array[i][j];
				}
			}
			w1=pix_number/size;//ǰ�����ص���ռ����ͼ��ı���
			w1_k=w1;
			u1=Pix_val/pix_number;//����ǰ����ƽ���Ҷ�
			u1_k=Pix_val_k/pix_number;//����ǰ��k�����ƽ���Ҷ�

			pix_number=0.0;
		    Pix_val=0.0;
			Pix_val_k=0.0;
			//ͳ�Ʊ������ص���ռ����ͼ��ı���
			for(;i<=Max_pix;i++){
				for(;j<=Max_pix;j++){
				   pix_number+=Pix_Array[i][j];
				   Pix_val+=i*Pix_Array[i][j];
				   Pix_val_k+=j*Pix_Array[i][j];
				}
			}
			w2=1-w1;
			w2_k=w2;
			u2=Pix_val/pix_number;//���㱳����ƽ���Ҷ�
			u2_k=Pix_val_k/pix_number;//���㱳��k�����ƽ���Ҷ�
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
    double s=0.0,M_s=0.0;//��䷽��
	double u1;//ǰ����ƽ���Ҷ�
	double s1;//ǰ�������ڷ���
	double u2;//������ƽ���Ҷ�
	double s2;//���������ڷ���
	int t;//��ֵ

	int i,j,k,l,size,pix,Min_pix,Max_pix;
	size=image.size();//���ͼ��Ĵ�С
	Min_pix=255;//������Сֵ��ʼ��
	Max_pix=0;//�������ֵ��ʼ��
	l=t_number;//��¼��ֵ����

	int Pix_array[256]={0};//ֱ��ͼ
	double pix_number1,pix_number2;//���ص�ͳ��
	double Pix_val;//����ֵͳ��
	// ����ֱ��ͼ����ͳ�����ֵ����Сֵ
	for(i=0;i<size;i++){
		pix=image.getXYZ(i);	
		Pix_array[pix]++;
		Min_pix=Min_pix<pix?Min_pix:pix;
		Max_pix=Max_pix>pix?Max_pix:pix;
	}
	t=Min_pix;
	//ֱ��ͼƽ��
	for(i=Min_pix;i<=Max_pix;i++){
        Pix_val=0.0;
		for(j=-2;j<=2;j++){
            k=i+j;
			k=k<Min_pix?Min_pix:k;//�½�Խ�紦��
			k=k>Max_pix?Max_pix:k;//�Ͻ�Խ�紦��
			Pix_val+=Pix_array[k];
        }
		Pix_array[i]=(int)(Pix_val/5.0+0.5);
	}
	ss->Min_val=Min_pix;//����ֵ�½�
	ss->Max_val=Max_pix;//����ֵ�Ͻ�
	for(i=0;i<t_number;i++){//�Է����ֵ��
		(ss+i)->s=0;
	}
	array.clear();
	do{//�෧ֵOTSU�ָ��㷨
		//����ֵOtsu�㷨����
		//cout<<"Min_pix="<< Min_pix<<"  Max_pix="<<Max_pix<<endl;
		t=Otsu1(image,Min_pix,Max_pix);//������ָֵt
        cout<<"t_number="<<l<<"  t="<<t<<endl;
		l--;
		array.push_back(t);//��ȡ��ֵ
		if(l){
		   //�������ڷ���
		   pix_number1=0.0;//���ظ���
		   Pix_val=0.0;//����ֵ
		   for(i=Min_pix;i<=t;i++){
			   pix_number1+=Pix_array[i];
			   Pix_val+=i*Pix_array[i];
		   }
		   u1=Pix_val/pix_number1;//����ǰ����ƽ���Ҷ�
		   s1=0.0;
		   for(i=Min_pix;i<=t;i++){
               s1+=Pix_array[i]*(i-u1)*(i-u1)/pix_number1;//����ǰ������
		   }

		   pix_number2=0.0;
		   Pix_val=0.0;
		   for(;i<=Max_pix;i++){
			   pix_number2+=Pix_array[i];
			   Pix_val+=i*Pix_array[i];
		   }
		   u2=Pix_val/pix_number2;//���㱳����ƽ���Ҷ�
		   s2=0.0;
		   for(i=t+1;i<=Max_pix;i++){
               s2+=Pix_array[i]*(i-u2)*(i-u2)/pix_number2;//���㱳������
		   }
		   //���淽�������½�
		   cout<<"s1="<<s1<<" s2="<<s2<<endl;
		   for(i=0;i<t_number;i++,ss++){
			    if(ss->Min_val==Min_pix && ss->Max_val==Max_pix){
			       ss->Max_val=t;//�ı��Ͻ�
			       ss->s=s1;//�ı䷽���С
				   break;
		        }
		   }
		   ss-=i;//���ص�ָ��ͷλ��
		   k=0;
		   while(ss->s){
			   ss++;//���ҷ���Ϊ���ָ��
			   k++;
		   }
		   //��ֵ
		   cout<<"ss->s="<<ss->s<<endl;
		   ss->Min_val=t;//
		   ss->Max_val=Max_pix;
		   ss->s=s2;
		   cout<<"ss->s="<<ss->s<<endl;
		   ss-=k;//���ص�ָ��ͷλ��
		   k=Max_number(ss,t_number);//���㷽������λ��
		   cout<<"k="<<k<<endl;
		   Min_pix=(ss+k)->Min_val;
		   Max_pix=(ss+k)->Max_val;
		}
	}while(l);
	delete(ss);
}
void OTSU::Otsu_MultiVal(Raw image){
	int t;//��ֵ
	int i,j,k,size,Min_val,t_number,pix,Min_pix,Max_pix;
	size=image.size();//���ͼ��Ĵ�С
	t_number=0;//��¼��ֵ�ĸ���
	Min_pix=255;//������Сֵ��ʼ��
	Max_pix=0;//�������ֵ��ʼ��
	Min_val=(int)(size/250);//�������Сֵ:�Դ���ƽ��ֵ 
	int temp_tvalue[256]={-129};//���沨��
	int Pix_array[256]={0};//ֱ��ͼ
	double Pix_val;//����ֵͳ��
	// ����ֱ��ͼ
	for(i=0;i<size;i++){  
		pix=image.getXYZ(i);
		Pix_array[pix]++;
		Min_pix=Min_pix<pix?Min_pix:pix;
		Max_pix=Max_pix>pix?Max_pix:pix;
	}
	t=Min_pix;//����ֵ
	for(i=Min_pix;i<=Max_pix;i++){
        Pix_val=0.0;
		for(j=-3;j<=3;j++){
            k=i+j;
			k=k<Min_pix?Min_pix:k;//�½�Խ�紦��
			k=k>Max_pix?Max_pix:k;//�Ͻ�Խ�紦��
			Pix_val+=Pix_array[k];
        }
		Pix_array[i]=(int)(Pix_val/5.0+0.5);
	}
	//���Ҳ���
	k=0;//��¼����ĸ���
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
	//��ȥα��
	for(i=0;i<k-1;i++){
		if(abs(temp_tvalue[i]-temp_tvalue[i+1]) < 10){//�ϲ�α��
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
	//�����ڵ������������з�ֵ�ָ�
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
	k=array.size();//��ֵ����
	if(k>1){
		step=(int)(250/k);//����
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
	fclose(f);//�ر��ļ�
}

//��ά���ݴ���
/*
int OTSU::Otsu(Raw2D image){
	double w1;//ǰ�������ص���ռ����ͼ��ı���
	double u1;//ǰ����ƽ���Ҷ�
	double w2;//���������ص���ռ����ͼ��ı���
	double u2;//������ƽ���Ҷ�
	double s=0.0,M_s=0.0;//��䷽��
	int t;//��ֵ

	int i,j,k,pix,size,Min_pix,Max_pix;
	size=image.size();//���ͼ��Ĵ�С
	Min_pix=255;//������Сֵ��ʼ��
	Max_pix=0;//�������ֵ��ʼ��

	int Pix_array[256]={0};
	double pix_number;//���ص�ͳ��
	double Pix_val;//����ֵͳ��
	// ����ֱ��ͼ����ͳ�Ƴ����ֵ����Сֵ
	for(i=0;i<size;i++){    
		pix=image.getXY(i);
		Pix_array[pix]++;
		Min_pix=Min_pix<pix?Min_pix:pix;
		Max_pix=Max_pix>pix?Max_pix:pix;
	}
	cout<<Min_pix<<"  "<<Max_pix<<endl;//������ֵ����Сֵ
	t=Min_pix;
	//ֱ��ͼƽ��
	for(i=Min_pix;i<=Max_pix;i++){
        Pix_val=0.0;
		for(j=-2;j<=2;j++){
            k=i+j;
			k=k<Min_pix?Min_pix:k;//�½�Խ�紦��
			k=k>Max_pix?Max_pix:k;//�Ͻ�Խ�紦��
			Pix_val+=Pix_array[k];
        }
		Pix_array[i]=(int)(Pix_val/5.0+0.5);
	}
	cout<<Min_pix<<"  "<<Max_pix<<endl;//�ٴ�������ֵ����Сֵ
	for(k=Min_pix;k<Max_pix;k++){
		//ͳ��ǰ�����ص���ռ����ͼ��ı���
		pix_number=0.0;
		Pix_val=0.0;
		for(i=Min_pix;i<=k;i++){
			pix_number+=Pix_array[i];
			Pix_val+=i*Pix_array[i];
		}
		w1=pix_number/size;//ǰ�����ص���ռ����ͼ��ı���
		//����ǰ����ƽ���Ҷ�
		if(pix_number){
			u1=Pix_val/pix_number;
		}
		else u1=Min_pix;
		w2=1-w1;//�����ص���ռ����ͼ��ı���
		pix_number=0.0;
		Pix_val=0.0;
		for(;i<=Max_pix;i++){
			pix_number+=Pix_array[i];
			Pix_val+=i*Pix_array[i];
		}
		//���㱳����ƽ���Ҷ�
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
	double w1;//ǰ�������ص���ռ����ͼ��ı���
	double u1;//ǰ����ƽ���Ҷ�
	double w2;//���������ص���ռ����ͼ��ı���
	double u2;//������ƽ���Ҷ�
	double s=0.0,M_s=0.0;//��䷽��
	int t;//��ֵ

	int i,j,k,pix,size;
	size=image.size();//���ͼ��Ĵ�С
	int Pix_array[256]={0};
	double pix_number1,pix_number2,pixs;//���ص�ͳ��
	double Pix_val;//����ֵͳ��
	// ����ֱ��ͼ
	for(i=0;i<size;i++){
		pix=image.getXY(i);
		Pix_array[pix]++;
	}
	//ֱ��ͼƽ��
	for(i=Min_pix;i<=Max_pix;i++){
        Pix_val=0.0;
		for(j=-2;j<=2;j++){
            k=i+j;
			k=k<Min_pix?Min_pix:k;//�½�Խ�紦��
			k=k>Max_pix?Max_pix:k;//�Ͻ�Խ�紦��
			Pix_val+=Pix_array[k];
        }
		Pix_array[i]=(int)(Pix_val/5.0+0.5);
	}
	for(k=Min_pix;k<Max_pix;k++){
		    //ͳ��ǰ�����ص���ռ����ͼ��ı���
		    pix_number1=0.0;
		    Pix_val=0.0;
		    for(i=Min_pix;i<=k;i++){
			    pix_number1+=Pix_array[i];
			    Pix_val+=i*Pix_array[i];
		    }		    
		    u1=Pix_val/pix_number1;//����ǰ����ƽ���Ҷ�	    
		    pix_number2=0.0;
		    Pix_val=0.0;
		    for(;i<=Max_pix;i++){
			    pix_number2+=Pix_array[i];
			    Pix_val+=i*Pix_array[i];
		    }
			pixs=pix_number1+pix_number2;//�ܵ����ص���
			w1=pix_number1/pixs;//ǰ�����ص���ռ����ͼ��ı���
			w2=1-w1;//�����ص���ռ����ͼ��ı���
		    u2=Pix_val/pix_number2;//���㱳����ƽ���Ҷ�
            s=w1*w2*(u1-u2)*(u1-u2); 
		    if(M_s<s){
			   M_s=s;
			   t=k;
		    }
	    }//������ָֵt
  return t;
}
int OTSU::Otsu_Step(Raw2D image){
	int step;//����
	double w1;//ǰ�������ص���ռ����ͼ��ı���
	double u1;//ǰ����ƽ���Ҷ�
	double w2;//���������ص���ռ����ͼ��ı���
	double u2;//������ƽ���Ҷ�
	double s=0.0,M_s=0.0;//��䷽��
	int t;//��ֵ

	int i,j,k,pix,size,Min_pix,Max_pix;
	size=image.size();//���ͼ��Ĵ�С
	Min_pix=255;//������Сֵ��ʼ��
	Max_pix=0;//�������ֵ��ʼ��

	int Pix_array[256]={0};
	double pix_number;//���ص�ͳ��
	double Pix_val;//����ֵͳ��
	// ����ֱ��ͼ,��ͳ�Ƴ����ֵ����Сֵ
	for(i=0;i<size;i++){
		pix=image.getXY(i);
		Pix_array[pix]++;
		Min_pix=Min_pix<pix?Min_pix:pix;
		Max_pix=Max_pix>pix?Max_pix:pix;
	}
	//ֱ��ͼƽ��
	for(i=Min_pix;i<=Max_pix;i++){
        Pix_val=0.0;
		for(j=-2;j<=2;j++){
            k=i+j;
			k=k<Min_pix?Min_pix:k;//�½�Խ�紦��
			k=k>Max_pix?Max_pix:k;//�Ͻ�Խ�紦��
			Pix_val+=Pix_array[k];
        }
		Pix_array[i]=(int)(Pix_val/5.0+0.5);
	}
	step=(Max_pix-Min_pix)/20;//���㲽��
	for(k=0;k<20;k++){
		//ͳ��ǰ�����ص���ռ����ͼ��ı���
		pix_number=0.0;
		Pix_val=0.0;
		for(i=Min_pix;i<=(k*step+Min_pix);i++){
			pix_number+=Pix_array[i];
			Pix_val+=i*Pix_array[i];
		}
		w1=pix_number/size;//ǰ�����ص���ռ����ͼ��ı���
		u1=Pix_val/pix_number;//����ǰ����ƽ���Ҷ�
		w2=1-w1;//�����ص���ռ����ͼ��ı���
		pix_number=0.0;
		Pix_val=0.0;
		for(;i<=Max_pix;i++){
			pix_number+=Pix_array[i];
			Pix_val+=i*Pix_array[i];
		}
		u2=Pix_val/pix_number;//���㱳����ƽ���Ҷ�
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
		    //ͳ��ǰ�����ص���ռ����ͼ��ı���
		    pix_number=0.0;
		    Pix_val=0.0;
		    for(i=Min_pix;i<=k;i++){
			    pix_number+=Pix_array[i];
			    Pix_val+=i*Pix_array[i];
		    }
		    w1=pix_number/size;//ǰ�����ص���ռ����ͼ��ı���
		    u1=Pix_val/pix_number;//����ǰ����ƽ���Ҷ�
		    w2=1-w1;//�����ص���ռ����ͼ��ı���
		    pix_number=0.0;
		    Pix_val=0.0;
		    for(;i<=Max_pix;i++){
			    pix_number+=Pix_array[i];
			    Pix_val+=i*Pix_array[i];
		    }
		    u2=Pix_val/pix_number;//���㱳����ƽ���Ҷ�
            s=w1*w2*(u1-u2)*(u1-u2);
		    if(M_s<s){
			  M_s=s;
			  t=k;
		    }
	    }
  return t;
}
int OTSU::Otsu_Recursion(Raw2D image){
	double u1;//ǰ����ƽ���Ҷ�
	double pix_number1;//ǰ�����ص�ͳ��
	double u2;//��������ƽ���Ҷ�
	double pix_number2;//�������ص�ͳ��
	int t;//��ֵ
	double s=0,S=0;//����ֵ
	double Pix_val;//����ֵͳ��
	int i,j,k,m,pix,size,Min_pix,Max_pix;
	size=image.size();//���ͼ��Ĵ�С
	Min_pix=255;
	Max_pix=0;
	int Pix_array[256]={0};
	//����ֱ��ͼ
	for(i=0;i<size;i++){	    
		pix=image.getXY(i);	
		if(pix>0){ 		   
			Pix_array[pix]++;		  
			Min_pix=Min_pix<pix?Min_pix:pix;		   
			Max_pix=Max_pix>pix?Max_pix:pix;
		}
	}
	//ֱ��ͼƽ��
	for(i=Min_pix;i<=Max_pix;i++){
        Pix_val=0.0;
		for(j=-2;j<=2;j++){
            k=i+j;
			k=k<Min_pix?Min_pix:k;//�½�Խ�紦��
			k=k>Max_pix?Max_pix:k;//�Ͻ�Խ�紦��
			Pix_val+=Pix_array[k];
        }
		Pix_array[i]=(int)(Pix_val/5.0+0.5);
	}
	k=0;
	do{
		t=Otsu1(image,Min_pix,Max_pix);//��÷�ֵ	
		pix_number1=0.0;
		Pix_val=0.0;
		for(m=Min_pix;m<=t;m++){
			pix_number1+=Pix_array[m];//ǰ�����ص�ͳ��
			Pix_val+=m*Pix_array[m];
		}
		u1=Pix_val/pix_number1;//����ǰ����ƽ���Ҷ�
		pix_number2=0.0;
		Pix_val=0.0;
		for(;m<=Max_pix;m++){
			pix_number2+=Pix_array[m];//�������ص�ͳ��
			Pix_val+=m*Pix_array[m];
		}
		u2=Pix_val/pix_number2;//���㱳����ƽ���Ҷ�
		S=(pix_number1+pix_number2)*fabs(u1-u2)/fabs(pix_number1-pix_number2);
		if(S<s) break;
		else s=S;
		Min_pix=t;				
	}while(k++<10);//��������
	return t;
}
int OTSU::Otsu_2D(Raw2D image){//��άOTSU�㷨
	double w1;//ǰ�������ص���ռ����ͼ��ı���
	double w1_k;//ǰ����k�������ص���ռ����ͼ��ı���
	double u1;//ǰ����ƽ���Ҷ�
	double u1_k;//ǰ����k����ƽ���Ҷ�
	double w2;//���������ص���ռ����ͼ��ı���
	double w2_k;//������k�������ص���ռ����ͼ��ı���
	double u2;//������ƽ���Ҷ�
	double u2_k;//������k����ƽ���Ҷ�
	double s=0.0,M_s=0.0;//��䷽��
	int t;//��ֵ

	int i,j,k,l,x,y,z,X_VALUE,Y_VALUE,size,pix,pix_k,Min_pix,Max_pix;
	X_VALUE=image.getXsize();//���x�᷽��Ĵ�С
	Y_VALUE=image.getYsize();//���y����Ĵ�С
	size=X_VALUE*Y_VALUE;//���ͼ��Ĵ�С
	Min_pix=255;//������Сֵ��ʼ��
	Max_pix=0;//�������ֵ��ʼ��
	
	int Pix_Array[256][256]={{0,0}};//����ֱ��ͼ����
	double pix_number;//���ص�ͳ��,pix_number_k
	double Pix_val,Pix_val_k;//����ֵͳ��
	// ����ֱ��ͼ����ͳ�Ƴ����ֵ����Сֵ
	for(i=0;i<size;i++){
		Pix_val_k=0.0;
		pix=image.getXY(i);
		Min_pix=Min_pix<pix?Min_pix:pix;//ͳ����Сֵ
		Max_pix=Max_pix>pix?Max_pix:pix;//ͳ�����ֵ
		for(j=-1;j<2;j++){//��Y����ƽ��	
			y=i/X_VALUE+j;//Y��	
			y=y<0?0:y;
			y=y>(Y_VALUE-1)?(Y_VALUE-1):y;
			for(k=-1;k<2;k++){//��X����ƽ��   
				x=i%X_VALUE+k;//X��
				x=x<0?0:x;
				x=x>(X_VALUE-1)?(X_VALUE-1):x;
				Pix_val_k+=image.getXY(y*X_VALUE+x);//����9�����ص�ֵ
			}
		}
		pix_k=(int)(Pix_val_k/9.0+0.5);//k��������ƽ��ֵ
		Pix_Array[pix+128][pix_k+128]++;//���������������СֵΪ-128
	}
	Min_pix+=128;//���������������СֵΪ-128
	Max_pix+=128;//���������������СֵΪ-128
	t=Min_pix;
	//�������ļ�
	for(k=Min_pix;k<Max_pix;k++){
		for(l=Min_pix;l<=Max_pix;l++){
		    pix_number=0.0;
			//pix_number_k=0.0;
		    Pix_val=0.0;
			Pix_val_k=0.0;
			//ͳ��ǰ�����ص���ռ����ͼ��ı���
			for(i=Min_pix;i<=k;i++){
				for(j=Min_pix;j<=l;j++){
				   pix_number+=Pix_Array[i][j];
				   Pix_val+=i*Pix_Array[i][j];
				   Pix_val_k+=j*Pix_Array[i][j];
				}
			}
			w1=pix_number/size;//ǰ�����ص���ռ����ͼ��ı���
			w1_k=w1;
			u1=Pix_val/pix_number;//����ǰ����ƽ���Ҷ�
			u1_k=Pix_val_k/pix_number;//����ǰ��k�����ƽ���Ҷ�

			pix_number=0.0;
			//pix_number_k=0.0;
		    Pix_val=0.0;
			Pix_val_k=0.0;
			//ͳ�Ʊ������ص���ռ����ͼ��ı���
			for(;i<=Max_pix;i++){
				for(;j<=Max_pix;j++){
				   pix_number+=Pix_Array[i][j];
				   Pix_val+=i*Pix_Array[i][j];
				   Pix_val_k+=j*Pix_Array[i][j];
				}
			}
			//w2=pix_number/SIZE;//�������ص���ռ����ͼ��ı���
			//w2_k=pix_number/SIZE;
			w2=1-w1;
			w2_k=w2;
			u2=Pix_val/pix_number;//���㱳����ƽ���Ҷ�
			u2_k=Pix_val_k/pix_number;//���㱳��k�����ƽ���Ҷ�
		    s=w1*w2*(u1-u2)*(u1-u2)+w1_k*w2_k*(u1_k-u2_k)*(u1_k-u2_k);
		    if(M_s<s){
		       M_s=s;
			   t=(int)(w2*k+w1*l);
		    }
		}
	 }
     return t-128;
}
int OTSU::Otsu_2D_Step(Raw2D image){//�Ľ��Ķ�άOTSU�㷨
	double w1;//ǰ�������ص���ռ����ͼ��ı���
	double w1_k;//ǰ����k�������ص���ռ����ͼ��ı���
	double u1;//ǰ����ƽ���Ҷ�
	double u1_k;//ǰ����k����ƽ���Ҷ�
	double w2;//���������ص���ռ����ͼ��ı���
	double w2_k;//������k�������ص���ռ����ͼ��ı���
	double u2;//������ƽ���Ҷ�
	double u2_k;//������k����ƽ���Ҷ�
	double s=0.0,M_s=0.0;//��䷽��
	int t;//��ֵ
	int step;//����
	int i,j,k,l,x,y,z,m,n,X_VALUE,Y_VALUE,size,pix,pix_k,Min_pix,Max_pix;
	X_VALUE=image.getXsize();//���x�᷽��Ĵ�С
	Y_VALUE=image.getYsize();//���y����Ĵ�С
	size=X_VALUE*Y_VALUE;//���ͼ��Ĵ�С
	Min_pix=255;//������Сֵ��ʼ��
	Max_pix=0;//�������ֵ��ʼ��

	int Pix_Array[256][256]={{0,0}};//����ֱ��ͼ����
	double pix_number;//���ص�ͳ��,pix_number_k
	double Pix_val,Pix_val_k;//����ֵͳ��
	// ����ֱ��ͼ����ͳ�Ƴ����ֵ����Сֵ
	for(i=0;i<size;i++){
		Pix_val_k=0.0;
		pix=image.getXY(i);
		Min_pix=Min_pix<pix?Min_pix:pix;//ͳ����Сֵ
		Max_pix=Max_pix>pix?Max_pix:pix;//ͳ�����ֵ
		for(j=-1;j<2;j++){//��Y����ƽ��	
			y=i/X_VALUE+j;//Y��	
			y=y<0?0:y;
			y=y>(Y_VALUE-1)?(Y_VALUE-1):y;
			for(k=-1;k<2;k++){//��X����ƽ��   
				x=i%X_VALUE+k;//X��
				x=x<0?0:x;
				x=x>(X_VALUE-1)?(X_VALUE-1):x;
				Pix_val_k+=image.getXY(y*X_VALUE+x);//����9�����ص�ֵ
			}
		}
		pix_k=(int)(Pix_val_k/9.0+0.5);//k��������ƽ��ֵ
		Pix_Array[pix+128][pix_k+128]++;//���������������СֵΪ-128
	}
	Min_pix+=128;//���������������СֵΪ-128
	Max_pix+=128;//���������������СֵΪ-128
	t=Min_pix;
	step=(Max_pix-Min_pix)/10;//���㲽��
	//Ѱ�����ŷ�ֵ����
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
		w1=pix_number/size;//ǰ�����ص���ռ����ͼ��ı���
		w1_k=w1;
		u1=Pix_val/pix_number;//����ǰ����ƽ���Ҷ�
		u1_k=Pix_val_k/pix_number;//����ǰ��k�����ƽ���Ҷ�
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
		w2=pix_number/size;//�������ص���ռ����ͼ��ı���
		w2_k=w2;
		if(pix_number){
		   u2=Pix_val/pix_number;//���㱳����ƽ���Ҷ�
		   u2_k=Pix_val_k/pix_number;//���㱳��k�����ƽ���Ҷ�
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
	Min_pix=Min_pix<m?Min_pix:m;//���»�������
	Max_pix=Max_pix>n?n:Max_pix;//���»�������
	//�ٴμ������ļ�
	M_s=0.0;
	for(k=Min_pix;k<=Max_pix;k++){
		for(l=Min_pix;l<=Max_pix;l++){
		    pix_number=0.0;
		    Pix_val=0.0;
			Pix_val_k=0.0;
			//ͳ��ǰ�����ص���ռ����ͼ��ı���
			for(i=Min_pix;i<=k;i++){
				for(j=Min_pix;j<=l;j++){
				   pix_number+=Pix_Array[i][j];
				   Pix_val+=i*Pix_Array[i][j];
				   Pix_val_k+=j*Pix_Array[i][j];
				}
			}
			w1=pix_number/size;//ǰ�����ص���ռ����ͼ��ı���
			w1_k=w1;
			u1=Pix_val/pix_number;//����ǰ����ƽ���Ҷ�
			u1_k=Pix_val_k/pix_number;//����ǰ��k�����ƽ���Ҷ�

			pix_number=0.0;
		    Pix_val=0.0;
			Pix_val_k=0.0;
			//ͳ�Ʊ������ص���ռ����ͼ��ı���
			for(;i<=Max_pix;i++){
				for(;j<=Max_pix;j++){
				   pix_number+=Pix_Array[i][j];
				   Pix_val+=i*Pix_Array[i][j];
				   Pix_val_k+=j*Pix_Array[i][j];
				}
			}
			w2=1-w1;
			w2_k=w2;
			u2=Pix_val/pix_number;//���㱳����ƽ���Ҷ�
			u2_k=Pix_val_k/pix_number;//���㱳��k�����ƽ���Ҷ�
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
    double s=0.0,M_s=0.0;//��䷽��
	double u1;//ǰ����ƽ���Ҷ�
	double s1;//ǰ�������ڷ���
	double u2;//������ƽ���Ҷ�
	double s2;//���������ڷ���
	int t;//��ֵ

	int i,j,k,l,size,pix,Min_pix,Max_pix;
	size=image.size();//���ͼ��Ĵ�С
	Min_pix=255;//������Сֵ��ʼ��
	Max_pix=0;//�������ֵ��ʼ��
	l=t_number;//��¼��ֵ����

	int Pix_array[256]={0};//ֱ��ͼ
	double pix_number1,pix_number2;//���ص�ͳ��
	double Pix_val;//����ֵͳ��
	// ����ֱ��ͼ����ͳ�����ֵ����Сֵ
	for(i=0;i<size;i++){
		pix=image.getXY(i);	
		Pix_array[pix]++;
		Min_pix=Min_pix<pix?Min_pix:pix;
		Max_pix=Max_pix>pix?Max_pix:pix;
	}
	t=Min_pix;
	//ֱ��ͼƽ��
	for(i=Min_pix;i<=Max_pix;i++){
        Pix_val=0.0;
		for(j=-2;j<=2;j++){
            k=i+j;
			k=k<Min_pix?Min_pix:k;//�½�Խ�紦��
			k=k>Max_pix?Max_pix:k;//�Ͻ�Խ�紦��
			Pix_val+=Pix_array[k];
        }
		Pix_array[i]=(int)(Pix_val/5.0+0.5);
	}
	ss->Min_val=Min_pix;//����ֵ�½�
	ss->Max_val=Max_pix;//����ֵ�Ͻ�
	for(i=0;i<t_number;i++){//�Է����ֵ��
		(ss+i)->s=0;
	}
	do{//�෧ֵOTSU�ָ��㷨
		//����ֵOtsu�㷨����
		//cout<<"Min_pix="<< Min_pix<<"  Max_pix="<<Max_pix<<endl;
		t=Otsu1(image,Min_pix,Max_pix);//������ָֵt
        cout<<"t_number="<<l<<"  t="<<t<<endl;
		l--;
		array.push_back(t);//��ȡ��ֵ
		if(l){
		   //�������ڷ���
		   pix_number1=0.0;//���ظ���
		   Pix_val=0.0;//����ֵ
		   for(i=Min_pix;i<=t;i++){
			   pix_number1+=Pix_array[i];
			   Pix_val+=i*Pix_array[i];
		   }
		   u1=Pix_val/pix_number1;//����ǰ����ƽ���Ҷ�
		   s1=0.0;
		   for(i=Min_pix;i<=t;i++){
               s1+=Pix_array[i]*(i-u1)*(i-u1)/pix_number1;//����ǰ������
		   }

		   pix_number2=0.0;
		   Pix_val=0.0;
		   for(;i<=Max_pix;i++){
			   pix_number2+=Pix_array[i];
			   Pix_val+=i*Pix_array[i];
		   }
		   u2=Pix_val/pix_number2;//���㱳����ƽ���Ҷ�
		   s2=0.0;
		   for(i=t+1;i<=Max_pix;i++){
               s2+=Pix_array[i]*(i-u2)*(i-u2)/pix_number2;//���㱳������
		   }
		   //���淽�������½�
		   cout<<"s1="<<s1<<" s2="<<s2<<endl;
		   for(i=0;i<t_number;i++,ss++){
			    if(ss->Min_val==Min_pix && ss->Max_val==Max_pix){
			       ss->Max_val=t;//�ı��Ͻ�
			       ss->s=s1;//�ı䷽���С
				   break;
		        }
		   }
		   ss-=i;//���ص�ָ��ͷλ��
		   k=0;
		   while(ss->s){
			   ss++;//���ҷ���Ϊ���ָ��
			   k++;
		   }
		   //��ֵ
		   cout<<"ss->s="<<ss->s<<endl;
		   ss->Min_val=t;//
		   ss->Max_val=Max_pix;
		   ss->s=s2;
		   cout<<"ss->s="<<ss->s<<endl;
		   ss-=k;//���ص�ָ��ͷλ��
		   k=Max_number(ss,t_number);//���㷽������λ��
		   cout<<"k="<<k<<endl;
		   Min_pix=(ss+k)->Min_val;
		   Max_pix=(ss+k)->Max_val;
		}
	}while(l);
	delete(ss);
}
void OTSU::Otsu_MultiVal(Raw2D image){
	int t;//��ֵ
	int i,j,k,size,Min_val,t_number,pix,Min_pix,Max_pix;
	size=image.size();//���ͼ��Ĵ�С
	t_number=0;//��¼��ֵ�ĸ���
	Min_pix=255;//������Сֵ��ʼ��
	Max_pix=0;//�������ֵ��ʼ��
	Min_val=(int)(size/250);//�������Сֵ:�Դ���ƽ��ֵ 
	int temp_tvalue[256]={-129};//���沨��
	int Pix_array[256]={0};//ֱ��ͼ
	double Pix_val;//����ֵͳ��
	// ����ֱ��ͼ
	for(i=0;i<size;i++){  
		pix=image.getXY(i);
		Pix_array[pix]++;
		Min_pix=Min_pix<pix?Min_pix:pix;
		Max_pix=Max_pix>pix?Max_pix:pix;
	}
	t=Min_pix;//����ֵ
	for(i=Min_pix;i<=Max_pix;i++){
        Pix_val=0.0;
		for(j=-3;j<=3;j++){
            k=i+j;
			k=k<Min_pix?Min_pix:k;//�½�Խ�紦��
			k=k>Max_pix?Max_pix:k;//�Ͻ�Խ�紦��
			Pix_val+=Pix_array[k];
        }
		Pix_array[i]=(int)(Pix_val/5.0+0.5);
	}
	//���Ҳ���
	k=0;//��¼����ĸ���
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
	//��ȥα��
	for(i=0;i<k-1;i++){
		if(abs(temp_tvalue[i]-temp_tvalue[i+1]) < 10){//�ϲ�α��
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
	//�����ڵ������������з�ֵ�ָ�
	array.clear();//��մ���
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
	int size=image.size();//ͼ���С
	int step;//����
	int i,j,k;
	FILE *f=NULL;
	float val;
	k=array.size();//��ֵ����
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
	f=fopen("img\\Image.raw","wb");//���ļ�
	fwrite(image.getdata(),sizeof(unsigned char),size,f);
	fclose(f);//�ر��ļ�
}
*/