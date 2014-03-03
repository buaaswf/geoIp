//watersheds.cpp
#include <iostream>
//#include <atlstr.h>
#include <vector>
#include <queue>
#include "vol_math_Watersheds.h"
#include "vol_math_Otsu.h"
//#include"windef.h"
using namespace std;

void Zero(int *p,int number){
	int i=0;
	for(;i<number;i++){
		*(p+i)=0;
	}
}
void Zero_2D(int **p,int row,int col){//����
	int i,j;
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			**(p+i*col+j)=0;
		}
	}
}
void Zero_3D(int ***p,int row,int col,int height){//����
	int i,j,k;
	for(k=0;k<height;k++){
		for(i=0;i<row;i++){
			for(j=0;j<col;j++){
				***(p+k*row*col+i*col+j)=0;
			}

		}
	}
}

//two Dimensions
/*====================================================================
��������            Watershed
���ܣ�              �ñ��-��ˮ���㷨������ͼ����зָ�
�㷨ʵ�֣�          ��
�������˵����      OriginalImage   --����ͼ�񣨻Ҷ�ͼ��0~255��
SeedImage       --���ͼ�񣨶�ֵͼ��0-�Ǳ�ǣ�1-��ǣ�
LabelImage      --���ͼ��1-��һ���ָ�����2-�ڶ����ָ�����...��
row             --ͼ������
col             --ͼ������
����ֵ˵����        ��        
====================================================================*/
void Watersheds( Raw2D &OriginalImage, Raw2D &SeedImage, Raw2D &LabelImage,int* classnum){
	int Num=0;//��������ʶ�ţ���1��ʼ
	vector<int*> SeedCounts;//����ÿ���������Ӹ���������
	queue<POINT2D> quetem;//��ʱ���Ӷ���
	/**================================================================
	��Ա����
	empty()�ж϶��пգ������п�ʱ������true��
	size()���ʶ����е�Ԫ�ظ�����
	push()�Ὣһ��Ԫ������queue�С�
	front()�᷵��queue�ڵ���һ��Ԫ��(Ҳ���ǵ�һ���������Ԫ��)��
	back()�᷵��queue�����һ��Ԫ��(Ҳ������󱻲����Ԫ��)��
	pop()���queue���Ƴ�һ��Ԫ�ء�
	ע�⣺pop()��Ȼ���Ƴ���һ��Ԫ�أ����ǲ�����������front()��back()������һ��Ԫ�ص������Ƴ���Ԫ�ء�
	================================================================**/
	vector<queue<POINT2D>*> vque;//�������б���������Ӷ��е����飬����ŵ������Ӷ��е�ָ��
	queue<POINT2D> *pque;//ָ�����Ӷ��е�ָ��
	POINT2D temp;//���ӵ� 
	bool actives;//��ĳһˮλ�������б�ǵ�����������ı�־
	int WaterLevel;//
	int* array;//
	int i,j,m,n,row,col,k=0;//����һЩ����
	bool up,down,right,left,upleft,upright,downleft,downright;//8 directions...
	int seednum;//���ӵ���Ŀ 
	int nonzeronum=0;//��������ʶ�ţ������ʼ����ֱ��Ϊ��
	col=OriginalImage.getYsize();
	row=OriginalImage.getXsize();
	//�ֽ����ͼ�����
	for(i=0;i<row;i++)
		for(j=0;j<col;j++)
			LabelImage.put(i,j,0);
	//Ԥ����,��ȡ����ÿ��������򣬲���ʼ��ÿ����ǵ����Ӷ���
	//������ָ��������Ե�ĵ㣬���ǿ�����ˮλ����ʱ������û������˵������
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			if(SeedImage.get(i,j) == 1){//����ҵ�һ���������  
				Num++;//����ı�ʶ�ż�һ
				//�������鲢��ʼ��Ϊ�㣬��ʾ����256���ҽ�
				array=new int[256];
				Zero(array,256);//���㴦��
				//���Ӹ��������vector��ÿ��ɨ��������һ�����飬���������ʶ��������һά���Ҷȼ����ڶ�ά��
				//��ʾĳ�����������ĳ�ҽ�����Ӧ�ĵ����Ŀ��
				SeedCounts.push_back(array);//push_back()������ʾ��������ӵ�vector��β����������Ҫ�������ڴ档
				//��ʾ��Ӧһ���ҽ���һ�����У�����ÿ�����п��Դ洢һ�����ϵĵ���Ϣ
				pque=new queue<POINT2D>[256];
				//���뵽���������У���Ӧ���Ǳ���Ǻ�Num��
				vque.push_back(pque);
				//��ǰ����뱾����������ʱ���Ӷ�����
				temp.x = i;
				temp.y = j;
				quetem.push(temp);
				//��ǰ����Ϊ�Ѵ���
				LabelImage.put(i,j,(unsigned char)Num);
				SeedImage.put(i,j,127);//��ʾ�Ѿ������
				//����ʱ���Ӷ����е����ӽ�������ֱ�����е����Ӷ��������
				//������Ϻ�Ķ�����Ϣ������vque�У���������źͻҽף���Ӧ�����洢��seedcounts��
				while(!quetem.empty()){ // ����ʱ���Ӷ��зǿ�ʱ
					up=down=right=left=false;    
					upleft=upright=downleft=downright=false;
					temp = quetem.front();//�Ӷ�����ȡ��һ������    
					m = temp.x;
					n = temp.y;
					quetem.pop();//��queue���Ƴ�һ��Ԫ�ء�
					//ע�⵽127��ɨ����̵�Ӱ�죬Ӱ������ıȽϣ����ǲ�Ӱ��while����е�ɨ�� 
					if(m > 0){
						if(SeedImage.get(m-1,n) == 1){ //�Ϸ���Ϊ�����������Ϊ������
							temp.x = m-1;
							temp.y = n;
							quetem.push(temp);//��������Ļ�����ô��Щ��ǹ��������ٴ���whileѭ���б�ɨ�赽�����ᣬ��Ϊֵ��127
							//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ���򣬲���¼����ŵ�labelImage
							LabelImage.put(m-1,n,(unsigned char) Num);
							SeedImage.put(m-1,n,127);
						}
						else{//�����Ϸ�Ϊ��������
							up = true; 			 
						}
					}
					if(m > 0  &&  n > 0){
						if(SeedImage.get(m-1,n-1)==1){//���Ϸ���Ϊ�����������Ϊ������	 
							temp.x=m-1;
							temp.y=n-1;
							quetem.push(temp);
							//�����ӵ���Ϊ����û���򣬼���һ��ѭ������127����ʶ����ɨ�裬�����ǵ�ǰ����
							LabelImage.put(m-1,n-1,(unsigned char) Num);
							SeedImage.put(m-1,n-1,127);
						}
						else{//�������Ϸ�Ϊ��������
							upleft = true;
						}
					}
					if(m > 0  &&  n < (col-1)){			 
						if(SeedImage.get(m-1,n+1)==1){//���Ϸ���Ϊ�����������Ϊ������					
							temp.x = m-1;						
							temp.y = n+1;						
							quetem.push(temp);						 
							//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
							LabelImage.put(m-1,n+1,(unsigned char) Num);					
							SeedImage.put(m-1,n+1,127);				
						}		
						else{//�������Ϸ�Ϊ��������			 
							upright = true;				 
						}			 
					}		 
					if(m < row-1){
						if(SeedImage.get(m+1,n)==1){//�·���Ϊ�����������Ϊ������ 			 
							temp.x = m+1;			 
							temp.y = n; 
							quetem.push(temp);				 
							//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����			 
							LabelImage.put(m+1,n,(unsigned char) Num); 
							SeedImage.put(m+1,n,127);				 
						}			 
						else{//�����·�Ϊ��������					 
							down = true;
						}				 
					}				 
					if(m < (row-1)  &&  n < (col-1)){ 					 
						if(SeedImage.get(m+1,n+1)==1){//���·���Ϊ�����������Ϊ������						 
							temp.x = m+1;						 
							temp.y = n+1;  						 
							quetem.push(temp);						 
							//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
							LabelImage.put(m+1,n+1,(unsigned char) Num);				
							SeedImage.put(m+1,n+1,127);			 
						}			 
						else{//�������·�Ϊ��������						 
							downright = true;					 
						}				
					}
					if(m < (row-1)  &&  n > 0){
						if(SeedImage.get(m+1,n-1)==1){//���·���Ϊ�����������Ϊ������
							temp.x = m+1;
							temp.y = n-1;
							quetem.push(temp);
							//�����ӵ���Ϊ����û����
							LabelImage.put(m+1,n-1,(unsigned char) Num);
							SeedImage.put(m+1,n-1,127);
						}  
						else{//�������·�Ϊ��������
							downleft = true;
						}
					}
					if(n > 0){			
						if(SeedImage.get(m,n-1) == 1){//����Ϊ�����������Ϊ������					 
							temp.x = m;					
							temp.y = n-1;				 
							quetem.push(temp);					
							//�����ӵ���Ϊ����û����						
							LabelImage.put(m,n-1,(unsigned char) Num);				
							SeedImage.put(m,n-1,127);		 
						}
						else{//������Ϊ��������
							left = true;
						}
					}
					if(n < col-1){					 
						if(SeedImage.get(m,n+1)==1){//�ҷ���Ϊ�����������Ϊ������									 
							temp.x = m;						
							temp.y = n+1;						
							quetem.push(temp);						
							//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						 
							LabelImage.put(m,n+1,(unsigned char) Num);						 
							SeedImage.put(m,n+1,127);					
						}					
						else{//�����ҷ�Ϊ��������						 
							right = true;					 
						}				
					}				 
					//��������ֻҪ��һ�㲻����������ô����Ϊ��ʼ���Ӷ����е�һ��
					//����ɷ���������seedimage�е�ֵ�������ġ�
					if(up||down||right||left|| upleft||downleft||upright||downright){	
						temp.x = m; 
						temp.y = n;
						//�������ʸ�����飺��һά�Ǳ�Ǻţ��ڶ�ά�Ǹ�ͼ���ĻҶȼ�
						//m,n���Ӧ����whileѭ������ɨ������ص㡣
						//Num�ǵ�ǰ�������
						//���������ά��Ϣ�ͱ�ʾ�ˣ�ĳ�������ж�Ӧĳ���Ҷȼ���Ӧ�ĳ�Ա��ļ��������
						//�ֱ������������������
						(vque[Num-1][(int)(OriginalImage).get(m,n)]).push(temp);
						SeedCounts[Num-1][(int)OriginalImage.get(m,n)]++;
					}    
				}//while������ɨ�赽quetemΪ�ն�ֹ��Ҳ���Ƕ�Ӧ���еĽڵ㶼�õ���������Ϊֹ����������Χ�ĵ�Ҫô����������Ҫô��������       
			}//if����
			// if(Num==5)
			// return;       
		} 
	} 

	//�����������У������ǵĵ�Ϊ0���ʾ��û��ɨ�赽�ĵ㣬���߱���������������ӵ�
	//�����൱������seedimage�������ĳ�ʼ����ķ�ˮ����ߵ����еĵ㣻�����ñ�ż�¼ÿ������ͬʱ��ˮ��ı�Ե�������С�
	//�������Ҽ�ˮ��ĳ���ͬʱҲ����ͨ����
	/*************************************/
	//test �������һ��ʣ�µķ�ˮ��صĵ�����
	seednum=0;
	//for(i=0;i<row;i++){
	//	for(j=0;j<col;j++){
	//		if(SeedImage.get(i,j) == 0)
	//			seednum++;
	//	}
	//}
	//cout<<"��ˮ��صĵ���:"<<seednum<<endl;

	//��û���̿�ʼ��ˮλ���㿪ʼ������ˮλ��Ӧ�Ҷȼ�����������ͨ��
	for(WaterLevel=0;WaterLevel<256;WaterLevel++){//�ڶ�ά������
		actives = true;//��ĳһˮλ�������б�ǵ�����������ı�־ 
		while(actives){//���е�����������ȫ
			actives = false;
			//���δ���ÿ����Ǻ�����Ӧ�������������ǺŶ�Ӧ������ĵ�ĸ�����SeedCounts����
			for(i=0;i<Num;i++){//��һά�������������������
				if(!vque[i][WaterLevel].empty()){//��Ӧ�ķ�ˮ�벻Ϊ�ռ�,i��ʾ����ţ�waterlevel��ʾ�ҽ�
					actives = true;//
					while(SeedCounts[i][WaterLevel]>0){//���ӻ���ʣ��������
						SeedCounts[i][WaterLevel]--;//ȡ��һ���㣬���Ӹ�������һ
						temp=vque[i][WaterLevel].front();//ȡ���������һ���㣬��������Ե�㣬��ʾ��ǰ�Ҷȼ��������Ѿ�������ˡ�
						vque[i][WaterLevel].pop();//���
						m = temp.x;//��ǰ���ӵ�����
						n = temp.y;
						if(m > 0){//�Ϸ�
							if(!LabelImage.get(m-1,n)){//�Ϸ���δ������ʾû�б�ţ�Ӧ��������ǰ�Ѿ�������ʼ��Ϊ0
								//���������ڿ�ͷҲ������ʼ��
								temp.x = m-1;
								temp.y = n;
								LabelImage.put(m-1,n,(PIXTYPE)(i+1));//�Ϸ�����Ϊ����û����
								//ע�⵽����������ɨ�����������ͬ��һ�����������������������ǵ�
								//��������һ�����ٻ�ɨ�赽����㣬ȷ������©��������һ�ֵĴ����ʹ������
								//���������⻹��������ǲ�û��һ���������뵽���Ӷ��С�Ҳ����˵��
								//ֻ�Ǳ���û������������û��ֻ�����������������������С�
								if(OriginalImage.get(m-1,n) <= WaterLevel){//�Ϸ���Ϊ������������뵱ǰ���У���ǰ�߶ȵĶ���
									vque[i][WaterLevel].push(temp);	  
								}
								else{//�������OriginalImage[m-1][n]��Ӧ�ĻҶȼ��Ķ��У�Ϊʲô��
									vque[i][(int)(OriginalImage.get(m-1,n))].push(temp);
									SeedCounts[i][(int)OriginalImage.get(m-1,n)]++;
								}
							}
						}
						if(m < row-1){//�·�
							if(!LabelImage.get(m+1,n)){//�·���δ����
								temp.x = m+1;
								temp.y = n;
								LabelImage.put(m+1,n,(unsigned char)(i+1));//�·�����Ϊ����û����
								if(OriginalImage.get(m+1,n) <= WaterLevel){//�·���Ϊ������������뵱ǰ����
									vque[i][WaterLevel].push(temp);
								}
								else{//�������OriginalImage[m+1][n]������
									vque[i][(int)OriginalImage.get(m+1,n)].push(temp);
									SeedCounts[i][(int)OriginalImage.get(m+1,n)]++;
								}
							}
						}
						if(n < col-1){//�ҷ�
							if(!LabelImage.get(m,n+1)){//�ҷ���δ����
								temp.x = m; 
								temp.y = n+1;      
								LabelImage.put(m,n+1,(unsigned char)(i+1));//�ҷ�����Ϊ����û����    
								if(OriginalImage.get(m,n+1) <= WaterLevel){//�ҷ���Ϊ������������뵱ǰ����
									vque[i][WaterLevel].push(temp);
								}
								else{//�������OriginalImage[m][n+1]������
									vque[i][(int)OriginalImage.get(m,n+1)].push(temp);
									SeedCounts[i][(int)OriginalImage.get(m,n+1)]++;
								}
							}
						}
						if(n > 0){//��
							if(!LabelImage.get(m,n-1)){//����δ����
								temp.x = m;
								temp.y = n-1;
								LabelImage.put(m,n-1,(unsigned char)(i+1));//�󷽵���Ϊ����û����
								if(OriginalImage.get(m,n-1) <= WaterLevel){//����Ϊ������������뵱ǰ����
									vque[i][WaterLevel].push(temp);
								}
								else{//�������OriginalImage[m][n-1]������
									vque[i][(int)OriginalImage.get(m,n-1)].push(temp);
									SeedCounts[i][(int)OriginalImage.get(m,n-1)]++;
								}
							}
						}
					}//whileѭ������
					SeedCounts[i][WaterLevel] = vque[i][WaterLevel].size();//�����Ӷ���ָ��ĸ������ݸ����Ӹ���
				}//if����:�ڶ�Ӧ�ķ�ˮ�벻Ϊ�ռ��������
			}//��һάforѭ����������������
		}//whileѭ������
	}//forѭ������

	/**********************************/
	//test whether the origional segmentation num is changed...
	nonzeronum = Num;
	for(m=0;m<Num;m++){
		for(i=0;i<row;i++){
			for(j=0;j<col;j++){
				if(LabelImage.get(i,j) == m)	
					break;
			}
			if(LabelImage.get(i,j) == m)
				break;
		}
		if(j == col-1  &&  i == row-1  &&  LabelImage.get(i,j) != m)
			nonzeronum--;
	}
	cout<<"nonzeronum="<<nonzeronum<<endl;
	*classnum =nonzeronum;
	//�ͷ�pque�ڴ�
	while(!vque.empty()){
		pque=vque.back();
		delete[] pque;
		vque.pop_back();
	}
	//�ͷ�array�ڴ�
	while(!SeedCounts.empty()){
		array=SeedCounts.back();
		delete[] array;
		SeedCounts.pop_back();
	}
}
//void Watersheds(const Raw2D *OriginalImage, Raw2D *LabelImage);
void Gradient( Raw2D &Image,Raw2D &dest){//��ά���ݶ� 
	int col=Image.getYsize();
	int row=Image.getXsize();
	int i,j,dx1,dx2,dx3,dx4,dx5,dx6,dx,dy1,dy2,dy3,dy4,dy5,dy6,dy;
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){ 
			if(i==0 || j==0 || i==row-1 || j==col-1) dest.put(i,j,0);
			else{
				dx1=(int)Image.get(i-1,j+1);
				dx2=2*(int)Image.get(i,j+1);
				dx3=(int)Image.get(i+1,j+1);
				dx4=(int)Image.get(i-1,j-1);
				dx5=2*(int)Image.get(i,j-1);
				dx6=(int)Image.get(i+1,j-1);
				dx=dx1+dx2+dx3-dx4-dx5-dx6;	

				dy1=(int)Image.get(i+1,j-1);
				dy2=2*(int)Image.get(i+1,j);
				dy3=(int)Image.get(i+1,j+1);
				dy4=(int)Image.get(i-1,j-1);
				dy5=2*(int)Image.get(i-1,j);
				dy6=(int)Image.get(i-1,j+1);

				dy=dy1+dy2+dy3-dy4-dy5-dy6;
				dest.put(i,j,(unsigned char)sqrt((float)dx*dx+dy*dy));
			}	
		}//forѭ������
	}//forѭ������
}

