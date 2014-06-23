#include "vol_math_ImageProc.h"
#include "vol_math_Watersheds.h"


//image smooth
void Smooth(Raw2D &image,int type){
	int i,j,k,l,m,n,col,row;
	PIXTYPE value;
	row=image.getXsize();
	col=image.getYsize();
	switch(type){
	case 0:{
		for(i=0;i<row;i++){
			for(j=0;j<col;j++){
				value=0.0;
				for(k=-1;k<2;k++){
					m=k+i;
					m=m<0?0:m;
					m=m>(row-1)?(row-1):m;	
					for(l=-1;l<2;l++){
						n=l+j; 
						n=n<0?0:n;
						n=n>(col-1)?(col-1):n;
						value+=image.get(m,n);
					}
				}
				image.put(i,j,value/9.0);
			}
		}
        break;
    }
	case 1:break;//��չ��˹�˲�
	case 2:break;//��ֵ�˲�
	case 3:break; //��ÿ������param1��param2���� ��Ͳ����߶ȱ任 
	case 4:break;//˫���˲�
	case 5:break;//����
	}
}
void Smooth(Raw2D *image,int type){
	int i,j,k,l,m,n,col,row;
	PIXTYPE value;
	row=image->getXsize();
	col=image->getYsize();
	switch(type){
	case 0:{
		for(i=0;i<row;i++){
			for(j=0;j<col;j++){
				value=0.0;
				for(k=-1;k<2;k++){
					m=k+i;
					m=m<0?0:m;
					m=m>(row-1)?(row-1):m;	
					for(l=-1;l<2;l++){
						n=l+j; 
						n=n<0?0:n;
						n=n>(col-1)?(col-1):n;
						value+=image->get(m,n);
					}
				}
				image->put(i,j,value/9);
			}
		}
        break;
    }
	case 1:break;//��չ��˹�˲�
	case 2:break;//��ֵ�˲�
	case 3:break; //��ÿ������param1��param2���� ��Ͳ����߶ȱ任 
	case 4:break;//˫���˲�
	case 5:break;//����
	}
}
//Noise process
void NoisePrc(Raw2D &image){
	int i,j,k,l,m,n,number,row,col;
	PIXTYPE value;
	row=image.getXsize();
	col=image.getYsize();
	//����һЩ�����С������
	//for(i=0;i<row;i++){
	//	for(j=0;j<col;j++){
	//		number=0;
	//		value=image.get(i,j);
	//		for(k=-5;k<6;k++){
	//			m=k+i;
	//			m=m<0?0:m;
	//			m=m>(row-1)?(row-1):m;
	//			for(l=-5;l<6;l++){
	//				n=l+j;
	//				n=n<0?0:n;
	//				n=n>(col-1)?(col-1):n;
	//				if(image.get(m,n)) number++;//��¼����ĸ���
	//			}
	//		}
	//		if((value && number<40)||(!value && number>60)) image.put(i,j,(PIXTYPE)(255-value));//����һЩ�����ĵ�
	//	}
	//}

	//ȡ������
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			number=0;
			value=image.get(i,j);
			for(k=-1;k<2;k++){
				m=k+i;
				m=m<0?0:m;
				m=m>(row-1)?(row-1):m;
				for(l=-1;l<2;l++){
					n=l+j;
					n=n<0?0:n;
					n=n>(col-1)?(col-1):n;
					if(image.get(m,n)) number++;//��¼����ĸ���
				}
			}
			if((value && number<4)||(!value && number>5)) image.put(i,j,(PIXTYPE)(255-value));//����һЩ�����ĵ�
		}
	}

}
//Extremum value
void MaxValue(Raw2D &image){
	int i,j,k,l,m,n,row,col;
	bool Is_Biger;//���ҵ�����ı��
	Raw2D temp(image);
	row=image.getXsize();
	col=image.getYsize();

	PIXTYPE value,Minval,Maxval;
	//ͼ��ƽ��
	Smooth(temp,CV_BLUR_NO_SCALE);	
	//�������ֵ����ֵ��
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			if(temp.get(i,j)>80){
				Is_Biger=false;
				for(k=-1;k<2;k++){
					m=k+i;
				    m=m<0?0:m;
				    m=m>(row-1)?(row-1):m;
					for(l=-1;l<2;l++){
						n=l+j;
					    n=n<0?0:n;
					    n=n>(col-1)?(col-1):n;
						if(temp.get(m,n) > temp.get(i,j)){
							Is_Biger=true;
							break;
						}
				    }
					if(Is_Biger) break;
				}
				if(Is_Biger) image.put(i,j,0);
				else {
					image.put(i,j,255);
				}
			}
			else image.put(i,j,0);
		}
	}
    NoisePrc(image);//��������
}
void MinValue(Raw2D &image){
	int i,j,k,l,m,n,row,col;
	bool Is_Smaller;//���ҵ���С�ı��
	Raw2D temp(image);
	row=image.getXsize();
	col=image.getYsize();
	//Image smooth
	Smooth(temp,CV_BLUR_NO_SCALE);
	//������Сֵ����ֵ��
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			if(temp.get(i,j)<80){
				Is_Smaller=false;
				for(k=-1;k<2;k++){
					m=k+i;
				    m=m<0?0:m;
				    m=m>(row-1)?(row-1):m;
					for(l=-1;l<2;l++){
						n=l+j;
					    n=n<0?0:n;
					    n=n>(col-1)?(col-1):n;
						if(temp.get(m,n) < temp.get(i,j)){
							Is_Smaller=true;
							break;
						}
				    }
					if(Is_Smaller) break;
				}
				if(Is_Smaller) image.put(i,j,0);
			    else image.put(i,j,255);
			}
			else image.put(i,j,0);
		}
	}
	NoisePrc(image);//��������
}
//three Dimensions
//gray image smooth function
void Smooth3D_7(Raw &image,int type){
	int i,j,k,l,m,n,x,y,z;
	long long col,row,height;
	PIXTYPE value;
	row=image.getYsize();
	col=image.getXsize();
	height=image.getZsize();
	switch(type){
	case 0:{
		for(k=0;k<height;k++){
			for(i=0;i<row;i++){
				for(j=0;j<col;j++){	
					value=0.0;
					for(l=-1;l<2;l++){
						z=k+l;
						z=z<0?0:z;
						z=z>(height-1)?(height-1):z;	
						for(m=-1;m<2;m++){
							y=m+i; 
							y=y<0?0:y;
							y=y>(row-1)?(row-1):y;
							for(n=-1;n<2;n++){
								x=n+j;
								x=x<0?0:x;
								x=x>(col-1)?(col-1):x;
								if((z==k && y==i) || (z==k && x==j) ||(y==i && x==j))
								value+=image.get(x,y,z);
							}
						}
					}
					image.put(j,i,k,value/27);
				}
			}
		}       
		break;
    }
	case 1:break;//��չ��˹�˲�
	case 2:break;//��ֵ�˲�
	case 3:break; //��ÿ������param1��param2���� ��Ͳ����߶ȱ任 
	case 4:break;//˫���˲�
	case 5:break;//����
	}
}
void Smooth3D_27(Raw &image,int type){
	int i,j,k,l,m,n,x,y,z;
	long long col,row,height;
	PIXTYPE value;
	row=image.getYsize();
	col=image.getXsize();
	height=image.getZsize();
	switch(type){
	case 0:{
		for(k=0;k<height;k++){
			for(i=0;i<row;i++){
				for(j=0;j<col;j++){	
					value=0.0;
					for(l=-1;l<2;l++){
						z=k+l;
						z=z<0?0:z;
						z=z>(height-1)?(height-1):z;	
						for(m=-1;m<2;m++){
							y=m+i; 
							y=y<0?0:y;
							y=y>(row-1)?(row-1):y;
							for(n=-1;n<2;n++){
								x=n+j;
								x=x<0?0:x;
								x=x>(col-1)?(col-1):x;
								value+=image.get(x,y,z);
							}
						}
					}
					image.put(j,i,k,value/27);
				}
			}
		}       
		break;
    }
	case 1:break;//��չ��˹�˲�
	case 2:break;//��ֵ�˲�
	case 3:break; //��ÿ������param1��param2���� ��Ͳ����߶ȱ任 
	case 4:break;//˫���˲�
	case 5:break;//����
	}
}
void Smooth3D(Raw *image,int type){
	int i,j,k,l,m,n,x,y,z;
	PIXTYPE value;
	long long col,row,height;
	row=image->getYsize();
	col=image->getXsize();
	height=image->getZsize();
	switch(type){
	case 0:{
		for(k=0;k<height;k++){
			for(i=0;i<row;i++){
				for(j=0;j<col;j++){	
					value=0.0;
					for(l=-1;l<2;l++){
						z=k+l;
						z=z<0?0:z;
						z=z>(height-1)?(height-1):z;	
						for(m=-1;m<2;m++){
							y=m+i; 
							y=y<0?0:y;
							y=y>(row-1)?(row-1):y;
							for(n=-1;n<2;n++){
								x=n+j;
								x=x<0?0:x;
								x=x>(col-1)?(col-1):x;
								value+=image->get(x,y,z);
							}
						}
					}
					image->put(j,i,k,value/27);
				}
			}
		}       
		break;
    }
	case 1:break;//��չ��˹�˲�
	case 2:break;//��ֵ�˲�
	case 3:break; //��ÿ������param1��param2���� ��Ͳ����߶ȱ任 
	case 4:break;//˫���˲�
	case 5:break;//����
	}
}
//Noise process gray image
void NoiseProcess(Raw &image){
	int i,j,k,l,m,n,x,y,z,number;
	long long row,col,height;
	PIXTYPE value;
	col=image.getXsize();
    row=image.getYsize();
	height=image.getZsize();
	//����һЩ�����С������
	for(k=0;k<height;k++){	
		for(i=0;i<row;i++){	
			for(j=0;j<col;j++){	
				number=0;
				value=image.get(j,i,k);
				for(l=-3;l<4;l++){		
					z=k+l;
					z=z<0?0:z;
					z=(z>(height-1))?(height-1):z;	
					for(m=-3;m<4;m++){	
						y=m+i; 
						y=y<0?0:y;
						y=(y>(row-1))?(row-1):y;	
						for(n=-3;n<4;n++){		
							x=n+j;					
							x=x<0?0:x;
							x=(x>(col-1))?(col-1):x;
							if(image.get(x,y,z)) number++;//��¼����ĸ���
						}
					}
				}
				if((value && number<100)||(!value && number>200)) image.put(j,i,k,(PIXTYPE)(255-value));//����һЩ�����ĵ�
			}
		}
	}  
	//ȡ������
	for(k=0;k<height;k++){	
		for(i=0;i<row;i++){	
			for(j=0;j<col;j++){	
				number=0;
				value=image.get(j,i,k);
				for(l=-1;l<2;l++){		
					z=k+l;
					z=z<0?0:z;
					z=(z>(height-1))?(height-1):z;	
					for(m=-1;m<2;m++){	
						y=m+i; 
						y=y<0?0:y;
						y=(y>(row-1))?(row-1):y;	
						for(n=-1;n<2;n++){		
							x=n+j;					
							x=x<0?0:x;
							x=(x>(col-1))?(col-1):x;
							if(image.get(x,y,z)) number++;//��¼����ĸ���
						}
					}
				}
				if((value && number<10)||(!value && number>20)) image.put(j,i,k,(PIXTYPE)(255-value));//����һЩ�����ĵ�
			}
		}
	}  
}
//Extremum value
void MaxValue(Raw &image,int smoothsize,int threshold){//����ֵ����
	if(threshold>255) {
		cout<<"The threshold is too big"<<endl;
		return ;
	}
	int i,j,k,l,m,n,x,y,z;
	long long row,col,height;
	bool Is_Biger;//���ҵ�����ı��
	Raw temp(image);
	col=image.getXsize();
	row=image.getYsize();
	height=image.getZsize();
	//ͼ��ƽ��
	if(1 == smoothsize) Smooth3D_7(temp,CV_BLUR_NO_SCALE);
	else if (2 == smoothsize)
	{
		Smooth3D_27(temp,CV_BLUR_NO_SCALE);
	}
	 
	//�������ֵ����ֵ��
	for(k=0;k<height;k++){	
		for(i=0;i<row;i++){	
			for(j=0;j<col;j++){	
				if(temp.get(j,i,k)>threshold){//50
					Is_Biger=false;
					for(l=-1;l<2;l++){
						z = l+k;
						z = z<0?0:z;
						z = (z>(height-1))?(height-1):z;
						for(m=-1;m<2;m++){	
							y=m+i; 
							y=y<0?0:y;
							y=(y>(row-1))?(row-1):y;	
							for(n=-1;n<2;n++){		
								x=n+j;					
								x=x<0?0:x;
								x=(x>(col-1))?(col-1):x;
								if(temp.get(x,y,z) > temp.get(j,i,k)){
									Is_Biger=true;
									break;
								}	
							}
							if(Is_Biger)break;
						}

					}
					if(Is_Biger)image.put(j,i,k,0);
					else image.put(j,i,k,255);
				}//if
				else image.put(j,i,k,0);
			}
		}
	} 
	NoiseProcess(image);//��������
}
void MinValue(Raw &image,int smoothsize,int threshold){//��Сֵ����
	if(threshold<0) {
		cout<<"The threshold is too small"<<endl;
		return ;
	}
	int i,j,k,l,m,n,x,y,z;
	long long row,col,height;
	bool Is_Smaller;//���ҵ���С�ı��
	Raw temp(image);
	col=image.getXsize();
	row=image.getYsize();
	height=image.getZsize();
	//ͼ��ƽ��
	if(1 == smoothsize) Smooth3D_7(temp,CV_BLUR_NO_SCALE);
	else  Smooth3D_27(temp,CV_BLUR_NO_SCALE);
	//�������ֵ����ֵ��
	for(k=0;k<height;k++){	
		for(i=0;i<row;i++){	
			for(j=0;j<col;j++){	
				if(temp.get(j,i,k)<threshold){//200
					Is_Smaller=false;
					for(m=-1;m<2;m++){	
						y=m+i; 
						y=y<0?0:y;
						y=(y>(row-1))?(row-1):y;	
						for(n=-1;n<2;n++){		
							x=n+j;					
							x=x<0?0:x;
							x=(x>(col-1))?(col-1):x;
							if(temp.get(x,y,k) < temp.get(j,i,k)){
								Is_Smaller=true;
								break;
							}	
						}
						if(Is_Smaller)break;
					}
					if(Is_Smaller)image.put(j,i,k,0);
					else image.put(j,i,k,255);
				}//if
				else image.put(j,i,k,0);
			}
		}
	} 
	NoiseProcess(image);//��������
}