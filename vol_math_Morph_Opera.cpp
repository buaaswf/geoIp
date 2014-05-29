//Morph_Opera.cpp
#include "vol_math_Morph_Opera.h"
#include <math.h>

void CReleaseMat(Mat &mat){
	 delete mat.data; 
	 mat.data=NULL;
}
//Mat compare
int CompareMat(const Mat &mat1,const Mat &mat2){
    if ((mat1.row != mat2.row) || (mat1.col != mat2.col))  return 1;
    int i,j,k,row,col;
	row=mat1.row;
	col=mat1.col;
	k=0;
	for (i=0;i<row;i++){
		for (j=0;j<col;j++){
			k=*(mat1.data+i*col+j)-(*(mat2.data+i*col+j));
			if (k) break;
		}
		if (k) break;
	}
    return k;
}
//Mat copy
Mat Copy(Mat &mat){
	Mat ma(mat.col,mat.row);
	int i,j,row,col;
	row=mat.row;
	col=mat.col;
	ma.data=new int[row*col];//memory
	for (i=0;i<row;i++)
		for (j=0;j<col;j++)	
			*(ma.data+i*col+j)=*(mat.data+i*col+j);
	return ma;	
}
Mat strel(string strings,int n){
	Mat mat;
	int i,j,m;
	float k;
	m=2*n+1;
	mat.row=m;
	mat.col=m;
	mat.data=new int[m*m];//初始分配内存
	if ("disc"==strings){//圆形
		for (i=0;i<m;i++)
		{
			for (j=0;j<m;j++)
			{
				k=powf((float)abs(i-n),2)+powf((float)abs(j-n),2);
				if (k>n*n)	*(mat.data+i*m+j)=0;
				else *(mat.data+i*m+j)=1;	
			}
		}
	}
	else if("octagon"==strings){//八角形
		for (i=0;i<m;i++)
		{
			for (j=0;j<m;j++)
			{
				k=4*(powf((float)abs(i-n),2)+powf((float)abs(j-n),2));
				if (k>5*powf((float)n,2)-2*n+1)	*(mat.data+i*m+j)=0;
				else *(mat.data+i*m+j)=1;	
			}
		}
	}
	else if("diamond"==strings){//钻石形
		for (i=0;i<m;i++)
		{
			for (j=0;j<m;j++)
			{
				k=(float)abs(i-n)+(float)abs(j-n);
				if (k>n) *(mat.data+i*m+j)=0;
				else *(mat.data+i*m+j)=1;	
			}
		}
	}
	else {
       cout<<"strings is error! "<<endl;
	   exit(0);
	}
    return mat;
}
Mat strel(int row,int col,int value){
	Mat mat;
	int i,j;
	mat.row=row;
	mat.col=col;
	mat.data=new int[row*col];//初始分配内存
	for (i=0;i<row;i++)
		for (j=0;j<col;j++)	
			*(mat.data+i*col+j)=value;	
	return mat;
}
Mat strel_B(string strings,int n){
	Mat mat;
	int i,j,m;
	//float k;
	m=2*n+1;
	mat.row=m;
	mat.col=m;
	mat.data=new int[m*m];//初始分配内存
	if ("disc"==strings){//圆形
		for (i=0;i<m;i++)
		{
			for (j=0;j<m;j++)
			{
				*(mat.data+i*m+j)=2*sqrt(n*n-powf((float)(i-n),2)-powf((float)(j-n),2))+0.5;
			}
		}
	}
	else if("octagon"==strings){//八角形
		for (i=0;i<m;i++)
		{
			for (j=0;j<m;j++)
			{
				*(mat.data+i*m+j)=2*sqrt(n*n-powf((float)(i-n),2)-powf((float)(j-n),2))+0.5;
			}
		}
	}
	else if("diamond"==strings){//钻石形
		for (i=0;i<m;i++)
		{
			for (j=0;j<m;j++)
			{
				*(mat.data+i*m+j)=m-abs(i-n)-abs(j-n);
			}
		}
	}
	else {
       cout<<"strings is error! "<<endl;
	   exit(0);
	}
    return mat;
}

