#ifndef IMAGEVOLUME_H
#define IMAGEVOLUME_H
#include <stdio.h>
#include <string.h>
#define IMAGE_INDEXTYPE size_t
const int PixelType_8bit = 1;
const int PixelType_16bit = 2;
const int PixelType_32bit = 3;

class ImageVolume
{
public:
	void* Data;
	int Width;
	int Height;
	int Depth;
	int PixelType;
	ImageVolume(int width,int height, int depth,int pixelType=1,bool createSpace=true);
	ImageVolume();
	~ImageVolume();
	IMAGE_INDEXTYPE GetLength();
	void ReadVolume(const char* fileName);
	void SaveRaw(const char* fileName);
};
class Image2D
{
public:
	void *data;
	int width;
	int height;
	int PixelType;
	Image2D();
	~Image2D();
	Image2D(int width,int height,int pixelType=1,bool createSpace=true);
	IMAGE_INDEXTYPE GetLength();
	void ReadImage(const char* fileName);
	void SaveImage(const char* fileName);
};
#endif

