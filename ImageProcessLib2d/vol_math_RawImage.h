#ifndef IMAGE_SWF_H
#define IMAGE_SWF_H
#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include <fstream>
#include "vol_math_ImageVolume.h"
#include"vol_math_Raw3D_Independt.h"
#include "KDetectMemoryLeak.h"


//#define u_char unsigned char
//#define PIXTYPE unsigned char
using namespace std;
/************************************************************************/
/* RawImage :three data types to be changed to double 
Raw:volume class
Raw2D:slices
RawArray:raw array or raw2D array
*/
/************************************************************************/




class Raw;
//for test ,long long algorithms is not usefull///
class RawImage
{
public:
	long long length;
	long long width;
	long long height;
public:
	PIXTYPE * buf;
	RawImage(long long length,long long height,long long width);
	RawImage(void);
	long long getlength()
	{
		return (height*length*width);
	}
	~RawImage(void);
	void readImage( unsigned char * buf,char const *file ,long long size);
	void readImagerecursive( unsigned char * buf,char const *file ,long long l,long long m,long long i,long long winsize);
	void readImagesi(short  * buf,char const *file ,long long size);
	void readStream(short *buf,char const *file,long long size);
	void writeImagecolon(Raw& destImg);
	void writeImage(Raw& destImg);
	void writeImagesesmic(Raw &destImg);
	void readImage2(float * buf,char const *file ,long long size);
	float * buf2float(unsigned char *buf);
	void writeImagesesmicarray(void * src, long long l ,long long m, long long n);
	void writeImageSesmicRecursive(void * src, long long l ,long long m, long long n);
	void save();
};



class Raw  {
private:   			//-----------------DATA----------------- 
	long long  xsize;		// # of pixels per scanline,
	long long  ysize;		// # of scanlines in this Raw.
	long long zsize;
	PIXTYPE *data;		// 1D array of PIXTYPE that are accessed as a 2D array.
	bool is_shared;
public:				//---------------init fcns-------------
	Raw(long long,long long,long long,PIXTYPE*,bool=false);	
	Raw(ImageVolume &src);
	Raw(const Raw& src,bool=false);
	Raw(long long,long long,long long);
	Raw(void);// constructor for 'empty' Raws
	~Raw(void);		// destructor; releases memory

	Raw& set_shared(bool);

	void sizer(long long ixsize, long long iysize,long long izsize);		// get mem for rectangle of pixels
	void sizer(Raw* src);					// get same amt. of mem as 'src'
	inline long long getXsize(void) {return xsize;};		// get # pixels per scanline
	inline long long getYsize(void) {return ysize;};		// get # of scanlines.
	inline long long getZsize(void) {return zsize;};		//get # of RawImage numbers
	long long wipecopy(Raw* src);			// copy, even with size mismatch change from bool swf 2013 4 16
	long long size(){return xsize*ysize*zsize;};

	inline void put(long long ix, long long iy,long long iz, PIXTYPE val)
	{	// write 'val' at location ix,iy.iz.
		data[ix + xsize*iy+xsize*ysize*iz] = val; 
	};
	inline PIXTYPE get(long long ix, long long iy,long long iz) {	// read the value at ix,iy.
		long long index=ix + xsize*iy+xsize*ysize*iz;
		return(data[index]); 
	};
	PIXTYPE *getdata(){return data;};
	PIXTYPE getXYZ(long long ixyz){		// read value at 1D address ixyz
		return data[ixyz];
	};
	inline void putXYZ(long long ixyz,PIXTYPE val){// write value at 1D address ixy
		data[ixyz] = val;
	};

	inline void swap(Raw & volume)
	{
		std::swap(this->xsize,volume.xsize);
		std::swap(this->ysize,volume.ysize);
		std::swap(this->zsize,volume.zsize);
		std::swap(this->data,volume.data);
	}

	inline Raw& operator = (Raw volume)
	{
		volume.swap(*this);
		return *this;
	}

	inline Raw& operator+=(const Raw &volume)
	{
		for (long long i = 0; i<size(); ++i)
		{
			this->data[i] += volume.data[i];
		}
		return *this;
	}

	Raw& operator+=(const PIXTYPE val)
	{
		for (long long i = 0; i < size(); ++i)
		{
			this->data[i] += val;
		}
		return *this;
	}