//two Dimensions
int CompareImage( Raw2D &src1, Raw2D &src2){
	if (src1.getXsize()!=src2.getXsize() || src1.getYsize()!=src2.getYsize())	return 1;
	int i,j,col,row;
	PIXTYPE value;
	row=src1.getXsize();
	col=src1.getYsize();
	value=0;
	for (i=0;i<row;i++){
		for (j=0;j<col;j++){
			value=src1.get(i,j) - src2.get(i,j);
			if (value!=0) break;
		}
		if (value!=0) break;
	}
	if (value!=0) return 1;
	else return 0;
}
//Gray image copy
void Copy( Raw2D &src,Raw2D &dest){
	if(src.getXsize()!=dest.getXsize() || src.getYsize()!=dest.getYsize()) {
		printf("Size no match!");
		return ;
	}
	int i,j,col,row;
	row=src.getXsize();
	col=src.getYsize();
	for (i=0;i<row;i++){
		for (j=0;j<col;j++){
			dest.put(i,j,src.get(i,j));
		}
	}
}
//Gray image reversion
void Reversion(Raw2D &source){
	int i,j,row,col;
	PIXTYPE val;
	row=source.getXsize();
	col=source.getYsize();
	for (i=0;i<row;i++){
		for (j=0;j<col;j++){
			val=(PIXTYPE)(255 - source.get(i,j));
			source.put(i,j,val);
		}
	}
}
//erode
void Erode_Gray( Raw2D &mask,Raw2D &dest,int n){//gray image
	Mat temp;
        temp=strel_B("disc",n);
	int i,j,k,l,row,col,x,y;
	PIXTYPE Minval,value;
	col=mask.getYsize();
	row=mask.getXsize();
	//marker eroded by temp
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			Minval=255.0;
			for(k=-n;k<=n;k++){
				x=i+k;
				x=x<0?0:x;
				x=x>(row-1)?(row-1):x;
				for(l=-n;l<=n;l++){
					y=j+l;
					y=y<0?0:y;
					y=y>(col-1)?(col-1):y;
					value=mask.get(x,y);//-(*(temp.data+(k+n)*temp.col+(l+n)));	
					if (Minval > value) Minval = value;
				}	
			}
			dest.put(i,j,Minval);
		}
	}
	CReleaseMat(temp);
}
//dilate
void Dilate_Gray( Raw2D &mask,Raw2D &dest,int n){//gray image
	Mat temp;
        temp=strel("disc",n);
	int i,j,k,l,row,col,x,y;
	PIXTYPE Maxval,value;
	row=mask.getXsize();
	col=mask.getYsize();
	//marker eroded by temp
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			Maxval=0.0;
			for(k=-n;k<=n;k++){
				x=i+k;
				x=x<0?0:x;
				x=x>(row-1)?(row-1):x;
				for(l=-n;l<=n;l++){
					y=j+l;
					y=y<0?0:y;
					y=y>(col-1)?(col-1):y;
					value=mask.get(x,y);//+(*(temp.data+(k+n)*temp.col+(l+n)));
					if (Maxval < value) Maxval = value;
				}	
			}
			dest.put(i,j,Maxval);
		}
	}
	CReleaseMat(temp);
}
//Morphological erode
void Morph_Erode_Gray( Raw2D &mask,Raw2D &dest,int n){//gray image
	Mat temp;
        temp =strel_B("disc",n);
	int i,j,k,l,row,col,x,y;
	PIXTYPE Minval,Maxval,value;
	col=mask.getYsize();
	row=mask.getXsize();
	//marker eroded by temp
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			Minval=255;
			for(k=-n;k<=n;k++){
				x=i+k;
				x=x<0?0:x;
				x=x>(row-1)?(row-1):x;
				for(l=-n;l<=n;l++){
					y=j+l;
					y=y<0?0:y;
					y=y>(col-1)?(col-1):y;
					value=mask.get(x,y);//-(*(temp.data+(k+n)*temp.col+(l+n)));
					if (Minval > value) Minval = value;
				}	
			}
			dest.put(i,j,Minval);
		}
	}
	//Geodedesic erosion
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			Maxval=dest.get(i,j) > mask.get(i,j)?dest.get(i,j):mask.get(i,j);
			dest.put(i,j,Maxval);
		}
	}
	CReleaseMat(temp);
}
//Morphological dilate
void Morph_Dilate_Gray( Raw2D &mask,Raw2D &dest,int n){//gray image
	//temp=strel_B("disc",n);
	Mat temp;
        temp=strel("disc",n);
	int i,j,k,l,row,col,x,y;
	PIXTYPE Maxval,Minval,value;
	row=mask.getXsize();
	col=mask.getYsize();	
	//marker eroded by temp
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			Maxval=0;
			for(k=-n;k<=n;k++){
				x=i+k;
				x=x<0?0:x;
				x=x>(row-1)?(row-1):x;
				for(l=-n;l<=n;l++){
					y=j+l;
					y=y<0?0:y;
					y=y>(col-1)?(col-1):y;
					value=mask.get(x,y);//+(*(temp.data+(k+n)*temp.col+(l+n)));
					if (Maxval < value) Maxval = value;
				}	
			}
			dest.put(i,j,Maxval);
		}
	}
	//Geodesic dilate
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			Minval=dest.get(i,j) < mask.get(i,j)?dest.get(i,j):mask.get(i,j);
			dest.put(i,j,Minval);
		}
	}
	CReleaseMat(temp);
}
//Morphological reconstuction 
void  Morph_reconstuct_Gray( Raw2D &mask,Raw2D &dest,string strings,int n){
	Raw2D temp(mask.getXsize(),mask.getYsize());
	int iterations=200;//control iterations
	Copy(mask,dest);//copy
	if("dilate"==strings){
		Morph_Dilate_Gray(mask,temp,n);
		while(iterations && CompareImage(dest,temp)){
			Copy(temp,dest);
			Morph_Dilate_Gray(dest,temp,n);
			iterations--;
		}
	}
	if("erode"==strings){
		Morph_Erode_Gray(mask,temp,n);
		while(iterations && CompareImage(dest,temp)){  
			Copy(temp,dest);
			Morph_Erode_Gray(dest,temp,n);
			iterations--;
		}
	}                                                     
}

