#include "ImageVolume.h"

ImageVolume::ImageVolume(int width,int height, int depth,int pixelType,bool createSpace)
{
	this->Width=width;
	this->Height=height;
	this->Depth=depth;
	this->PixelType=pixelType;
	if(createSpace)
	{
		if(PixelType==1)
		{
			unsigned char  * cData=new unsigned char[GetLength()];
			Data = (void *)cData;
		}
		if(PixelType==2)
		{
			unsigned short * sData=new unsigned short[GetLength()];
			Data = (void *)sData;
		}
		if(PixelType==3)
		{
			float * fData=new float[GetLength()];
			Data = (void *)fData;
		}
	}
	else
	{
		Data=NULL;
	}
}
ImageVolume::~ImageVolume()
{
	if(PixelType==1)
	{
		unsigned char * image_8 = (unsigned char * )Data;
		if(image_8 != NULL)
			delete []image_8;
	}
	if(PixelType==2)
	{
		unsigned short * image_16 = (unsigned short * )Data;
		if(image_16 != NULL)
			delete []image_16;
	}
	if(PixelType==3)
	{
		float * image_32 = (float * )Data;
		if(image_32 != NULL)
			delete []image_32;
	}
}
IMAGE_INDEXTYPE ImageVolume::GetLength()
{
	return (IMAGE_INDEXTYPE)Width*Height*Depth;
}
void ImageVolume::ReadVolume(const char* fileName)
{
	if(PixelType==1)
	{
		FILE* file = fopen(fileName,"rb");
		if( file == NULL )
		{
			printf("open the file failed\n");
		}
		fread(Data,sizeof(unsigned char),GetLength(),file );
		fclose(file);
		return;
	}
	if(PixelType==2)
	{
		FILE* file = fopen(fileName,"rb");
		if( file == NULL )
		{
			printf("open the file failed\n");
		}
		fread(Data,sizeof(unsigned short),GetLength(),file );
		fclose(file);
		return;
	}
	if(PixelType==3)
	{
		FILE* file = fopen(fileName,"rb");
		if( file == NULL )
		{
			printf("open the file failed\n");
		}
		fread(Data,sizeof(unsigned short),GetLength(),file );
		fclose(file);
		return;
	}
}
void ImageVolume::SaveRaw(const char* fileName)
{
	if(PixelType==1)
	{
		FILE *const nfile = fopen(fileName,"wb");
		fwrite(Data,sizeof(unsigned char),GetLength(),nfile);
		fclose(nfile);
		return;
	}
	if(PixelType==2)
	{
		FILE *const nfile = fopen(fileName,"wb");
		fwrite(Data,sizeof(unsigned short),GetLength(),nfile);
		fclose(nfile);
		return;
	}
	if(PixelType==3)
	{
		FILE *const nfile = fopen(fileName,"wb");
		fwrite(Data,sizeof(float),GetLength(),nfile);
		fclose(nfile);
		return;
	}
}