void WatershedsProcess(Raw2D &OriginalImage){
	int i,j,row,col,number,size;
	row=OriginalImage.getXsize();
	col=OriginalImage.getYsize();
	number=0;
	size=col*row;
	int space =1;
	int *classnum = &space;
	string strings="erode";
	Raw2D raw1(row,col);	
	Raw2D raw2(row,col);	
	//OTSU test;
	//ƽ��ͼ���ݶ�
	Gradient(OriginalImage,raw1);
	//erode reconstruction
	Dilate_Gray(OriginalImage,raw2,2);//image dilate
	Erode_Gray(raw2,OriginalImage,2);//image Erode
	Morph_reconstuct_Gray(OriginalImage,raw2,strings,2);
	////Get Max image
	MaxValue(raw2);
	//get seedimage and initialize it
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			if(raw2.get(i,j)) {
				raw2.put(i,j,1);
				number++;
			}	
			else  raw2.put(i,j,0);
		}
	}
	cout<<"��ǵ�ĸ���:"<<number<<endl;
	////watersheds
	Watersheds(raw1,raw2,OriginalImage,classnum);
	//save image
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			OriginalImage.put(i,j,OriginalImage.get(i,j)*255/(*classnum));
		}
	}
	//test.SaveImage(OriginalImage);
}
void WatershedIterface(Raw2D *src,Raw2D *ret)
{
	//delete ret;
	memcpy(ret->getdata(),src->getdata(),src->getXsize()*src->getYsize());
	WatershedsProcess(*ret);
}
void Watersheds( Raw2D *OriginalImage, Raw2D *SeedImage, Raw2D *LabelImage){
	int Num=0;//��������ʶ�ţ���1��ʼ
	vector<int*> SeedCounts;//����ÿ���������Ӹ���������
	queue<POINT2D> quetem;//��ʱ���Ӷ���
	/**================================================================
	��Ա����
	empty()�ж϶��пգ������п�ʱ������true��
	size()���ʶ����е�Ԫ�ظ�����
	push()�Ὣһ��Ԫ������queue�С�
	front()�᷵��queue�ڵ���һ��Ԫ��(Ҳ���ǵ�һ���������Ԫ��)��
	back()�᷵��queue�����һ��Ԫ��(Ҳ������󱻲����Ԫ��)��
	pop()���queue���Ƴ�һ��Ԫ�ء�
	ע�⣺pop()��Ȼ���Ƴ���һ��Ԫ�أ����ǲ�����������front()��back()������һ��Ԫ�ص������Ƴ���Ԫ�ء�
	================================================================**/
	vector<queue<POINT2D>*> vque;//�������б���������Ӷ��е����飬����ŵ������Ӷ��е�ָ��
	queue<POINT2D> *pque;//ָ�����Ӷ��е�ָ��
	POINT2D temp;//���ӵ� 
	bool actives;//��ĳһˮλ�������б�ǵ�����������ı�־
	int WaterLevel;//
	int* array;//
	int i,j,m,n,row,col,k = 0;//����һЩ����
	bool up,down,right,left,upleft,upright,downleft,downright;//8 directions...
	int seednum;//���ӵ���Ŀ 
	int nonzeronum=0;//��������ʶ�ţ������ʼ����ֱ��Ϊ��
	col=OriginalImage->getYsize();
	row=OriginalImage->getXsize();
	//�ֽ����ͼ�����
	for(i=0;i<row;i++)
		for(j=0;j<col;j++)
			LabelImage->put(i,j,0);
	//Ԥ����,��ȡ����ÿ��������򣬲���ʼ��ÿ����ǵ����Ӷ���
	//������ָ��������Ե�ĵ㣬���ǿ�����ˮλ����ʱ������û������˵������
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			if(SeedImage->get(i,j) == 1){//����ҵ�һ���������  
				Num++;//����ı�ʶ�ż�һ
				//�������鲢��ʼ��Ϊ�㣬��ʾ����256���ҽ�
				array=new int[256];
				Zero(array,256);//���㴦��
				//���Ӹ��������vector��ÿ��ɨ��������һ�����飬���������ʶ��������һά���Ҷȼ����ڶ�ά��
				//��ʾĳ�����������ĳ�ҽ�����Ӧ�ĵ����Ŀ��
				SeedCounts.push_back(array);//push_back()������ʾ��������ӵ�vector��β����������Ҫ�������ڴ档
				//��ʾ��Ӧһ���ҽ���һ�����У�����ÿ�����п��Դ洢һ�����ϵĵ���Ϣ
				pque = new queue<POINT2D>[256];
				//���뵽���������У���Ӧ���Ǳ���Ǻ�Num��
				vque.push_back(pque);
				//��ǰ����뱾����������ʱ���Ӷ�����
				temp.x = i;
				temp.y = j;
				quetem.push(temp);
				//��ǰ����Ϊ�Ѵ���
				LabelImage->put(i,j,(unsigned char)Num);
				SeedImage->put(i,j,127);//��ʾ�Ѿ������
				//����ʱ���Ӷ����е����ӽ�������ֱ�����е����Ӷ��������
				//������Ϻ�Ķ�����Ϣ������vque�У���������źͻҽף���Ӧ�����洢��seedcounts��
				while(!quetem.empty()){ // ����ʱ���Ӷ��зǿ�ʱ
					up=down=right=left=false;    
					upleft=upright=downleft=downright=false;
					temp = quetem.front();//�Ӷ�����ȡ��һ������    
					m = temp.x;
					n = temp.y;
					quetem.pop();//��queue���Ƴ�һ��Ԫ�ء�
					//ע�⵽127��ɨ����̵�Ӱ�죬Ӱ������ıȽϣ����ǲ�Ӱ��while����е�ɨ�� 
					if(m > 0){
						if(SeedImage->get(m-1,n) == 1){ //�Ϸ���Ϊ�����������Ϊ������
							temp.x = m-1;
							temp.y = n;
							quetem.push(temp);//��������Ļ�����ô��Щ��ǹ��������ٴ���whileѭ���б�ɨ�赽�����ᣬ��Ϊֵ��127
							//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ���򣬲���¼����ŵ�labelImage
							LabelImage->put(m-1,n,(unsigned char) Num);
							SeedImage->put(m-1,n,127);
						}
						else{//�����Ϸ�Ϊ��������
							up=true; 			 
						}
					}
					if(m > 0  &&  n > 0){
						if(SeedImage->get(m-1,n-1) == 1){//���Ϸ���Ϊ�����������Ϊ������	 
							temp.x = m-1;
							temp.y = n-1;
							quetem.push(temp);
							//�����ӵ���Ϊ����û���򣬼���һ��ѭ������127����ʶ����ɨ�裬�����ǵ�ǰ����
							LabelImage->put(m-1,n-1,(unsigned char) Num);
							SeedImage->put(m-1,n-1,127);
						}
						else{//�������Ϸ�Ϊ��������
							upleft = true;
						}
					}
					if(m > 0  &&  n < (col-1)){			 
						if(SeedImage->get(m-1,n+1) == 1){//���Ϸ���Ϊ�����������Ϊ������					
							temp.x = m-1;						
							temp.y = n+1;						
							quetem.push(temp);						 
							//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
							LabelImage->put(m-1,n+1,(unsigned char) Num);					
							SeedImage->put(m-1,n+1,127);				
						}		
						else{//�������Ϸ�Ϊ��������			 
							upright = true;				 
						}			 
					}		 
					if(m < (row-1)){
						if(SeedImage->get(m+1,n) == 1){//�·���Ϊ�����������Ϊ������ 			 
							temp.x = m+1;			 
							temp.y = n; 
							quetem.push(temp);				 
							//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����			 
							LabelImage->put(m+1,n,(unsigned char) Num); 
							SeedImage->put(m+1,n,127);				 
						}			 
						else{//�����·�Ϊ��������					 
							down = true;
						}				 
					}				 
					if(m < (row-1)  &&  n < (col-1)){ 					 
						if(SeedImage->get(m+1,n+1) == 1){//���·���Ϊ�����������Ϊ������						 
							temp.x = m+1;						 
							temp.y = n+1;  						 
							quetem.push(temp);						 
							//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
							LabelImage->put(m+1,n+1,(unsigned char) Num);				
							SeedImage->put(m+1,n+1,127);			 
						}			 
						else{//�������·�Ϊ��������						 
							downright = true;					 
						}				
					}
					if(m < (row-1)  &&  n > 0){
						if(SeedImage->get(m+1,n-1) == 1){//���·���Ϊ�����������Ϊ������
							temp.x = m+1;
							temp.y = n-1;
							quetem.push(temp);
							//�����ӵ���Ϊ����û����
							LabelImage->put(m+1,n-1,(unsigned char) Num);
							SeedImage->put(m+1,n-1,127);
						}  
						else{//�������·�Ϊ��������
							downleft = true;
						}
					}
					if(n > 0){			
						if(SeedImage->get(m,n-1) == 1){//����Ϊ�����������Ϊ������					 
							temp.x = m;					
							temp.y = n-1;				 
							quetem.push(temp);					
							//�����ӵ���Ϊ����û����						
							LabelImage->put(m,n-1,(unsigned char) Num);				
							SeedImage->put(m,n-1,127);		 
						}
						else{//������Ϊ��������
							left = true;
						}
					}
					if(n < (col-1)){					 
						if(SeedImage->get(m,n+1) == 1){//�ҷ���Ϊ�����������Ϊ������									 
							temp.x = m;						
							temp.y = n+1;						
							quetem.push(temp);						
							//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						 
							LabelImage->put(m,n+1,(unsigned char) Num);						 
							SeedImage->put(m,n+1,127);					
						}					
						else{//�����ҷ�Ϊ��������						 
							right = true;					 
						}				
					}				 
					//��������ֻҪ��һ�㲻����������ô����Ϊ��ʼ���Ӷ����е�һ��
					//����ɷ���������seedimage�е�ֵ�������ġ�
					if(up||down||right||left|| upleft||downleft||upright||downright){	
						temp.x=m; 
						temp.y=n;
						//�������ʸ�����飺��һά�Ǳ�Ǻţ��ڶ�ά�Ǹ�ͼ���ĻҶȼ�
						//m,n���Ӧ����whileѭ������ɨ������ص㡣
						//Num�ǵ�ǰ�������
						//���������ά��Ϣ�ͱ�ʾ�ˣ�ĳ�������ж�Ӧĳ���Ҷȼ���Ӧ�ĳ�Ա��ļ��������
						//�ֱ������������������
						vque[Num-1][(int)OriginalImage->get(m,n)].push(temp);
						SeedCounts[Num-1][(int)OriginalImage->get(m,n)]++;
					}    
				}//while������ɨ�赽quetemΪ�ն�ֹ��Ҳ���Ƕ�Ӧ���еĽڵ㶼�õ���������Ϊֹ����������Χ�ĵ�Ҫô����������Ҫô��������       
			}//if����
			// if(Num==5)
			// return;       
		} 
	} 

	//�����������У������ǵĵ�Ϊ0���ʾ��û��ɨ�赽�ĵ㣬���߱���������������ӵ�
	//�����൱������seedimage�������ĳ�ʼ����ķ�ˮ����ߵ����еĵ㣻�����ñ�ż�¼ÿ������ͬʱ��ˮ��ı�Ե�������С�
	//�������Ҽ�ˮ��ĳ���ͬʱҲ����ͨ����
	/*************************************/
	//test �������һ��ʣ�µķ�ˮ��صĵ�����
	seednum=0;
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			if(SeedImage->get(i,j) == 0)
				seednum++;
		}
	}
	cout<<"��ˮ��صĵ���:"<<seednum<<endl;

	//��û���̿�ʼ��ˮλ���㿪ʼ������ˮλ��Ӧ�Ҷȼ�����������ͨ��
	for(WaterLevel=0;WaterLevel<256;WaterLevel++){//�ڶ�ά������
		actives = true;//��ĳһˮλ�������б�ǵ�����������ı�־ 
		while(actives){//���е�����������ȫ
			actives = false;
			//���δ���ÿ����Ǻ�����Ӧ�������������ǺŶ�Ӧ������ĵ�ĸ�����SeedCounts����
			for(i=0;i<Num;i++){//��һά�������������������
				if(!vque[i][WaterLevel].empty()){//��Ӧ�ķ�ˮ�벻Ϊ�ռ�,i��ʾ����ţ�waterlevel��ʾ�ҽ�
					actives = true;//
					while(SeedCounts[i][WaterLevel]>0){//���ӻ���ʣ��������
						SeedCounts[i][WaterLevel]--;//ȡ��һ���㣬���Ӹ�������һ
						temp=vque[i][WaterLevel].front();//ȡ���������һ���㣬��������Ե�㣬��ʾ��ǰ�Ҷȼ��������Ѿ�������ˡ�
						vque[i][WaterLevel].pop();//���
						m = temp.x;//��ǰ���ӵ�����
						n = temp.y;
						if(m > 0){//�Ϸ�
							if(!LabelImage->get(m-1,n)){//����δ������ʾû�б�ţ�Ӧ��������ǰ�Ѿ�������ʼ��Ϊ0
								//���������ڿ�ͷҲ������ʼ��
								temp.x = m-1;
								temp.y = n;
								LabelImage->put(m-1,n,(unsigned char)(i+1));//�Ϸ�����Ϊ����û����
								//ע�⵽����������ɨ�����������ͬ��һ�����������������������ǵ�
								//��������һ�����ٻ�ɨ�赽����㣬ȷ������©��������һ�ֵĴ����ʹ������
								//���������⻹��������ǲ�û��һ���������뵽���Ӷ��С�Ҳ����˵��
								//ֻ�Ǳ���û������������û��ֻ�����������������������С�
								if(OriginalImage->get(m-1,n) <= WaterLevel){//�Ϸ���Ϊ������������뵱ǰ���У���ǰ�߶ȵĶ���
									vque[i][WaterLevel].push(temp);	  
								}
								else{//�������OriginalImage[m-1][n]��Ӧ�ĻҶȼ��Ķ��У�Ϊʲô��
									vque[i][(int)OriginalImage->get(m-1,n)].push(temp);
									SeedCounts[i][(int)OriginalImage->get(m-1,n)]++;
								}
							}
						}
						if(m < (row-1)){//�·�
							if(!LabelImage->get(m+1,n)){//�·���δ����
								temp.x = m+1;
								temp.y = n;
								LabelImage->put(m+1,n,(unsigned char)(i+1));//�·�����Ϊ����û����
								if(OriginalImage->get(m+1,n) <= WaterLevel){//�·���Ϊ������������뵱ǰ����
									vque[i][WaterLevel].push(temp);
								}
								else{//�������OriginalImage[m+1][n]������
									vque[i][(int)OriginalImage->get(m+1,n)].push(temp);
									SeedCounts[i][(int)OriginalImage->get(m+1,n)]++;
								}
							}
						}
						if(n < (col-1)){//�ҷ�
							if(!LabelImage->get(m,n+1)){//�ҷ���δ����
								temp.x = m; 
								temp.y = n+1;      
								LabelImage->put(m,n+1,(unsigned char)(i+1));//�ҷ�����Ϊ����û����    
								if(OriginalImage->get(m,n+1) <= WaterLevel){//�ҷ���Ϊ������������뵱ǰ����
									vque[i][WaterLevel].push(temp);
								}
								else{//�������OriginalImage[m][n+1]������
									vque[i][(int)OriginalImage->get(m,n+1)].push(temp);
									SeedCounts[i][(int)OriginalImage->get(m,n+1)]++;
								}
							}
						}
						if(n > 0){//��
							if(!LabelImage->get(m,n-1)){//����δ����
								temp.x = m;
								temp.y = n-1;
								LabelImage->put(m,n-1,(unsigned char)(i+1));//�󷽵���Ϊ����û����
								if(OriginalImage->get(m,n-1) <= WaterLevel){//����Ϊ������������뵱ǰ����
									vque[i][WaterLevel].push(temp);
								}
								else{//�������OriginalImage[m][n-1]������
									vque[i][(int)OriginalImage->get(m,n-1)].push(temp);
									SeedCounts[i][(int)OriginalImage->get(m,n-1)]++;
								}
							}
						}
					}//whileѭ������
					SeedCounts[i][WaterLevel] = vque[i][WaterLevel].size();//�����Ӷ���ָ��ĸ������ݸ����Ӹ���
				}//if����:�ڶ�Ӧ�ķ�ˮ�벻Ϊ�ռ��������
			}//��һάforѭ����������������
		}//whileѭ������
	}//forѭ������

	/**********************************/
	//test whether the origional segmentation num is changed...
	nonzeronum=Num;
	for(m=0;m<Num;m++){
		for(i=0;i<row;i++){
			for(j=0;j<col;j++){
				if(LabelImage->get(i,j) == m)	
					break;
			}
			if(LabelImage->get(i,j) == m)
				break;
		}
		if(j==col-1 && i==row-1 &&  LabelImage->get(i,j) != m)
			nonzeronum--;
	}
	cout<<"nonzeronum="<<nonzeronum<<endl;

	//�ͷ�pque�ڴ�
	while(!vque.empty()){
		pque=vque.back();
		delete[] pque;
		vque.pop_back();
	}
	//�ͷ�array�ڴ�
	while(!SeedCounts.empty()){
		array=SeedCounts.back();
		delete[] array;
		SeedCounts.pop_back();
	}
}
void Gradient( Raw2D *Image,Raw2D *dest){//��ά���ݶ� 
	int col = Image->getYsize();
	int row = Image->getXsize();
	int i,j,dx1,dx2,dx3,dx4,dx5,dx6,dx,dy1,dy2,dy3,dy4,dy5,dy6,dy;
	for(i=1;i<row-1;i++){
		for(j=1;j<col-1;j++){
			dx1 = (int)Image->get(i-1,j+1);
			dx2 = 2*(int)Image->get(i,j+1);
			dx3 = (int)Image->get(i+1,j+1);
			dx4 = (int)Image->get(i-1,j-1);
			dx5 = 2*(int)Image->get(i,j-1);
			dx6 = (int)Image->get(i+1,j-1);
			dx = dx1+dx2+dx3-dx4-dx5-dx6;


			dy1 = (int)Image->get(i+1,j-1);
			dy2 = 2*(int)Image->get(i+1,j);
			dy3 = (int)Image->get(i+1,j+1);
			dy4 = (int)Image->get(i-1,j-1);
			dy5 = 2*(int)Image->get(i-1,j);
			dy6 = (int)Image->get(i-1,j+1);
			dy = dy1+dy2+dy3-dy4-dy5-dy6;

			dest->put(i,j,(unsigned char)sqrt((float)dx*dx+dy*dy));
		}//forѭ������
	}//forѭ������
}

