#include "vol_math_RawImage.h"
#define T PIXTYPE

#define RAW_3x3x3(I,T) T I[27]; \
	T& I##ppp = I[0]; T& I##cpp = I[1]; T& I##npp = I[2]; \
	T& I##pcp = I[3]; T& I##ccp = I[4]; T& I##ncp = I[5]; \
	T& I##pnp = I[6]; T& I##cnp = I[7]; T& I##nnp = I[8]; \
	T& I##ppc = I[9]; T& I##cpc = I[10]; T& I##npc = I[11]; \
	T& I##pcc = I[12]; T& I##ccc = I[13]; T& I##ncc = I[14]; \
	T& I##pnc = I[15]; T& I##cnc = I[16]; T& I##nnc = I[17]; \
	T& I##ppn = I[18]; T& I##cpn = I[19]; T& I##npn = I[20]; \
	T& I##pcn = I[21]; T& I##ccn = I[22]; T& I##ncn = I[23]; \
	T& I##pnn = I[24]; T& I##cnn = I[25]; T& I##nnn = I[26]; \
	I##ppp = I##cpp = I##npp = \
	I##pcp = I##ccp = I##ncp = \
	I##pnp = I##cnp = I##nnp = \
	I##ppc = I##cpc = I##npc = \
	I##pcc = I##ccc = I##ncc = \
	I##pnc = I##cnc = I##nnc = \
	I##ppn = I##cpn = I##npn = \
	I##pcn = I##ccn = I##ncn = \
	I##pnn = I##cnn = I##nnn = 0