	Raw operator+(const Raw &volume)
	{
		return Raw(*this, true) += volume;
	}
	Raw operator+(const PIXTYPE val)
	{
		return Raw(*this, true) += val;

	}
	Raw& operator-=(const Raw &volume)
	{
		for (long long i = 0; i < size(); ++i)
		{
			this->data[i] -= volume.data[i];
		}
		return *this;
	}
	Raw& operator-=(const PIXTYPE val)
	{
		for (long long i = 0; i< size(); ++i)
		{
			this->data[i] -= val;
		}
		return *this;
	}
	Raw operator -(const Raw &volume)
	{
		return Raw(*this, true) -= volume;

	}
	Raw operator -(const PIXTYPE val)
	{
		return Raw(*this, true) -= val;

	}
	Raw& operator *=(const Raw& img)
	{
		for (long long i = 0; i < size(); ++i)
			this->data[i] *= img.data[i];
		return *this;
	}

	Raw& operator *=(const PIXTYPE val)
	{
		for (long long i = 0; i < size(); ++i)
			this->data[i] *= val;
		return *this;
	}

	Raw operator *(const Raw& img)
	{
		return  Raw(*this, true) *= img;
	}

	Raw operator *(const PIXTYPE val)
	{
		return Raw(*this, true) *= val;
	}

	Raw& operator /=(const Raw& img)
	{
		for (long long i = 0; i < size(); ++i)
			this->data[i] /= img.data[i];
		return *this;
	}

	Raw& operator/=(const PIXTYPE val)
	{
		for (long long i = 0; i < size(); ++i)
			this->data[i] /= val;
		return *this;
	}

	Raw operator /(const Raw& img)
	{
		return Raw(*this, true) /= img;
	}

	Raw operator/(const PIXTYPE val)
	{
		return Raw(*this, true) /= val;
	}

	friend Raw operator/(const PIXTYPE val, const Raw& volume);
};


//raw data slices used for trilaterfilter//
class RawArray {
public:
	Raw *z;	// dynam. allocated space for a set of Raw objects.
	long long rawNum;	// # of Raw objects stored.

public:							
	RawArray(void);// 'empty' RawArray constructor.
	RawArray(long long rawNum,Raw *src);//swf add for read data 
	~RawArray(void);	// destructor.
	void sizer(long long ixsize, long long iysize, long long izsize,long long irawNum); // reserve memory
	void sizer(RawArray* src);			// get same amt. of mem as 'src
	long long getrawNum(void) {				// return # of Raw's we hold;
		return(rawNum); 
	};
	long long getYsize() {					// # of Raw1D's in zval-th Raw;
		return(z[0].getYsize()); 
	};
	long long getXsize(){						// # of pixels on yval,zval-th line
		return(z[0].getXsize()); 
	};
	long long getZsize(){
		return (z[0].getZsize());}

	PIXTYPE get(long long ix, long long iy, long long iz,long long rawNum) {
		return(z[rawNum].get(ix,iy,iz));	// write 'val' at location ix,iy,iz. 
	};
	void put(long long ix, long long iy, long long iz,long long rawNum,  PIXTYPE val) { 
		z[rawNum].put(ix,iy,iz,val);		//write 'val' at location ix,iy,iz.
	};
	void wipecopy(RawArray& src);			// copy, resize as needed.
};
//==============================RawND reserved to be extended for dimesions >3
class RawND
{
private:   			//-----------------DATA----------------- 
	unsigned long long xsize, ysize, zsize, _spectrum;
	bool _is_shared;
	//vector <PIXTYPE> *data;	
	
	//vector<bool> *data2;
	// 1D array of PIXTYPE that are accessed as a 2D array.
public:				//---------------init fcns-------------
	RawND(unsigned long long,unsigned long long,unsigned long long,unsigned long long ,PIXTYPE*,bool=false);	
	RawND( RawND& src,bool=false);
	RawND(long long,long long,long long,bool=false);
	RawND(void);// constructor for 'empty' Raws
	~RawND(void);		// destructor; releases memory
	RawND& set_shared(bool);

};



//global functions to change data type//
void *  ImageVolume2Raw(ImageVolume &src);

void *  ImageVolume2Raw(ImageVolume *src);

void *  Raw2ImageVolume(Raw  &src,long long type);

Raw2D Image2D2Raw2D(Image2D &src);

void * Raw2D2Image2D(Raw2D &src,long long type);
void *  ImageVolume2Raw(ImageVolume *src);




#endif
