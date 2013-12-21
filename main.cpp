//#include "memoryLeak.h"
#include <stdio.h>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "CImg.h"
#include "test.h"

#include <assert.h>


#include "semaphore.h"

#include "vol_math_Interface.h"
//#include <unistd.h>
#include <stdio.h>
#include <tchar.h>
using namespace cimg_library;

//
//int main(int argc,char **argv)
//{
//	//RawImage *img=new RawImage(281,481,2501);
//	//img->readImage(img->buf,"F:\\lab\\VTKproj\\mig.raw",img->getlength());
//	//Trilateralfilter f(img);
//	//f.TrilateralFilter(1);
//	//test3dbilateralfilter(argc,argv);
//	//test3dguass();
//	testanistropic(argc,argv);
//	system("pause");
//	return 0;
//
//
//}

void* Function_t(void* Param)
{
	printf("I am a thread! ");
	pthread_t myid = pthread_self();
	printf("thread ID=%d ", myid);
	return NULL;
}

int myglobal;
pthread_mutex_t mymutex=PTHREAD_MUTEX_INITIALIZER;
void *thread_function(void *arg) {
	int i,j;
	for ( i=0; i<20; i++) {
		pthread_mutex_lock(&mymutex);
		j=myglobal;
		j=j+1;
		printf(".");
		fflush(stdout);
		//sleep(1);
		myglobal=j;
		pthread_mutex_unlock(&mymutex);
	}
	return NULL;
}
Raw * testinterface(ImageVolume &src)
{
	//AnistropicI anis(1,1,1,5);
	TrilateralfilterI tril(5,1);
	//BilateralFilterI bil(1,1,5);
	//Raw * ret=(Raw *)doBilateralI(src,bil);
	//Raw *ret=(Raw *)doAnistropicI(src,anis);
	Raw *ret=(Raw *)doTrilateralfilterI(src,tril);
	return ret;

}
void GetMemory(char *p, int num)
{
	p = (char*)malloc(sizeof(char) * num);
}
int main(int argc, char* argv[])
{

	_CrtDumpMemoryLeaks();
	//char * str="a";
	//GetMemory(str,100);
	int l=281,m=481,n=1000;
	RawImage test;
	unsigned char * indata=new unsigned char [l*m*n];
	test.readImage(indata,"F:\\lab\\VTKproj\\mig.raw",l*m*n*sizeof(unsigned char));
	ImageVolume *imagevol=new ImageVolume(l,m,n,1,indata);
	Raw  * ret = testinterface(*imagevol);
	//test.writeImagesesmic(ret);
	unsigned char* data = (unsigned char*)Raw2ImageVolume(*ret,1);
	test.writeImagesesmicarray(data ,ret->getXsize(),ret->getYsize(),ret->getZsize());
	system("pause");
	return 1;
}