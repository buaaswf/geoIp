#include "vol_math_filter_Interface.h"

//qym
#include "vol_math_RawImage.h"

Raw& MultiThread(int method,int threadcount,Raw &src,void *para);
Raw& MultiThreadsipl(int method,int threadcount,Raw &src,void *para);
void * doAnistropicI(ImageVolume & src,AnistropicI & para)
{
	//int method,int threadcount,Raw &src,void *para
	Raw &indata=*(Raw *)ImageVolume2Raw(src);
	Raw *ret =new Raw( MultiThreadsipl(2,para.threadcount,indata,(void *)&para)); 
	//WipeNioisePde * pde=new WipeNioisePde(indata,para.time,para.val,para.method);
	//delete pde;
	//return Raw2ImageVolume(indata,src.PixelType);
	return ret;
}

void * doBilateralI( ImageVolume & src,BilateralFilterI & para)
{
	Raw &indata=*(Raw *)ImageVolume2Raw(src);
	//ThreeDim_Bilateral  *bila=new ThreeDim_Bilateral(&indata,para.sigmaD,para.sigmaR);
	//indata=bila->apply(indata);
	//return Raw2ImageVolume(indata,src.PixelType);
	MultiThreadsipl(3,para.threadcount,indata,(void *)&para);
	return &indata;
}
void * doGuassFilterI( ImageVolume & src,GuassFilterI & para)
{
	Raw &indata=*(Raw *)ImageVolume2Raw(src);
	//Filter *guass = new Filter();
	//Raw *ret=guass->guass3DFilter(&indata,para.halfsize);
	//return Raw2ImageVolume(*ret,src.PixelType);
	MultiThreadsipl(4,para.threadcount,indata,(void *) &para);
	return & indata;
}
void * doTrilateralfilterI(ImageVolume &src, TrilateralfilterI & para)
{
	Raw &indata =*(Raw*)ImageVolume2Raw(src);
	MultiThreadsipl(1,para.threadcount,indata,(void *)&para);
	return & indata;
}

extern void * doMultiOstuI (ImageVolume &src,MultiOstuI &para)
{
	Raw &indata =*(Raw *)ImageVolume2Raw(src);
	OTSU *ostu =new OTSU(indata);
	if (para.method==1)
	{
		
		ostu->Otsu_MultiVal(indata,para.classnum);
	} 
	else
	{
		//OTSU *ostu =new OTSU(indata);
		ostu->Otsu_MultiVal(indata);
	}
	//delete ostu;
	return &ostu->array;

}
extern void *dolowPassI (ImageVolume &src,lowPassI &para)
{
		return NULL;
}
 void *doAnistropicI2D (Image2D &src,AnistropicI & para)
{
    //qym non-const must be an lvalue
	//Raw2D &indata=Image2D2Raw2D(src);//to be test
	Raw2D indata=Image2D2Raw2D(src);//to be test
	Anistropic2D * pde=new Anistropic2D(indata,para.time,para.val,para.method);//to be ask
	delete pde;
	return &indata;
}
extern void *doBilateralI2D (Image2D & src, BilateralFilterI &para)
{
    //qym non-const must be an lvalue
	//Raw2D &indata=Image2D2Raw2D(src);
	Raw2D indata=Image2D2Raw2D(src);
	BilateralFilter *b=new BilateralFilter(&indata,6,3);
	b->apply(indata);
	return Raw2D2Image2D(indata,src.PixelType);
}
extern void * doGuassFilterI2D (Image2D &src, GuassFilterI &para)
{
    //qym non-const must be an lvalue
	//Raw2D &indata = Image2D2Raw2D(src);
	Raw2D indata = Image2D2Raw2D(src);
	Filter *guass = new Filter(); 
	Raw2D ret=guass->guassFilter(&indata,para.halfsize);
	return Raw2D2Image2D(ret,src.PixelType);
}
extern void * doTrilateralfilterI2D ( Image2D &src, TrilateralfilterI &para)
{
    //qym non-const must be an lvalue
	//Raw2D &indata = Image2D2Raw2D(src);
	Raw2D indata = Image2D2Raw2D(src);
	Trilateralfilter2D * tf=new Trilateralfilter2D();
	Raw2D *ret=tf->TrilateralFilter2D(&indata,para.sigmaC);
	return Raw2D2Image2D(*ret,src.PixelType);
}
extern void * doMultiOstuI2D (Image2D &src,MultiOstuI &para)
{
	return NULL;

}
extern void * dolowPassI2D (Image2D &src,lowPassI &)
{
		return NULL;
}
///=========================================================================================================////
struct  AnistropicP
{
	Raw *src;
	int time;
	int val; //val=1
	int method; 

