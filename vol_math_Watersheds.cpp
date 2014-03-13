//watersheds.cpp
#include <iostream>
//#include <atlstr.h>
#include <vector>
#include <queue>
#include "vol_math_Watersheds.h"
#include "vol_math_Otsu.h"
#include "vol_math_ImageProc.h"
using namespace std;

void Zero(long long *p,long long number){
	int i=0;
	for(;i<number;i++){
		*(p+i)=0;
	}
}
void Zero(int *p,int number){
	int i=0;
	for(;i<number;i++){
		*(p+i)=0;
	}
}
void Zero_2D(int **p,int row,int col){//归零
	int i,j;
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			**(p+i*col+j)=0;
		}
	}
}
void Zero_3D(int ***p,int row,int col,int height){//归零
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
函数名：            Watershed
功能：              用标记-分水岭算法对输入图像进行分割
算法实现：          无
输入参数说明：      OriginalImage   --输入图像（灰度图，0~255）
SeedImage       --标记图像（二值图，0-非标记，1-标记）
LabelImage      --输出图像（1-第一个分割区域，2-第二个分割区域，...）
row             --图像行数
col             --图像列数
返回值说明：        无        
====================================================================*/
void Watersheds( Raw2D &OriginalImage, Raw2D &SeedImage, Raw2D &LabelImage,int* classnum){
	int Num=0;//标记区域标识号，从1开始
	vector<int*> SeedCounts;//保存每个队列种子个数的数组
	queue<POINT2D> quetem;//临时种子队列
	/**================================================================
	成员函数
	empty()判断队列空，当队列空时，返回true。
	size()访问队列中的元素个数。
	push()会将一个元素置入queue中。
	front()会返回queue内的下一个元素(也就是第一个被置入的元素)。
	back()会返回queue中最后一个元素(也就是最后被插入的元素)。
	pop()会从queue中移除一个元素。
	注意：pop()虽然会移除下一个元素，但是并不返回它，front()和back()返回下一个元素但并不移除该元素。
	================================================================**/
	vector<queue<POINT2D>*> vque;//保存所有标记区域种子队列的数组，里面放的是种子队列的指针
	queue<POINT2D> *pque;//指向种子队列的指针
	POINT2D temp;//种子点 
	bool actives;//在某一水位处，所有标记的种子生长完的标志
	int WaterLevel;//
	int* array;//
	int i,j,m,n,row,col,k=0;//定义一些变量
	bool up,down,right,left,upleft,upright,downleft,downright;//8 directions...
	int seednum;//种子的数目 
	int nonzeronum=0;//标记区域标识号，从最大开始减少直到为零
	col=OriginalImage.getYsize();
	row=OriginalImage.getXsize();
	//现将输出图像归零
	for(i=0;i<row;i++)
		for(j=0;j<col;j++)
			LabelImage.put(i,j,0);
	//预处理,提取区分每个标记区域，并初始化每个标记的种子队列
	//种子是指标记区域边缘的点，他们可以在水位上升时向外淹没（或者说生长）
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			if(SeedImage.get(i,j) == 1){//如果找到一个标记区域  
				Num++;//区域的标识号加一
				//分配数组并初始化为零，表示可有256个灰阶
				array=new int[256];
				Zero(array,256);//归零处理
				//种子个数数组进vector，每次扫描则生成一个数组，并用区域标识号来做第一维。灰度级做第二维。
				//表示某个盆地区域中某灰阶所对应的点的数目。
				SeedCounts.push_back(array);//push_back()函数表示将数据添加到vector的尾部，并按需要来分配内存。
				//表示对应一个灰阶有一个队列，并且每个队列可以存储一个集合的点信息
				pque=new queue<POINT2D>[256];
				//加入到队列数组中，对应的是本标记号Num的
				vque.push_back(pque);
				//当前点放入本标记区域的临时种子队列中
				temp.x = i;
				temp.y = j;
				quetem.push(temp);
				//当前点标记为已处理
				LabelImage.put(i,j,(PIXTYPE)Num);
				SeedImage.put(i,j,127);//表示已经处理过
				//让临时种子队列中的种子进行生长直到所有的种子都生长完毕
				//生长完毕后的队列信息保存在vque中，包括区域号和灰阶，对应点数存储在seedcounts中
				while(!quetem.empty()){ // 在临时种子队列非空时
					up=down=right=left=false;    
					upleft=upright=downleft=downright=false;
					temp = quetem.front();//从队列中取出一个种子    
					m = temp.x;
					n = temp.y;
					quetem.pop();//从queue中移除一个元素。
					//注意到127对扫描过程的影响，影响下面的比较，但是不影响while语句中的扫描 
					if(m > 0){
						if(SeedImage.get(m-1,n) == 1){ //上方若为可生长点则加为新种子
							temp.x = m-1;
							temp.y = n;
							quetem.push(temp);//如果这样的话，那么这些标记过的区域将再次在while循环中被扫描到，不会，因为值是127
							//新种子点标记为已淹没区域，而且是当前区域，并记录区域号到labelImage
							LabelImage.put(m-1,n,(PIXTYPE) Num);
							SeedImage.put(m-1,n,127);
						}
						else{//否则上方为不可生长
							up = true; 			 
						}
					}
					if(m > 0  &&  n > 0){
						if(SeedImage.get(m-1,n-1)==1){//左上方若为可生长点则加为新种子	 
							temp.x=m-1;
							temp.y=n-1;
							quetem.push(temp);
							//新种子点标记为已淹没区域，即下一个循环中以127来标识不再扫描，而且是当前区域
							LabelImage.put(m-1,n-1,(PIXTYPE) Num);
							SeedImage.put(m-1,n-1,127);
						}
						else{//否则左上方为不可生长
							upleft = true;
						}
					}
					if(m > 0  &&  n < (col-1)){			 
						if(SeedImage.get(m-1,n+1)==1){//右上方若为可生长点则加为新种子					
							temp.x = m-1;						
							temp.y = n+1;						
							quetem.push(temp);						 
							//新种子点标记为已淹没区域，而且是当前区域						
							LabelImage.put(m-1,n+1,(PIXTYPE) Num);					
							SeedImage.put(m-1,n+1,127);				
						}		
						else{//否则右上方为不可生长			 
							upright = true;				 
						}			 
					}		 
					if(m < row-1){
						if(SeedImage.get(m+1,n)==1){//下方若为可生长点则加为新种子 			 
							temp.x = m+1;			 
							temp.y = n; 
							quetem.push(temp);				 
							//新种子点标记为已淹没区域，而且是当前区域			 
							LabelImage.put(m+1,n,(PIXTYPE) Num); 
							SeedImage.put(m+1,n,127);				 
						}			 
						else{//否则下方为不可生长					 
							down = true;
						}				 
					}				 
					if(m < (row-1)  &&  n < (col-1)){ 					 
						if(SeedImage.get(m+1,n+1)==1){//右下方若为可生长点则加为新种子						 
							temp.x = m+1;						 
							temp.y = n+1;  						 
							quetem.push(temp);						 
							//新种子点标记为已淹没区域，而且是当前区域						
							LabelImage.put(m+1,n+1,(PIXTYPE) Num);				
							SeedImage.put(m+1,n+1,127);			 
						}			 
						else{//否则右下方为不可生长						 
							downright = true;					 
						}				
					}
					if(m < (row-1)  &&  n > 0){
						if(SeedImage.get(m+1,n-1)==1){//左下方若为可生长点则加为新种子
							temp.x = m+1;
							temp.y = n-1;
							quetem.push(temp);
							//新种子点标记为已淹没区域
							LabelImage.put(m+1,n-1,(PIXTYPE) Num);
							SeedImage.put(m+1,n-1,127);
						}  
						else{//否则左下方为不可生长
							downleft = true;
						}
					}
					if(n > 0){			
						if(SeedImage.get(m,n-1) == 1){//左方若为可生长点则加为新种子					 
							temp.x = m;					
							temp.y = n-1;				 
							quetem.push(temp);					
							//新种子点标记为已淹没区域						
							LabelImage.put(m,n-1,(PIXTYPE) Num);				
							SeedImage.put(m,n-1,127);		 
						}
						else{//否则左方为不可生长
							left = true;
						}
					}
					if(n < col-1){					 
						if(SeedImage.get(m,n+1)==1){//右方若为可生长点则加为新种子									 
							temp.x = m;						
							temp.y = n+1;						
							quetem.push(temp);						
							//新种子点标记为已淹没区域，而且是当前区域						 
							LabelImage.put(m,n+1,(PIXTYPE) Num);						 
							SeedImage.put(m,n+1,127);					
						}					
						else{//否则右方为不可生长						 
							right = true;					 
						}				
					}				 
					//上下左右只要有一点不可生长，那么本点为初始种子队列中的一个
					//这里可否生长是由seedimage中的值来决定的。
					if(up||down||right||left|| upleft||downleft||upright||downright){	
						temp.x = m; 
						temp.y = n;
						//下面这个矢量数组：第一维是标记号；第二维是该图像点的灰度级
						//m,n点对应的是while循环里面扫描的像素点。
						//Num是当前的区域号
						//这样这个二维信息就表示了，某个区域中对应某个灰度级对应的成员点的集合与个数
						//分别由下面两个量来表达
						(vque[Num-1][(int)(OriginalImage.get(m,n))]).push(temp);
						SeedCounts[Num-1][(int)(OriginalImage.get(m,n))]++;
					}    
				}//while结束，扫描到quetem为空而止。也就是对应所有的节点都得到不可生长为止（或者是周围的点要么不可生长，要么已生长）       
			}//if结束
			// if(Num==5)
			// return;       
		} 
	} 

	//在上述过程中，如果标记的点为0则表示，没有扫描到的点，或者表明不是输入的种子点
	//这里相当于是找seedimage传过来的初始区域的分水岭界线的所有的点；并且用标号记录每个区域，同时集水盆的边缘点进入队列。
	//上面是找集水盆的程序。同时也是连通区域。
	/*************************************/
	//test 这里测试一下剩下的非水盆地的点数。
	seednum=0;
	//for(i=0;i<row;i++){
	//	for(j=0;j<col;j++){
	//		if(SeedImage.get(i,j) == 0)
	//			seednum++;
	//	}
	//}
	//cout<<"非水盆地的点数:"<<seednum<<endl;

	//淹没过程开始，水位从零开始上升，水位对应灰度级，采用四连通法
	for(WaterLevel=0;WaterLevel<256;WaterLevel++){//第二维。。。
		actives = true;//在某一水位处，所有标记的种子生长完的标志 
		while(actives){//所有的区域生长完全
			actives = false;
			//依次处理每个标记号所对应的区域，且这个标记号对应的区域的点的个数在SeedCounts里面
			for(i=0;i<Num;i++){//第一维。。。分区域进行生长
				if(!vque[i][WaterLevel].empty()){//对应的分水岭不为空集,i表示区域号，waterlevel表示灰阶
					actives = true;//
					while(SeedCounts[i][WaterLevel]>0){//种子还有剩余的情况下
						SeedCounts[i][WaterLevel]--;//取出一个点，种子个数减少一
						temp=vque[i][WaterLevel].front();//取出该区域的一个点，清空这个边缘点，表示当前灰度级该像素已经处理掉了。
						vque[i][WaterLevel].pop();//清空
						m = temp.x;//当前种子的坐标
						n = temp.y;
						if(m > 0){//上方
							if(!LabelImage.get(m-1,n)){//上方若未处理，表示没有标号，应该在输入前已经作过初始化为0
								//本函数中在开头也作过初始化
								temp.x = m-1;
								temp.y = n;
								LabelImage.put(m-1,n,(PIXTYPE)(i+1));//上方点标记为已淹没区域
								//注意到这个标记是与扫描点的区域号相同，一定在这个标号所属的区域吗？是的
								//这样在下一轮至少会扫描到这个点，确保不遗漏，但是下一轮的处理会使它合理
								//归类吗？问题还有这样标记并没有一定将它加入到种子队列。也就是说它
								//只是被淹没而不能向上淹没。只有满足下述可生长条件才行。
								if(OriginalImage.get(m-1,n) <= WaterLevel){//上方若为可生长点则加入当前队列，当前高度的队列
									vque[i][WaterLevel].push(temp);	  
								}
								else{//否则加入OriginalImage[m-1][n]对应的灰度级的队列，为什么？
									vque[i][(int)(OriginalImage.get(m-1,n))].push(temp);
									SeedCounts[i][(int)(OriginalImage.get(m-1,n))]++;
								}
							}
						}
						if(m < row-1){//下方
							if(!LabelImage.get(m+1,n)){//下方若未处理
								temp.x = m+1;
								temp.y = n;
								LabelImage.put(m+1,n,(PIXTYPE)(i+1));//下方点标记为已淹没区域
								if(OriginalImage.get(m+1,n) <= WaterLevel){//下方若为可生长点则加入当前队列
									vque[i][WaterLevel].push(temp);
								}
								else{//否则加入OriginalImage[m+1][n]级队列
									vque[i][(int)(OriginalImage.get(m+1,n))].push(temp);
									SeedCounts[i][(int)(OriginalImage.get(m+1,n))]++;
								}
							}
						}
						if(n < col-1){//右方
							if(!LabelImage.get(m,n+1)){//右方若未处理
								temp.x = m; 
								temp.y = n+1;      
								LabelImage.put(m,n+1,(PIXTYPE)(i+1));//右方点标记为已淹没区域    
								if(OriginalImage.get(m,n+1) <= WaterLevel){//右方若为可生长点则加入当前队列
									vque[i][WaterLevel].push(temp);
								}
								else{//否则加入OriginalImage[m][n+1]级队列
									vque[i][(int)(OriginalImage.get(m,n+1))].push(temp);
									SeedCounts[i][(int)(OriginalImage.get(m,n+1))]++;
								}
							}
						}
						if(n > 0){//左方
							if(!LabelImage.get(m,n-1)){//左方若未处理
								temp.x = m;
								temp.y = n-1;
								LabelImage.put(m,n-1,(PIXTYPE)(i+1));//左方点标记为已淹没区域
								if(OriginalImage.get(m,n-1) <= WaterLevel){//左方若为可生长点则加入当前队列
									vque[i][WaterLevel].push(temp);
								}
								else{//否则加入OriginalImage[m][n-1]级队列
									vque[i][(int)(OriginalImage.get(m,n-1))].push(temp);
									SeedCounts[i][(int)(OriginalImage.get(m,n-1))]++;
								}
							}
						}
					}//while循环结束
					SeedCounts[i][WaterLevel] = vque[i][WaterLevel].size();//将种子队列指针的个数传递给种子个数
				}//if结束:在对应的分水岭不为空集的情况下
			}//第一维for循环结束：遍历区域
		}//while循环结束
	}//for循环结束

	/**********************************/
	//test whether the origional segmentation num is changed...
	//nonzeronum = Num;
	//for(m=0;m<Num;m++){
	//	for(i=0;i<row;i++){
	//		for(j=0;j<col;j++){
	//			if(LabelImage.get(i,j) == m)	
	//				break;
	//		}
	//		if(LabelImage.get(i,j) == m)
	//			break;
	//	}
	//	if(j == col-1  &&  i == row-1  &&  LabelImage.get(i,j) != m)
	//		nonzeronum--;
	//}
	//cout<<"nonzeronum="<<nonzeronum<<endl;
	*classnum =Num;
	//释放pque内存
	while(!vque.empty()){
		pque=vque.back();
		delete[] pque;
		vque.pop_back();
	}
	//释放array内存
	while(!SeedCounts.empty()){
		array=SeedCounts.back();
		delete[] array;
		SeedCounts.pop_back();
	}
}
//void Watersheds(const Raw2D *OriginalImage, Raw2D *LabelImage);
void Gradient( Raw2D &Image,Raw2D &dest){//二维求梯度 
	int col=Image.getYsize();
	int row=Image.getXsize();
	int i,j;
	PIXTYPE dx1,dx2,dx3,dx4,dx5,dx6,dx,dy1,dy2,dy3,dy4,dy5,dy6,dy,val;
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){ 
			if(i==0 || j==0 || i==row-1 || j==col-1) dest.put(i,j,0.0);
			else{
				dx1=Image.get(i-1,j+1);
				dx2=2*Image.get(i,j+1);
				dx3=Image.get(i+1,j+1);
				dx4=Image.get(i-1,j-1);
				dx5=2*Image.get(i,j-1);
				dx6=Image.get(i+1,j-1);
				dx=dx1+dx2+dx3-dx4-dx5-dx6;	

				dy1=Image.get(i+1,j-1);
				dy2=2*Image.get(i+1,j);
				dy3=Image.get(i+1,j+1);
				dy4=Image.get(i-1,j-1);
				dy5=2*Image.get(i-1,j);
				dy6=Image.get(i-1,j+1);

				dy=dy1+dy2+dy3-dy4-dy5-dy6;
				val=sqrt(dx*dx+dy*dy);
				dest.put(i,j,val);
			}	
		}//for循环结束
	}//for循环结束
}

