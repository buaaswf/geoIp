#ifndef IMAGE_SWF_H
#define IMAGE_SWF_H
#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include <fstream>
#include "ImageVolume.h"
//#include"vol_math_Raw3D_Independt.h"
//#define u_char unsigned char
#define PIXTYPE float
using namespace std;
/************************************************************************/
/* RawImage :three data types to be changed to double 
Raw:volume class
Raw2D:slices
RawArray:raw array or raw2D array
*/
/************************************************************************/
class Raw;
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
//==============================RAW4D
class Raw4D
{
private:   			//-----------------DATA----------------- 
	unsigned int xsize, ysize, zsize, _spectrum;
	bool _is_shared;
	PIXTYPE *data;		// 1D array of PIXTYPE that are accessed as a 2D array.
public:				//---------------init fcns-------------
	Raw4D(unsigned int,unsigned int,unsigned int,unsigned int ,PIXTYPE*,bool=false);	
	Raw4D( Raw4D& src,bool=false);
	Raw4D(int,int,int,bool=false);
	Raw4D(void);// constructor for 'empty' Raws
	~Raw4D(void);		// destructor; releases memory
	Raw4D& set_shared(bool);
	int getXsize(void) {return xsize;};		// get # pixels per scanline
	int getYsize(void) {return ysize;};		// get # of scanlines.
	int getZsize(void) {return zsize;};		//get # of RawImage numbers
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

	inline void swap(Raw4D & volume)
	{
		std::swap(this->xsize,volume.xsize);
		std::swap(this->ysize,volume.ysize);
		std::swap(this->zsize,volume.zsize);
		std::swap(this->data,volume.data);
	}

	inline Raw4D& operator = (Raw4D volume)
	{
		volume.swap(*this);
		return *this;
	}

	Raw4D& operator+=(const Raw4D &volume)
	{
		for (int i = 0; i<size(); ++i)
		{
			this->data[i] += volume.data[i];
		}
		return *this;
	}

	Raw4D& operator+=(const PIXTYPE val)
	{
		for (int i = 0; i < size(); ++i)
		{
			this->data[i] += val;
		}
		return *this;
	}

	Raw4D operator+(const Raw4D &volume)
	{
		return Raw4D(*this, true) += volume;
	}
	Raw4D operator+(const PIXTYPE val)
	{
		return Raw4D(*this, true) += val;

	}
	Raw4D& operator-=(const Raw4D &volume)
	{
		for (int i = 0; i < size(); ++i)
		{
			this->data[i] -= volume.data[i];
		}
		return *this;
	}
	Raw4D& operator-=(const PIXTYPE val)
	{
		for (int i = 0; i< size(); ++i)
		{
			this->data[i] -= val;
		}
		return *this;
	}
	Raw4D operator -(const Raw4D &volume)
	{
		return Raw4D(*this, true) -= volume;

	}
	Raw4D operator -(const PIXTYPE val)
	{
		return Raw4D(*this, true) -= val;

	}
	Raw4D& operator *=(const Raw4D& img)
	{
		for (int i = 0; i < size(); ++i)
			this->data[i] *= img.data[i];
		return *this;
	}

	Raw4D& operator *=(const PIXTYPE val)
	{
		for (int i = 0; i < size(); ++i)
			this->data[i] *= val;
		return *this;
	}

	Raw4D operator *(const Raw4D& img)
	{
		return  Raw4D(*this, true) *= img;
	}

	Raw4D operator *(const PIXTYPE val)
	{
		return Raw4D(*this, true) *= val;
	}

	Raw4D& operator /=(const Raw4D& img)
	{
		for (int i = 0; i < size(); ++i)
			this->data[i] /= img.data[i];
		return *this;
	}

	Raw4D& operator/=(const PIXTYPE val)
	{
		for (int i = 0; i < size(); ++i)
			this->data[i] /= val;
		return *this;
	}

	Raw4D operator /(const Raw4D& img)
	{
		return Raw4D(*this, true) /= img;
	}

	Raw4D operator/(const PIXTYPE val)
	{
		return Raw4D(*this, true) /= val;
	}

	friend Raw4D operator/(const PIXTYPE val, const Raw4D& volume);
	PIXTYPE& operator()(const unsigned int x) {
		return data[x];
	}

	const PIXTYPE& operator()(const unsigned int x) const {
		return data[x];
	}

	PIXTYPE& operator()(const unsigned int x, const unsigned int y) {
		return data[x + y*xsize];
	}

	const PIXTYPE& operator()(const unsigned int x, const unsigned int y) const {
		return data[x + y*xsize];
	}

	PIXTYPE& operator()(const unsigned int x, const unsigned int y, const unsigned int z) {
		return data[x + y*(unsigned long)xsize + z*(unsigned long)xsize*ysize];
	}

	const PIXTYPE& operator()(const unsigned int x, const unsigned int y, const unsigned int z) const {
		return data[x + y*(unsigned long)xsize + z*(unsigned long)xsize*ysize];
	}

	PIXTYPE& operator()(const unsigned int x, const unsigned int y, const unsigned int z, const unsigned int c) {
		return data[x + y*(unsigned long)xsize + z*(unsigned long)xsize*ysize + c*(unsigned long)xsize*ysize*zsize];
	}

	const PIXTYPE& operator()(const unsigned int x, const unsigned int y, const unsigned int z, const unsigned int c) const {
		return data[x + y*(unsigned long)xsize + z*(unsigned long)xsize*ysize + c*(unsigned long)xsize*ysize*zsize];
	}

	PIXTYPE& operator()(const unsigned int x, const unsigned int y, const unsigned int z, const unsigned int,
		const unsigned long wh) {
			return data[x + y*x + z*wh];
	}

	const PIXTYPE& operator()(const unsigned int x, const unsigned int y, const unsigned int z, const unsigned int,
		const unsigned long wh) const {
			return data[x + y*xsize + z*wh];
	}

	PIXTYPE& operator()(const unsigned int x, const unsigned int y, const unsigned int z, const unsigned int c,
		const unsigned long wh, const unsigned long whd) {
			return data[x + y*xsize + z*wh + c*whd];
	}

	const PIXTYPE& operator()(const unsigned int x, const unsigned int y, const unsigned int z, const unsigned int c,
		const unsigned long wh, const unsigned long whd) const {
			return data[x + y*xsize + z*wh + c*whd];
	}





};
#endif