	/*
	method:
	1:Perona_Malik();
	2:four_diff
	*/
	AnistropicP(Raw *data,int time,int val,int method)
	{
		this->src=data;
		this->time=time;
		this->val=val;
		this->method=method;
	}
	AnistropicP()
	{

	}


};
struct  AnistropicPsipl
{
	Raw *src;
	Raw *ret;
	int iter;
	int time;
	int val; //val=1
	int method; 

	/*
	method:
	1:Perona_Malik();
	2:four_diff
	*/
	AnistropicPsipl(Raw *data,Raw *ret,int iter,int time,int val,int method)
	{
		this->src=data;
		this->ret=ret;
		this->iter= iter;
		this->time=time;
		this->val=val;
		this->method=method;
	}
	AnistropicPsipl()
	{

	}


};
struct  TrilateralfilterP
{
	float sigmaC;//sigmaC=1
	Raw *src;
	//Raw * ret;
	TrilateralfilterP( Raw *src, float sigmaC )
	{
		this->src = src;
		//this->ret = ret;
		this->sigmaC = sigmaC;

	}
	TrilateralfilterP()
	{

	}
};
struct  TrilateralfilterPSipl
{
	float sigmaC;//sigmaC=1
	Raw *src;
	Raw * ret;
	int iter;
	TrilateralfilterPSipl( Raw *src, Raw *ret, float sigmaC,int iter )
	{
		this->src = src;
		this->ret = ret;
		this->sigmaC = sigmaC;
		this->iter = iter;

	}
	TrilateralfilterPSipl()
	{

	}
};
struct BilateralFilterP
{
	Raw *src;
	double sigmaD;
	double sigmaR;
	BilateralFilterP(Raw *src,double sigmaD,double sigmaR)
	{
		this->src=src;
		this->sigmaD=sigmaD;
		this->sigmaR=sigmaR;
	}
	BilateralFilterP()
	{
		//this->src=NULL;
		this->sigmaD=0;
		this->sigmaR=0;
	}
};
struct BilateralFilterPSipl
{
	Raw *src;
	Raw *ret;
	int iter;
	double sigmaD;
	double sigmaR;
	BilateralFilterPSipl(Raw *src,Raw *ret,int iter,double sigmaD,double sigmaR)
	{
		this->src = src;
		this->ret = ret;
		this->iter = iter;
		this->sigmaD = sigmaD;
		this->sigmaR = sigmaR;
	}
	BilateralFilterPSipl()
	{
		//this->src=NULL;
		this->sigmaD = 0;
		this->sigmaR = 0;
	}
};

struct GuassFilterP
{
	Raw *src;
	int halfsize;
	GuassFilterP(Raw *src,int halfsize)
	{
		this->src=src;
		this->halfsize=halfsize;
	}
	GuassFilterP()
	{

	}
};

struct GuassFilterPSipl
{
	Raw *src;
	Raw *ret;
	int halfsize;
	GuassFilterPSipl(Raw *src,Raw *ret,int halfsize)
	{
		this->src = src;
		this->ret = ret;
		this->halfsize = halfsize;
	}
	GuassFilterPSipl()
	{

	}
};

struct lowPassP
{

};
struct MultiOstuP
{

};