//three Dimensions
//compare
int CompareImage( Raw &src1, Raw &src2){
	if (src1.getXsize()!=src2.getXsize() || src1.getYsize()!=src2.getYsize() || src1.getZsize()!=src2.getZsize())	return 1;
	int i,j,k;
	long long col,row,height;
	PIXTYPE value;
	col=src1.getXsize();
	row=src1.getYsize();
	height=src1.getZsize();
	value=0;
	for(k=0;k<height;k++){
		for (i=0;i<row;i++){
			for (j=0;j<col;j++){
				value=src1.get(j,i,k) - src2.get(j,i,k);
				if (value!=0) break;
			}
			if (value!=0) break;
		}
		if (value!=0) break;
	}
	if (value!=0) return 1;
	else return 0;
}
//Gray image copy
void Copy( Raw &src,Raw &dest){
	if(src.getXsize()!=dest.getXsize() || src.getYsize()!=dest.getYsize()|| src.getZsize()!=dest.getZsize()) {
		printf("Size no match!");
		return ;
	}
	int i,j,k;
	long long col,row,height;
	col=src.getXsize();
	row=src.getYsize();
	height=src.getZsize();
	for(k=0;k<height;k++){
		for (i=0;i<row;i++){
			for (j=0;j<col;j++){
				dest.put(j,i,k,src.get(j,i,k));
			}
		}
	}
}
//Gray image reversion
void Reversion(Raw &image){
	int i,j,k;
	long long row,col,height;
	PIXTYPE val;
	col=image.getXsize();
	row=image.getYsize();
	height=image.getZsize();
	for(k=0;k<height;k++){
		for (i=0;i<row;i++){
			for (j=0;j<col;j++){
				val=(PIXTYPE)(255 - image.get(j,i,k));
				image.put(j,i,k,val);
			}
		}
	}
}
//gray image erode
void  Erode_Gray( Raw &mask,Raw &dest,int n){
	int i,j,k,m,p,q,x,y,z;
	long long row,col,height;
	PIXTYPE Minval,value;
	row=mask.getYsize();
	col=mask.getXsize();
	height=mask.getZsize();
	Mat temp;
        temp=strel_B("disc",n);
	//marker eroded by temp
	for(k=0;k<height;k++){
		for(i=0;i<row;i++){
			for(j=0;j<col;j++){
				Minval=255;
				for(m=-n;m<=n;m++){
					z=m+k;
					z=z<0?0:z;
					z=z>(height-1)?(height-1):z;
					for(p=-n;p<=n;p++){
						y=p+i;
						y=y<0?0:y;
						y=y>(row-1)?(row-1):y;
						for(q=-n;q<=n;q++){
							x=q+j;
							x=x<0?0:x;
							x=x>(col-1)?(col-1):x;
							value=mask.get(x,y,z);//-(*(temp.data+(p+n)*temp.col+q+n));
						    if (Minval > value) Minval = value;
						}
					}	
				}
				dest.put(j,i,k,Minval);
			}
		}
	}
	 CReleaseMat(temp);
}
//dilate
void  Dilate_Gray( Raw &mask,Raw &dest,int n){//gray image
	int i,j,k,m,p,q,x,y,z;
	long long row,col,height;
	PIXTYPE Maxval,value;
	row=mask.getYsize();
	col=mask.getXsize();
	height=mask.getZsize();
	Mat temp;
        temp=strel_B("disc",n);
	//marker eroded by temp
	for(k=0;k<height;k++){
		for(i=0;i<row;i++){
			for(j=0;j<col;j++){
				Maxval=0;
				for(m=-n;m<=n;m++){
					z=m+k;
					z=z<0?0:z;
					z=z>(height-1)?(height-1):z;
					for(p=-n;p<=n;p++){
						y=p+i;
						y=y<0?0:y;
						y=y>(row-1)?(row-1):y;
						for(q=-n;q<=n;q++){
							x=q+j;
							x=x<0?0:x;
							x=x>(col-1)?(col-1):x;
							value=mask.get(x,y,z);//+(*(temp.data+(p+n)*temp.col+q+n));
						    if (Maxval < value) Maxval = value;
						}
					}	
				}
				dest.put(j,i,k,Maxval);
			}
		}
	}
    CReleaseMat(temp);
}
//Morphological erode
void  Morph_Erode_Gray( Raw &mask,Raw &dest,int n){//gray image
	int i,j,k,m,p,q,x,y,z;
	long long row,col,height;
	PIXTYPE Minval,Maxval,value;
	row=mask.getYsize();
	col=mask.getXsize();
	height=mask.getZsize();
    Mat temp;
        temp=strel_B("disc",n);
	//marker eroded by temp
	for(k=0;k<height;k++){
		for(i=0;i<row;i++){
			for(j=0;j<col;j++){
				Minval=255;
				for(m=-n;m<=n;m++){
					z=m+k;
					z=z<0?0:z;
					z=z>(height-1)?(height-1):z;
					for(p=-n;p<=n;p++){
						y=p+i;
						y=y<0?0:y;
						y=y>(row-1)?(row-1):y;
						for(q=-n;q<=n;q++){
							x=q+j;
							x=x<0?0:x;
							x=x>(col-1)?(col-1):x;
							value=mask.get(x,y,z);//-(*(temp.data+(p+n)*temp.col+q+n));
							//value=value<0?0:value;
						    if (Minval > value) Minval = value;
						}
					}	
				}
				dest.put(j,i,k,Minval);
			}
		}
	}
	//Geodedesic erosion
	for(k=0;k<height;k++){
		for(i=0;i<row;i++){
			for(j=0;j<col;j++){
				Maxval=dest.get(j,i,k) > mask.get(j,i,k)?dest.get(j,i,k):mask.get(j,i,k);
				dest.put(j,i,k,Maxval);
			}
		}
	}
	 CReleaseMat(temp);
}
//Morphological dilate
// n halfsize of structure data
void Morph_Dilate_Gray( Raw &mask,Raw &dest,int n){//gray image
	int i,j,k,m,p,q,x,y,z;
	long long row,col,height;
	PIXTYPE Maxval,Minval,value;
	row=mask.getYsize();
	col=mask.getXsize();
	height=mask.getZsize();
	Mat temp;
        temp=strel_B("disc",n);
	//marker eroded by temp
	for(k=0;k<height;k++){
		for(i=0;i<row;i++){
			for(j=0;j<col;j++){
				Maxval=0;
				for(m=-n;m<=n;m++){
					z=m+k;
					z=z<0?0:z;
					z=z>(height-1)?height-1:z;
					for(p=-n;p<=n;p++){
						y=p+i;
						y=y<0?0:y;
						y=y>(row-1)?row-1:y;
						for(q=-n;q<=n;q++){
							x=q+j;
							x=x<0?0:x;
							x=x>(col-1)?col-1:x;
							value=mask.get(x,y,z);//+(*(temp.data+(p+n)*temp.row+q+n));
							//value=value>255?255:value;
						    if (Maxval < value) Maxval = value;
						}
					}	
				}
				dest.put(j,i,k,Maxval);
			}
		}
	}
	//Geodedesic dilation
	for(k=0;k<height;k++){
		for(i=0;i<row;i++){
			for(j=0;j<col;j++){
				Minval=dest.get(j,i,k) < mask.get(j,i,k)?dest.get(j,i,k):mask.get(j,i,k);
				dest.put(j,i,k,Minval);
			}
		}
	}
	CReleaseMat(temp);
}
//Morphological reconstruction
//strings : dilate or erode
void Morph_reconstuct_Gray( Raw &mask,Raw &dest,string strings,int n){//gray image reconstruction
	Raw temp(mask.getXsize(),mask.getYsize(),mask.getZsize());
	int iterations=200;//control iterations
	Copy(mask,dest);
	if("dilate"==strings){
		Morph_Dilate_Gray(mask,temp,n);
		while(iterations && CompareImage(dest,temp)){
			Copy(temp,dest);
			Morph_Dilate_Gray(dest,temp,n);
			iterations--;
		}
	}
	if("erode"==strings){
		Morph_Erode_Gray(mask,temp,n);
		while(iterations && CompareImage(dest,temp)){  
			Copy(temp,dest);
			Morph_Erode_Gray(dest,temp,n);
			iterations--;
		}
	}                                                        
}
