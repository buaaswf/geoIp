#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "vol_math_filter_interface.h"
#include <stdio.h>
#include <tchar.h>
#include "vol_math_Otsu.h"
#include "vol_math_RawImage.h"
#include "vol_math_Morph_Opera.h"
#include "vol_math_ImageProc.h"
#include "vol_math_Watersheds.h"
#include <fstream> 
using namespace std;

int main(int argc, char* argv[])
{
	//Otsu test

	////three Dimensions
	//int i,j,k,X_VALUE,Y_VALUE,Z_VALUE;
	//int size;
	//Points point;
	//FILE *f=NULL;
	//k=0;
	//X_VALUE=281;
	//Y_VALUE=481;
	//Z_VALUE=2501;
	//size=X_VALUE*Y_VALUE*Z_VALUE;
	//unsigned char *buf = new unsigned char [size];
	//ifstream in("img\\mig.raw", ios::in | ios::binary); 
	//if(!in) { 	
	//	cout << "Cannot open file.\n"; 
	//	return 1; 
	//} 
	//else {
	//	in.read((char *) buf, size*sizeof(unsigned char)); 
	//	cout<<"开始处理数据"<<endl;
	//    Raw raw(X_VALUE,Y_VALUE,Z_VALUE,buf,false);
	//	in.close();//关闭文件
	//	//Otsu test
	//	class OTSU test(raw);
	//	//k=test.Otsu(raw);
	//	//k=test.Otsu(raw,100,250);
	//	//cout<<"k="<<k<<endl;
	//	test.Otsu_MultiVal(raw,10);//Multi-threshold Otsu process one
	//	//test.Otsu_MultiVal(raw);//Multi-threshold Otsu process two
	//	k=(int)test.arrays.size();
	//	cout<<k<<endl;
	//	test.SaveImage();
	//	//Raw R(X_VALUE,Y_VALUE,Z_VALUE);
	//	//test.Output(raw,R);
	//	//test.SaveImage(R);
	//}
	//delete []buf;//释放内存
	//buf = NULL;

	////two Dimensions
	//int k,X_VALUE,Y_VALUE,size;
	//k=0;
	//X_VALUE=481;
	//Y_VALUE=281;
	//size=X_VALUE*Y_VALUE;  
	//unsigned char *buf=new unsigned char [size];
	//ifstream in("img\\mig.raw", ios::in | ios::binary); 
	//if(!in) { 	
	//	cout << "Cannot open file.\n"; 
	//	return 1; 
	//} 
	//else {//data process
	//	in.seekg(size*2300,ios::beg);
	//	in.read((char *) buf, size*sizeof(unsigned char)); 
	//	cout<<"开始处理数据"<<endl;
	//	class Raw2D raw(X_VALUE,Y_VALUE,buf);
	//	class OTSU test(raw);
	//	//k=test.Otsu(raw);
	//	//k=test.Otsu(raw,100,250);
	//	//cout<<"k="<<k<<endl;
	//	//test.Otsu_MultiVal(raw,10);//Multi-threshold Otsu process one
	//	test.Otsu_MultiVal(raw);//Multi-threshold Otsu process two
	//	test.SaveImage2D();
	//	//class Raw2D r(X_VALUE,Y_VALUE);
	//	//test.Output(raw,r);
	//	//test.SaveImage2D(r);
	//	in.close();//关闭文件
	//}
	//delete []buf;//释放内存
	//buf = NULL;

	//watersheds test
	//two  Dimensions
	int row,col,size;
	row=481;
	col=281;
	size=col*row;
	//Points point;
	unsigned char *buf;
	buf = new unsigned char [size];
	ifstream in("F:\\lab\\VTKproj\\mig.raw", ios::in | ios::binary); 
	if(!in) { 	
		cout << "Cannot open file.\n"; 
		return 1; 
	} 
	else {//data process
		in.seekg(size*2000,ios::beg);
		in.read((char *) buf, size*sizeof(unsigned char)); 
		cout<<"开始处理数据"<<endl;
		Raw2D raw(row,col,buf);//download data
		in.close();//关闭文件
		WatershedsProcess(raw);
	}
	delete []buf;//释放内存
	buf = NULL;
	//three  Dimensions
	//int i,j,k,l,x,y,z,row,col,height;
	//int number,size,val;
	//int dx1,dx2,dx3,dx4,dx5,dx6,dx;
	//int dy1,dy2,dy3,dy4,dy5,dy6,dy;
	//int dz1,dz2,dz3,dz4,dz5,dz6,dz;
	//col=281;
	//row=481;
	//height=15;
	//number=0;
	//	size=col*row*height;
	//bool change=false;
	//string strings="erode";
	//Points point;
	//unsigned char *buf;
	//buf = new unsigned char [size];
	//k=0;
	//ifstream in("img\\mig.raw", ios::in | ios::binary); 
	//if(!in) { 	
	//	cout << "Cannot open file.\n"; 
	//	return 1; 
	//} 
	//else {//data process
	//	in.seekg(row*col*2000,ios::beg);
	//	in.read((char *) buf, size*sizeof(unsigned char)); 
	//	cout<<"开始处理数据"<<endl;
	//    Raw raw(col,row,height,buf);//download data
	//	Raw raw1(col,row,height);
	//	Raw raw2(col,row,height);
	//	Raw raw3(col,row,height);
	//	in.close();//关闭文件
	//	//平滑图求梯度
	//	for(k=0;k<height;k++){
	//		for(i=0;i<row;i++){
	//			for(j=0;j<col;j++){ 
	//				if(j==0 || i==0 ||j==col-1 || i==row-1 ) raw1.put(j,i,k,0);
	//				else {	
	//					dx1=raw.get(j+1,i-1,k);
	//					dx2=raw.get(j+1,i,k);
	//					dx3=raw.get(j+1,i+1,k);
	//					dx4=raw.get(j-1,i-1,k);
	//					dx5=raw.get(j-1,i,k);
	//					dx6=raw.get(j-1,i+1,k);
	//					dx=dx1+2*dx2+dx3-dx4-2*dx5-dx6;
	//			
	//					dy1=raw.get(j-1,i+1,k);
	//					dy2=raw.get(j,i+1,k);			
	//					dy3=raw.get(j+1,i+1,k);
	//					dy4=raw.get(j-1,i-1,k);
	//					dy5=raw.get(j,i-1,k);				
	//					dy6=raw.get(j+1,i-1,k);
	//					dy=dy1+2*dy2+dy3-dy4-2*dy5-dy6;

	//					l=(int)(sqrt(pow((double)dx,2)+pow((double)dy,2))+0.5);//四舍五入	
	//					val=l>255?255:l;		
	//					raw1.put(j,i,k,(unsigned char)val);
	//				}
	//			}
	//		}
	//	}
	//	//erode reconstruction
	//	Dilate_Gray(raw,raw2,2);//image dilate
	//	Erode_Gray(raw2,raw3,2);//image Erode
	//	Morph_reconstuct_Gray(raw3,raw2,strings,2);
	//	//Get Max image
	//	MaxValue(raw2);
	//	//get seedimage and initialize it
	//	for(k=0;k<height;k++){
	//		for(i=0;i<row;i++){
	//			for(j=0;j<col;j++){ 
	//                 if(raw2.get(j,i,k)) {
	//				raw2.put(j,i,k,1);
	//				number++;
	//			}
	//			else  raw2.put(j,i,k,0);
	//			}
	//		}
	//	}
	//	cout<<"标记点的个数："<<number<<endl;
	//	//watersheds
	//	Watersheds(raw1,raw2,raw3);
	//	ofstream f("img\\Image.raw", ios::out | ios::binary); 
	//    f.write((char*)raw3.getdata(),raw3.size());
	//    f.close();//关闭文件
	//}
	//delete []buf;//释放内存
	//buf = NULL;
	return 0;
}
