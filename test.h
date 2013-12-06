#ifndef _TEST_H_
#define _TEST_H_

#include<iostream>
#include <crtdbg.h> 
#include "vol_math_Raw3D_Independt.h"
#include "vol_math_RawImage.h"
#include "CImg.h" 
using namespace cimg_library;
#undef min
#undef max
static void IShowImg(Raw2D& img)
{
	CImg <double> sourceimage(img.getXsize(),img.getYsize(),1,1,0);
	cimg_for_insideXY(sourceimage,x,y,0)
	{
		PIXTYPE val=img.get(x,y);
		//if (val<1&&val>0)
		//{
			sourceimage(x,y,0)=(double)(val);
		//}
		//else if (val>=1)
		//{
		//	sourceimage(x,y,0)=(double)(1);
		//}

	}
	sourceimage.display("hello");
}
static void Ishowsurface(CImg<double> & img, int argc,char **argv)
{
	argc=argc;
	// Compute surface triangularization by the marching cube algorithm (isovalue 0)
	CImg<> points0;
	const bool both = cimg_option("-both",false,"Show both evolving and initial surface");
	CImgList<unsigned int> faces;
	CImg<> points = img.get_isosurface3d(faces,0);
	CImgList<unsigned int> faces0;
	if (both) points0 = img.get_isosurface3d(faces0,0);
	const CImgList<unsigned char> colors0(faces0.size(),CImg<unsigned char>::vector(100,200,255));
	const CImgList<> opacities0(faces0.size(),1,1,1,1,0.2f);

	// Perform MCF evolution.
	CImgDisplay disp(512,512,0,1), disp3d(512,512,0,0);
	float alpha = 0, beta = 0;
	CImgList<unsigned char> colors(faces.size(),CImg<unsigned char>::vector(200,128,100));
	CImgList<> opacities(faces.size(),CImg<>::vector(1.0f));
	const float fact = 3*cimg::max(disp3d.width(),disp3d.height())/(4.0f*cimg::max(img.width(),img.height()));

	// Append initial object if necessary.
	if (both) {
		points.append_object3d(faces,points0,faces0);
		colors.insert(colors0);
		opacities.insert(opacities0);
	}

	// center and rescale the objects
	cimg_forX(points,l) {
		points(l,0)=(points(l,0)-img.width()/2)*fact;
		points(l,1)=(points(l,1)-img.height()/2)*fact;
		points(l,2)=(points(l,2)-img.depth()/2)*fact;
	}

	// Display 3D object on the display window.
	CImg<unsigned char> visu(disp3d.width(),disp3d.height(),1,3,0);
	const CImg<> rot = CImg<>::rotation_matrix(1,0,0,(beta+=0.01f))*CImg<>::rotation_matrix(0,1,1,(alpha+=0.05f));
	if (points.size()) {
		visu.draw_object3d(visu.width()/2.0f,visu.height()/2.0f,0.0f,
			rot*points,faces,colors,opacities,3,
			false,500.0,0.0f,0.0f,-8000.0f).display(disp3d);
	} else visu.fill(0).display(disp3d);
	img.display(disp.wait(2000));

	if ((disp3d.button() || disp3d.key()) && points.size() && !disp3d.is_keyESC() && !disp3d.is_keyQ()) {
		const unsigned char white[3] = { 255, 255, 255 };
		visu.fill(0).draw_text(10,10,"Time stopped, press any key to start again",white).
			display_object3d(disp3d,points,faces,colors,opacities,true,4,3,false,500,0,0,-5000,0.4f,0.3f);
		disp3d.set_key();
	}
	if (disp.is_resized()) disp.resize(false);
	if (disp3d.is_resized()) disp3d.resize(false);
}
static void IShowraw (Raw& raw,int argc,char **argv)
{
	CImg <float> sourceimage(raw.getXsize(),raw.getYsize(),raw.getZsize(),1,0);
	cimg_for_insideXYZ(sourceimage,x,y,z,0)
	{
		PIXTYPE val=raw.get(x,y,z);
		//if (val<1&&val>0)
		//{
			sourceimage(x,y,z,0)=(float)(val);
		//}
		//else if (val>=1)
		//{
		//	sourceimage(x,y,z,0)=(float)(1);
		//}
		//
	}
	sourceimage.display("hello");
	//Ishowsurface(sourceimage,argc,argv);
}


#endif