void * singleBilateral( void *para)
{
	BilateralFilterP *p= (BilateralFilterP *) para;
	Raw *indata = p->src;
	ThreeDim_Bilateral  *bila=new ThreeDim_Bilateral(indata,p->sigmaD,p->sigmaR);
	bila->apply(*indata);
	return NULL;

}
void * singleBilateralSipl( void *para)
{
	BilateralFilterPSipl *p= (BilateralFilterPSipl *) para;
	Raw *indata = p->src;
	Raw *outdata = p->ret;
	ThreeDim_Bilateral  *bila=new ThreeDim_Bilateral(indata,*outdata,p->sigmaD,p->sigmaR);
	bila->applySipl(p->iter);
	return NULL;

}
void * singleGuassFilter(void * para)
{
	GuassFilterP *p=(GuassFilterP*) para;
	Raw *indata = p->src;
	Filter *guass=new Filter();
	guass->guass3DFilter(indata,p->halfsize);
	return NULL;
}
void * singleGuassFilterSipl(void * para)
{
	GuassFilterP *p=(GuassFilterP*) para;
	Raw *indata = p->src;
	Filter *guass=new Filter();
	guass->guass3DFilter(indata,p->halfsize);
	return NULL;
}

void * singleTrilateralfilter(void *para)
{
	TrilateralfilterP *p=(TrilateralfilterP*) para; 
	Raw *indata=p->src;
	Trilateralfilter f(indata);
	//f.TrilateralFilter(p->sigmaC);

	return &indata;
}
void * singleTrilateralfilterSipl(void *para)
{
	TrilateralfilterPSipl *p=(TrilateralfilterPSipl*) para; 
	Raw *indata = p->src;
	Raw *outdata = p->ret;
	Trilateralfilter f(indata,outdata,p->iter);
	f.TrilateralFilter(*indata,*outdata,p->sigmaC);

	return &indata;
}
void * singleAnistropicFilter(void * para)
{
	AnistropicP *p=(AnistropicP*) para; 
	Raw *indata=(p->src);
	WipeNioisePde * pde=new WipeNioisePde(*indata,p->time,p->val,p->method);
	delete pde;
	return NULL;
}

void * singleAnistropicFilterSipl(void * para)
{
	AnistropicPsipl *p=(AnistropicPsipl*) para; 
	Raw *indata=(p->src);
	Raw *outdata=(p->ret);
	WipeNioisePde * pde=new WipeNioisePde(*indata,*outdata,p->iter,p->time,p->val,p->method);//two pointers,one in,one out
	delete pde;
	return NULL;
}

