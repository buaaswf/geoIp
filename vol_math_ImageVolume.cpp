#include "vol_math_ImageVolume.h"

ImageVolume::ImageVolume()
{

}
ImageVolume::ImageVolume(int width,int height, int depth,int pixelType,bool createSpace)
{
	this->Width=width;
	this->Height=height;
	this->Depth=depth;
	this->PixelType=pixelType;

    //qym 2013-12-4
    this->CreateSpace = createSpace;


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

ImageVolume::ImageVolume(int width,int height, int depth,int pixelType,void *data,bool createspace)
{
	this->Width=width;
	this->Height=height;
	this->Depth=depth;
	this->PixelType=pixelType;
	this->CreateSpace=createspace;
		if(PixelType==1)
		{
			if (CreateSpace ==true)
			{
				unsigned char  * cData=new unsigned char[GetLength()];
				this->Data =(void*) cData;
				memcpy(Data,data,sizeof(unsigned char)*GetLength());
			}
			else
				this->Data = data;
	


		}
		if(PixelType==2)
		{
			if (CreateSpace ==true)
			{
				unsigned short  * cData=new unsigned short[GetLength()];
				this->Data =(void*) cData;
				memcpy(Data,data,sizeof(unsigned short)*GetLength());
			}
			else
				this->Data = data;
		}
		if(PixelType==3)
		{
			if (CreateSpace ==true)
			{
				float  * cData=new float[GetLength()];
				this->Data =(void*) cData;
				memcpy(Data,data,sizeof(float)*GetLength());
			}
			else
				this->Data = data;
		}

}

ImageVolume::~ImageVolume()
{
	//qym 2013-12-4
    if(!CreateSpace)
        return;

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
//============Image2d=====================================================================//
Image2D::Image2D()
{

}
Image2D::Image2D(int width,int height,int pixelType,bool createSpace)
{
	this->width=width;
	this->height=height;
	this->PixelType=pixelType;
	if(createSpace)
	{
		if(PixelType==1)
		{
			unsigned char  * cData=new unsigned char[GetLength()];
			data =(void*)cData;
		
		}
		if(PixelType==2)
		{
			unsigned short * sData=new unsigned short[GetLength()];
			data = (void *)sData;
		}
		if(PixelType==3)
		{
			float * fData=new float[GetLength()];
			data = (void *)fData;
		}
	}
	else
	{
		data=NULL;
	}
}
Image2D::~Image2D()
{
	if(PixelType==1)
	{
		unsigned char * image_8 = (unsigned char * )data;
		if(image_8 != NULL)
			delete []image_8;
	}
	if(PixelType==2)
	{
		unsigned short * image_16 = (unsigned short * )data;
		if(image_16 != NULL)
			delete []image_16;
	}
	if(PixelType==3)
	{
		float * image_32 = (float * )data;
		if(image_32 != NULL)
			delete []image_32;
	}
}
IMAGE_INDEXTYPE Image2D::GetLength()
{
	return (IMAGE_INDEXTYPE)width*height;
}
void Image2D::ReadImage(const char* fileName)
{
	if(PixelType==1)
	{
		FILE* file = fopen(fileName,"rb");
		if( file == NULL )
		{
			printf("open the file failed\n");
		}
		fread(data,sizeof(unsigned char),GetLength(),file );
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
		fread(data,sizeof(unsigned short),GetLength(),file );
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
		fread(data,sizeof(unsigned short),GetLength(),file );
		fclose(file);
		return;
	}
}
void Image2D::SaveImage(const char* fileName)
{
	if(PixelType==1)
	{
		FILE *const nfile = fopen(fileName,"wb");
		fwrite(data,sizeof(unsigned char),GetLength(),nfile);
		fclose(nfile);
		return;
	}
	if(PixelType==2)
	{
		FILE *const nfile = fopen(fileName,"wb");
		fwrite(data,sizeof(unsigned short),GetLength(),nfile);
		fclose(nfile);
		return;
	}
	if(PixelType==3)
	{
		FILE *const nfile = fopen(fileName,"wb");
		fwrite(data,sizeof(float),GetLength(),nfile);
		fclose(nfile);
		return;
	}
}
