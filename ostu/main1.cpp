#include <iostream>
#include "Otsu.h"
//#include "cv.h"
//#include "highgui.h"
#include "vol_math_RawImage.h"
using namespace std;

int main(int argc, char* argv[])
{
	int i,j,k,X_VALUE,Y_VALUE,Z_VALUE;
	X_VALUE=281;
	Y_VALUE=481;
	Z_VALUE=2501;
	int test_arr[256]={-129};
	FILE *f=NULL;
	int size=X_VALUE*Y_VALUE*Z_VALUE;
	char *buf=new char [size];
	char *val;
	class OTSU test;
	//class Raw raw;
	/*IplImage*source=NULL;*/

	k=0;
	
	f=fopen("img\\mig.raw","rb");
	if(f==NULL) cout<<"The file is empty!"<<endl;
	else {
		fread(buf,sizeof(unsigned char),size,f);//读取文件到一个字符指针中
		//cout<<"开始处理数据"<<endl;
		//source=cvCreateImage(cvSize(X_VALUE,Y_VALUE),8,1);
		//cvZero(source);
		//for(k=0;k<Z_VALUE;k++){
		//	for(i=0;i<Y_VALUE;i++){
		//		for(j=0;j<X_VALUE;j++){
		//			if(k==1500){
		//				//if(i<100 && j<100 )cout<<(int)(*(buf+i*X_VALUE+j));
		//			    CV_IMAGE_ELEM(source,uchar,i,j)=(int)(*(buf+i*X_VALUE+j+k*X_VALUE*Y_VALUE))+128;
		//			}

		//	    }
		//    }
		//}
		cout<<endl;
		Raw raw(X_VALUE,Y_VALUE,Z_VALUE,buf);
		test.setData(raw);
		//k=test.Otsu(buf);
		//k=test.Otsu1(buf,-128,127);
		//k=test.Otsu_Step(buf);
		//k=test.Otsu_Recursion(buf);
		//k=test.Otsu_2D_Step(buf);
		//test.Otsu_MultiVal(buf,test_arr,3);
		
		
        test.Otsu_MultiVal(raw);
		for(i=0;i<10;i++){
			if(test_arr[i]>-129)
			cout<<"t="<<test_arr[i]<<endl;
		}
		
		test.SaveImage(raw);
		//cout<<"k="<<k<<endl;
		cvNamedWindow("source",1);
		cvShowImage("source",source);
		cvWaitKey(0);
		cvDestroyWindow("source");
		//对于文件的处理
		cvReleaseImage(&source);
	}
	fclose(f);//关闭文件
	
	delete buf;//释放内存
	return 0;
}

