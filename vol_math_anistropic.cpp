//#include "vol_math_RawImage.h"
//#include "vol_math_Raw3D_Independt.h"
//#include "vol_math_anistropic.h"
//
//
//
//PIXTYPE max_min( PIXTYPE val_min ) 
//{
//	throw std::exception("The method or operation is not implemented.");
//}
//
//PIXTYPE  Anistropic:: _linear_atXY(const float fx, const float fy, const int z, const int c, const PIXTYPE out_value) 
//	 {
//	const int
//		x = (int)fx - (fx>=0?0:1), nx = x + 1,
//		y = (int)fy - (fy>=0?0:1), ny = y + 1;
//	const float
//		dx = fx - x,
//		dy = fy - y;
//	const PIXTYPE
//		Icc = (PIXTYPE)atXY(x,y,z,c,out_value),  Inc = (PIXTYPE)atXY(nx,y,z,c,out_value),
//		Icn = (PIXTYPE)atXY(x,ny,z,c,out_value), Inn = (PIXTYPE)atXY(nx,ny,z,c,out_value);
//	return Icc + dx*(Inc-Icc + dy*(Icc+Inn-Icn-Inc)) + dy*(Icn-Icc);
//}
//
//PIXTYPE Anistropic:: _linear_atXYZ(const float fx, const float fy=0, const float fz=0, const int c=0)  {
//		const float
//			nfx = fx<0?0:(fx>_width-1?_width-1:fx),
//			nfy = fy<0?0:(fy>_height-1?_height-1:fy),
//			nfz = fz<0?0:(fz>_depth-1?_depth-1:fz);
//		const unsigned int
//			x = (unsigned int)nfx,
//			y = (unsigned int)nfy,
//			z = (unsigned int)nfz;
//		const float
//			dx = nfx - x,
//			dy = nfy - y,
//			dz = nfz - z;
//		const unsigned int
//			nx = dx>0?x+1:x,
//			ny = dy>0?y+1:y,
//			nz = dz>0?z+1:z;
//		const PIXTYPE
//			Iccc = (T)(raw4d)(x,y,z,c), Incc = (T)(raw4d)(nx,y,z,c),
//			Icnc = (T)(raw4d)(x,ny,z,c), Innc = (T)(raw4d)(nx,ny,z,c),
//			Iccn = (T)(raw4d)(x,y,nz,c), Incn = (T)(raw4d)(nx,y,nz,c),
//			Icnn = (T)(raw4d)(x,ny,nz,c), Innn = (T)(raw4d)(nx,ny,nz,c);
//		return Iccc +
//			dx*(Incc-Iccc +
//			dy*(Iccc+Innc-Icnc-Incc +
//			dz*(Iccn+Innn+Icnc+Incc-Icnn-Incn-Iccc-Innc)) +
//			dz*(Iccc+Incn-Iccn-Incc)) +
//			dy*(Icnc-Iccc +
//			dz*(Iccc+Icnn-Iccn-Icnc)) +
//			dz*(Iccn-Iccc);
//	}
//
////void raw2d_3x3( Raw2D I, PIXTYPE T ) 
////{
////	throw std::exception("The method or operation is not implemented.");
////}
//
//	Raw4D& blur_anisotropic( Raw4D & G,
//		const float amplitude=60, const float dl=0.8f, const float da=30,
//		const float gauss_prec=2, const unsigned int interpolation_type=0,
//		const bool is_fast_approx=1) 
//	{
//			Raw4D ret(G);
//			// Check arguments and init variables
//			/*if (!is_sameXYZ(G) || (G._spectrum!=3 && G._spectrum!=6))
//				throw CImgArgumentException(_cimg_instance
//				"blur_anisotropic(): Invalid specified diffusion tensor field (%u,%u,%u,%u,%p).",
//				cimg_instance,
//				G._width,G._height,G._depth,G._spectrum,G._data);*/
//
//			//if (is_empty() || amplitude<=0 || dl<0) return *this;
//			const bool is_3d = 1;
//			PIXTYPE val_min, val_max = max_min(val_min);
//			int _width,_height,_depth,_spectrum;
//			if (da<=0) {  // Iterated oriented Laplacians
//				
//				Raw4D    velocity(_width,_height,_depth);
//				for (unsigned int iteration = 0; iteration<(unsigned int)amplitude; ++iteration) {
//					PIXTYPE *ptrd = velocity.getdata(), veloc_max = 0;
//					if (is_3d) { // 3d version
//						RAW_3x3x3(I,PIXTYPE);
//						RAW_forC(ret,c) RAW_for3x3x3(ret,x,y,z,c,I,PIXTYPE) {
//							const PIXTYPE
//								ixx = Incc + Ipcc - 2*Iccc,
//								ixy = (Innc + Ippc - Inpc - Ipnc)/4,
//								ixz = (Incn + Ipcp - Incp - Ipcn)/4,
//								iyy = Icnc + Icpc - 2*Iccc,
//								iyz = (Icnn + Icpp - Icnp - Icpn)/4,
//								izz = Iccn + Iccp - 2*Iccc,
//								veloc = (PIXTYPE)(G.get(x,y,z,0)*ixx + 2*G.get(x,y,z,1)*ixy + 2*G(x,y,z,2)*ixz + G(x,y,z,3)*iyy + 2*G(x,y,z,4)*iyz + G(x,y,z,5)*izz);
//							*(ptrd++) = veloc;
//							if (veloc>veloc_max) veloc_max = veloc; else if (-veloc>veloc_max) veloc_max = -veloc;
//						}
//					} else { // 2d version
//						Raw2d_3x3(I,PIXTYPE);
//						RAW_forZC(ret,z,c) RAW_for3x3(ret,x,y,z,c,I,PIXTYPE) {
//							const PIXTYPE
//								ixx = Inc + Ipc - 2*Icc,
//								ixy = (Inn + Ipp - Inp - Ipn)/4,
//								iyy = Icn + Icp - 2*Icc,
//								veloc = (PIXTYPE)(G(x,y,0,0)*ixx + 2*G(x,y,0,1)*ixy + G(x,y,0,2)*iyy);
//							*(ptrd++) = veloc;
//							if (veloc>veloc_max) veloc_max = veloc; else if (-veloc>veloc_max) veloc_max = -veloc;
//						}
//					}
//					if (veloc_max>0) ret+=(velocity*=dl/veloc_max);
//				}
//			} else { // LIC-based smoothing.
//				const unsigned long whd = (unsigned long)_width*_height*_depth;
//				const float sqrt2amplitude = (float)std::sqrt(2*amplitude);
//				const int dx1 = G.getXsize() - 1, dy1 = G.getYsize() - 1, dz1 = G.getZsize() - 1;
//				Raw4D res(_width,_height,_depth,_spectrum,0), W(_width,_height,_depth,is_3d?4:3), val(_spectrum);
//				int N = 0;
//				if (is_3d) { // 3d version
//					for (float phi = (180%(int)da)/2.0f; phi<=180; phi+=da) {
//						const float phir = (float)(phi*PI/180), datmp = (float)(da/std::cos(phir)), da2 = datmp<1?360.0f:datmp;
//						for (float theta = 0; theta<360; (theta+=da2),++N) {
//							const float
//								thetar = (float)(theta*PI/180),
//								vx = (float)(std::cos(thetar)*std::cos(phir)),
//								vy = (float)(std::sin(thetar)*std::cos(phir)),
//								vz = (float)std::sin(phir);
//							const PIXTYPE
//								*pa = G.get(0,0,0), *pb = G.data(0,0,0,1), *pc = G.data(0,0,0,2),
//								*pd = G.data(0,0,0,3), *pe = G.data(0,0,0,4), *pf = G.data(0,0,0,5);
//							PIXTYPE *pd0 = W.data(0,0,0,0), *pd1 = W.data(0,0,0,1), *pd2 = W.data(0,0,0,2), *pd3 = W.data(0,0,0,3);
//							RAW_forXYZ(ret,xg,yg,zg) {
//								const PIXTYPE a = *(pa++), b = *(pb++), c = *(pc++), d = *(pd++), e = *(pe++), f = *(pf++);
//								const float
//									u = (float)(a*vx + b*vy + c*vz),
//									v = (float)(b*vx + d*vy + e*vz),
//									w = (float)(c*vx + e*vy + f*vz),
//									n = (float)std::sqrt(1e-5+u*u+v*v+w*w),
//									dln = dl/n;
//								*(pd0++) = (PIXTYPE)(u*dln);
//								*(pd1++) = (PIXTYPE)(v*dln);
//								*(pd2++) = (PIXTYPE)(w*dln);
//								*(pd3++) = (PIXTYPE)n;
//							}
//
//							PIXTYPE *ptrd = res.getdata();
//							RAW_forXYZ(ret,x,y,z) {
//								val.put(0);
//								const float
//									n = (float)W(x,y,z,3),
//									fsigma = (float)(n*sqrt2amplitude),
//									fsigma2 = 2*fsigma*fsigma,
//									length = gauss_prec*fsigma;
//								float
//									S = 0,
//									X = (float)x,
//									Y = (float)y,
//									Z = (float)z;
//								switch (interpolation_type) {
//								case 0 : { // Nearest neighbor
//									for (float l = 0; l<length && X>=0 && X<=dx1 && Y>=0 && Y<=dy1 && Z>=0 && Z<=dz1; l+=dl) {
//										const int
//											cx = (int)(X+0.5f),
//											cy = (int)(Y+0.5f),
//											cz = (int)(Z+0.5f);
//										const float
//											u = (float)W(cx,cy,cz,0),
//											v = (float)W(cx,cy,cz,1),
//											w = (float)W(cx,cy,cz,2);
//										if (is_fast_approx) { RAW_forC(*this,c) val[c]+=(PIXTYPE)(ret)(cx,cy,cz,c); ++S; }
//										else {
//											const float coef = (float)std::exp(-l*l/fsigma2);
//											RAW_forC(ret,c) val[c]+=(PIXTYPE)(coef*(ret)(cx,cy,cz,c));
//											S+=coef;
//										}
//										X+=u; Y+=v; Z+=w;
//									}
//										 } break;
//								case 1 : { // Linear interpolation
//									for (float l = 0; l<length && X>=0 && X<=dx1 && Y>=0 && Y<=dy1 && Z>=0 && Z<=dz1; l+=dl) {
//										const float
//											u = (float)(W._linear_atXYZ(X,Y,Z,0)),
//											v = (float)(W._linear_atXYZ(X,Y,Z,1)),
//											w = (float)(W._linear_atXYZ(X,Y,Z,2));
//										if (is_fast_approx) {RAW_forC(*this,c) val[c]+=(PIXTYPE)_linear_atXYZ(X,Y,Z,c); ++S; }
//										else {
//											const float coef = (float)std::exp(-l*l/fsigma2);
//											RAW_forC(*this,c) val[c]+=(PIXTYPE)(coef*_linear_atXYZ(X,Y,Z,c));
//											S+=coef;
//										}
//										X+=u; Y+=v; Z+=w;
//									}
//										 } break;
//								default : { // 2nd order Runge Kutta
//									for (float l = 0; l<length && X>=0 && X<=dx1 && Y>=0 && Y<=dy1 && Z>=0 && Z<=dz1; l+=dl) {
//										const float
//											u0 = (float)(0.5f*W._linear_atXYZ(X,Y,Z,0)),
//											v0 = (float)(0.5f*W._linear_atXYZ(X,Y,Z,1)),
//											w0 = (float)(0.5f*W._linear_atXYZ(X,Y,Z,2)),
//											u = (float)(W._linear_atXYZ(X+u0,Y+v0,Z+w0,0)),
//											v = (float)(W._linear_atXYZ(X+u0,Y+v0,Z+w0,1)),
//											w = (float)(W._linear_atXYZ(X+u0,Y+v0,Z+w0,2));
//										if (is_fast_approx) { RAW_forC(*this,c) val[c]+=(PIXTYPE)_linear_atXYZ(X,Y,Z,c); ++S; }
//										else {
//											const float coef = (float)std::exp(-l*l/fsigma2);
//											RAW_forC(*this,c) val[c]+=(PIXTYPE)(coef*_linear_atXYZ(X,Y,Z,c));
//											S+=coef;
//										}
//										X+=u; Y+=v; Z+=w;
//									}
//										  } break;
//								}
//								PIXTYPE *_ptrd = ptrd++;
//								if (S>0) RAW_forC(res,c) { *_ptrd+=val[c]/S; _ptrd+=whd; }
//								else RAW_forC(res,c) { *_ptrd+=(PIXTYPE)((ret)(x,y,z,c)); _ptrd+=whd; }
//							}
//						}
//					}
//				} else { // 2d LIC algorithm
//					for (float theta = (360%(int)da)/2.0f; theta<360; (theta+=da),++N) {
//						const float thetar = (float)(theta*PI/180), vx = (float)(std::cos(thetar)), vy = (float)(std::sin(thetar));
//						const PIXTYPE *pa = G.data(0,0,0,0), *pb = G.data(0,0,0,1), *pc = G.data(0,0,0,2);
//						PIXTYPE *pd0 = W.data(0,0,0,0), *pd1 = W.data(0,0,0,1), *pd2 = W.data(0,0,0,2);
//						RAW_forXY(G,xg,yg) {
//							const PIXTYPE a = *(pa++), b = *(pb++), c = *(pc++);
//							const float
//								u = (float)(a*vx + b*vy),
//								v = (float)(b*vx + c*vy),
//								n = (float)std::sqrt(1e-5+u*u+v*v),
//								dln = dl/n;
//							*(pd0++) = (PIXTYPE)(u*dln);
//							*(pd1++) = (PIXTYPE)(v*dln);
//							*(pd2++) = (PIXTYPE)n;
//						}
//						PIXTYPE *ptrd = res.getdata();
//						RAW_forXY(ret,x,y) {
//							val.fill(0);
//							const float
//								n = (float)W(x,y,0,2),
//								fsigma = (float)(n*sqrt2amplitude),
//								fsigma2 = 2*fsigma*fsigma,
//								length = gauss_prec*fsigma;
//							float
//								S = 0,
//								X = (float)x,
//								Y = (float)y;
//							switch (interpolation_type) {
//							case 0 : { // Nearest-neighbor
//								for (float l = 0; l<length && X>=0 && X<=dx1 && Y>=0 && Y<=dy1; l+=dl) {
//									const int
//										cx = (int)(X+0.5f),
//										cy = (int)(Y+0.5f);
//									const float
//										u = (float)W(cx,cy,0,0),
//										v = (float)W(cx,cy,0,1);
//									if (is_fast_approx) { RAW_forC(*this,c) val[c]+=(PIXTYPE)(ret)(cx,cy,0,c); ++S; }
//									else {
//										const float coef = (float)std::exp(-l*l/fsigma2);
//										RAW_forC(*this,c) val[c]+=(PIXTYPE)(coef*(ret)(cx,cy,0,c));
//										S+=coef;
//									}
//									X+=u; Y+=v;
//								}
//									 } break;
//							case 1 : { // Linear interpolation
//								for (float l = 0; l<length && X>=0 && X<=dx1 && Y>=0 && Y<=dy1; l+=dl) {
//									const float
//										u = (float)(W._linear_atXY(X,Y,0,0)),
//										v = (float)(W._linear_atXY(X,Y,0,1));
//									if (is_fast_approx) { RAW_forC(*this,c) val[c]+=(PIXTYPE)_linear_atXY(X,Y,0,c); ++S; }
//									else {
//										const float coef = (float)std::exp(-l*l/fsigma2);
//										RAW_forC(*this,c) val[c]+=(PIXTYPE)(coef*_linear_atXY(X,Y,0,c));
//										S+=coef;
//									}
//									X+=u; Y+=v;
//								}
//									 } break;
//							default : { // 2nd-order Runge-kutta interpolation
//								for (float l = 0; l<length && X>=0 && X<=dx1 && Y>=0 && Y<=dy1; l+=dl) {
//									const float
//										u0 = (float)(0.5f*W._linear_atXY(X,Y,0,0)),
//										v0 = (float)(0.5f*W._linear_atXY(X,Y,0,1)),
//										u = (float)(W._linear_atXY(X+u0,Y+v0,0,0)),
//										v = (float)(W._linear_atXY(X+u0,Y+v0,0,1));
//									if (is_fast_approx) { RAW_forC(*this,c) val[c]+=(PIXTYPE)_linear_atXY(X,Y,0,c); ++S; }
//									else {
//										const float coef = (float)std::exp(-l*l/fsigma2);
//										RAW_forC(*this,c) val[c]+=(PIXTYPE)(coef*_linear_atXY(X,Y,0,c));
//										S+=coef;
//									}
//									X+=u; Y+=v;
//								}
//									  }
//							}
//							PIXTYPE *_ptrd = ptrd++;
//							if (S>0) RAW_forC(res,c) { *_ptrd+=val[c]/S; _ptrd+=whd; }
//							else RAW_forC(res,c) { *_ptrd+=(PIXTYPE)((ret)(x,y,0,c)); _ptrd+=whd; }
//						}
//					}
//				}
//				const PIXTYPE *ptrs = res.getdata();
//				RAW_for(G,ptrd,PIXTYPE) { const PIXTYPE val = *(ptrs++)/N; *ptrd = val<val_min?val_min:(val>val_max?val_max:(PIXTYPE)val); }
//			}
//			return G;
//	}
//
//	//! Blur image anisotropically, directed by a field of diffusion tensors \newinstance.
//	template<typename PIXTYPE>
//	CImg<PIXTYPE> get_blur_anisotropic(const CImg<PIXTYPE>& G,
//		const float amplitude=60, const float dl=0.8f, const float da=30,
//		const float gauss_prec=2, const unsigned int interpolation_type=0,
//		const bool is_fast_approx=true) const {
//			return (+*this).blur_anisotropic(G,amplitude,dl,da,gauss_prec,interpolation_type,is_fast_approx);
//	}
