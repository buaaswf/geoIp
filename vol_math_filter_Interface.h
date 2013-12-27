#ifndef INTERFACE_SWF
#define INTERFACE_SWF


#include "vol_math_ImageVolume.h"
#include "vol_math_RawImage.h"
#include "vol_math_WipeNioisePde.h" //3d ansitropic filter
#include "vol_math_BilateralFilter.h" //2d bilateralfilter
#include "vol_math_ThreeDim_Bilateral.h"//3d bilateralfilter
#include "vol_math_GuassFilter.h"//2d \3d guass filter
#include "vol_math_trilateralfilter.h"//3d trilaterfilter
#include "vol_math_Anistropic2D.h"
#include "vol_math_2Dtrilateralfilter.h"
#include "vol_math_Otsu.h"
struct GeoBodyDataStruct
{
    const char* file_name;
    const char* project_name;
    const char* survey_name;
    //    GC_TIME = 1,
    //    GC_DEPTH = 2,
    //    GC_ANY = 3
    int field_system;
    int coordinate_system;
    float grid_inc[3];
    float global_min_value;
    float global_max_value;
    std::vector<float> min_property_value;
    std::vector<float> max_property_value;
    long long points_count;
    int body_count;
    std::vector<long long> body_index_number;
        //qym 
        std::vector<double> z_projection_area_size;//project subvolume on Z plane 
 /*   std::vector<Box3Int> coordinate_ranges;
    std::vector<std::vector<Int16Triple>*> body_data;*/
    std::vector<std::vector<unsigned char>*> body_data_value8;
     std::vector<std::vector<unsigned short>*> body_data_value16;
     std::vector<std::vector<float>*> body_data_value32;
        /*std::vector<unsigned char*> body_data_value8;
        std::vector<unsigned short*> body_data_value16;
        std::vector<float*>  body_data_value32;*/
    //1 unsigned char
    //2 unsigned short
    //3 float
    int data_type;//



    GeoBodyDataStruct()
    {
        field_system = 1;
        coordinate_system = 0;//0 cdp, 1 x-y
        grid_inc[0] = 1;// if coordinate_system == 0 ,line cdp time inc
        grid_inc[1] = 1;// if coordinate_system == 0 ,line cdp time inc
        grid_inc[2] = 1;// if coordinate_system == 0 ,line cdp time inc
        global_min_value = 0;
        global_max_value = 254;
        /*min_property_value = 0;
          max_property_value = 254;*/
        points_count = 0; //how many points of all point set
        body_count = 0; //how many point set 

        this->data_type = 1;

    }
};
struct Points
{
	long long x;
	long long y;
	long long z;
	void * pixel;
};
struct  AnistropicI
{
	int time;
	int val; //val=1
	int method; 
	int threadcount;
	/*
	method:
	1:Perona_Malik();
	2:four_diff
	*/
	AnistropicI(int time,int val,int method,int threadcount)
	{
		this->time=time;
		this->val=val;
		this->method=method;
		this->threadcount=threadcount;
	}


};
struct  TrilateralfilterI
{
	float sigmaC;//sigmaC=1
	int threadcount;
	TrilateralfilterI(int threadcount,float sigmaC =3)
	{
		this->sigmaC = sigmaC;
		this->threadcount=threadcount;
	}
};

struct BilateralFilterI
{
	double sigmaD;//guass fliter cooper std for space 
	double sigmaR;//guass filter cooper std for pixel
	int threadcount;
	BilateralFilterI(double sigmaD,double sigmaR,int threadcount)
	{
		this->sigmaD=sigmaD;
		this->sigmaR=sigmaR;
		this->threadcount=threadcount;
	}
};

struct GuassFilterI
{
	int halfsize;
	int threadcount;
	GuassFilterI(int halfsize,int threadcount)
	{
		this->halfsize=halfsize;
		this->threadcount=threadcount;
	}
};

struct lowPassI
{


};
struct MultiOstuI
{
	int classnum; //expexted numbers
	int method;
	/*
	1\user write the number 
	2\algarithm compute the number
	*/
	MultiOstuI(int classnum,int method)
	{
		this->classnum=classnum;
		this->method=method;
	}
};
extern void * doAnistropicI (ImageVolume &src,AnistropicI &);
extern void * doBilateralI (ImageVolume &, BilateralFilterI &);
extern void *doGuassFilterI (ImageVolume &, GuassFilterI &);
extern void *doTrilateralfilterI ( ImageVolume &, TrilateralfilterI &);
extern void *doMultiOstuI (ImageVolume &src,MultiOstuI &);
extern void *dolowPassI (ImageVolume &src,lowPassI &);
extern void *doMultiOstuI(ImageVolume &,MultiOstuI &);
extern void *doAnistropicI2D (Image2D &src,AnistropicI &);
extern void *doBilateralI2D (Image2D&, BilateralFilterI &);
extern void *doGuassFilterI2D (Image2D &, GuassFilterI &);
extern void *doTrilateralfilterI2D ( Image2D &, TrilateralfilterI &);
extern void *doMultiOstuI2D (Image2D &src,MultiOstuI &);
extern void *dolowPassI2D (Image2D &src,lowPassI &);
extern void *doMultiOstuI (Image2D &src,MultiOstuI &);
//extern void *domultiThread (int medthod,int threadcount);
/*
1
2
3
4
5
6
7
8
9
10
11
12
*/

#endif