void WatershedsProcess(Raw2D &OriginalImage){
	int i,j,row,col,number,size;
	row=OriginalImage.getXsize();
	col=OriginalImage.getYsize();
	PIXTYPE value,Minval,Maxval;
	Minval = 255.0;
	Maxval = 0.0;
	size=col*row;
	int *classnum = new int[1];
	string strings="erode";
	Raw2D raw1(row,col);	
	Raw2D raw2(row,col);	
	//平滑图求梯度
	Gradient(OriginalImage,raw1);
	//get M value
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			value = raw1.get(i,j);
			Minval = Minval > value ? value : Minval;
			Maxval = Maxval < value ? value : Maxval;
		}
	}
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			value = 255*(raw1.get(i,j)-Minval)/(Maxval-Minval);//将梯度值规划到0-255
			raw1.put(i,j,value);
		}
	}
	//erode reconstruction
	Dilate_Gray(OriginalImage,raw2,2);//image dilate
	Erode_Gray(raw2,OriginalImage,2);//image Erode
	Morph_reconstuct_Gray(OriginalImage,raw2,strings,2);
	////Get Max image
	MaxValue(raw2);
	//get seedimage and initialize it
	number=0;
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			if(raw2.get(i,j) == 0) raw2.put(i,j,0); 
			else {
				raw2.put(i,j,1);
				number++;
			}	
		}
	}
	cout<<"标记点的个数:"<<number<<endl;
	////watersheds
	Watersheds(raw1,raw2,OriginalImage,classnum);
	number=*classnum;
	for (int i=0;i<row*col;i++){
		OriginalImage.putXY(i,(OriginalImage.getXY(i)*255/number));
	}
	delete classnum;
	classnum = NULL;
}
//void WatershedIterface(Raw2D *src,Raw2D *ret)
//{
//	//delete ret;
//	memcpy(ret->getdata(),src->getdata(),src->getXsize()*src->getYsize());
//	WatershedsProcess(*ret);
//}
void Watersheds( Raw2D *OriginalImage, Raw2D *SeedImage, Raw2D *LabelImage){
	int Num=0;//标记区域标识号，从1开始
	vector<int*> SeedCounts;//保存每个队列种子个数的数组
	queue<POINT2D> quetem;//临时种子队列
	/**================================================================
	成员函数
	empty()判断队列空，当队列空时，返回true。
	size()访问队列中的元素个数。
	push()会将一个元素置入queue中。
	front()会返回queue内的下一个元素(也就是第一个被置入的元素)。
	back()会返回queue中最后一个元素(也就是最后被插入的元素)。
	pop()会从queue中移除一个元素。
	注意：pop()虽然会移除下一个元素，但是并不返回它，front()和back()返回下一个元素但并不移除该元素。
	================================================================**/
	vector<queue<POINT2D>*> vque;//保存所有标记区域种子队列的数组，里面放的是种子队列的指针
	queue<POINT2D> *pque;//指向种子队列的指针
	POINT2D temp;//种子点 
	bool actives;//在某一水位处，所有标记的种子生长完的标志
	int WaterLevel;//
	int* array;//
	int i,j,m,n,row,col,k = 0;//定义一些变量
	bool up,down,right,left,upleft,upright,downleft,downright;//8 directions...
	int seednum;//种子的数目 
	int nonzeronum=0;//标记区域标识号，从最大开始减少直到为零
	col=OriginalImage->getYsize();
	row=OriginalImage->getXsize();
	//现将输出图像归零
	for(i=0;i<row;i++)
		for(j=0;j<col;j++)
			LabelImage->put(i,j,0);
	//预处理,提取区分每个标记区域，并初始化每个标记的种子队列
	//种子是指标记区域边缘的点，他们可以在水位上升时向外淹没（或者说生长）
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			if(SeedImage->get(i,j) == 1){//如果找到一个标记区域  
				Num++;//区域的标识号加一
				//分配数组并初始化为零，表示可有256个灰阶
				array=new int[256];
				Zero(array,256);//归零处理
				//种子个数数组进vector，每次扫描则生成一个数组，并用区域标识号来做第一维。灰度级做第二维。
				//表示某个盆地区域中某灰阶所对应的点的数目。
				SeedCounts.push_back(array);//push_back()函数表示将数据添加到vector的尾部，并按需要来分配内存。
				//表示对应一个灰阶有一个队列，并且每个队列可以存储一个集合的点信息
				pque = new queue<POINT2D>[256];
				//加入到队列数组中，对应的是本标记号Num的
				vque.push_back(pque);
				//当前点放入本标记区域的临时种子队列中
				temp.x = i;
				temp.y = j;
				quetem.push(temp);
				//当前点标记为已处理
				LabelImage->put(i,j,(PIXTYPE)Num);
				SeedImage->put(i,j,127);//表示已经处理过
				//让临时种子队列中的种子进行生长直到所有的种子都生长完毕
				//生长完毕后的队列信息保存在vque中，包括区域号和灰阶，对应点数存储在seedcounts中
				while(!quetem.empty()){ // 在临时种子队列非空时
					up=down=right=left=false;    
					upleft=upright=downleft=downright=false;
					temp = quetem.front();//从队列中取出一个种子    
					m = temp.x;
					n = temp.y;
					quetem.pop();//从queue中移除一个元素。
					//注意到127对扫描过程的影响，影响下面的比较，但是不影响while语句中的扫描 
					if(m > 0){
						if(SeedImage->get(m-1,n) == 1){ //上方若为可生长点则加为新种子
							temp.x = m-1;
							temp.y = n;
							quetem.push(temp);//如果这样的话，那么这些标记过的区域将再次在while循环中被扫描到，不会，因为值是127
							//新种子点标记为已淹没区域，而且是当前区域，并记录区域号到labelImage
							LabelImage->put(m-1,n,(PIXTYPE) Num);
							SeedImage->put(m-1,n,127);
						}
						else{//否则上方为不可生长
							up=true; 			 
						}
					}
					if(m > 0  &&  n > 0){
						if(SeedImage->get(m-1,n-1) == 1){//左上方若为可生长点则加为新种子	 
							temp.x = m-1;
							temp.y = n-1;
							quetem.push(temp);
							//新种子点标记为已淹没区域，即下一个循环中以127来标识不再扫描，而且是当前区域
							LabelImage->put(m-1,n-1,(PIXTYPE) Num);
							SeedImage->put(m-1,n-1,127);
						}
						else{//否则左上方为不可生长
							upleft = true;
						}
					}
					if(m > 0  &&  n < (col-1)){			 
						if(SeedImage->get(m-1,n+1) == 1){//右上方若为可生长点则加为新种子					
							temp.x = m-1;						
							temp.y = n+1;						
							quetem.push(temp);						 
							//新种子点标记为已淹没区域，而且是当前区域						
							LabelImage->put(m-1,n+1,(PIXTYPE) Num);					
							SeedImage->put(m-1,n+1,127);				
						}		
						else{//否则右上方为不可生长			 
							upright = true;				 
						}			 
					}		 
					if(m < (row-1)){
						if(SeedImage->get(m+1,n) == 1){//下方若为可生长点则加为新种子 			 
							temp.x = m+1;			 
							temp.y = n; 
							quetem.push(temp);				 
							//新种子点标记为已淹没区域，而且是当前区域			 
							LabelImage->put(m+1,n,(PIXTYPE) Num); 
							SeedImage->put(m+1,n,127);				 
						}			 
						else{//否则下方为不可生长					 
							down = true;
						}				 
					}				 
					if(m < (row-1)  &&  n < (col-1)){ 					 
						if(SeedImage->get(m+1,n+1) == 1){//右下方若为可生长点则加为新种子						 
							temp.x = m+1;						 
							temp.y = n+1;  						 
							quetem.push(temp);						 
							//新种子点标记为已淹没区域，而且是当前区域						
							LabelImage->put(m+1,n+1,(PIXTYPE) Num);				
							SeedImage->put(m+1,n+1,127);			 
						}			 
						else{//否则右下方为不可生长						 
							downright = true;					 
						}				
					}
					if(m < (row-1)  &&  n > 0){
						if(SeedImage->get(m+1,n-1) == 1){//左下方若为可生长点则加为新种子
							temp.x = m+1;
							temp.y = n-1;
							quetem.push(temp);
							//新种子点标记为已淹没区域
							LabelImage->put(m+1,n-1,(PIXTYPE) Num);
							SeedImage->put(m+1,n-1,127);
						}  
						else{//否则左下方为不可生长
							downleft = true;
						}
					}
					if(n > 0){			
						if(SeedImage->get(m,n-1) == 1){//左方若为可生长点则加为新种子					 
							temp.x = m;					
							temp.y = n-1;				 
							quetem.push(temp);					
							//新种子点标记为已淹没区域						
							LabelImage->put(m,n-1,(PIXTYPE) Num);				
							SeedImage->put(m,n-1,127);		 
						}
						else{//否则左方为不可生长
							left = true;
						}
					}
					if(n < (col-1)){					 
						if(SeedImage->get(m,n+1) == 1){//右方若为可生长点则加为新种子									 
							temp.x = m;						
							temp.y = n+1;						
							quetem.push(temp);						
							//新种子点标记为已淹没区域，而且是当前区域						 
							LabelImage->put(m,n+1,(PIXTYPE) Num);						 
							SeedImage->put(m,n+1,127);					
						}					
						else{//否则右方为不可生长						 
							right = true;					 
						}				
					}				 
					//上下左右只要有一点不可生长，那么本点为初始种子队列中的一个
					//这里可否生长是由seedimage中的值来决定的。
					if(up||down||right||left|| upleft||downleft||upright||downright){	
						temp.x=m; 
						temp.y=n;
						//下面这个矢量数组：第一维是标记号；第二维是该图像点的灰度级
						//m,n点对应的是while循环里面扫描的像素点。
						//Num是当前的区域号
						//这样这个二维信息就表示了，某个区域中对应某个灰度级对应的成员点的集合与个数
						//分别由下面两个量来表达
						vque[Num-1][(int)OriginalImage->get(m,n)].push(temp);
						SeedCounts[Num-1][(int)OriginalImage->get(m,n)]++;
					}    
				}//while结束，扫描到quetem为空而止。也就是对应所有的节点都得到不可生长为止（或者是周围的点要么不可生长，要么已生长）       
			}//if结束
			// if(Num==5)
			// return;       
		} 
	} 

	//在上述过程中，如果标记的点为0则表示，没有扫描到的点，或者表明不是输入的种子点
	//这里相当于是找seedimage传过来的初始区域的分水岭界线的所有的点；并且用标号记录每个区域，同时集水盆的边缘点进入队列。
	//上面是找集水盆的程序。同时也是连通区域。
	/*************************************/
	//test 这里测试一下剩下的非水盆地的点数。
	seednum=0;
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			if(SeedImage->get(i,j) == 0)
				seednum++;
		}
	}
	cout<<"非水盆地的点数:"<<seednum<<endl;

	//淹没过程开始，水位从零开始上升，水位对应灰度级，采用四连通法
	for(WaterLevel=0;WaterLevel<256;WaterLevel++){//第二维。。。
		actives = true;//在某一水位处，所有标记的种子生长完的标志 
		while(actives){//所有的区域生长完全
			actives = false;
			//依次处理每个标记号所对应的区域，且这个标记号对应的区域的点的个数在SeedCounts里面
			for(i=0;i<Num;i++){//第一维。。。分区域进行生长
				if(!vque[i][WaterLevel].empty()){//对应的分水岭不为空集,i表示区域号，waterlevel表示灰阶
					actives = true;//
					while(SeedCounts[i][WaterLevel]>0){//种子还有剩余的情况下
						SeedCounts[i][WaterLevel]--;//取出一个点，种子个数减少一
						temp=vque[i][WaterLevel].front();//取出该区域的一个点，清空这个边缘点，表示当前灰度级该像素已经处理掉了。
						vque[i][WaterLevel].pop();//清空
						m = temp.x;//当前种子的坐标
						n = temp.y;
						if(m > 0){//上方
							if(!LabelImage->get(m-1,n)){//左方若未处理，表示没有标号，应该在输入前已经作过初始化为0
								//本函数中在开头也作过初始化
								temp.x = m-1;
								temp.y = n;
								LabelImage->put(m-1,n,(PIXTYPE)(i+1));//上方点标记为已淹没区域
								//注意到这个标记是与扫描点的区域号相同，一定在这个标号所属的区域吗？是的
								//这样在下一轮至少会扫描到这个点，确保不遗漏，但是下一轮的处理会使它合理
								//归类吗？问题还有这样标记并没有一定将它加入到种子队列。也就是说它
								//只是被淹没而不能向上淹没。只有满足下述可生长条件才行。
								if(OriginalImage->get(m-1,n) <= WaterLevel){//上方若为可生长点则加入当前队列，当前高度的队列
									vque[i][WaterLevel].push(temp);	  
								}
								else{//否则加入OriginalImage[m-1][n]对应的灰度级的队列，为什么？
									vque[i][(int)OriginalImage->get(m-1,n)].push(temp);
									SeedCounts[i][(int)OriginalImage->get(m-1,n)]++;
								}
							}
						}
						if(m < (row-1)){//下方
							if(!LabelImage->get(m+1,n)){//下方若未处理
								temp.x = m+1;
								temp.y = n;
								LabelImage->put(m+1,n,(PIXTYPE)(i+1));//下方点标记为已淹没区域
								if(OriginalImage->get(m+1,n) <= WaterLevel){//下方若为可生长点则加入当前队列
									vque[i][WaterLevel].push(temp);
								}
								else{//否则加入OriginalImage[m+1][n]级队列
									vque[i][(int)OriginalImage->get(m+1,n)].push(temp);
									SeedCounts[i][(int)OriginalImage->get(m+1,n)]++;
								}
							}
						}
						if(n < (col-1)){//右方
							if(!LabelImage->get(m,n+1)){//右方若未处理
								temp.x = m; 
								temp.y = n+1;      
								LabelImage->put(m,n+1,(PIXTYPE)(i+1));//右方点标记为已淹没区域    
								if(OriginalImage->get(m,n+1) <= WaterLevel){//右方若为可生长点则加入当前队列
									vque[i][WaterLevel].push(temp);
								}
								else{//否则加入OriginalImage[m][n+1]级队列
									vque[i][(int)OriginalImage->get(m,n+1)].push(temp);
									SeedCounts[i][(int)OriginalImage->get(m,n+1)]++;
								}
							}
						}
						if(n > 0){//左方
							if(!LabelImage->get(m,n-1)){//左方若未处理
								temp.x = m;
								temp.y = n-1;
								LabelImage->put(m,n-1,(PIXTYPE)(i+1));//左方点标记为已淹没区域
								if(OriginalImage->get(m,n-1) <= WaterLevel){//左方若为可生长点则加入当前队列
									vque[i][WaterLevel].push(temp);
								}
								else{//否则加入OriginalImage[m][n-1]级队列
									vque[i][(int)OriginalImage->get(m,n-1)].push(temp);
									SeedCounts[i][(int)OriginalImage->get(m,n-1)]++;
								}
							}
						}
					}//while循环结束
					SeedCounts[i][WaterLevel] = vque[i][WaterLevel].size();//将种子队列指针的个数传递给种子个数
				}//if结束:在对应的分水岭不为空集的情况下
			}//第一维for循环结束：遍历区域
		}//while循环结束
	}//for循环结束

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

	//释放pque内存
	while(!vque.empty()){
		pque=vque.back();
		delete[] pque;
		vque.pop_back();
	}
	//释放array内存
	while(!SeedCounts.empty()){
		array=SeedCounts.back();
		delete[] array;
		SeedCounts.pop_back();
	}
}
void Gradient( Raw2D *Image,Raw2D *dest){//二维求梯度 
	int col = Image->getYsize();
	int row = Image->getXsize();
	int i,j;
	PIXTYPE dx1,dx2,dx3,dx4,dx5,dx6,dx,dy1,dy2,dy3,dy4,dy5,dy6,dy;
	for(i=1;i<row-1;i++){
		for(j=1;j<col-1;j++){
			dx1 = Image->get(i-1,j+1);
			dx2 = 2*Image->get(i,j+1);
			dx3 = Image->get(i+1,j+1);
			dx4 = Image->get(i-1,j-1);
			dx5 = 2*Image->get(i,j-1);
			dx6 = Image->get(i+1,j-1);
			dx = dx1+dx2+dx3-dx4-dx5-dx6;


			dy1 = Image->get(i+1,j-1);
			dy2 = 2*Image->get(i+1,j);
			dy3 = Image->get(i+1,j+1);
			dy4 = Image->get(i-1,j-1);
			dy5 = 2*Image->get(i-1,j);
			dy6 = Image->get(i-1,j+1);
			dy = dy1+dy2+dy3-dy4-dy5-dy6;

			dest->put(i,j,sqrt(dx*dx+dy*dy));
		}//for循环结束
	}//for循环结束
}