//three  Dimensions
/*====================================================================
��������            Watershed
���ܣ�              �ñ��-��ˮ���㷨������ͼ����зָ�
�㷨ʵ�֣�          ��
�������˵����      OriginalImage   --����ͼ�񣨻Ҷ�ͼ��0~255��
SeedImage       --���ͼ�񣨶�ֵͼ��0-�Ǳ�ǣ�1-��ǣ�
LabelImage      --���ͼ��1-��һ���ָ�����2-�ڶ����ָ�����...��
row             --ͼ������
col             --ͼ������
����ֵ˵����        ��        
====================================================================*/

//void Watersheds(const Raw &OriginalImage, Raw &SeedImage, Raw &LabelImage){
//	int Num=0;//��������ʶ�ţ���1��ʼ
//	vector<int*> SeedCounts;//����ÿ���������Ӹ���������
//	queue<POINT3D> quetem;//��ʱ���Ӷ���
//	/**================================================================
//	��Ա����
//	empty()�ж϶��пգ������п�ʱ������true��
//	size()���ʶ����е�Ԫ�ظ�����
//	push()�Ὣһ��Ԫ������queue�С�
//	front()�᷵��queue�ڵ���һ��Ԫ��(Ҳ���ǵ�һ���������Ԫ��)��
//	back()�᷵��queue�����һ��Ԫ��(Ҳ������󱻲����Ԫ��)��
//	pop()���queue���Ƴ�һ��Ԫ�ء�
//	ע�⣺pop()��Ȼ���Ƴ���һ��Ԫ�أ����ǲ�����������front()��back()������һ��Ԫ�ص������Ƴ���Ԫ�ء�
//	================================================================**/
//	vector<queue<POINT3D>*> vque;//�������б���������Ӷ��е����飬����ŵ������Ӷ��е�ָ��
//	queue<POINT3D> *pque;//ָ�����Ӷ��е�ָ��
//	POINT3D temp;//���ӵ� 
//	bool actives;//��ĳһˮλ�������б�ǵ�����������ı�־
//	int WaterLevel;//
//	int* array;//
//	int i,j,k,l,m,n,x,y,z,row,col,height;//����һЩ����
//	bool z_up,z_down,_up,up,up_,_down,down,down_,_right,right,right_,_left,left,left_,_upleft,upleft,upleft_,
//		_upright,upright, upright_,_downleft,downleft,downleft_,_downright,downright,downright_;//27 directions...
//	int seednum;//���ӵ���Ŀ 
//	int nonzeronum = 0;//��������ʶ�ţ������ʼ����ֱ��Ϊ��
//	col = OriginalImage.getXsize();
//	row = OriginalImage.getYsize();
//	height = OriginalImage.getZsize();
//	cout<<col<<" "<<row<<" "<<height<<endl;
//	//�ֽ����ͼ�����
//	for(k=0;k<height;k++)
//		for(i=0;i<row;i++)
//			for(j=0;j<col;j++)
//				LabelImage.put(j,i,k,0);
//	//Ԥ����,��ȡ����ÿ��������򣬲���ʼ��ÿ����ǵ����Ӷ���
//	//������ָ��������Ե�ĵ㣬���ǿ�����ˮλ����ʱ������û������˵������
//	for(k=0;k<height;k++){
//		for(i=0;i<row;i++){
//			for(j=0;j<col;j++){
//				if(SeedImage.get(j,i,k) == 1){//����ҵ�һ���������  
//					Num++;//����ı�ʶ�ż�һ
//					//�������鲢��ʼ��Ϊ�㣬��ʾ����256���ҽ�
//					array = new int[256];
//					Zero(array,256);//���㴦��
//					//���Ӹ��������vector��ÿ��ɨ��������һ�����飬���������ʶ��������һά���Ҷȼ����ڶ�ά��
//					//��ʾĳ�����������ĳ�ҽ�����Ӧ�ĵ����Ŀ��
//					SeedCounts.push_back(array);//push_back()������ʾ��������ӵ�vector��β����������Ҫ�������ڴ档
//					//��ʾ��Ӧһ���ҽ���һ�����У�����ÿ�����п��Դ洢һ�����ϵĵ���Ϣ
//					pque = new queue<POINT3D>[256];
//					//���뵽���������У���Ӧ���Ǳ���Ǻ�Num��
//					vque.push_back(pque);
//					//��ǰ����뱾����������ʱ���Ӷ�����
//					temp.x = j;
//					temp.y = i;
//					temp.z = k;
//					quetem.push(temp);
//					//��ǰ����Ϊ�Ѵ���
//					LabelImage.put(j,i,k,(unsigned char)Num);
//					SeedImage.put(j,i,k,127);//��ʾ�Ѿ������
//					//����ʱ���Ӷ����е����ӽ�������ֱ�����е����Ӷ��������
//					//������Ϻ�Ķ�����Ϣ������vque�У���������źͻҽף���Ӧ�����洢��seedcounts��
//					while(!quetem.empty()){ // ����ʱ���Ӷ��зǿ�ʱ
//						z_up=z_down=false;
//						_up=_down=_right=_left=false;  
//						up=down=right=left=false; 
//						up_=down_=right_=left_=false; 
//						_upleft=_upright=_downleft=_downright=false;
//						upleft=upright=downleft=downright=false;
//						upleft_=upright_=downleft_=downright_=false;
//						temp=quetem.front();//�Ӷ�����ȡ��һ������
//						x = temp.x;
//						y = temp.y;
//						z = temp.z;
//						quetem.pop();//��queue���Ƴ�һ��Ԫ�ء�
//						//ע�⵽127��ɨ����̵�Ӱ�죬Ӱ������ıȽϣ����ǲ�Ӱ��while����е�ɨ�� 
//						if(x > 0){
//							if(SeedImage.get(x-1,y,z) == 1){ //ͬ������Ϊ�����������Ϊ������
//								temp.x = x-1;
//								temp.y = y;
//								temp.z = z;
//								quetem.push(temp);//��������Ļ�����ô��Щ��ǹ��������ٴ���whileѭ���б�ɨ�赽�����ᣬ��Ϊֵ��127
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ���򣬲���¼����ŵ�labelImage
//								LabelImage.put(x-1,y,z,(unsigned char) Num);
//								SeedImage.put(x-1,y,z,127);
//							}
//							else{//����ͬ����Ϊ��������
//								left = true; 			 
//							}
//						}
//						if(x > 0  &&  y > 0){
//							if(SeedImage.get(x-1,y-1,z) == 1){//ͬ�����Ϸ���Ϊ�����������Ϊ������	 
//								temp.x = x-1;
//								temp.y = y-1;
//								temp.z = z;
//								quetem.push(temp);
//								//�����ӵ���Ϊ����û���򣬼���һ��ѭ������127����ʶ����ɨ�裬�����ǵ�ǰ����
//								LabelImage.put(x-1,y-1,z,(unsigned char) Num);
//								SeedImage.put(x-1,y-1,z,127);
//							}
//							else{//����ͬ�����Ϸ�Ϊ��������
//								upleft=true;
//							}
//						}
//						if(x > 0  &&  y > 0  &&  z > 0){
//							if(SeedImage.get(x-1,y-1,z-1) == 1){ //�²����Ϸ���Ϊ�����������Ϊ������
//								temp.x = x-1;
//								temp.y = y-1;
//								temp.z = z-1;
//								quetem.push(temp);//��������Ļ�����ô��Щ��ǹ��������ٴ���whileѭ���б�ɨ�赽�����ᣬ��Ϊֵ��127
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ���򣬲���¼����ŵ�labelImage
//								LabelImage.put(x-1,y-1,z-1,(unsigned char) Num);
//								SeedImage.put(x-1,y-1,z-1,127);
//							}
//							else{//�����²����Ϸ�Ϊ��������
//								_upleft = true; 			 
//							}
//						}
//						if(x > 0  &&  y > 0  &&  z < (height-1)){
//							if(SeedImage.get(x-1,y-1,z+1) == 1){//�ϲ����Ϸ���Ϊ�����������Ϊ������	 
//								temp.x = x-1;
//								temp.y = y-1;
//								temp.z = z+1;
//								quetem.push(temp);
//								//�����ӵ���Ϊ����û���򣬼���һ��ѭ������127����ʶ����ɨ�裬�����ǵ�ǰ����
//								LabelImage.put(x-1,y-1,z+1,(unsigned char) Num);
//								SeedImage.put(x-1,y-1,z+1,127);
//							}
//							else{//�����ϲ����Ϸ�Ϊ��������
//								upleft_ = true;
//							}
//						}
//						if(x > 0  &&  y < (row-1)){			 
//							if(SeedImage.get(x-1,y+1,z) == 1){//ͬ�����·���Ϊ�����������Ϊ������					
//								temp.x = x-1;						
//								temp.y = y+1;						
//								temp.z = z;
//								quetem.push(temp);						 
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
//								LabelImage.put(x-1,y+1,z,(unsigned char) Num);					
//								SeedImage.put(x-1,y+1,z,127);				
//							}		
//							else{//����ͬ�����·�Ϊ��������			 
//								downleft = true;				 
//							}			 
//						}	
//						if(x > 0  &&  y < (row-1)  &&  z > 0){			 
//							if(SeedImage.get(x-1,y+1,z-1) == 1){//�²����·���Ϊ�����������Ϊ������					
//								temp.x = x-1;						
//								temp.y = y+1;						
//								temp.z = z-1;
//								quetem.push(temp);						 
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
//								LabelImage.put(x-1,y+1,z-1,(unsigned char) Num);					
//								SeedImage.put(x-1,y+1,z-1,127);				
//							}		
//							else{//�����²����·�Ϊ��������			 
//								_downleft = true;				 
//							}			 
//						}	
//						if(x > 0  &&  y < (row-1)  &&  z < (height-1)){			 
//							if(SeedImage.get(x-1,y+1,z+1) == 1){//�ϲ����·���Ϊ�����������Ϊ������					
//								temp.x = x-1;						
//								temp.y = y+1;						
//								temp.z = z+1;
//								quetem.push(temp);						 
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
//								LabelImage.put(x-1,y+1,z+1,(unsigned char) Num);					
//								SeedImage.put(x-1,y+1,z+1,127);				
//							}		
//							else{//�����ϲ����·�Ϊ��������			 
//								downleft_ = true;				 
//							}			 
//						}
//						if(x > 0  &&  z > 0){			 
//							if(SeedImage.get(x-1,y,z-1) == 1){//�²�����Ϊ�����������Ϊ������					
//								temp.x = x-1;						
//								temp.y = y;						
//								temp.z = z-1;
//								quetem.push(temp);						 
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
//								LabelImage.put(x-1,y,z-1,(unsigned char) Num);					
//								SeedImage.put(x-1,y,z-1,127);				
//							}		
//							else{//�����²���Ϊ��������			 
//								_left = true;				 
//							}			 
//						}
//						if(x > 0  &&  z < (height-1)){			 
//							if(SeedImage.get(x-1,y,z+1) == 1){//�ϲ�����Ϊ�����������Ϊ������					
//								temp.x = x-1;						
//								temp.y = y;						
//								temp.z = z+1;
//								quetem.push(temp);						 
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
//								LabelImage.put(x-1,y,z+1,(unsigned char) Num);					
//								SeedImage.put(x-1,y,z+1,127);				
//							}		
//							else{//�����ϲ���Ϊ��������			 
//								left_ = true;				 
//							}			 
//						}
//						if(x < (col-1)){
//							if(SeedImage.get(x+1,y,z) == 1){ //ͬ���ҷ���Ϊ�����������Ϊ������
//								temp.x = x+1;
//								temp.y = y;
//								temp.z = z;
//								quetem.push(temp);//��������Ļ�����ô��Щ��ǹ��������ٴ���whileѭ���б�ɨ�赽�����ᣬ��Ϊֵ��127
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ���򣬲���¼����ŵ�labelImage
//								LabelImage.put(x+1,y,z,(unsigned char) Num);
//								SeedImage.put(x+1,y,z,127);
//							}
//							else{//����ͬ���ҷ�Ϊ��������
//								right = true; 			 
//							}
//						}
//						if(x < (col-1)  &&  y > 0){
//							if(SeedImage.get(x+1,y-1,z) == 1){//ͬ�����Ϸ���Ϊ�����������Ϊ������	 
//								temp.x = x+1;
//								temp.y = y-1;
//								temp.z = z;
//								quetem.push(temp);
//								//�����ӵ���Ϊ����û���򣬼���һ��ѭ������127����ʶ����ɨ�裬�����ǵ�ǰ����
//								LabelImage.put(x+1,y-1,z,(unsigned char) Num);
//								SeedImage.put(x+1,y-1,z,127);
//							}
//							else{//����ͬ�����Ϸ�Ϊ��������
//								upright = true;
//							}
//						}
//						if(x <(col-1)  &&  y > 0  &&  z > 0){
//							if(SeedImage.get(x+1,y-1,z-1) == 1){ //�²����Ϸ���Ϊ�����������Ϊ������
//								temp.x = x+1;
//								temp.y = y-1;
//								temp.z = z-1;
//								quetem.push(temp);//��������Ļ�����ô��Щ��ǹ��������ٴ���whileѭ���б�ɨ�赽�����ᣬ��Ϊֵ��127
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ���򣬲���¼����ŵ�labelImage
//								LabelImage.put(x+1,y-1,z-1,(unsigned char) Num);
//								SeedImage.put(x+1,y-1,z-1,127);
//							}
//							else{//�����²����Ϸ�Ϊ��������
//								_upright = true; 			 
//							}
//						}
//						if(x < (col-1)  &&  y > 0  &&  z < (height-1)){
//							if(SeedImage.get(x+1,y-1,z+1) == 1){//�ϲ����Ϸ���Ϊ�����������Ϊ������	 
//								temp.x = x+1;
//								temp.y = y-1;
//								temp.z = z+1;
//								quetem.push(temp);
//								//�����ӵ���Ϊ����û���򣬼���һ��ѭ������127����ʶ����ɨ�裬�����ǵ�ǰ����
//								LabelImage.put(x+1,y-1,z+1,(unsigned char) Num);
//								SeedImage.put(x+1,y-1,z+1,127);
//							}
//							else{//�����ϲ����Ϸ�Ϊ��������
//								upright_ = true;
//							}
//						}
//						if(x < (col-1)   &&  y < (row-1)){			 
//							if(SeedImage.get(x+1,y+1,z) == 1){//ͬ�����·���Ϊ�����������Ϊ������					
//								temp.x = x+1;						
//								temp.y = y+1;						
//								temp.z = z;
//								quetem.push(temp);						 
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
//								LabelImage.put(x+1,y+1,z,(unsigned char) Num);					
//								SeedImage.put(x+1,y+1,z,127);				
//							}		
//							else{//����ͬ�����·�Ϊ��������			 
//								downright = true;				 
//							}			 
//						}	
//						if(x < (col-1)  &&  y < (row-1)  &&  z > 0){			 
//							if(SeedImage.get(x+1,y+1,z-1) == 1){//�²����·���Ϊ�����������Ϊ������					
//								temp.x = x+1;						
//								temp.y = y+1;						
//								temp.z = z-1;
//								quetem.push(temp);						 
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
//								LabelImage.put(x+1,y+1,z-1,(unsigned char) Num);					
//								SeedImage.put(x+1,y+1,z-1,127);				
//							}		
//							else{//�����²����·�Ϊ��������			 
//								_downright = true;				 
//							}			 
//						}	
//						if(x < (col-1)  &&  y < (row-1)  &&  z < (height-1)){			 
//							if(SeedImage.get(x+1,y+1,z+1) == 1){//�ϲ����·���Ϊ�����������Ϊ������					
//								temp.x = x+1;						
//								temp.y = y+1;						
//								temp.z = z+1;
//								quetem.push(temp);						 
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
//								LabelImage.put(x+1,y+1,z+1,(unsigned char) Num);					
//								SeedImage.put(x+1,y+1,z+1,127);				
//							}		
//							else{//�����ϲ����·�Ϊ��������			 
//								downright_ = true;				 
//							}			 
//						}
//						if(x < (col-1)  &&  z > 0){			 
//							if(SeedImage.get(x+1,y,z-1) == 1){//�²��ҷ���Ϊ�����������Ϊ������					
//								temp.x = x+1;						
//								temp.y = y;						
//								temp.z = z-1;
//								quetem.push(temp);						 
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
//								LabelImage.put(x+1,y,z-1,(unsigned char) Num);					
//								SeedImage.put(x+1,y,z-1,127);				
//							}		
//							else{//�����²��ҷ�Ϊ��������			 
//								_right = true;				 
//							}			 
//						}
//						if(x < (col-1)  &&  z < (height-1)){			 
//							if(SeedImage.get(x+1,y,z+1) == 1){//�ϲ��ҷ���Ϊ�����������Ϊ������					
//								temp.x = x+1;						
//								temp.y = y;						
//								temp.z = z+1;
//								quetem.push(temp);						 
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
//								LabelImage.put(x+1,y,z+1,(unsigned char) Num);					
//								SeedImage.put(x+1,y,z+1,127);				
//							}		
//							else{//�����ϲ��ҷ�Ϊ��������			 
//								right_ = true;				 
//							}			 
//						}
//						if(y > 0){
//							if(SeedImage.get(x,y-1,z) == 1){ //ͬ���Ϸ���Ϊ�����������Ϊ������
//								temp.x = x;
//								temp.y = y-1;
//								temp.z = z;
//								quetem.push(temp);//��������Ļ�����ô��Щ��ǹ��������ٴ���whileѭ���б�ɨ�赽�����ᣬ��Ϊֵ��127
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ���򣬲���¼����ŵ�labelImage
//								LabelImage.put(x,y-1,z,(unsigned char) Num);
//								SeedImage.put(x,y-1,z,127);
//							}
//							else{//����ͬ���Ϸ�Ϊ��������
//								up = true; 			 
//							}
//						}
//						if(y > 0  &&  z > 0 ){
//							if(SeedImage.get(x,y-1,z-1) == 1){//�²��Ϸ���Ϊ�����������Ϊ������	 
//								temp.x = x;
//								temp.y = y-1;
//								temp.z = z-1;
//								quetem.push(temp);
//								//�����ӵ���Ϊ����û���򣬼���һ��ѭ������127����ʶ����ɨ�裬�����ǵ�ǰ����
//								LabelImage.put(x,y-1,z-1,(unsigned char) Num);
//								SeedImage.put(x,y-1,z-1,127);
//							}
//							else{//�����²��Ϸ�Ϊ��������
//								_up = true;
//							}
//						}
//						if(y > 0  &&  z < (height-1)){
//							if(SeedImage.get(x,y-1,z+1) == 1){ //�ϲ��Ϸ���Ϊ�����������Ϊ������
//								temp.x = x;
//								temp.y = y-1;
//								temp.z = z+1;
//								quetem.push(temp);//��������Ļ�����ô��Щ��ǹ��������ٴ���whileѭ���б�ɨ�赽�����ᣬ��Ϊֵ��127
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ���򣬲���¼����ŵ�labelImage
//								LabelImage.put(x,y-1,z+1,(unsigned char) Num);
//								SeedImage.put(x,y-1,z+1,127);
//							}
//							else{//�����ϲ��Ϸ�Ϊ��������
//								up_ = true; 			 
//							}
//						}
//						if( y < (row-1)){
//							if(SeedImage.get(x,y+1,z) == 1){//ͬ���·���Ϊ�����������Ϊ������	 
//								temp.x = x;
//								temp.y = y+1;
//								temp.z = z;
//								quetem.push(temp);
//								//�����ӵ���Ϊ����û���򣬼���һ��ѭ������127����ʶ����ɨ�裬�����ǵ�ǰ����
//								LabelImage.put(x,y+1,z,(unsigned char) Num);
//								SeedImage.put(x,y+1,z,127);
//							}
//							else{//����ͬ���·�Ϊ��������
//								down = true;
//							}
//						}
//						if(y < (row-1)  &&  z > 0){			 
//							if(SeedImage.get(x,y+1,z-1) == 1){//�²��·���Ϊ�����������Ϊ������					
//								temp.x = x;						
//								temp.y = y+1;						
//								temp.z = z-1;
//								quetem.push(temp);						 
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
//								LabelImage.put(x,y+1,z-1,(unsigned char) Num);					
//								SeedImage.put(x,y+1,z-1,127);				
//							}		
//							else{//�����²��·�Ϊ��������			 
//								_down = true;				 
//							}			 
//						}	
//						if(y < (row-1)  &&  z < (height-1)){			 
//							if(SeedImage.get(x,y+1,z+1) == 1){//�ϲ��·���Ϊ�����������Ϊ������					
//								temp.x = x;						
//								temp.y = y+1;						
//								temp.z = z+1;
//								quetem.push(temp);						 
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
//								LabelImage.put(x,y+1,z+1,(unsigned char) Num);					
//								SeedImage.put(x,y+1,z+1,127);				
//							}		
//							else{//�����ϲ��·�Ϊ��������			 
//								down_ = true;				 
//							}			 
//						}	
//						if(z > 0){			 
//							if(SeedImage.get(x,y,z-1) == 1){//�²��Ӧ����Ϊ�����������Ϊ������					
//								temp.x = x;						
//								temp.y = y;						
//								temp.z = z-1;
//								quetem.push(temp);						 
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
//								LabelImage.put(x,y,z-1,(unsigned char) Num);					
//								SeedImage.put(x,y,z-1,127);				
//							}		
//							else{//�����²��Ӧ��Ϊ��������			 
//								z_down = true;				 
//							}			 
//						}
//						if(z < (height-1)){			 
//							if(SeedImage.get(x,y,z+1) == 1){//�ϲ��Ӧ����Ϊ�����������Ϊ������					
//								temp.x = x;						
//								temp.y = y;						
//								temp.z = z+1;
//								quetem.push(temp);						 
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
//								LabelImage.put(x,y,z+1,(unsigned char) Num);					
//								SeedImage.put(x,y,z+1,127);				
//							}		
//							else{//�����ϲ��Ӧ��Ϊ��������			 
//								z_up = true;				 
//							}			 
//						}
//						//��������ֻҪ��һ�㲻����������ô����Ϊ��ʼ���Ӷ����е�һ��
//
//						//����ɷ���������seedimage�е�ֵ�������ġ�
//
//						if( _up||_down||_right||_left||_upleft||_downleft||_upright||_downright||
//							up || down|| right|| left|| upleft|| downleft|| upright|| downright||
//							up_||down_||right_||left_||upleft_||downleft_||upright_||downright_||z_up || z_down){	
//								temp.x = x; 
//								temp.y = y;
//								temp.z = z;
//								//�������ʸ�����飺��һά�Ǳ�Ǻţ��ڶ�ά�Ǹ�ͼ���ĻҶȼ�
//								//x,y,z���Ӧ����whileѭ������ɨ������ص㡣
//								//Num�ǵ�ǰ�������
//								//���������ά��Ϣ�ͱ�ʾ�ˣ�ĳ�������ж�Ӧĳ���Ҷȼ���Ӧ�ĳ�Ա��ļ��������
//								//�ֱ������������������
//								vque[Num-1][OriginalImage.get(x,y,z)].push(temp);
//								SeedCounts[Num-1][OriginalImage.get(x,y,z)]++;
//
//						}    
//					}//while������ɨ�赽quetemΪ�ն�ֹ��Ҳ���Ƕ�Ӧ���еĽڵ㶼�õ���������Ϊֹ����������Χ�ĵ�Ҫô����������Ҫô��������       
//				}//if����
//				// if(Num==5)  return;       
//			}//for 
//		}//for 
//	}//for
//	//�����������У������ǵĵ�Ϊ0���ʾ��û��ɨ�赽�ĵ㣬���߱���������������ӵ�
//	//�����൱������seedimage�������ĳ�ʼ����ķ�ˮ����ߵ����еĵ㣻�����ñ�ż�¼ÿ������ͬʱ��ˮ��ı�Ե�������С�
//	//�������Ҽ�ˮ��ĳ���ͬʱҲ����ͨ����
//	/*************************************/
//	//test �������һ��ʣ�µķ�ˮ��صĵ�����
//	seednum=0;
//	//for(k=0;k<height;k++){
//	//	for(i=0;i<row;i++){
//	//		for(j=0;j<col;j++){
//	//			if(SeedImage.get(j,i,k) == 0)  seednum++;
//	//		}
//	//	}
//	//}
//	//cout<<"��ˮ��صĵ���:"<<seednum<<endl;
//	//��û���̿�ʼ��ˮλ���㿪ʼ������ˮλ��Ӧ�Ҷȼ�����������ͨ��
//	for(WaterLevel=0;WaterLevel<256;WaterLevel++){//�ڶ�ά������
//		actives = true;//��ĳһˮλ�������б�ǵ�����������ı�־ 
//		while(actives){//���е�����������ȫ
//			actives = false;
//			//���δ���ÿ����Ǻ�����Ӧ�������������ǺŶ�Ӧ������ĵ�ĸ�����SeedCounts����
//			for(i=0;i<Num;i++){//��һά�������������������
//				if(!vque[i][WaterLevel].empty()){//��Ӧ�ķ�ˮ�벻Ϊ�ռ�,i��ʾ����ţ�waterlevel��ʾ�ҽ�
//					actives = true;//
//					while(SeedCounts[i][WaterLevel]>0){//���ӻ���ʣ��������
//						SeedCounts[i][WaterLevel]--;//ȡ��һ���㣬���Ӹ�������һ
//						temp=vque[i][WaterLevel].front();//ȡ���������һ���㣬��������Ե�㣬��ʾ��ǰ�Ҷȼ��������Ѿ�������ˡ�
//						vque[i][WaterLevel].pop();//���
//						x = temp.x;//��ǰ���ӵ�����
//						y = temp.y;
//						z = temp.z;
//						if(x > 0){//��
//							if(!LabelImage.get(x-1,y,z)){//����δ������ʾû�б�ţ�Ӧ��������ǰ�Ѿ�������ʼ��Ϊ0
//								//���������ڿ�ͷҲ������ʼ��
//								temp.x = x-1;
//								temp.y = y;
//								temp.z = z;
//								LabelImage.put(x-1,y,z,(unsigned char)(i+1));//�󷽵���Ϊ����û����
//								//ע�⵽����������ɨ�����������ͬ��һ�����������������������ǵ�
//								//��������һ�����ٻ�ɨ�赽����㣬ȷ������©��������һ�ֵĴ����ʹ������
//								//���������⻹��������ǲ�û��һ���������뵽���Ӷ��С�Ҳ����˵��
//								//ֻ�Ǳ���û������������û��ֻ�����������������������С�
//								if(OriginalImage.get(x-1,y,z) <= WaterLevel){//����Ϊ������������뵱ǰ���У���ǰ�߶ȵĶ���
//									vque[i][WaterLevel].push(temp);	  
//								}
//								else{//�������OriginalImage[m-1][n]��Ӧ�ĻҶȼ��Ķ��У�Ϊʲô��
//									vque[i][OriginalImage.get(x-1,y,z)].push(temp);
//									SeedCounts[i][OriginalImage.get(x-1,y,z)]++;
//								}
//							}
//						}
//						if(x < (col-1)){//��
//							if(!LabelImage.get(x+1,y,z)){//�ҷ���δ����
//								temp.x = x+1;
//								temp.y = y;
//								temp.z = z;
//								LabelImage.put(x+1,y,z,(unsigned char)(i+1));//�ҷ�����Ϊ����û����
//								if(OriginalImage.get(x+1,y,z) <= WaterLevel){//�ҷ���Ϊ������������뵱ǰ����
//									vque[i][WaterLevel].push(temp);
//								}
//								else{//�������OriginalImage[m+1][n]������
//									vque[i][OriginalImage.get(x+1,y,z)].push(temp);
//									SeedCounts[i][OriginalImage.get(x+1,y,z)]++;
//								}
//							}
//						}
//						if(y > 0){//�Ϸ�
//							if(!LabelImage.get(x,y-1,z)){//�Ϸ���δ����
//								temp.x = x; 
//								temp.y = y-1;
//								temp.z = z;
//								LabelImage.put(x,y-1,z,(unsigned char)(i+1));//�Ϸ�����Ϊ����û����    
//								if(OriginalImage.get(x,y-1,z) <= WaterLevel){//�Ϸ���Ϊ������������뵱ǰ����
//									vque[i][WaterLevel].push(temp);
//								}
//								else{//�������OriginalImage[m][n+1]������
//									vque[i][OriginalImage.get(x,y-1,z)].push(temp);
//									SeedCounts[i][OriginalImage.get(x,y-1,z)]++;
//								}
//							}
//						}
//						if(y < (row-1)){//�·�
//							if(!LabelImage.get(x,y+1,z)){//�·���δ����
//								temp.x = x;
//								temp.y = y+1;
//								temp.z = z;
//								LabelImage.put(x,y+1,z,(unsigned char)(i+1));//�·�����Ϊ����û����
//								if(OriginalImage.get(x,y+1,z) <= WaterLevel){//�·���Ϊ������������뵱ǰ����
//									vque[i][WaterLevel].push(temp);
//								}
//								else{//�������OriginalImage[m][n-1]������
//									vque[i][OriginalImage.get(x,y+1,z)].push(temp);
//									SeedCounts[i][OriginalImage.get(x,y+1,z)]++;
//								}
//							}
//						}
//						if(z>0){//ǰ��
//							if(!LabelImage.get(x,y,z-1)){//ǰ����δ����
//								temp.x = x;
//								temp.y = y;
//								temp.z = z-1;
//								LabelImage.put(x,y,z-1,(unsigned char)(i+1));//ǰ������Ϊ����û����
//								if(OriginalImage.get(x,y,z-1) <= WaterLevel){//ǰ����Ϊ������������뵱ǰ����
//									vque[i][WaterLevel].push(temp);
//								}
//								else{//�������OriginalImage[m][n-1]������
//									vque[i][OriginalImage.get(x,y,z-1)].push(temp);
//									SeedCounts[i][OriginalImage.get(x,y,z-1)]++;
//								}
//							}
//						}
//						if(z < (height-1)){//��
//							if(!LabelImage.get(x,y,z+1)){//����δ����
//								temp.x = x;
//								temp.y = y;
//								temp.z = z+1;
//								LabelImage.put(x,y,z+1,(unsigned char)(i+1));//�󷽵���Ϊ����û����
//								if(OriginalImage.get(x,y,z+1) <= WaterLevel){//����Ϊ������������뵱ǰ����
//									vque[i][WaterLevel].push(temp);
//								}
//								else{//�������OriginalImage[m][n-1]������
//									vque[i][OriginalImage.get(x,y,z+1)].push(temp);
//									SeedCounts[i][OriginalImage.get(x,y,z+1)]++;
//								}
//							}
//						}
//					}//whileѭ������
//					SeedCounts[i][WaterLevel] = vque[i][WaterLevel].size();//�����Ӷ���ָ��ĸ������ݸ����Ӹ���
//				}//if����:�ڶ�Ӧ�ķ�ˮ�벻Ϊ�ռ��������
//			}//��һάforѭ����������������
//		}//whileѭ������
//	}//forѭ������
//
//	/**********************************/
//	//test whether the origional segmentation num is changed...
//	nonzeronum=Num;
//	for(m=0;m<Num;m++){
//		for(k=0;k<height;k++){	 
//			for(i=0;i<row;i++){
//				for(j=0;j<col;j++){
//					if(LabelImage.get(j,i,k) == m)	break;
//				}
//				if(LabelImage.get(j,i,k) == m) break;
//			}
//			if(LabelImage.get(j,i,k) == m) break;
//		}
//		if(j==col-1 && i==row-1 && k==height-1 && LabelImage.get(j,i,k)!=m)  nonzeronum--;
//	}
//	cout<<"nonzeronum="<<nonzeronum<<endl;
//	//�ͷ�pque�ڴ�
//	while(!vque.empty()){
//		pque=vque.back();
//		delete[] pque;
//		vque.pop_back();
//	}
//	//�ͷ�array�ڴ�
//	while(!SeedCounts.empty()){
//		array=SeedCounts.back();
//		delete[] array;
//		SeedCounts.pop_back();
//	}
//}
//void Watersheds(const Raw *OriginalImage, Raw *SeedImage, Raw *LabelImage){
//	int Num=0;//��������ʶ�ţ���1��ʼ
//	vector<int*> SeedCounts;//����ÿ���������Ӹ���������
//	queue<POINT3D> quetem;//��ʱ���Ӷ���
//	/**================================================================
//	��Ա����
//	empty()�ж϶��пգ������п�ʱ������true��
//	size()���ʶ����е�Ԫ�ظ�����
//	push()�Ὣһ��Ԫ������queue�С�
//	front()�᷵��queue�ڵ���һ��Ԫ��(Ҳ���ǵ�һ���������Ԫ��)��
//	back()�᷵��queue�����һ��Ԫ��(Ҳ������󱻲����Ԫ��)��
//	pop()���queue���Ƴ�һ��Ԫ�ء�
//	ע�⣺pop()��Ȼ���Ƴ���һ��Ԫ�أ����ǲ�����������front()��back()������һ��Ԫ�ص������Ƴ���Ԫ�ء�
//	================================================================**/
//	vector<queue<POINT3D>*> vque;//�������б���������Ӷ��е����飬����ŵ������Ӷ��е�ָ��
//	queue<POINT3D> *pque;//ָ�����Ӷ��е�ָ��
//	POINT3D temp;//���ӵ� 
//	bool actives;//��ĳһˮλ�������б�ǵ�����������ı�־
//	int WaterLevel;//
//	int* array;//
//	int i,j,k,l,m,n,x,y,z,row,col,height;//����һЩ����
//	bool z_up,z_down,_up,up,up_,_down,down,down_,_right,right,right_,_left,left,left_,_upleft,upleft,upleft_,
//		_upright,upright, upright_,_downleft,downleft,downleft_,_downright,downright,downright_;//27 directions...
//	int seednum;//���ӵ���Ŀ 
//	int nonzeronum = 0;//��������ʶ�ţ������ʼ����ֱ��Ϊ��
//	col = OriginalImage->getXsize();
//	row = OriginalImage->getYsize();
//	height = OriginalImage->getZsize();
//	//�ֽ����ͼ�����
//	for(k=0;k<height;k++)
//		for(i=0;i<row;i++)
//			for(j=0;j<col;j++)
//				LabelImage->put(j,i,k,0);
//	//Ԥ����,��ȡ����ÿ��������򣬲���ʼ��ÿ����ǵ����Ӷ���
//	//������ָ��������Ե�ĵ㣬���ǿ�����ˮλ����ʱ������û������˵������
//	for(k=0;k<height;k++){
//		for(i=0;i<row;i++){
//			for(j=0;j<col;j++){
//				if(SeedImage->get(j,i,k) == 1){//����ҵ�һ���������  
//					Num++;//����ı�ʶ�ż�һ
//					//�������鲢��ʼ��Ϊ�㣬��ʾ����256���ҽ�
//					array = new int[256];
//					Zero(array,256);//���㴦��
//					//���Ӹ��������vector��ÿ��ɨ��������һ�����飬���������ʶ��������һά���Ҷȼ����ڶ�ά��
//					//��ʾĳ�����������ĳ�ҽ�����Ӧ�ĵ����Ŀ��
//					SeedCounts.push_back(array);//push_back()������ʾ��������ӵ�vector��β����������Ҫ�������ڴ档
//					//��ʾ��Ӧһ���ҽ���һ�����У�����ÿ�����п��Դ洢һ�����ϵĵ���Ϣ
//					pque = new queue<POINT3D>[256];
//					//���뵽���������У���Ӧ���Ǳ���Ǻ�Num��
//					vque.push_back(pque);
//					//��ǰ����뱾����������ʱ���Ӷ�����
//					temp.x = j;
//					temp.y = i;
//					temp.z = k;
//					quetem.push(temp);
//					//��ǰ����Ϊ�Ѵ���
//					LabelImage->put(j,i,k,(unsigned char)Num);
//					SeedImage->put(j,i,k,127);//��ʾ�Ѿ������
//					//����ʱ���Ӷ����е����ӽ�������ֱ�����е����Ӷ��������
//					//������Ϻ�Ķ�����Ϣ������vque�У���������źͻҽף���Ӧ�����洢��seedcounts��
//					while(!quetem.empty()){ // ����ʱ���Ӷ��зǿ�ʱ
//						z_up=z_down=false;
//						_up=_down=_right=_left=false;  
//						up=down=right=left=false; 
//						up_=down_=right_=left_=false; 
//						_upleft=_upright=_downleft=_downright=false;
//						upleft=upright=downleft=downright=false;
//						upleft_=upright_=downleft_=downright_=false;
//						temp=quetem.front();//�Ӷ�����ȡ��һ������
//						x = temp.x;
//						y = temp.y;
//						z = temp.z;
//						quetem.pop();//��queue���Ƴ�һ��Ԫ�ء�
//						//ע�⵽127��ɨ����̵�Ӱ�죬Ӱ������ıȽϣ����ǲ�Ӱ��while����е�ɨ�� 
//						if(x > 0){
//							if(SeedImage->get(x-1,y,z) == 1){ //ͬ������Ϊ�����������Ϊ������
//								temp.x = x-1;
//								temp.y = y;
//								temp.z = z;
//								quetem.push(temp);//��������Ļ�����ô��Щ��ǹ��������ٴ���whileѭ���б�ɨ�赽�����ᣬ��Ϊֵ��127
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ���򣬲���¼����ŵ�labelImage
//								LabelImage->put(x-1,y,z,(unsigned char) Num);
//								SeedImage->put(x-1,y,z,127);
//							}
//							else{//����ͬ����Ϊ��������
//								left = true; 			 
//							}
//						}
//						if(x > 0  &&  y > 0){
//							if(SeedImage->get(x-1,y-1,z) == 1){//ͬ�����Ϸ���Ϊ�����������Ϊ������	 
//								temp.x = x-1;
//								temp.y = y-1;
//								temp.z = z;
//								quetem.push(temp);
//								//�����ӵ���Ϊ����û���򣬼���һ��ѭ������127����ʶ����ɨ�裬�����ǵ�ǰ����
//								LabelImage->put(x-1,y-1,z,(unsigned char) Num);
//								SeedImage->put(x-1,y-1,z,127);
//							}
//							else{//����ͬ�����Ϸ�Ϊ��������
//								upleft=true;
//							}
//						}
//						if(x > 0  &&  y > 0  &&  z > 0){
//							if(SeedImage->get(x-1,y-1,z-1) == 1){ //�²����Ϸ���Ϊ�����������Ϊ������
//								temp.x = x-1;
//								temp.y = y-1;
//								temp.z = z-1;
//								quetem.push(temp);//��������Ļ�����ô��Щ��ǹ��������ٴ���whileѭ���б�ɨ�赽�����ᣬ��Ϊֵ��127
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ���򣬲���¼����ŵ�labelImage
//								LabelImage->put(x-1,y-1,z-1,(unsigned char) Num);
//								SeedImage->put(x-1,y-1,z-1,127);
//							}
//							else{//�����²����Ϸ�Ϊ��������
//								_upleft = true; 			 
//							}
//						}
//						if(x > 0  &&  y > 0  &&  z < (height-1)){
//							if(SeedImage->get(x-1,y-1,z+1) == 1){//�ϲ����Ϸ���Ϊ�����������Ϊ������	 
//								temp.x = x-1;
//								temp.y = y-1;
//								temp.z = z+1;
//								quetem.push(temp);
//								//�����ӵ���Ϊ����û���򣬼���һ��ѭ������127����ʶ����ɨ�裬�����ǵ�ǰ����
//								LabelImage->put(x-1,y-1,z+1,(unsigned char) Num);
//								SeedImage->put(x-1,y-1,z+1,127);
//							}
//							else{//�����ϲ����Ϸ�Ϊ��������
//								upleft_ = true;
//							}
//						}
//						if(x > 0  &&  y < (row-1)){			 
//							if(SeedImage->get(x-1,y+1,z) == 1){//ͬ�����·���Ϊ�����������Ϊ������					
//								temp.x = x-1;						
//								temp.y = y+1;						
//								temp.z = z;
//								quetem.push(temp);						 
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
//								LabelImage->put(x-1,y+1,z,(unsigned char) Num);					
//								SeedImage->put(x-1,y+1,z,127);				
//							}		
//							else{//����ͬ�����·�Ϊ��������			 
//								downleft = true;				 
//							}			 
//						}	
//						if(x > 0  &&  y < (row-1)  &&  z > 0){			 
//							if(SeedImage->get(x-1,y+1,z-1) == 1){//�²����·���Ϊ�����������Ϊ������					
//								temp.x = x-1;						
//								temp.y = y+1;						
//								temp.z = z-1;
//								quetem.push(temp);						 
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
//								LabelImage->put(x-1,y+1,z-1,(unsigned char) Num);					
//								SeedImage->put(x-1,y+1,z-1,127);				
//							}		
//							else{//�����²����·�Ϊ��������			 
//								_downleft = true;				 
//							}			 
//						}	
//						if(x > 0  &&  y < (row-1)  &&  z < (height-1)){			 
//							if(SeedImage->get(x-1,y+1,z+1) == 1){//�ϲ����·���Ϊ�����������Ϊ������					
//								temp.x = x-1;						
//								temp.y = y+1;						
//								temp.z = z+1;
//								quetem.push(temp);						 
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
//								LabelImage->put(x-1,y+1,z+1,(unsigned char) Num);					
//								SeedImage->put(x-1,y+1,z+1,127);				
//							}		
//							else{//�����ϲ����·�Ϊ��������			 
//								downleft_ = true;				 
//							}			 
//						}
//						if(x > 0  &&  z > 0){			 
//							if(SeedImage->get(x-1,y,z-1) == 1){//�²�����Ϊ�����������Ϊ������					
//								temp.x = x-1;						
//								temp.y = y;						
//								temp.z = z-1;
//								quetem.push(temp);						 
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
//								LabelImage->put(x-1,y,z-1,(unsigned char) Num);					
//								SeedImage->put(x-1,y,z-1,127);				
//							}		
//							else{//�����²���Ϊ��������			 
//								_left = true;				 
//							}			 
//						}
//						if(x > 0  &&  z < (height-1)){			 
//							if(SeedImage->get(x-1,y,z+1) == 1){//�ϲ�����Ϊ�����������Ϊ������					
//								temp.x = x-1;						
//								temp.y = y;						
//								temp.z = z+1;
//								quetem.push(temp);						 
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
//								LabelImage->put(x-1,y,z+1,(unsigned char) Num);					
//								SeedImage->put(x-1,y,z+1,127);				
//							}		
//							else{//�����ϲ���Ϊ��������			 
//								left_ = true;				 
//							}			 
//						}
//						if(x < (col-1)){
//							if(SeedImage->get(x+1,y,z) == 1){ //ͬ���ҷ���Ϊ�����������Ϊ������
//								temp.x = x+1;
//								temp.y = y;
//								temp.z = z;
//								quetem.push(temp);//��������Ļ�����ô��Щ��ǹ��������ٴ���whileѭ���б�ɨ�赽�����ᣬ��Ϊֵ��127
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ���򣬲���¼����ŵ�labelImage
//								LabelImage->put(x+1,y,z,(unsigned char) Num);
//								SeedImage->put(x+1,y,z,127);
//							}
//							else{//����ͬ���ҷ�Ϊ��������
//								right = true; 			 
//							}
//						}
//						if(x < (col-1)  &&  y > 0){
//							if(SeedImage->get(x+1,y-1,z) == 1){//ͬ�����Ϸ���Ϊ�����������Ϊ������	 
//								temp.x = x+1;
//								temp.y = y-1;
//								temp.z = z;
//								quetem.push(temp);
//								//�����ӵ���Ϊ����û���򣬼���һ��ѭ������127����ʶ����ɨ�裬�����ǵ�ǰ����
//								LabelImage->put(x+1,y-1,z,(unsigned char) Num);
//								SeedImage->put(x+1,y-1,z,127);
//							}
//							else{//����ͬ�����Ϸ�Ϊ��������
//								upright = true;
//							}
//						}
//						if(x <(col-1)  &&  y > 0  &&  z > 0){
//							if(SeedImage->get(x+1,y-1,z-1) == 1){ //�²����Ϸ���Ϊ�����������Ϊ������
//								temp.x = x+1;
//								temp.y = y-1;
//								temp.z = z-1;
//								quetem.push(temp);//��������Ļ�����ô��Щ��ǹ��������ٴ���whileѭ���б�ɨ�赽�����ᣬ��Ϊֵ��127
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ���򣬲���¼����ŵ�labelImage
//								LabelImage->put(x+1,y-1,z-1,(unsigned char) Num);
//								SeedImage->put(x+1,y-1,z-1,127);
//							}
//							else{//�����²����Ϸ�Ϊ��������
//								_upright = true; 			 
//							}
//						}
//						if(x < (col-1)  &&  y > 0  &&  z < (height-1)){
//							if(SeedImage->get(x+1,y-1,z+1) == 1){//�ϲ����Ϸ���Ϊ�����������Ϊ������	 
//								temp.x = x+1;
//								temp.y = y-1;
//								temp.z = z+1;
//								quetem.push(temp);
//								//�����ӵ���Ϊ����û���򣬼���һ��ѭ������127����ʶ����ɨ�裬�����ǵ�ǰ����
//								LabelImage->put(x+1,y-1,z+1,(unsigned char) Num);
//								SeedImage->put(x+1,y-1,z+1,127);
//							}
//							else{//�����ϲ����Ϸ�Ϊ��������
//								upright_ = true;
//							}
//						}
//						if(x < (col-1)   &&  y < (row-1)){			 
//							if(SeedImage->get(x+1,y+1,z) == 1){//ͬ�����·���Ϊ�����������Ϊ������					
//								temp.x = x+1;						
//								temp.y = y+1;						
//								temp.z = z;
//								quetem.push(temp);						 
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
//								LabelImage->put(x+1,y+1,z,(unsigned char) Num);					
//								SeedImage->put(x+1,y+1,z,127);				
//							}		
//							else{//����ͬ�����·�Ϊ��������			 
//								downright = true;				 
//							}			 
//						}	
//						if(x < (col-1)  &&  y < (row-1)  &&  z > 0){			 
//							if(SeedImage->get(x+1,y+1,z-1) == 1){//�²����·���Ϊ�����������Ϊ������					
//								temp.x = x+1;						
//								temp.y = y+1;						
//								temp.z = z-1;
//								quetem.push(temp);						 
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
//								LabelImage->put(x+1,y+1,z-1,(unsigned char) Num);					
//								SeedImage->put(x+1,y+1,z-1,127);				
//							}		
//							else{//�����²����·�Ϊ��������			 
//								_downright = true;				 
//							}			 
//						}	
//						if(x < (col-1)  &&  y < (row-1)  &&  z < (height-1)){			 
//							if(SeedImage->get(x+1,y+1,z+1) == 1){//�ϲ����·���Ϊ�����������Ϊ������					
//								temp.x = x+1;						
//								temp.y = y+1;						
//								temp.z = z+1;
//								quetem.push(temp);						 
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
//								LabelImage->put(x+1,y+1,z+1,(unsigned char) Num);					
//								SeedImage->put(x+1,y+1,z+1,127);				
//							}		
//							else{//�����ϲ����·�Ϊ��������			 
//								downright_ = true;				 
//							}			 
//						}
//						if(x < (col-1)  &&  z > 0){			 
//							if(SeedImage->get(x+1,y,z-1) == 1){//�²��ҷ���Ϊ�����������Ϊ������					
//								temp.x = x+1;						
//								temp.y = y;						
//								temp.z = z-1;
//								quetem.push(temp);						 
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
//								LabelImage->put(x+1,y,z-1,(unsigned char) Num);					
//								SeedImage->put(x+1,y,z-1,127);				
//							}		
//							else{//�����²��ҷ�Ϊ��������			 
//								_right = true;				 
//							}			 
//						}
//						if(x < (col-1)  &&  z < (height-1)){			 
//							if(SeedImage->get(x+1,y,z+1) == 1){//�ϲ��ҷ���Ϊ�����������Ϊ������					
//								temp.x = x+1;						
//								temp.y = y;						
//								temp.z = z+1;
//								quetem.push(temp);						 
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
//								LabelImage->put(x+1,y,z+1,(unsigned char) Num);					
//								SeedImage->put(x+1,y,z+1,127);				
//							}		
//							else{//�����ϲ��ҷ�Ϊ��������			 
//								right_ = true;				 
//							}			 
//						}
//						if(y > 0){
//							if(SeedImage->get(x,y-1,z) == 1){ //ͬ���Ϸ���Ϊ�����������Ϊ������
//								temp.x = x;
//								temp.y = y-1;
//								temp.z = z;
//								quetem.push(temp);//��������Ļ�����ô��Щ��ǹ��������ٴ���whileѭ���б�ɨ�赽�����ᣬ��Ϊֵ��127
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ���򣬲���¼����ŵ�labelImage
//								LabelImage->put(x,y-1,z,(unsigned char) Num);
//								SeedImage->put(x,y-1,z,127);
//							}
//							else{//����ͬ���Ϸ�Ϊ��������
//								up = true; 			 
//							}
//						}
//						if(y > 0  &&  z > 0 ){
//							if(SeedImage->get(x,y-1,z-1) == 1){//�²��Ϸ���Ϊ�����������Ϊ������	 
//								temp.x = x;
//								temp.y = y-1;
//								temp.z = z-1;
//								quetem.push(temp);
//								//�����ӵ���Ϊ����û���򣬼���һ��ѭ������127����ʶ����ɨ�裬�����ǵ�ǰ����
//								LabelImage->put(x,y-1,z-1,(unsigned char) Num);
//								SeedImage->put(x,y-1,z-1,127);
//							}
//							else{//�����²��Ϸ�Ϊ��������
//								_up = true;
//							}
//						}
//						if(y > 0  &&  z < (height-1)){
//							if(SeedImage->get(x,y-1,z+1) == 1){ //�ϲ��Ϸ���Ϊ�����������Ϊ������
//								temp.x = x;
//								temp.y = y-1;
//								temp.z = z+1;
//								quetem.push(temp);//��������Ļ�����ô��Щ��ǹ��������ٴ���whileѭ���б�ɨ�赽�����ᣬ��Ϊֵ��127
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ���򣬲���¼����ŵ�labelImage
//								LabelImage->put(x,y-1,z+1,(unsigned char) Num);
//								SeedImage->put(x,y-1,z+1,127);
//							}
//							else{//�����ϲ��Ϸ�Ϊ��������
//								up_ = true; 			 
//							}
//						}
//						if( y < (row-1)){
//							if(SeedImage->get(x,y+1,z) == 1){//ͬ���·���Ϊ�����������Ϊ������	 
//								temp.x = x;
//								temp.y = y+1;
//								temp.z = z;
//								quetem.push(temp);
//								//�����ӵ���Ϊ����û���򣬼���һ��ѭ������127����ʶ����ɨ�裬�����ǵ�ǰ����
//								LabelImage->put(x,y+1,z,(unsigned char) Num);
//								SeedImage->put(x,y+1,z,127);
//							}
//							else{//����ͬ���·�Ϊ��������
//								down = true;
//							}
//						}
//						if(y < (row-1)  &&  z > 0){			 
//							if(SeedImage->get(x,y+1,z-1) == 1){//�²��·���Ϊ�����������Ϊ������					
//								temp.x = x;						
//								temp.y = y+1;						
//								temp.z = z-1;
//								quetem.push(temp);						 
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
//								LabelImage->put(x,y+1,z-1,(unsigned char) Num);					
//								SeedImage->put(x,y+1,z-1,127);				
//							}		
//							else{//�����²��·�Ϊ��������			 
//								_down = true;				 
//							}			 
//						}	
//						if(y < (row-1)  &&  z < (height-1)){			 
//							if(SeedImage->get(x,y+1,z+1) == 1){//�ϲ��·���Ϊ�����������Ϊ������					
//								temp.x = x;						
//								temp.y = y+1;						
//								temp.z = z+1;
//								quetem.push(temp);						 
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
//								LabelImage->put(x,y+1,z+1,(unsigned char) Num);					
//								SeedImage->put(x,y+1,z+1,127);				
//							}		
//							else{//�����ϲ��·�Ϊ��������			 
//								down_ = true;				 
//							}			 
//						}	
//						if(z > 0){			 
//							if(SeedImage->get(x,y,z-1) == 1){//�²��Ӧ����Ϊ�����������Ϊ������					
//								temp.x = x;						
//								temp.y = y;						
//								temp.z = z-1;
//								quetem.push(temp);						 
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
//								LabelImage->put(x,y,z-1,(unsigned char) Num);					
//								SeedImage->put(x,y,z-1,127);				
//							}		
//							else{//�����²��Ӧ��Ϊ��������			 
//								z_down = true;				 
//							}			 
//						}
//						if(z < (height-1)){			 
//							if(SeedImage->get(x,y,z+1) == 1){//�ϲ��Ӧ����Ϊ�����������Ϊ������					
//								temp.x = x;						
//								temp.y = y;						
//								temp.z = z+1;
//								quetem.push(temp);						 
//								//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����						
//								LabelImage->put(x,y,z+1,(unsigned char) Num);					
//								SeedImage->put(x,y,z+1,127);				
//							}		
//							else{//�����ϲ��Ӧ��Ϊ��������			 
//								z_up = true;				 
//							}			 
//						}
//						//��������ֻҪ��һ�㲻����������ô����Ϊ��ʼ���Ӷ����е�һ��
//
//						//����ɷ���������seedimage�е�ֵ�������ġ�
//
//						if( _up||_down||_right||_left||_upleft||_downleft||_upright||_downright||
//							up || down|| right|| left|| upleft|| downleft|| upright|| downright||
//							up_||down_||right_||left_||upleft_||downleft_||upright_||downright_||z_up || z_down){	
//								temp.x = x; 
//								temp.y = y;
//								temp.z = z;
//								//�������ʸ�����飺��һά�Ǳ�Ǻţ��ڶ�ά�Ǹ�ͼ���ĻҶȼ�
//								//x,y,z���Ӧ����whileѭ������ɨ������ص㡣
//								//Num�ǵ�ǰ�������
//								//���������ά��Ϣ�ͱ�ʾ�ˣ�ĳ�������ж�Ӧĳ���Ҷȼ���Ӧ�ĳ�Ա��ļ��������
//								//�ֱ������������������
//								vque[Num-1][OriginalImage->get(x,y,z)].push(temp);
//								SeedCounts[Num-1][OriginalImage->get(x,y,z)]++;
//
//						}    
//					}//while������ɨ�赽quetemΪ�ն�ֹ��Ҳ���Ƕ�Ӧ���еĽڵ㶼�õ���������Ϊֹ����������Χ�ĵ�Ҫô����������Ҫô��������       
//				}//if����
//				// if(Num==5)  return;       
//			}//for 
//		}//for 
//	}//for
//	//�����������У������ǵĵ�Ϊ0���ʾ��û��ɨ�赽�ĵ㣬���߱���������������ӵ�
//	//�����൱������seedimage�������ĳ�ʼ����ķ�ˮ����ߵ����еĵ㣻�����ñ�ż�¼ÿ������ͬʱ��ˮ��ı�Ե�������С�
//	//�������Ҽ�ˮ��ĳ���ͬʱҲ����ͨ����
//	/*************************************/
//	//test �������һ��ʣ�µķ�ˮ��صĵ�����
//	seednum=0;
//	for(k=0;k<height;k++){
//		for(i=0;i<row;i++){
//			for(j=0;j<col;j++){
//				if(SeedImage->get(j,i,k) == 0)  seednum++;
//			}
//		}
//	}
//	cout<<"��ˮ��صĵ���:"<<seednum<<endl;
//	//��û���̿�ʼ��ˮλ���㿪ʼ������ˮλ��Ӧ�Ҷȼ�����������ͨ��
//	for(WaterLevel=0;WaterLevel<256;WaterLevel++){//�ڶ�ά������
//		actives = true;//��ĳһˮλ�������б�ǵ�����������ı�־ 
//		while(actives){//���е�����������ȫ
//			actives = false;
//			//���δ���ÿ����Ǻ�����Ӧ�������������ǺŶ�Ӧ������ĵ�ĸ�����SeedCounts����
//			for(i=0;i<Num;i++){//��һά�������������������
//				if(!vque[i][WaterLevel].empty()){//��Ӧ�ķ�ˮ�벻Ϊ�ռ�,i��ʾ����ţ�waterlevel��ʾ�ҽ�
//					actives = true;//
//					while(SeedCounts[i][WaterLevel]>0){//���ӻ���ʣ��������
//						SeedCounts[i][WaterLevel]--;//ȡ��һ���㣬���Ӹ�������һ
//						temp=vque[i][WaterLevel].front();//ȡ���������һ���㣬��������Ե�㣬��ʾ��ǰ�Ҷȼ��������Ѿ�������ˡ�
//						vque[i][WaterLevel].pop();//���
//						x = temp.x;//��ǰ���ӵ�����
//						y = temp.y;
//						z = temp.z;
//						if(x > 0){//��
//							if(!LabelImage->get(x-1,y,z)){//����δ������ʾû�б�ţ�Ӧ��������ǰ�Ѿ�������ʼ��Ϊ0
//								//���������ڿ�ͷҲ������ʼ��
//								temp.x = x-1;
//								temp.y = y;
//								temp.z = z;
//								LabelImage->put(x-1,y,z,(unsigned char)(i+1));//�󷽵���Ϊ����û����
//								//ע�⵽����������ɨ�����������ͬ��һ�����������������������ǵ�
//								//��������һ�����ٻ�ɨ�赽����㣬ȷ������©��������һ�ֵĴ����ʹ������
//								//���������⻹��������ǲ�û��һ���������뵽���Ӷ��С�Ҳ����˵��
//								//ֻ�Ǳ���û������������û��ֻ�����������������������С�
//								if(OriginalImage->get(x-1,y,z) <= WaterLevel){//����Ϊ������������뵱ǰ���У���ǰ�߶ȵĶ���
//									vque[i][WaterLevel].push(temp);	  
//								}
//								else{//�������OriginalImage[m-1][n]��Ӧ�ĻҶȼ��Ķ��У�Ϊʲô��
//									vque[i][OriginalImage->get(x-1,y,z)].push(temp);
//									SeedCounts[i][OriginalImage->get(x-1,y,z)]++;
//								}
//							}
//						}
//						if(x < (col-1)){//��
//							if(!LabelImage->get(x+1,y,z)){//�ҷ���δ����
//								temp.x = x+1;
//								temp.y = y;
//								temp.z = z;
//								LabelImage->put(x+1,y,z,(unsigned char)(i+1));//�ҷ�����Ϊ����û����
//								if(OriginalImage->get(x+1,y,z) <= WaterLevel){//�ҷ���Ϊ������������뵱ǰ����
//									vque[i][WaterLevel].push(temp);
//								}
//								else{//�������OriginalImage[m+1][n]������
//									vque[i][OriginalImage->get(x+1,y,z)].push(temp);
//									SeedCounts[i][OriginalImage->get(x+1,y,z)]++;
//								}
//							}
//						}
//						if(y > 0){//�Ϸ�
//							if(!LabelImage->get(x,y-1,z)){//�Ϸ���δ����
//								temp.x = x; 
//								temp.y = y-1;
//								temp.z = z;
//								LabelImage->put(x,y-1,z,(unsigned char)(i+1));//�Ϸ�����Ϊ����û����    
//								if(OriginalImage->get(x,y-1,z) <= WaterLevel){//�Ϸ���Ϊ������������뵱ǰ����
//									vque[i][WaterLevel].push(temp);
//								}
//								else{//�������OriginalImage[m][n+1]������
//									vque[i][OriginalImage->get(x,y-1,z)].push(temp);
//									SeedCounts[i][OriginalImage->get(x,y-1,z)]++;
//								}
//							}
//						}
//						if(y < (row-1)){//�·�
//							if(!LabelImage->get(x,y+1,z)){//�·���δ����
//								temp.x = x;
//								temp.y = y+1;
//								temp.z = z;
//								LabelImage->put(x,y+1,z,(unsigned char)(i+1));//�·�����Ϊ����û����
//								if(OriginalImage->get(x,y+1,z) <= WaterLevel){//�·���Ϊ������������뵱ǰ����
//									vque[i][WaterLevel].push(temp);
//								}
//								else{//�������OriginalImage[m][n-1]������
//									vque[i][OriginalImage->get(x,y+1,z)].push(temp);
//									SeedCounts[i][OriginalImage->get(x,y+1,z)]++;
//								}
//							}
//						}
//						if(z>0){//ǰ��
//							if(!LabelImage->get(x,y,z-1)){//ǰ����δ����
//								temp.x = x;
//								temp.y = y;
//								temp.z = z-1;
//								LabelImage->put(x,y,z-1,(unsigned char)(i+1));//ǰ������Ϊ����û����
//								if(OriginalImage->get(x,y,z-1) <= WaterLevel){//ǰ����Ϊ������������뵱ǰ����
//									vque[i][WaterLevel].push(temp);
//								}
//								else{//�������OriginalImage[m][n-1]������
//									vque[i][OriginalImage->get(x,y,z-1)].push(temp);
//									SeedCounts[i][OriginalImage->get(x,y,z-1)]++;
//								}
//							}
//						}
//						if(z < (height-1)){//��
//							if(!LabelImage->get(x,y,z+1)){//����δ����
//								temp.x = x;
//								temp.y = y;
//								temp.z = z+1;
//								LabelImage->put(x,y,z+1,(unsigned char)(i+1));//�󷽵���Ϊ����û����
//								if(OriginalImage->get(x,y,z+1) <= WaterLevel){//����Ϊ������������뵱ǰ����
//									vque[i][WaterLevel].push(temp);
//								}
//								else{//�������OriginalImage[m][n-1]������
//									vque[i][OriginalImage->get(x,y,z+1)].push(temp);
//									SeedCounts[i][OriginalImage->get(x,y,z+1)]++;
//								}
//							}
//						}
//					}//whileѭ������
//					SeedCounts[i][WaterLevel] = vque[i][WaterLevel].size();//�����Ӷ���ָ��ĸ������ݸ����Ӹ���
//				}//if����:�ڶ�Ӧ�ķ�ˮ�벻Ϊ�ռ��������
//			}//��һάforѭ����������������
//		}//whileѭ������
//	}//forѭ������
//
//	/**********************************/
//	//test whether the origional segmentation num is changed...
//	nonzeronum=Num;
//	for(m=0;m<Num;m++){
//		for(k=0;k<height;k++){	 
//			for(i=0;i<row;i++){
//				for(j=0;j<col;j++){
//					if(LabelImage->get(j,i,k) == m)	 break;
//				}
//				if(LabelImage->get(j,i,k) == m) break;
//			}
//			if(LabelImage->get(j,i,k) == m) break;
//		}
//		if(j==col-1 && i==row-1 && k==height-1 && LabelImage->get(j,i,k)!=m)  nonzeronum--;
//	}
//	cout<<"nonzeronum="<<nonzeronum<<endl;
//	//�ͷ�pque�ڴ�
//	while(!vque.empty()){
//		pque=vque.back();
//		delete[] pque;
//		vque.pop_back();
//	}
//	//�ͷ�array�ڴ�
//	while(!SeedCounts.empty()){
//		array=SeedCounts.back();
//		delete[] array;
//		SeedCounts.pop_back();
//	}
//}
//Raw * Gradientconst Raw *Image,Raw *dest);//��ά���ݶ� 