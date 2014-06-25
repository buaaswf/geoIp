#include <iostream>
#include <math.h>
#include<algorithm>
#include "vol_math_Otsu.h"

using namespace std;

int Max_number(struct Point_3 *p,int n){
    double Maxs=p->s;
	int m=0,number=0;
	//Ѱ����󷽲�
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

//three Dimensions data process
int OTSU::Otsu(Raw &image,int Min_pix,int Max_pix){
	double w1;//ǰ�������ص�����ռ����
	double u1;//ǰ����ƽ���Ҷ�
	double w2;//���������ص�����ռ����
	double u2;//������ƽ���Ҷ�
	double s=0.0,M_s=0.0;//��䷽��
	int t;//��ֵ
	int i,j,k,pix,size;//�����м����
	size=image.size();//���ͼ��Ĵ�С
	int Pix_array[256]={0};
	double pix_number1,pix_number2,pixs;//���ص�ͳ��
	double Pix_val;//����ֵͳ��
	// ����ֱ��ͼ
	for(i=0;i<size;i++){
		pix=(int)image.getXYZ(i);
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
		if(pix_number1) u1=Pix_val/pix_number1;//����ǰ����ƽ���Ҷ�	 
		else  u1=Min_pix; 
		pix_number2=0.0;
		Pix_val=0.0;
		for(;i<=Max_pix;i++){
			pix_number2+=Pix_array[i];
			Pix_val+=i*Pix_array[i];
		}
		pixs=pix_number1+pix_number2;//�ܵ����ص���
		w1=pix_number1/pixs;//ǰ�����ص�����ռ����
		w2=1-w1;//�����ص�����ռ����
		if(pix_number2) u2=Pix_val/pix_number2;//���㱳����ƽ���Ҷ�
		else u2=Max_pix;
		s=w1*w2*(u1-u2)*(u1-u2); //��䷽��
		if(M_s<s){  
			M_s=s;
			t=k;
		}
	}//������ָ���ֵt
	return t;
}
int OTSU::Otsu(Raw &image){
	int step;//����
	double w1;//ǰ�������ص���ռ����ͼ��ı���
	double u1;//ǰ����ƽ���Ҷ�
	double w2;//���������ص���ռ����ͼ��ı���
	double u2;//������ƽ���Ҷ�
	double s=0.0,M_s=0.0;//��䷽��
	int t;//��ֵ
	int i,j,k,pix,size,Min_pix,Max_pix,Min_val,Max_val;
	size=image.size();//���ͼ��Ĵ�С
	Min_pix=255;//������Сֵ��ʼ��
	Max_pix=0;//�������ֵ��ʼ��

	int Pix_array[256]={0};
	double pix_number;//���ص�ͳ��
	double Pix_val;//����ֵͳ��
	// ����ֱ��ͼ,��ͳ�Ƴ����ֵ����Сֵ
	for(i=0;i<size;i++){
		pix=(int)image.getXYZ(i);
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
	for(k=1;k<=20;k++){
		//ͳ��ǰ�����ص���ռ����ͼ��ı���
		pix_number=0.0;
		Pix_val=0.0;
		for(i=Min_pix;i<(k*step+Min_pix);i++){
			pix_number+=Pix_array[i];
			Pix_val+=i*Pix_array[i];
		}
		w1=pix_number/size;//ǰ�����ص���ռ����ͼ��ı���
		if(pix_number) u1=Pix_val/pix_number;//����ǰ����ƽ���Ҷ�
		else u1=Min_pix;
		w2=1-w1;//�����ص���ռ����ͼ��ı���
		pix_number=0.0;
		Pix_val=0.0;
		for(;i<=Max_pix;i++){
			pix_number+=Pix_array[i];
			Pix_val+=i*Pix_array[i];
		}
		if(pix_number) u2=Pix_val/pix_number;//���㱳����ƽ���Ҷ�
		else u2=Max_pix;
        s=w1*w2*(u1-u2)*(u1-u2);//��䷽��
		if(M_s<=s){
			M_s=s;
			t=k*step+Min_pix;
		}
	}	
	//��ȡ��tΪ�м����������step����Ϊ���ȵ�����
	i=t+step;
	Max_val=i>Max_pix?Max_pix:i;//�Ͻ�Խ�紦��
	j=t-step;
	Min_val=j<Min_pix?Min_pix:j;//�½�Խ�紦��
	
    for(k=Min_val;k<Max_val;k++){   
		//ͳ��ǰ�����ص���ռ����ͼ��ı���
		pix_number=0.0;
		Pix_val=0.0;
		for(i=Min_pix;i<=k;i++){  
			pix_number+=Pix_array[i];  
			Pix_val+=i*Pix_array[i];
		}
		w1=pix_number/size;//ǰ�����ص���ռ����ͼ��ı��� 
		if(pix_number) u1=Pix_val/pix_number;//����ǰ����ƽ���Ҷ�  
		else u1=Min_pix;
		w2=1-w1;//�����ص���ռ����ͼ��ı��� 
		pix_number=0.0;  
		Pix_val=0.0; 
		for(;i<=Max_pix;i++){    
			pix_number+=Pix_array[i];    
			Pix_val+=i*Pix_array[i];
		}
		if(pix_number) u2=Pix_val/pix_number;//���㱳����ƽ���Ҷ�    
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
		pix=(int)image.getXYZ(i);	
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
	int total =t_number;
	bool flag=false;
	arrays.clear();//����
	do{//����ֵOTSU�ָ��㷨
		//==========================
		// 
		if (  ProgressChanged != NULL )
		{
			ProgressChanged (0,1,(total-l+1)*100/total,flag);
		}


		//����ֵOtsu�㷨����
		t=Otsu(image,Min_pix,Max_pix);//������ָ���ֵt
		//cout<<"t="<<t<<endl;
		arrays.push_back((unsigned char)t);//������ֵ
		l--;
		if(l){
		   //������䷽��
		   pix_number1=0.0;//���ظ���
		   Pix_val=0.0;//����ֵ
		   for(i=Min_pix;i<=t;i++){
			   pix_number1+=Pix_array[i];
			   Pix_val+=i*Pix_array[i];
		   } 
		   s1=0.0;
		   if(pix_number1){
			   u1=Pix_val/pix_number1;//����ǰ����ƽ���Ҷ�
			   for(i=Min_pix;i<=t;i++){
				   s1+=Pix_array[i]*(i-u1)*(i-u1)/pix_number1;//����ǰ������
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
			   u2=Pix_val/pix_number2;//���㱳����ƽ���Ҷ�
			   for(i=t+1;i<=Max_pix;i++){
				   s2+=Pix_array[i]*(i-u2)*(i-u2)/pix_number2;//���㱳������
			   }
		   }
		   else u2=Max_pix;

		   //���淽�������½�
		   for(i=0;i<t_number;i++){
			    if((ss+i)->Min_val==Min_pix && (ss+i)->Max_val==Max_pix){
					if(s1<s2){
						(ss+i)->Min_val=t;//�ı��½�
						(ss+i)->s=s2;
					}
					else {
						(ss+i)->Max_val=t;//�ı��Ͻ�
						(ss+i)->s=s1;//�ı䷽���С
					}
				   break;
		        }
		   }  
		   i=0;
		   while((ss+(i++))->s){//���ҷ���Ϊ���ָ��
		   }
		   //��ֵ
		   j=i-1;
		   if(s1<s2){		
			   (ss+j)->Max_val=t;//�ı��Ͻ�	
			   (ss+j)->Min_val=Min_pix;//�ı��½�
			   (ss+j)->s=s1;//�ı䷽���С
		   }		
		   else {		
			   (ss+j)->Max_val=Max_pix;//�ı��Ͻ�	
			   (ss+j)->Min_val=t;//�ı��½�	
			   (ss+j)->s=s2;//�ı䷽���С	
		   }
		   k=Max_number(ss,t_number);//���㷽������λ��
		   Min_pix=(ss+k)->Min_val;
		   Max_pix=(ss+k)->Max_val;
		}
	}while(l);
	delete []ss;
}
void OTSU::Otsu_MultiVal(Raw &image){
	int t;//��ֵ
	int i,j,k,size,Min_val,t_number,pix,Min_pix,Max_pix;
	size=image.size();//���ͼ��Ĵ�С
	t_number=0;//��¼��ֵ�ĸ���
	Min_pix=255;//������Сֵ��ʼ��
	Max_pix=0;//�������ֵ��ʼ��
	Min_val=(int)(size/image.getZsize());//�������Сֵ 
	int temp_tvalue[256]={-129};//���沨��
	int Pix_array[256]={0};//ֱ��ͼ
	double Pix_val;//����ֵͳ��
	// ����ֱ��ͼ
	for(i=0;i<size;i++){  
		pix=(int)image.getXYZ(i);
		Pix_array[pix]++;
		Min_pix=Min_pix<pix?Min_pix:pix;
		Max_pix=Max_pix>pix?Max_pix:pix;
	}
	t=Min_pix;//����ֵ
	//image smooth 
	for(i=Min_pix;i<=Max_pix;i++){
        Pix_val=0.0;

		for(j=-3;j<=3;j++){
            k=i+j;
			k=k<Min_pix?Min_pix:k;//�½�Խ�紦��
			k=k>Max_pix?Max_pix:k;//�Ͻ�Խ�紦��
			Pix_val+=Pix_array[k];
        }
		Pix_array[i]=(int)(Pix_val/5.0+0.5);		
		//cout<<Pix_array[i]<<" ";
		//if(i%10==0)cout<<endl;
	}
	//���Ҳ���
	k=0;//��¼����ĸ���
	for(i=Min_pix+1;i<Max_pix;i++){
		if(i == (Min_pix+1)){
			if(Pix_array[i-1] > Pix_array[i]  &&  Pix_array[i-1] > Min_val){	
			   temp_tvalue[k]=i-1;
			   k++;
			}
		}
		else if(i == (Max_pix-1)){
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
	//cout<<"k="<<k<<endl;
	if(k>1){
		//��ȥα��
		for(i=0;i<k-1;i++){
			if(abs(temp_tvalue[i]-temp_tvalue[i+1]) < 10){//�ϲ�α��
				if(i+2<k){
					if(abs(temp_tvalue[i]-temp_tvalue[i+1]) <= abs(temp_tvalue[i+1]-temp_tvalue[i+2])){
						if(temp_tvalue[i]<temp_tvalue[i+1]){
							temp_tvalue[i]=Min_pix-1;
						}
						else temp_tvalue[i+1]=Min_pix-1;
					}
					else {
						if(temp_tvalue[i+1] < temp_tvalue[i+2]){
							temp_tvalue[i+1]=Min_pix-1;
						}
						else temp_tvalue[i+2]=Min_pix-1;
					}
				}
				else {
					if(temp_tvalue[i] < temp_tvalue[i+1]){
						temp_tvalue[i]=Min_pix-1;
					}
					else temp_tvalue[i+1]=Min_pix-1;
				} 		
			}
		}
		//cout<<"k="<<k<<endl;
		//�����ڵ���������������ֵ�ָ�
		arrays.clear();
		for(i=0;i<k-1;i++){
			if(temp_tvalue[i]>=Min_pix){
				for(j=i+1;i<k;j++){
					if(temp_tvalue[j]>=Min_pix){
						t=Otsu(image,temp_tvalue[i],temp_tvalue[j]);
						arrays.push_back((unsigned char)t);
						break;
					}
				}
			}
		}
	}
	else arrays.push_back(Otsu(image));
}
void OTSU::SaveImage(){
	int size=image.size();
	int i,j,k,step;
	unsigned char val;
	k=arrays.size();//��ֵ����
	if(k==0) return ;
	if(k>1) sort(arrays.begin(),arrays.end());//order
	if(k==1){
		for( i=0;i<size;i++){
			val=image.getXYZ(i);
			if(val<arrays.at(0)) image.putXYZ(i,0);
			else  image.putXYZ(i,255);
		}
	}
	else{
		step=(int)(250/k);//step longth
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
	ofstream f("img\\Image.raw", ios::out | ios::binary); 
	f.write((char*)image.getdata(),size);
	f.close();//�ر��ļ�
}
void OTSU::SaveImage(Raw &dest){
	ofstream f("img\\Image.raw", ios::out | ios::binary); 
	f.write((char*)dest.getdata(),dest.size());
	f.close();//�ر��ļ�
}
void OTSU::Output(Raw &image,Raw &dest){
	int k,m,n,size;
	unsigned char val;
	//ͼ���С
	size=image.size();
	m=arrays.size();//��ֵ����
	if(m>1)sort(arrays.begin(),arrays.end());//order
	if(m==0)return;
	else if(m>1){//����ֵ����
		//����
		for(k=0;k<size;k++){
			val=image.getXYZ(k);//���ͼ�������ֵ�Ĵ�С	
			if(val < arrays.at(0)) {	
				dest.putXYZ(k,1);
			}	
			else if(val >= arrays.at(m-1)){	
				dest.putXYZ(k,m+1);	
			}	
			else{//�м����	
				for(n=1;n<m;n++){		
					if(val >= arrays.at(n-1)  &&  val < arrays.at(n)){			
						dest.putXYZ(k,n+1);	
					}		
				}//for	
			}//else
		}//for	
	}
	else{//����ֵ����
		//����
		for(k=0;k<size;k++){		
			val=image.getXYZ(k);//���ͼ�������ֵ�Ĵ�С		
			if(val < arrays.at(0)) dest.putXYZ(k,1);	
			else  dest.putXYZ(k,2);	
		}//for
	}
}


//two Dimensions data process 
int OTSU::Otsu(Raw2D &image,int Min_pix,int Max_pix){
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
		pix=(int)image.getXY(i);
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
		if(pix_number1) u1=Pix_val/pix_number1;//����ǰ����ƽ���Ҷ�	    
		else  u1=Min_pix; 
		pix_number2=0.0;   
		Pix_val=0.0;    
		for(;i<=Max_pix;i++){	  
			pix_number2+=Pix_array[i];	   
			Pix_val+=i*Pix_array[i];
		}
		pixs=pix_number1+pix_number2;//�ܵ����ص���	
		w1=pix_number1/pixs;//ǰ�����ص���ռ����ͼ��ı���
		w2=1-w1;//�����ص���ռ����ͼ��ı��� 
		if(pix_number2) u2=Pix_val/pix_number2;//���㱳����ƽ���Ҷ�  
		else u2=Max_pix;
		s=w1*w2*(u1-u2)*(u1-u2); //������䷽�� 
		if(M_s<s){  
			M_s=s; 
			t=k;
		}
	}//������ָ���ֵt
	return t;
}
int OTSU::Otsu(Raw2D &image){
	int step;//����
	double w1;//ǰ�������ص���ռ����ͼ��ı���
	double u1;//ǰ����ƽ���Ҷ�
	double w2;//���������ص���ռ����ͼ��ı���
	double u2;//������ƽ���Ҷ�
	double s=0.0,M_s=0.0;//��䷽��
	int t;//��ֵ
	int i,j,k,pix,size,Min_pix,Max_pix,Min_val,Max_val;
	size=image.size();//���ͼ��Ĵ�С
	Min_pix=255;//������Сֵ��ʼ��
	Max_pix=0;//�������ֵ��ʼ��
	int Pix_array[256]={0};
	double pix_number;//���ص�ͳ��
	double Pix_val;//����ֵͳ��
	// ����ֱ��ͼ,��ͳ�Ƴ����ֵ����Сֵ
	for(i=0;i<size;i++){
		pix=(int)image.getXY(i);
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
	step=(Max_pix-Min_pix)/20;//���㲽��
	for(k=1;k<=20;k++){
		//ͳ��ǰ�����ص���ռ����ͼ��ı���
		pix_number=0.0;
		Pix_val=0.0;
		for(i=Min_pix;i<=(k*step+Min_pix);i++){
			pix_number+=Pix_array[i];
			Pix_val+=i*Pix_array[i];
		}
		w1=pix_number/size;//ǰ�����ص���ռ����ͼ��ı���
		if(pix_number) u1=Pix_val/pix_number;//����ǰ����ƽ���Ҷ�
		else u1=Min_pix;
		w2=1-w1;//�����ص���ռ����ͼ��ı���
		pix_number=0.0;
		Pix_val=0.0;
		for(;i<=Max_pix;i++){
			pix_number+=Pix_array[i];
			Pix_val+=i*Pix_array[i];
		}
		if(pix_number) u2=Pix_val/pix_number;//���㱳����ƽ���Ҷ�
		else u2=Max_pix;
        s=w1*w2*(u1-u2)*(u1-u2);
		if(M_s<=s){
			M_s=s;
			t=k*step+Min_pix;
		}
	}
	//��ȡ��tΪ�м����������step����Ϊ���ȵ�����
	i=t+step;
	Max_val=i>Max_pix?Max_pix:i;//�Ͻ�Խ�紦��
	j=t-step;
	Min_val=j<Min_pix?Min_pix:j;//�½�Խ�紦��
    for(k=Min_pix;k<Max_pix;k++){ 
		//ͳ��ǰ�����ص���ռ����ͼ��ı���
		pix_number=0.0;
		Pix_val=0.0;
		for(i=Min_pix;i<=k;i++){ 
			pix_number+=Pix_array[i]; 
			Pix_val+=i*Pix_array[i];
		}
		w1=pix_number/size;//ǰ�����ص���ռ����ͼ��ı���
		if(pix_number)  u1=Pix_val/pix_number;//����ǰ����ƽ���Ҷ�
		else u1=Min_pix; 
		w2=1-w1;//�����ص���ռ����ͼ��ı���
		pix_number=0.0;   
		Pix_val=0.0;   
		for(;i<=Max_pix;i++){  
			pix_number+=Pix_array[i]; 
			Pix_val+=i*Pix_array[i];
		}
		if(pix_number)  u2=Pix_val/pix_number;//���㱳����ƽ���Ҷ�
		else u2=Max_pix; 
		s=w1*w2*(u1-u2)*(u1-u2);//������䷽��
		if(M_s<s){
			M_s=s;
			t=k;
		}
	}
	return t;
}
void OTSU::Otsu_MultiVal(Raw2D &image,int t_number){
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
		pix=(int)image.getXY(i);	
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
	arrays.clear();//����
	do{//����ֵOTSU�ָ��㷨
		//����ֵOtsu�㷨����
		//cout<<"Min_pix="<< Min_pix<<"  Max_pix="<<Max_pix<<endl;
		t=Otsu(image,Min_pix,Max_pix);//������ָ���ֵt
		l--;
		arrays.push_back((unsigned char)t);//��ȡ��ֵ
		if(l){
		   //�������ڷ���
		   pix_number1=0.0;//���ظ���
		   Pix_val=0.0;//����ֵ
		   for(i=Min_pix;i<=t;i++){
			   pix_number1+=Pix_array[i];
			   Pix_val+=i*Pix_array[i];
		   }
		   s1=0.0;
		   if(pix_number1){
			   u1=Pix_val/pix_number1;//����ǰ����ƽ���Ҷ�
			   for(i=Min_pix;i<=t;i++){
				   s1+=Pix_array[i]*(i-u1)*(i-u1)/pix_number1;//����ǰ������
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
			   u2=Pix_val/pix_number2;//���㱳����ƽ���Ҷ�
			   for(i=t+1;i<=Max_pix;i++){
				   s2+=Pix_array[i]*(i-u2)*(i-u2)/pix_number2;//���㱳������
			   }
		   }
		   else u2=Max_pix;
		  
		   //���淽�������½�
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
		   ss->Min_val=t;//
		   ss->Max_val=Max_pix;
		   ss->s=s2;
		   ss-=k;//���ص�ָ��ͷλ��
		   k=Max_number(ss,t_number);//���㷽������λ��
		   Min_pix=(ss+k)->Min_val;
		   Max_pix=(ss+k)->Max_val;
		}
	}while(l);
	delete []ss;
}
void OTSU::Otsu_MultiVal(Raw2D &image){
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
		pix=(int)image.getXY(i);
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
	//�����ڵ���������������ֵ�ָ�
	arrays.clear();//��մ���
	if(k>1){
		for(i=0;i<k-1;i++){
			if(temp_tvalue[i]>=Min_pix){
				for(j=i+1;i<k;j++){
					if(temp_tvalue[j]>=Min_pix){
						t=Otsu(image,temp_tvalue[i],temp_tvalue[j]);
						arrays.push_back((unsigned char)t);
						break;
					}
				}
			}
		}
	}
	else arrays.push_back(Otsu(image));
	
}
void OTSU::SaveImage2D(){
	int size=image_2D.size();
	int i,j,k,step;
	unsigned char val;
	k=arrays.size();//��ֵ����
	if(k>1)sort(arrays.begin(),arrays.end());//order
	if(k==0)return;
	else if(k==1){
		for( i=0;i<size;i++){
			val=image_2D.getXY(i);
			if(val<arrays.at(0)) image_2D.putXY(i,0);
			else  image_2D.putXY(i,255);
		}
	}
	else{
		step=(int)(250/k);//����
		for( i=0;i<size;i++){
			val=image_2D.getXY(i);
			if(val<arrays.at(0)) image_2D.putXY(i,0);
			else if(val>=arrays.at(k-1)) image_2D.putXY(i,255);	
			else{
				for(j=1;j<k;j++){
					if(val>=arrays.at(j-1) && val<arrays.at(j)) image_2D.putXY(i,step*j);	
				}
			}
		}	
	}
	ofstream f("img\\Image.raw", ios::out | ios::binary); 
	f.write((char*)image_2D.getdata(),image_2D.size());
	f.close();//�ر��ļ�
}
void OTSU::SaveImage(Raw2D &dest){
	ofstream f("Image.raw", ios::out | ios::binary); 
	f.write((char*)dest.getdata(),dest.size());
	f.close();//�ر��ļ�
}
void OTSU::Output(Raw2D &image_2D,Raw2D &dest){
	int k,m,n,size;
	unsigned char val;
	////ͼ���С
	size = image_2D.size();
	m = arrays.size();//��ֵ����
	cout<<"m="<<m<<endl;
	if(m>1)sort(arrays.begin(),arrays.end());//order
	if(m==0)return;
	else if(m > 1){//����ֵ����
		//����
		for(k=0;k<size;k++){
			val = image_2D.getXY(k);//���ͼ�������ֵ�Ĵ�С		
			if(val < arrays.at(0)) {	
				dest.putXY(k,1);	
			}	
			else if(val >= arrays.at(m-1)){		
				dest.putXY(k,m+1);	
			}	
			else{//�м����	
				for(n=1;n<m;n++){		
					if(val >= arrays.at(n-1)  &&  val < arrays.at(n)){			
						dest.putXY(k,n+1);	
					}		
				}//for	
			}//else
		}//for	
	}
	else{//����ֵ����
		//����
		for(k=0;k<size;k++){		
			val = image_2D.getXY(k);//���ͼ�������ֵ�Ĵ�С		
			if(val < arrays.at(0)) dest.putXY(k,1);	
			else  dest.putXY(k,2);	
		}//for
	}
}