Raw & MultiThread(int method,int threadcount,Raw &src,void *para)
{
	//divide into slices
	//create+join
	//single
	Raw  *ret = new Raw(src);
	int countvar=0;
	if (threadcount>=1)
	{
		vector <Raw *> raw;
		std::vector<pthread_t>threads;threads.resize(threadcount);
		std::vector<int>pids;pids.resize(threadcount);
		std::vector<int> res;res.resize(threadcount);
		switch (method)
		{
		case 1 :
			{
				vector<TrilateralfilterP>parms;parms.resize(threadcount);
				int znewsize = src.getZsize()/threadcount;
				for (int i = 0; i < threadcount; i++ )
				{
					
					PIXTYPE *data=ret->getdata()+znewsize*i*src.getXsize()*src.getYsize();
					Raw * d=new Raw(src.getXsize(),src.getYsize(),znewsize,data,true);
					raw.push_back(d);
					int ret;
					TrilateralfilterI *p=(TrilateralfilterI*)para;
					parms[i]=TrilateralfilterP(raw[i],p->sigmaC);
					pthread_attr_t *attr;
					ret=pthread_create(&threads[i],NULL,singleTrilateralfilter,&parms[i]);
					cout <<i<<endl;
				}
				for(int i=0;i<threadcount;i++)
				{
					pthread_join(threads[i], NULL);
				}
				

			}
			break;
		case 2:
			{
				{
					
					vector<AnistropicP> parms; 
					parms.resize(threadcount);
					PIXTYPE *data;
					int znewsize = src.getZsize()/threadcount;
					for (int i = 0; i < threadcount; i++ )
					{
						data=ret->getdata()+znewsize*i*src.getXsize()*src.getYsize();
						Raw * d= new Raw(src.getXsize(),src.getYsize(),znewsize,data,true);
						raw.push_back(d);
						int pret;
						AnistropicI *p = (AnistropicI*)para;
						parms[i] = AnistropicP(raw[i],p->time,p->val,p->method);
						pthread_attr_t *attr;
						pret=pthread_create(&threads[i],NULL,singleAnistropicFilter,(void *)&parms[i]);
						cout <<i<<endl;
			
					}
					for(int i=0;i<threadcount;i++)
					{
						pthread_join(threads[i], NULL);
					}
				}
				break;
			}
		case 3:
			{
				{
					vector<BilateralFilterP>parms;parms.resize(threadcount);
					int znewsize = src.getZsize()/threadcount;
					PIXTYPE *data;
					for (int i = 0; i < threadcount; i++ )
					{
						data=ret->getdata()+znewsize*i*src.getXsize()*src.getYsize();
						raw.push_back(new Raw(src.getXsize(),src.getYsize(),znewsize,data,true));
						int retid;
						BilateralFilterI *p=(BilateralFilterI*)para;
						parms[i]=BilateralFilterP(raw[i],p->sigmaD,p->sigmaR);
						pthread_attr_t *attr;
						retid=pthread_create(&threads[i],NULL,singleBilateral,&parms[i]);

						cout <<i<<endl;
					}
					for(int i=0;i<threadcount;i++)
					{
						pthread_join(threads[i], NULL);
					}

				}
				break;
			}
		case 4:
			{
				{
					vector<GuassFilterP>parms;parms.resize(threadcount);
					int znewsize = src.getZsize()/threadcount;
					PIXTYPE *data;
					for (int i = 0; i < threadcount; i++ )
					{
						data=ret->getdata()+znewsize*i*src.getXsize()*src.getYsize();
						raw.push_back(new Raw(src.getXsize(),src.getYsize(),znewsize,data,true));
						int ret;
						GuassFilterI *p=(GuassFilterI*)para;
						parms[i]=GuassFilterP(raw[i],p->halfsize);
						pthread_attr_t *attr;
						ret=pthread_create(&threads[i],NULL,singleGuassFilter,&parms[i]);
						cout <<i<<endl;
					}
					for(int i=0;i<threadcount;i++)
					{
						pthread_join(threads[i], NULL);
					}

				}
				break;
			}
		}//switch... 
		//ret=new Raw(ret->getXsize(),ret->getYsize(),ret->getZsize(),out);
			//ret = ret1;
	}//if...
	else
	{
		cout << "threadcount is too few"<<endl;
	}
	for (int i = 0; i < src.size(); i++ )
	{
		if (src.getXYZ(i)!= ret->getXYZ(i))
		{
			countvar ++;
		}
		
	}
	
	cout << countvar <<endl;
	return *ret;

	/*
	1\trilaterfilter
	2\anistropic
	3\bilateralfilter
	4\guass

	*/
}
Raw & MultiThreadsipl(int method,int threadcount,Raw &src,void *para)
{
	//divide into slices
	//create+join
	//single
	Raw  *ret = new Raw(src);
	int countvar=0;
	if (threadcount>=1)
	{
		vector <Raw *> raw;
		std::vector<pthread_t>threads;threads.resize(threadcount);
		std::vector<int>pids;pids.resize(threadcount);
		std::vector<int> res;res.resize(threadcount);
		switch (method)
		{
		case 1 :
			{
				vector<TrilateralfilterPSipl>parms;parms.resize(threadcount);
				int znewsize = src.getZsize()/threadcount;
				for (int i = 0; i < threadcount; i++ )
				{
					
					PIXTYPE *data=ret->getdata()+znewsize*i*src.getXsize()*src.getYsize();
					Raw * d=new Raw(src.getXsize(),src.getYsize(),znewsize,data,true);
					raw.push_back(d);
					int ret;
					TrilateralfilterI *p=(TrilateralfilterI*)para;
					parms[i]=TrilateralfilterPSipl(&src,raw[i],p->sigmaC,i);
					pthread_attr_t *attr;
					ret=pthread_create(&threads[i],NULL,singleTrilateralfilterSipl,&parms[i]);
					cout <<i<<endl;
				}
				for(int i=0;i<threadcount;i++)
				{
					pthread_join(threads[i], NULL);
				}
				

			}
			break;
		case 2:
			{
				{
					
					vector<AnistropicPsipl> parms; 
					parms.resize(threadcount);
					PIXTYPE *data;
					int znewsize = src.getZsize()/threadcount;
					for (int i = 0; i < threadcount; i++ )
					{
						data=ret->getdata()+znewsize*i*src.getXsize()*src.getYsize();
						Raw * d= new Raw(src.getXsize(),src.getYsize(),znewsize,data,true);
						raw.push_back(d);
						int pret;
						AnistropicI *p = (AnistropicI*)para;
						parms[i] = AnistropicPsipl(&src,raw[i],i,p->time,p->val,p->method);
						pthread_attr_t *attr;
						pret=pthread_create(&threads[i],NULL,singleAnistropicFilterSipl,(void *)&parms[i]);
						cout <<i<<endl;
			
					}
					for(int i=0;i<threadcount;i++)
					{
						pthread_join(threads[i], NULL);
					}
				}
				break;
			}
		case 3:
			{
				{
					vector<BilateralFilterPSipl>parms;parms.resize(threadcount);
					int znewsize = src.getZsize()/threadcount;
					PIXTYPE *data;
					for (int i = 0; i < threadcount; i++ )
					{
						data=ret->getdata()+znewsize*i*src.getXsize()*src.getYsize();
						raw.push_back(new Raw(src.getXsize(),src.getYsize(),znewsize,data,true));
						int retid;
						BilateralFilterI *p=(BilateralFilterI*)para;
						parms[i]=BilateralFilterPSipl(&src,raw[i],i,p->sigmaD,p->sigmaR);
						pthread_attr_t *attr;
						retid=pthread_create(&threads[i],NULL,singleBilateralSipl,&parms[i]);

						cout <<i<<endl;
					}
					for(int i=0;i<threadcount;i++)
					{
						pthread_join(threads[i], NULL);
					}

				}
				break;
			}
		case 4:
			{
				{
					vector<GuassFilterPSipl>parms;parms.resize(threadcount);
					int znewsize = src.getZsize()/threadcount;
					PIXTYPE *data;
					for (int i = 0; i < threadcount; i++ )
					{
						data=ret->getdata()+znewsize*i*src.getXsize()*src.getYsize();
						raw.push_back(new Raw(src.getXsize(),src.getYsize(),znewsize,data,true));
						int ret;
						GuassFilterI *p=(GuassFilterI*)para;
						parms[i]=GuassFilterPSipl(&src,raw[i],p->halfsize);
						pthread_attr_t *attr;
						ret=pthread_create(&threads[i],NULL,singleGuassFilterSipl,&parms[i]);
						cout <<i<<endl;
					}
					for(int i=0;i<threadcount;i++)
					{
						pthread_join(threads[i], NULL);
					}

				}
				break;
			}
		}//switch... 
		//ret=new Raw(ret->getXsize(),ret->getYsize(),ret->getZsize(),out);
			//ret = ret1;
	}//if...
	else
	{
		cout << "threadcount is too few"<<endl;
	}
	for (int i = 0; i < src.size(); i++ )
	{
		if (src.getXYZ(i)!= ret->getXYZ(i))
		{
			countvar ++;
		}
		
	}
	
	cout << countvar <<endl;
	return *ret;

	/*
	1\trilaterfilter
	2\anistropic
	3\bilateralfilter
	4\guass

	*/
}