#define RAW_forC(img,c) RAW_for1(1,c)
#define RAW_for3(bound,i) \
	for (int i = 0, _p1##i = 0, \
	_n1##i = 1>=(bound)?(int)(bound)-1:1; \
	_n1##i<(int)(bound) || i==--_n1##i; \
	_p1##i = i++, ++_n1##i)
#define RAW_for3x3(img,x,y,z,c,I,T) \
	RAW_for3((img).getYsize(),y) for (int x = 0, \
	_p1##x = 0, \
	_n1##x = (int)( \
	(I[0] = I[1] = (T)(img)(_p1##x,_p1##y,z,c)), \
	(I[3] = I[4] = (T)(img)(0,y,z,c)), \
	(I[6] = I[7] = (T)(img)(0,_n1##y,z,c)),      \
	1>=(img).get?(img).width()-1:1); \
	(_n1##x<(img).width() && ( \
	(I[2] = (T)(img)(_n1##x,_p1##y,z,c)), \
	(I[5] = (T)(img)(_n1##x,y,z,c)), \
	(I[8] = (T)(img)(_n1##x,_n1##y,z,c)),1)) || \
	x==--_n1##x; \
	I[0] = I[1], I[1] = I[2], \
	I[3] = I[4], I[4] = I[5], \
	I[6] = I[7], I[7] = I[8], \
	_p1##x = x++, ++_n1##x)

#define RAW_for1(bound,i) for (int i = 0; i<(int)(bound); ++i)
#define RAW_forX(img,x) RAW_for1((img).getXsize(),x)
#define RAW_forY(img,y) RAW_for1((img).getYsize(),y)
#define RAW_forZ(img,z) RAW_for1((img).getZsize(),z)
#define RAW_for3x3x3(img,x,y,z,c,I,T) \
	RAW_for3((img).getZsize(),z) RAW_for3((img).getYsize(),y) for (int x = 0, \
	_p1##x = 0, \
	_n1##x = (int)( \
	(I[0] = I[1] = (T)(img)(_p1##x,_p1##y,_p1##z,c)), \
	(I[3] = I[4] = (T)(img)(0,y,_p1##z,c)),  \
	(I[6] = I[7] = (T)(img)(0,_n1##y,_p1##z,c)), \
	(I[9] = I[10] = (T)(img)(0,_p1##y,z,c)), \
	(I[12] = I[13] = (T)(img)(0,y,z,c)), \
	(I[15] = I[16] = (T)(img)(0,_n1##y,z,c)), \
	(I[18] = I[19] = (T)(img)(0,_p1##y,_n1##z,c)), \
	(I[21] = I[22] = (T)(img)(0,y,_n1##z,c)), \
	(I[24] = I[25] = (T)(img)(0,_n1##y,_n1##z,c)), \
	1>=(img).getXsize()?(img).width()-1:1); \
	(_n1##x<(img).width() && ( \
	(I[2] = (T)(img)(_n1##x,_p1##y,_p1##z,c)), \
	(I[5] = (T)(img)(_n1##x,y,_p1##z,c)), \
	(I[8] = (T)(img)(_n1##x,_n1##y,_p1##z,c)), \
	(I[11] = (T)(img)(_n1##x,_p1##y,z,c)), \
	(I[14] = (T)(img)(_n1##x,y,z,c)), \
	(I[17] = (T)(img)(_n1##x,_n1##y,z,c)), \
	(I[20] = (T)(img)(_n1##x,_p1##y,_n1##z,c)), \
	(I[23] = (T)(img)(_n1##x,y,_n1##z,c)), \
	(I[26] = (T)(img)(_n1##x,_n1##y,_n1##z,c)),1)) || \
	x==--_n1##x; \
	I[0] = I[1], I[1] = I[2], I[3] = I[4], I[4] = I[5], I[6] = I[7], I[7] = I[8], \
	I[9] = I[10], I[10] = I[11], I[12] = I[13], I[13] = I[14], I[15] = I[16], I[16] = I[17], \
	I[18] = I[19], I[19] = I[20], I[21] = I[22], I[22] = I[23], I[24] = I[25], I[25] = I[26], \
	_p1##x = x++, ++_n1##x)


#define Raw2d_3x3(I,T) T I[9]; \
	T& I##pp = I[0]; T& I##cp = I[1]; T& I##np = I[2]; \
	T& I##pc = I[3]; T& I##cc = I[4]; T& I##nc = I[5]; \
	T& I##pn = I[6]; T& I##cn = I[7]; T& I##nn = I[8]; \
	I##pp = I##cp = I##np = \
	I##pc = I##cc = I##nc = \
	I##pn = I##cn = I##nn = 0
#define RAW_forXY(img,x,y) RAW_forY(img,y) RAW_forX(img,x)
#define RAW_forZC(img,z,c) RAW_forC(img,c) RAW_forZ(img,z)
#define RAW_forXYZ(img,x,y,z) RAW_forZ(img,z) RAW_forXY(img,x,y)
#define RAW_for(img,ptrs,T_ptrs) for (T_ptrs *ptrs = (img).getdata(), *_max##ptrs = (img).getdata() + (img).size(); ptrs<_max##ptrs; ++ptrs)
#define PI 3.141592653
class Anistropic
{
public:
	Raw4D raw4d;
	unsigned int _width;
	unsigned int _height;
	unsigned int _depth;
	
public:
	Anistropic()
	{
		this->_width=this->raw4d.getXsize();
		this->_height=this->raw4d.getYsize();
		this->_depth=this->raw4d.getZsize();
	}
	~Anistropic();
	PIXTYPE _linear_atXY(const float fx, const float fy, const int z, const int c, const PIXTYPE out_value) ;
	PIXTYPE  _linear_atXYZ(const float fx, const float fy=0, const float fz=0, const int c=0);
	long size(){return raw4d.getXsize()*raw4d.getYsize()*raw4d.getZsize();}
	//! Return a reference to a temporary variable of type T.
	
	inline T& temporary(const T&) {
		static T temp;
		return temp;
	}
	    /**
       Return a reference to the pixel value of the image instance located at a specified \c offset,
       or to a specified default value in case of out-of-bounds access.
       \param offset Offset to the desired pixel value.
       \param out_value Default value returned if \c offset is outside image bounds.
       \note
       - Writing \c img.at(offset,out_value) is similar to <tt>img[offset]</tt>, except that if \c offset
         is outside bounds (e.g. \c offset<0 or \c offset>=img.size()), a reference to a value \c out_value
         is safely returned instead.
       - Due to the additional boundary checking operation, this method is slower than operator()(). Use it when
         you are \e not sure about the validity of the specified pixel offset.
    **/
    PIXTYPE& at(const int offset, const PIXTYPE out_value) {
      return (offset<0 || offset>=(int)size())?(temporary(out_value)=out_value):(*this)[offset];
    }

    ////! Access to a pixel value at a specified offset, using Dirichlet boundary conditions \const.
    //PIXTYPE at(const int offset, const PIXTYPE out_value) const {
    //  return (offset<0 || offset>= (int)size())?out_value:(*this)[offset];
    //}

    //! Access to a pixel value at a specified offset, using Neumann boundary conditions.
    /**
       Return a reference to the pixel value of the image instance located at a specified \c offset,
       or to the nearest pixel location in the image instance in case of out-of-bounds access.
       \param offset Offset to the desired pixel value.
       \note
       - Similar to at(int,const PIXTYPE), except that an out-of-bounds access returns the value of the
         nearest pixel in the image instance, regarding the specified offset, i.e.
         - If \c offset<0, then \c img[0] is returned.
         - If \c offset>=img.size(), then \c img[img.size()-1] is returned.
       - Due to the additional boundary checking operation, this method is slower than operator()(). Use it when
         you are \e not sure about the validity of the specified pixel offset.
       - If you know your image instance is \e not empty, you may rather use the slightly faster method \c _at(int).
     **/
    //PIXTYPE& at(const int offset) {
    //  if (is_empty())
    //    throw CImgInstanceException(_cimg_instance
    //                                "at(): Empty instance.",
    //                                cimg_instance);
    //  return _at(offset);
    //}

    //PIXTYPE& _at(const int offset) {
    //  const unsigned int siz = (unsigned int)size();
    //  return (*this)[offset<0?0:(unsigned int)offset>=siz?siz-1:offset];
    //}

    ////! Access to a pixel value at a specified offset, using Neumann boundary conditions \const.
    //PIXTYPE at(const int offset) const {
    //  if (is_empty())
    //    throw CImgInstanceException(_cimg_instance
    //                                "at(): Empty instance.",
    //                                cimg_instance);
    //  return _at(offset);
    //}

    //PIXTYPE _at(const int offset) const {
    //  const unsigned int siz = (unsigned int)size();
    //  return (*this)[offset<0?0:(unsigned int)offset>=siz?siz-1:offset];
    //}

    //! Access to a pixel value, using Dirichlet boundary conditions for the X-coordinate.
    /**
       Return a reference to the pixel value of the image instance located at (\c x,\c y,\c z,\c c),
       or to a specified default value in case of out-of-bounds access along the X-axis.
       \param x X-coordinate of the pixel value.
       \param y Y-coordinate of the pixel value.
       \param z Z-coordinate of the pixel value.
       \param c C-coordinate of the pixel value.
       \param out_value Default value returned if \c (\c x,\c y,\c z,\c c) is outside image bounds.
       \note
       - Similar to operator()(), except that an out-of-bounds access along the X-axis returns the specified value \c out_value.
       - Due to the additional boundary checking operation, this method is slower than operator()(). Use it when
         you are \e not sure about the validity of the specified pixel coordinates.
       \warning
       - There is \e no boundary checking performed for the Y,Z and C-coordinates, so they must be inside image bounds.
    **/
    //PIXTYPE& atX(const int x, const int y, const int z, const int c, const PIXTYPE out_value) {
    //  return (x<0 || x>raw4d.getXsize())?(temporary(out_value)=out_value):(raw4d)(x,y,z,c);
    //}

    ////! Access to a pixel value, using Dirichlet boundary conditions for the X-coordinate \const.
    //PIXTYPE atX(const int x, const int y, const int z, const int c, const PIXTYPE out_value) const {
    //  return (x<0 || x>=raw4d.getXsize())?out_value:(raw4d)(x,y,z,c);
    //}

    //! Access to a pixel value, using Neumann boundary conditions for the X-coordinate.
    /**
       Return a reference to the pixel value of the image instance located at (\c x,\c y,\c z,\c c),
       or to the nearest pixel location in the image instance in case of out-of-bounds access along the X-axis.
       \param x X-coordinate of the pixel value.
       \param y Y-coordinate of the pixel value.
       \param z Z-coordinate of the pixel value.
       \param c C-coordinate of the pixel value.
       \note
       - Similar to at(int,int,int,int,const PIXTYPE), except that an out-of-bounds access returns the value of the
         nearest pixel in the image instance, regarding the specified X-coordinate.
       - Due to the additional boundary checking operation, this method is slower than operator()(). Use it when
         you are \e not sure about the validity of the specified pixel coordinates.
       - If you know your image instance is \e not empty, you may rather use the slightly faster method \c _at(int,int,int,int).
       \warning
       - There is \e no boundary checking performed for the Y,Z and C-coordinates, so they must be inside image bounds.
     **/



    //! Access to a pixel value, using Dirichlet boundary conditions for the X and Y-coordinates.
    /**
       Similar to atX(int,int,int,int,const PIXTYPE), except that boundary checking is performed both on X and Y-coordinates.
    **/
    PIXTYPE& atXY(const int x, const int y, const int z, const int c, const PIXTYPE out_value) {
      return (x<0 || y<0 || x>=raw4d.getXsize()|| y>=raw4d.getYsize())?(temporary(out_value)=out_value):(raw4d)(x,y,z,c);
    }

    //! Access to a pixel value, using Dirichlet boundary conditions for the X and Y coordinates \const.
    //PIXTYPE atXY(const int x, const int y, const int z, const int c, const PIXTYPE out_value) const {
    //  return (x<0 || y<0 || x>= raw4d.getXsize() || y>=height())?out_value:(*this)(x,y,z,c);
    //}

    //! Access to a pixel value, using Neumann boundary conditions for the X and Y-coordinates.
	    /**
       Similar to atX(int,int,int,int), except that boundary checking is performed both on X and Y-coordinates.
       \note
       - If you know your image instance is \e not empty, you may rather use the slightly faster method \c _atXY(int,int,int,int).
     **/
    //PIXTYPE & atXY(const int x, const int y, const int z=0, const int c=0) {
    //  if (is_empty())
    //    throw CImgInstanceException(_cimg_instance
    //                                "atXY(): Empty instance.",
    //                                cimg_instance);
    //  return _atXY(x,y,z,c);
    //}

    //PIXTYPE& _atXY(const int x, const int y, const int z=0, const int c=0) {
    //  return (*this)(x<0?0:(x>=raw4d.getXsize()?width()-1:x), y<0?0:(y>=height()?height()-1:y),z,c);
    //}

    ////! Access to a pixel value, using Neumann boundary conditions for the X and Y-coordinates \const.
    //PIXTYPE atXY(const int x, const int y, const int z=0, const int c=0) const {
    //  if (is_empty())
    //    throw CImgInstanceException(_cimg_instance
    //                                "atXY(): Empty instance.",
    //                                cimg_instance);
    //  return _atXY(x,y,z,c);
    //}
};