//three  Dimensions
/*====================================================================
函数名：            Watershed
功能：              用标记-分水岭算法对输入图像进行分割
算法实现：          无
输入参数说明：      OriginalImage   --输入图像（灰度图，0~255）
SeedImage       --标记图像（二值图，0-非标记，1-标记）
LabelImage      --输出图像（1-第一个分割区域，2-第二个分割区域，...）
row             --图像行数
col             --图像列数
返回值说明：        无        
====================================================================*/

void Watersheds( Raw &OriginalImage, Raw &SeedImage, Raw &LabelImage,int *classnum,void (*progresschanged)(int,int,int,bool &)){
	int Num=0;//标记区域标识号，从1开始
	vector<long long *> SeedCounts;//保存每个队列种子个数的数组
	queue<POINT3D> quetem;//临时种子队列
	/**================================================================
	成员函数
	empty()判断队列空，当队列空时，返回true。
	size()访问队列中的元素个数。
	push()会将一个元素置入queue中。
	front()会返回queue内的下一个元素(也就是第一个被置入的元素)。
	back()会返回queue中最后一个元素(也就是最后被插入的元素)。
	pop()会从queue中移除一个元素。
	注意：pop()虽然会移除下一个元素，但是并不返回它，front()和back()返回下一个元素但并不移除该元素。
	================================================================**/
	vector<queue<POINT3D>*> vque;//保存所有标记区域种子队列的数组，里面放的是种子队列的指针
	queue<POINT3D> *pque;//指向种子队列的指针
	POINT3D temp;//种子点 
	bool actives;//在某一水位处，所有标记的种子生长完的标志
	int WaterLevel;//
	long long* array;//
	int i,j,k,m,n,x,y,z;
	long long row,col,height;//定义一些变量
	bool z_up,z_down,_up,up,up_,_down,down,down_,_right,right,right_,_left,left,left_,_upleft,upleft,upleft_,
		_upright,upright, upright_,_downleft,downleft,downleft_,_downright,downright,downright_;//27 directions...
	long long seednum;//种子的数目 
	int nonzeronum = 0;//标记区域标识号，从最大开始减少直到为零
	col = OriginalImage.getXsize();
	row = OriginalImage.getYsize();
	height = OriginalImage.getZsize();
	//现将输出图像归零
	for(k=0;k<height;k++)
		for(i=0;i<row;i++)
			for(j=0;j<col;j++)
				LabelImage.put(j,i,k,0);
	//预处理,提取区分每个标记区域，并初始化每个标记的种子队列
	//种子是指标记区域边缘的点，他们可以在水位上升时向外淹没（或者说生长）
	for(k=0;k<height;k++){
		for(i=0;i<row;i++){
			for(j=0;j<col;j++){
				if(SeedImage.get(j,i,k) == 1){//如果找到一个标记区域  
					Num++;//区域的标识号加一
					//分配数组并初始化为零，表示可有256个灰阶
					array = new long long[256];
					Zero(array,256);//归零处理
					//种子个数数组进vector，每次扫描则生成一个数组，并用区域标识号来做第一维。灰度级做第二维。
					//表示某个盆地区域中某灰阶所对应的点的数目。
					SeedCounts.push_back(array);//push_back()函数表示将数据添加到vector的尾部，并按需要来分配内存。
					//表示对应一个灰阶有一个队列，并且每个队列可以存储一个集合的点信息
					pque = new queue<POINT3D>[256];
					//加入到队列数组中，对应的是本标记号Num的
					vque.push_back(pque);
					//当前点放入本标记区域的临时种子队列中
					temp.x = j;
					temp.y = i;
					temp.z = k;
					quetem.push(temp);
					//当前点标记为已处理
					LabelImage.put(j,i,k,(PIXTYPE)Num);
					SeedImage.put(j,i,k,127);//表示已经处理过
					//让临时种子队列中的种子进行生长直到所有的种子都生长完毕
					//生长完毕后的队列信息保存在vque中，包括区域号和灰阶，对应点数存储在seedcounts中
					while(!quetem.empty()){ // 在临时种子队列非空时
						z_up=z_down=false;
						_up=_down=_right=_left=false;  
						up=down=right=left=false; 
						up_=down_=right_=left_=false; 
						_upleft=_upright=_downleft=_downright=false;
						upleft=upright=downleft=downright=false;
						upleft_=upright_=downleft_=downright_=false;
						temp=quetem.front();//从队列中取出一个种子
						x = temp.x;
						y = temp.y;
						z = temp.z;
						quetem.pop();//从queue中移除一个元素。
						//注意到127对扫描过程的影响，影响下面的比较，但是不影响while语句中的扫描 
						if(x > 0){
							if(SeedImage.get(x-1,y,z) == 1){ //同层左方若为可生长点则加为新种子
								temp.x = x-1;
								temp.y = y;
								temp.z = z;
								quetem.push(temp);//如果这样的话，那么这些标记过的区域将再次在while循环中被扫描到，不会，因为值是127
								//新种子点标记为已淹没区域，而且是当前区域，并记录区域号到labelImage
								LabelImage.put(x-1,y,z,(PIXTYPE) Num);
								SeedImage.put(x-1,y,z,127);
							}
							else{//否则同层左方为不可生长
								left = true; 			 
							}
						}
						if(x > 0  &&  y > 0){
							if(SeedImage.get(x-1,y-1,z) == 1){//同层左上方若为可生长点则加为新种子	 
								temp.x = x-1;
								temp.y = y-1;
								temp.z = z;
								quetem.push(temp);
								//新种子点标记为已淹没区域，即下一个循环中以127来标识不再扫描，而且是当前区域
								LabelImage.put(x-1,y-1,z,(PIXTYPE) Num);
								SeedImage.put(x-1,y-1,z,127);
							}
							else{//否则同层左上方为不可生长
								upleft=true;
							}
						}
						if(x > 0  &&  y > 0  &&  z > 0){
							if(SeedImage.get(x-1,y-1,z-1) == 1){ //下层左上方若为可生长点则加为新种子
								temp.x = x-1;
								temp.y = y-1;
								temp.z = z-1;
								quetem.push(temp);//如果这样的话，那么这些标记过的区域将再次在while循环中被扫描到，不会，因为值是127
								//新种子点标记为已淹没区域，而且是当前区域，并记录区域号到labelImage
								LabelImage.put(x-1,y-1,z-1,(PIXTYPE) Num);
								SeedImage.put(x-1,y-1,z-1,127);
							}
							else{//否则下层左上方为不可生长
								_upleft = true; 			 
							}
						}
						if(x > 0  &&  y > 0  &&  z < (height-1)){
							if(SeedImage.get(x-1,y-1,z+1) == 1){//上层左上方若为可生长点则加为新种子	 
								temp.x = x-1;
								temp.y = y-1;
								temp.z = z+1;
								quetem.push(temp);
								//新种子点标记为已淹没区域，即下一个循环中以127来标识不再扫描，而且是当前区域
								LabelImage.put(x-1,y-1,z+1,(PIXTYPE) Num);
								SeedImage.put(x-1,y-1,z+1,127);
							}
							else{//否则上层左上方为不可生长
								upleft_ = true;
							}
						}
						if(x > 0  &&  y < (row-1)){			 
							if(SeedImage.get(x-1,y+1,z) == 1){//同层左下方若为可生长点则加为新种子					
								temp.x = x-1;						
								temp.y = y+1;						
								temp.z = z;
								quetem.push(temp);						 
								//新种子点标记为已淹没区域，而且是当前区域						
								LabelImage.put(x-1,y+1,z,(PIXTYPE) Num);					
								SeedImage.put(x-1,y+1,z,127);				
							}		
							else{//否则同层左下方为不可生长			 
								downleft = true;				 
							}			 
						}	
						if(x > 0  &&  y < (row-1)  &&  z > 0){			 
							if(SeedImage.get(x-1,y+1,z-1) == 1){//下层左下方若为可生长点则加为新种子					
								temp.x = x-1;						
								temp.y = y+1;						
								temp.z = z-1;
								quetem.push(temp);						 
								//新种子点标记为已淹没区域，而且是当前区域						
								LabelImage.put(x-1,y+1,z-1,(PIXTYPE) Num);					
								SeedImage.put(x-1,y+1,z-1,127);				
							}		
							else{//否则下层左下方为不可生长			 
								_downleft = true;				 
							}			 
						}	
						if(x > 0  &&  y < (row-1)  &&  z < (height-1)){			 
							if(SeedImage.get(x-1,y+1,z+1) == 1){//上层左下方若为可生长点则加为新种子					
								temp.x = x-1;						
								temp.y = y+1;						
								temp.z = z+1;
								quetem.push(temp);						 
								//新种子点标记为已淹没区域，而且是当前区域						
								LabelImage.put(x-1,y+1,z+1,(PIXTYPE) Num);					
								SeedImage.put(x-1,y+1,z+1,127);				
							}		
							else{//否则上层左下方为不可生长			 
								downleft_ = true;				 
							}			 
						}
						if(x > 0  &&  z > 0){			 
							if(SeedImage.get(x-1,y,z-1) == 1){//下层左方若为可生长点则加为新种子					
								temp.x = x-1;						
								temp.y = y;						
								temp.z = z-1;
								quetem.push(temp);						 
								//新种子点标记为已淹没区域，而且是当前区域						
								LabelImage.put(x-1,y,z-1,(PIXTYPE) Num);					
								SeedImage.put(x-1,y,z-1,127);				
							}		
							else{//否则下层左方为不可生长			 
								_left = true;				 
							}			 
						}
						if(x > 0  &&  z < (height-1)){			 
							if(SeedImage.get(x-1,y,z+1) == 1){//上层左方若为可生长点则加为新种子					
								temp.x = x-1;						
								temp.y = y;						
								temp.z = z+1;
								quetem.push(temp);						 
								//新种子点标记为已淹没区域，而且是当前区域						
								LabelImage.put(x-1,y,z+1,(PIXTYPE) Num);					
								SeedImage.put(x-1,y,z+1,127);				
							}		
							else{//否则上层左方为不可生长			 
								left_ = true;				 
							}			 
						}
						if(x < (col-1)){
							if(SeedImage.get(x+1,y,z) == 1){ //同层右方若为可生长点则加为新种子
								temp.x = x+1;
								temp.y = y;
								temp.z = z;
								quetem.push(temp);//如果这样的话，那么这些标记过的区域将再次在while循环中被扫描到，不会，因为值是127
								//新种子点标记为已淹没区域，而且是当前区域，并记录区域号到labelImage
								LabelImage.put(x+1,y,z,(PIXTYPE) Num);
								SeedImage.put(x+1,y,z,127);
							}
							else{//否则同层右方为不可生长
								right = true; 			 
							}
						}
						if(x < (col-1)  &&  y > 0){
							if(SeedImage.get(x+1,y-1,z) == 1){//同层右上方若为可生长点则加为新种子	 
								temp.x = x+1;
								temp.y = y-1;
								temp.z = z;
								quetem.push(temp);
								//新种子点标记为已淹没区域，即下一个循环中以127来标识不再扫描，而且是当前区域
								LabelImage.put(x+1,y-1,z,(PIXTYPE) Num);
								SeedImage.put(x+1,y-1,z,127);
							}
							else{//否则同层右上方为不可生长
								upright = true;
							}
						}
						if(x <(col-1)  &&  y > 0  &&  z > 0){
							if(SeedImage.get(x+1,y-1,z-1) == 1){ //下层右上方若为可生长点则加为新种子
								temp.x = x+1;
								temp.y = y-1;
								temp.z = z-1;
								quetem.push(temp);//如果这样的话，那么这些标记过的区域将再次在while循环中被扫描到，不会，因为值是127
								//新种子点标记为已淹没区域，而且是当前区域，并记录区域号到labelImage
								LabelImage.put(x+1,y-1,z-1,(PIXTYPE) Num);
								SeedImage.put(x+1,y-1,z-1,127);
							}
							else{//否则下层右上方为不可生长
								_upright = true; 			 
							}
						}
						if(x < (col-1)  &&  y > 0  &&  z < (height-1)){
							if(SeedImage.get(x+1,y-1,z+1) == 1){//上层右上方若为可生长点则加为新种子	 
								temp.x = x+1;
								temp.y = y-1;
								temp.z = z+1;
								quetem.push(temp);
								//新种子点标记为已淹没区域，即下一个循环中以127来标识不再扫描，而且是当前区域
								LabelImage.put(x+1,y-1,z+1,(PIXTYPE) Num);
								SeedImage.put(x+1,y-1,z+1,127);
							}
							else{//否则上层右上方为不可生长
								upright_ = true;
							}
						}
						if(x < (col-1)   &&  y < (row-1)){			 
							if(SeedImage.get(x+1,y+1,z) == 1){//同层右下方若为可生长点则加为新种子					
								temp.x = x+1;						
								temp.y = y+1;						
								temp.z = z;
								quetem.push(temp);						 
								//新种子点标记为已淹没区域，而且是当前区域						
								LabelImage.put(x+1,y+1,z,(PIXTYPE) Num);					
								SeedImage.put(x+1,y+1,z,127);				
							}		
							else{//否则同层右下方为不可生长			 
								downright = true;				 
							}			 
						}	
						if(x < (col-1)  &&  y < (row-1)  &&  z > 0){			 
							if(SeedImage.get(x+1,y+1,z-1) == 1){//下层右下方若为可生长点则加为新种子					
								temp.x = x+1;						
								temp.y = y+1;						
								temp.z = z-1;
								quetem.push(temp);						 
								//新种子点标记为已淹没区域，而且是当前区域						
								LabelImage.put(x+1,y+1,z-1,(PIXTYPE) Num);					
								SeedImage.put(x+1,y+1,z-1,127);				
							}		
							else{//否则下层右下方为不可生长			 
								_downright = true;				 
							}			 
						}	
						if(x < (col-1)  &&  y < (row-1)  &&  z < (height-1)){			 
							if(SeedImage.get(x+1,y+1,z+1) == 1){//上层右下方若为可生长点则加为新种子					
								temp.x = x+1;						
								temp.y = y+1;						
								temp.z = z+1;
								quetem.push(temp);						 
								//新种子点标记为已淹没区域，而且是当前区域						
								LabelImage.put(x+1,y+1,z+1,(PIXTYPE) Num);					
								SeedImage.put(x+1,y+1,z+1,127);				
							}		
							else{//否则上层右下方为不可生长			 
								downright_ = true;				 
							}			 
						}
						if(x < (col-1)  &&  z > 0){			 
							if(SeedImage.get(x+1,y,z-1) == 1){//下层右方若为可生长点则加为新种子					
								temp.x = x+1;						
								temp.y = y;						
								temp.z = z-1;
								quetem.push(temp);						 
								//新种子点标记为已淹没区域，而且是当前区域						
								LabelImage.put(x+1,y,z-1,(PIXTYPE) Num);					
								SeedImage.put(x+1,y,z-1,127);				
							}		
							else{//否则下层右方为不可生长			 
								_right = true;				 
							}			 
						}
						if(x < (col-1)  &&  z < (height-1)){			 
							if(SeedImage.get(x+1,y,z+1) == 1){//上层右方若为可生长点则加为新种子					
								temp.x = x+1;						
								temp.y = y;						
								temp.z = z+1;
								quetem.push(temp);						 
								//新种子点标记为已淹没区域，而且是当前区域						
								LabelImage.put(x+1,y,z+1,(PIXTYPE) Num);					
								SeedImage.put(x+1,y,z+1,127);				
							}		
							else{//否则上层右方为不可生长			 
								right_ = true;				 
							}			 
						}
						if(y > 0){
							if(SeedImage.get(x,y-1,z) == 1){ //同层上方若为可生长点则加为新种子
								temp.x = x;
								temp.y = y-1;
								temp.z = z;
								quetem.push(temp);//如果这样的话，那么这些标记过的区域将再次在while循环中被扫描到，不会，因为值是127
								//新种子点标记为已淹没区域，而且是当前区域，并记录区域号到labelImage
								LabelImage.put(x,y-1,z,(PIXTYPE) Num);
								SeedImage.put(x,y-1,z,127);
							}
							else{//否则同层上方为不可生长
								up = true; 			 
							}
						}
						if(y > 0  &&  z > 0 ){
							if(SeedImage.get(x,y-1,z-1) == 1){//下层上方若为可生长点则加为新种子	 
								temp.x = x;
								temp.y = y-1;
								temp.z = z-1;
								quetem.push(temp);
								//新种子点标记为已淹没区域，即下一个循环中以127来标识不再扫描，而且是当前区域
								LabelImage.put(x,y-1,z-1,(PIXTYPE) Num);
								SeedImage.put(x,y-1,z-1,127);
							}
							else{//否则下层上方为不可生长
								_up = true;
							}
						}
						if(y > 0  &&  z < (height-1)){
							if(SeedImage.get(x,y-1,z+1) == 1){ //上层上方若为可生长点则加为新种子
								temp.x = x;
								temp.y = y-1;
								temp.z = z+1;
								quetem.push(temp);//如果这样的话，那么这些标记过的区域将再次在while循环中被扫描到，不会，因为值是127
								//新种子点标记为已淹没区域，而且是当前区域，并记录区域号到labelImage
								LabelImage.put(x,y-1,z+1,(PIXTYPE) Num);
								SeedImage.put(x,y-1,z+1,127);
							}
							else{//否则上层上方为不可生长
								up_ = true; 			 
							}
						}
						if( y < (row-1)){
							if(SeedImage.get(x,y+1,z) == 1){//同层下方若为可生长点则加为新种子	 
								temp.x = x;
								temp.y = y+1;
								temp.z = z;
								quetem.push(temp);
								//新种子点标记为已淹没区域，即下一个循环中以127来标识不再扫描，而且是当前区域
								LabelImage.put(x,y+1,z,(PIXTYPE) Num);
								SeedImage.put(x,y+1,z,127);
							}
							else{//否则同层下方为不可生长
								down = true;
							}
						}
						if(y < (row-1)  &&  z > 0){			 
							if(SeedImage.get(x,y+1,z-1) == 1){//下层下方若为可生长点则加为新种子					
								temp.x = x;						
								temp.y = y+1;						
								temp.z = z-1;
								quetem.push(temp);						 
								//新种子点标记为已淹没区域，而且是当前区域						
								LabelImage.put(x,y+1,z-1,(PIXTYPE) Num);					
								SeedImage.put(x,y+1,z-1,127);				
							}		
							else{//否则下层下方为不可生长			 
								_down = true;				 
							}			 
						}	
						if(y < (row-1)  &&  z < (height-1)){			 
							if(SeedImage.get(x,y+1,z+1) == 1){//上层下方若为可生长点则加为新种子					
								temp.x = x;						
								temp.y = y+1;						
								temp.z = z+1;
								quetem.push(temp);						 
								//新种子点标记为已淹没区域，而且是当前区域						
								LabelImage.put(x,y+1,z+1,(PIXTYPE) Num);					
								SeedImage.put(x,y+1,z+1,127);				
							}		
							else{//否则上层下方为不可生长			 
								down_ = true;				 
							}			 
						}	
						if(z > 0){			 
							if(SeedImage.get(x,y,z-1) == 1){//下层对应点若为可生长点则加为新种子					
								temp.x = x;						
								temp.y = y;						
								temp.z = z-1;
								quetem.push(temp);						 
								//新种子点标记为已淹没区域，而且是当前区域						
								LabelImage.put(x,y,z-1,(PIXTYPE) Num);					
								SeedImage.put(x,y,z-1,127);				
							}		
							else{//否则下层对应点为不可生长			 
								z_down = true;				 
							}			 
						}
						if(z < (height-1)){			 
							if(SeedImage.get(x,y,z+1) == 1){//上层对应点若为可生长点则加为新种子					
								temp.x = x;						
								temp.y = y;						
								temp.z = z+1;
								quetem.push(temp);						 
								//新种子点标记为已淹没区域，而且是当前区域						
								LabelImage.put(x,y,z+1,(PIXTYPE) Num);					
								SeedImage.put(x,y,z+1,127);				
							}		
							else{//否则上层对应点为不可生长			 
								z_up = true;				 
							}			 
						}
						//上下左右只要有一点不可生长，那么本点为初始种子队列中的一个

						//这里可否生长是由seedimage中的值来决定的。

						if( _up||_down||_right||_left||_upleft||_downleft||_upright||_downright||
							up || down|| right|| left|| upleft|| downleft|| upright|| downright||
							up_||down_||right_||left_||upleft_||downleft_||upright_||downright_||z_up || z_down){	
								temp.x = x; 
								temp.y = y;
								temp.z = z;
								//下面这个矢量数组：第一维是标记号；第二维是该图像点的灰度级
								//x,y,z点对应的是while循环里面扫描的像素点。
								//Num是当前的区域号
								//这样这个二维信息就表示了，某个区域中对应某个灰度级对应的成员点的集合与个数
								//分别由下面两个量来表达
								vque[Num-1][(int)OriginalImage.get(x,y,z)].push(temp);
								SeedCounts[Num-1][(int)OriginalImage.get(x,y,z)]++;

						}    
					}//while结束，扫描到quetem为空而止。也就是对应所有的节点都得到不可生长为止（或者是周围的点要么不可生长，要么已生长）       
				}//if结束
				// if(Num==5)  return;       
			}//for 
		}//for 
	}//for
	//在上述过程中，如果标记的点为0则表示，没有扫描到的点，或者表明不是输入的种子点
	//这里相当于是找seedimage传过来的初始区域的分水岭界线的所有的点；并且用标号记录每个区域，同时集水盆的边缘点进入队列。
	//上面是找集水盆的程序。同时也是连通区域。
	/*************************************/
	//test 这里测试一下剩下的非水盆地的点数。
	seednum=0;
	//for(k=0;k<height;k++){
	//	for(i=0;i<row;i++){
	//		for(j=0;j<col;j++){
	//			if(SeedImage.get(j,i,k) == 0)  seednum++;
	//		}
	//	}
	//}
	//cout<<"非水盆地的点数:"<<seednum<<endl;
	//淹没过程开始，水位从零开始上升，水位对应灰度级，采用六连通法
	bool flag=false;
	for(WaterLevel=0;WaterLevel<256;WaterLevel++){//第二维。。。
		//========================================
		if (  progresschanged != NULL )
		{
			progresschanged (0,1,WaterLevel*100/255,flag);
		}
		actives = true;//在某一水位处，所有标记的种子生长完的标志 
		while(actives){//所有的区域生长完全
			actives = false;
			//依次处理每个标记号所对应的区域，且这个标记号对应的区域的点的个数在SeedCounts里面
			for(i=0;i<Num;i++){//第一维。。。分区域进行生长
				if(!vque[i][WaterLevel].empty()){//对应的分水岭不为空集,i表示区域号，waterlevel表示灰阶
					actives = true;//
					while(SeedCounts[i][WaterLevel]>0){//种子还有剩余的情况下
						SeedCounts[i][WaterLevel]--;//取出一个点，种子个数减少一
						temp=vque[i][WaterLevel].front();//取出该区域的一个点，清空这个边缘点，表示当前灰度级该像素已经处理掉了。
						vque[i][WaterLevel].pop();//清空
						x = temp.x;//当前种子的坐标
						y = temp.y;
						z = temp.z;
						if(x > 0){//左方
							if(!LabelImage.get(x-1,y,z)){//左方若未处理，表示没有标号，应该在输入前已经作过初始化为0
								//本函数中在开头也作过初始化
								temp.x = x-1;
								temp.y = y;
								temp.z = z;
								LabelImage.put(x-1,y,z,(PIXTYPE)(i+1));//左方点标记为已淹没区域
								//注意到这个标记是与扫描点的区域号相同，一定在这个标号所属的区域吗？是的
								//这样在下一轮至少会扫描到这个点，确保不遗漏，但是下一轮的处理会使它合理
								//归类吗？问题还有这样标记并没有一定将它加入到种子队列。也就是说它
								//只是被淹没而不能向上淹没。只有满足下述可生长条件才行。
								if(OriginalImage.get(x-1,y,z) <= WaterLevel){//左方若为可生长点则加入当前队列，当前高度的队列
									vque[i][WaterLevel].push(temp);	  
								}
								else{//否则加入OriginalImage[m-1][n]对应的灰度级的队列，为什么？
									vque[i][(int)OriginalImage.get(x-1,y,z)].push(temp);
									SeedCounts[i][(int)OriginalImage.get(x-1,y,z)]++;
								}
							}
						}
						if(x < (col-1)){//右
							if(!LabelImage.get(x+1,y,z)){//右方若未处理
								temp.x = x+1;
								temp.y = y;
								temp.z = z;
								LabelImage.put(x+1,y,z,(PIXTYPE)(i+1));//右方点标记为已淹没区域
								if(OriginalImage.get(x+1,y,z) <= WaterLevel){//右方若为可生长点则加入当前队列
									vque[i][WaterLevel].push(temp);
								}
								else{//否则加入OriginalImage[m+1][n]级队列
									vque[i][(int)OriginalImage.get(x+1,y,z)].push(temp);
									SeedCounts[i][(int)OriginalImage.get(x+1,y,z)]++;
								}
							}
						}
						if(y > 0){//上方
							if(!LabelImage.get(x,y-1,z)){//上方若未处理
								temp.x = x; 
								temp.y = y-1;
								temp.z = z;
								LabelImage.put(x,y-1,z,(PIXTYPE)(i+1));//上方点标记为已淹没区域    
								if(OriginalImage.get(x,y-1,z) <= WaterLevel){//上方若为可生长点则加入当前队列
									vque[i][WaterLevel].push(temp);
								}
								else{//否则加入OriginalImage[m][n+1]级队列
									vque[i][(int)OriginalImage.get(x,y-1,z)].push(temp);
									SeedCounts[i][(int)OriginalImage.get(x,y-1,z)]++;
								}
							}
						}
						if(y < (row-1)){//下方
							if(!LabelImage.get(x,y+1,z)){//下方若未处理
								temp.x = x;
								temp.y = y+1;
								temp.z = z;
								LabelImage.put(x,y+1,z,(PIXTYPE)(i+1));//下方点标记为已淹没区域
								if(OriginalImage.get(x,y+1,z) <= WaterLevel){//下方若为可生长点则加入当前队列
									vque[i][WaterLevel].push(temp);
								}
								else{//否则加入OriginalImage[m][n-1]级队列
									vque[i][(int)OriginalImage.get(x,y+1,z)].push(temp);
									SeedCounts[i][(int)OriginalImage.get(x,y+1,z)]++;
								}
							}
						}
						if(z>0){//前方
							if(!LabelImage.get(x,y,z-1)){//前方若未处理
								temp.x = x;
								temp.y = y;
								temp.z = z-1;
								LabelImage.put(x,y,z-1,(PIXTYPE)(i+1));//前方点标记为已淹没区域
								if(OriginalImage.get(x,y,z-1) <= WaterLevel){//前方若为可生长点则加入当前队列
									vque[i][WaterLevel].push(temp);
								}
								else{//否则加入OriginalImage[m][n-1]级队列
									vque[i][(int)OriginalImage.get(x,y,z-1)].push(temp);
									SeedCounts[i][(int)OriginalImage.get(x,y,z-1)]++;
								}
							}
						}
						if(z < (height-1)){//后方
							if(!LabelImage.get(x,y,z+1)){//后方若未处理
								temp.x = x;
								temp.y = y;
								temp.z = z+1;
								LabelImage.put(x,y,z+1,(PIXTYPE)(i+1));//后方点标记为已淹没区域
								if(OriginalImage.get(x,y,z+1) <= WaterLevel){//后方若为可生长点则加入当前队列
									vque[i][WaterLevel].push(temp);
								}
								else{//否则加入OriginalImage[m][n-1]级队列
									vque[i][(int)OriginalImage.get(x,y,z+1)].push(temp);
									SeedCounts[i][(int)OriginalImage.get(x,y,z+1)]++;
								}
							}
						}
					}//while循环结束
					SeedCounts[i][WaterLevel] = vque[i][WaterLevel].size();//将种子队列指针的个数传递给种子个数
				}//if结束:在对应的分水岭不为空集的情况下
			}//第一维for循环结束：遍历区域
		}//while循环结束
	}//for循环结束

	/**********************************/
	//test whether the origional segmentation num is changed...
	nonzeronum=Num;
	for(m=0;m<Num;m++){
		for(k=0;k<height;k++){	 
			n=0;
			for(i=0;i<row;i++){
				for(j=0;j<col;j++){
					if(LabelImage.get(j,i,k) == m){
						n++;
						break;
					}	
				}
				if(n) break;
			}
			if(n) break;
		}
		if(j==col-1 && i==row-1 && k==height-1 && LabelImage.get(j,i,k)!=m)  nonzeronum--;
	}
	cout<<"nonzeronum="<<nonzeronum<<endl;
	*classnum=Num;
	//释放pque内存
	while(!vque.empty()){
		pque=vque.back();
		delete[] pque;
		vque.pop_back();
	}
	//释放array内存
	while(!SeedCounts.empty()){
		array=SeedCounts.back();
		delete[] array;
		SeedCounts.pop_back();
	}
}
void Gradient( Raw &src,Raw &dest){	
	long long i,j,k,row,col,height;
	row = src.getYsize();
	col = src.getXsize();
	height = src.getZsize();
	PIXTYPE val;
	PIXTYPE dx1,dx2,dx3,dx4,dx5,dx6,dx;
	PIXTYPE dy1,dy2,dy3,dy4,dy5,dy6,dy;
	for(k=0;k<height;k++){
		for(i=0;i<row;i++){
			for(j=0;j<col;j++){ 
				if(j==0 || i==0 ||j==col-1 || i==row-1 ) dest.put(j,i,k,0);
				else {	
					dx1=src.get(j+1,i-1,k);
					dx2=src.get(j+1,i,k);
					dx3=src.get(j+1,i+1,k);
					dx4=src.get(j-1,i-1,k);
					dx5=src.get(j-1,i,k);
					dx6=src.get(j-1,i+1,k);
					dx=dx1+2*dx2+dx3-dx4-2*dx5-dx6;
				
					dy1=src.get(j-1,i+1,k);
					dy2=src.get(j,i+1,k);			
					dy3=src.get(j+1,i+1,k);
					dy4=src.get(j-1,i-1,k);
					dy5=src.get(j,i-1,k);				
					dy6=src.get(j+1,i-1,k);
					dy=dy1+2*dy2+dy3-dy4-2*dy5-dy6;
					val=sqrt(dx*dx+dy*dy);
					dest.put(j,i,k,val);
				}
			}
		}
	}
}
void WatershedsProcess(Raw &OriginalImage,void (*progresschanged)(int,int,int,bool &)){
	if(OriginalImage.getdata() == NULL) return ;
	int number=0;
	PIXTYPE val,Minval = 255.0,Maxval = 0.0;
	long long i,j,k,size,m,row,col,height;
	col=OriginalImage.getXsize();
	row=OriginalImage.getYsize();
	height=OriginalImage.getZsize();
    size=col*row*height;
	int *classnumber=new int[1];
	string strings="erode";
	Raw raw1(col,row,height);
	Raw raw2(col,row,height);
	//平滑图求梯度
	Gradient(OriginalImage,raw1);	
	for(i=0;i<size;i++){
			val = raw1.getXYZ(i);
			Minval = Minval > val ? val : Minval;
			Maxval = Maxval < val ? val : Maxval;
	}
	//cout<<"Minval="<<(int)Minval<<"    Maxval="<<(int)Maxval<<endl;
	for(i=0;i<size;i++){
			val = 255*(raw1.getXYZ(i)-Minval)/(Maxval-Minval);//将梯度值规划到0-255
			raw1.putXYZ(i,val);	
	}
	//erode reconstruction
	Dilate_Gray(OriginalImage,raw2,2);//image dilate
	Erode_Gray(raw2,OriginalImage,2);//image Erode
	Morph_reconstuct_Gray(OriginalImage,raw2,strings,2);
	//Get Max image
	MaxValue(raw2);
	//get seedimage and initialize it
	for(k=0;k<height;k++){
		for(i=0;i<row;i++){
			for(j=0;j<col;j++){ 
                if(raw2.get(j,i,k)) {
				raw2.put(j,i,k,1);
				number++;
			}
			else  raw2.put(j,i,k,0);
			}
		}
	}
	cout<<"标记点的个数："<<number<<endl;
	//watersheds
	Watersheds(raw1,raw2,OriginalImage,classnumber,progresschanged);
	number=*classnumber;
	cout<<number<<endl;
	for (m=0;m<size;m++){
		OriginalImage.putXYZ(m,(OriginalImage.getXYZ(m)*255/number));
	}
	delete classnumber;
	classnumber=NULL;
}

