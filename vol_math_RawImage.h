#ifndef IMAGE_SWF_H
#define IMAGE_SWF_H
#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include <fstream>
#include "vol_math_ImageVolume.h"
#include"vol_math_Raw3D_Independt.h"
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
//for test ,int algrithms is not usefull///
class RawImage
{
public:
	int length;
	int width;
	int height;
public:
	PIXTYPE * buf;
	RawImage(int length,int height,int width);
	RawImage(void);
	int getlength()
	{
		return (height*length*width);
	}
	~RawImage(void);
	void readImage( unsigned char * buf,char const *file ,int size);
	void readImagesi(short  * buf,char const *file ,int size);
	void readStream(short *buf,char const *file,int size);
	void writeImagecolon(Raw& destImg);
	void writeImage(Raw& destImg);
	void writeImagesesmic(Raw &destImg);
	void readImage2(float * buf,char const *file ,int size);
	float * buf2float(unsigned char *buf);
	void writeImagesesmicarray(void * src, int l ,int m, int n);
	void save();
};



class Raw  {
private:   			//-----------------DATA----------------- 
	int xsize;		// # of pixels per scanline,
	int ysize;		// # of scanlines in this Raw.
	int zsize;
	PIXTYPE *data;		// 1D array of PIXTYPE that are accessed as a 2D array.
	bool is_shared;
public:				//---------------init fcns-------------
	Raw(int,int,int,PIXTYPE*);	
	Raw(ImageVolume &src);
	Raw(const Raw& src,bool=false);
	Raw(int,int,int);
	Raw(void);// constructor for 'empty' Raws
	~Raw(void);		// destructor; releases memory

	Raw& set_shared(bool);

	void sizer(int ixsize, int iysize,int izsize);		// get mem for rectangle of pixels
	void sizer(Raw* src);					// get same amt. of mem as 'src'
	int getXsize(void) {return xsize;};		// get # pixels per scanline
	int getYsize(void) {return ysize;};		// get # of scanlines.
	int getZsize(void) {return zsize;};		//get # of RawImage numbers
	int wipecopy(Raw* src);			// copy, even with size mismatch change from bool swf 2013 4 16
	int size(){return xsize*ysize*zsize;};

	void put(int ix, int iy,int iz, PIXTYPE val) {	// write 'val' at location ix,iy.iz.
		data[ix + xsize*iy+xsize*ysize*iz] = val; 
	};
	inline PIXTYPE get(int ix, int iy,int iz) {	// read the value at ix,iy.
		int index=ix + xsize*iy+xsize*ysize*iz;
		return(data[index]); 
	};
	PIXTYPE *getdata(){return data;};
	PIXTYPE getXYZ(int ixyz){		// read value at 1D address ixyz
		return data[ixyz];
	};
	void putXYZ(int ixyz,PIXTYPE val){// write value at 1D address ixy
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

	Raw& operator+=(const Raw &volume)
	{
		for (int i = 0; i<size(); ++i)
		{
			this->data[i] += volume.data[i];
		}
		return *this;
	}
	
	Raw& operator+=(const PIXTYPE val)
	{
		for (int i = 0; i < size(); ++i)
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
		for (int i = 0; i < size(); ++i)
		{
			this->data[i] -= volume.data[i];
		}
		return *this;
	}
	Raw& operator-=(const PIXTYPE val)
	{
		for (int i = 0; i< size(); ++i)
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
		for (int i = 0; i < size(); ++i)
			this->data[i] *= img.data[i];
		return *this;
	}

	Raw& operator *=(const PIXTYPE val)
	{
		for (int i = 0; i < size(); ++i)
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
		for (int i = 0; i < size(); ++i)
			this->data[i] /= img.data[i];
		return *this;
	}

	Raw& operator/=(const PIXTYPE val)
	{
		for (int i = 0; i < size(); ++i)
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
	int rawNum;	// # of Raw objects stored.

public:							
	RawArray(void);// 'empty' RawArray constructor.
	RawArray(int rawNum,Raw *src);//swf add for read data 
	~RawArray(void);	// destructor.
	void sizer(int ixsize, int iysize, int izsize,int irawNum); // reserve memory
	void sizer(RawArray* src);			// get same amt. of mem as 'src
	int getrawNum(void) {				// return # of Raw's we hold;
		return(rawNum); 
	};
	int getYsize() {					// # of Raw1D's in zval-th Raw;
		return(z[0].getYsize()); 
	};
	int getXsize(){						// # of pixels on yval,zval-th line
		return(z[0].getXsize()); 
	};
	int getZsize(){
		return (z[0].getZsize());}

	PIXTYPE get(int ix, int iy, int iz,int rawNum) {
		return(z[rawNum].get(ix,iy,iz));	// write 'val' at location ix,iy,iz. 
	};
	void put(int ix, int iy, int iz,int rawNum,  PIXTYPE val) { 
		z[rawNum].put(ix,iy,iz,val);		//write 'val' at location ix,iy,iz.
	};
	void wipecopy(RawArray& src);			// copy, resize as needed.
};
//==============================RawND reserved to be extended for dimesions >3
class RawND
{
private:   			//-----------------DATA----------------- 
	unsigned int xsize, ysize, zsize, _spectrum;
	bool _is_shared;
	//vector <PIXTYPE> *data;	
	
	//vector<bool> *data2;
	// 1D array of PIXTYPE that are accessed as a 2D array.
public:				//---------------init fcns-------------
	RawND(unsigned int,unsigned int,unsigned int,unsigned int ,PIXTYPE*,bool=false);	
	RawND( RawND& src,bool=false);
	RawND(int,int,int,bool=false);
	RawND(void);// constructor for 'empty' Raws
	~RawND(void);		// destructor; releases memory
	RawND& set_shared(bool);

};



//global functions to change data type//
void *  ImageVolume2Raw(ImageVolume &src);

void *  Raw2ImageVolume(Raw  &src,int type);

Raw2D Image2D2Raw2D(Image2D &src);

void * Raw2D2Image2D(Raw2D &src,int type);




#endif
