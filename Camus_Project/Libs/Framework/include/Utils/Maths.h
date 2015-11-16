#ifndef CA_MATHS_H
#define CA_MATHS_H


#include <algorithm>
#include <iostream>
#include <cmath>

#include <string.h>

#ifndef ATOF_ANGEL
#define ATOF_ANGEL 0
#endif

#ifndef ATOF_TOM_VAN 
#define ATOF_TOM_VAN  0
#endif

#ifndef ATOF_VS 
#define ATOF_VS 0
#endif

#if ATOF_VS

#ifndef ATOF_ANGEL
#undef ATOF_ANGEL
#define ATOF_ANGEL 0
#endif
#ifndef ATOF_TOM_VAN
#undef ATOF_TOM_VAN
#define ATOF_TOM_VAN  0
#endif
#endif


#if (ATOF_ANGEL) || (ATOF_TOM_VAN)

#ifdef atof
#undef atof
#endif

#if ATOF_ANGEL
#define atof(a) CadenaAnumeroFlotantePersonalizada(a)
#elif ATOF_TOM_VAN
#define atof(a) atof_tom(a)
#endif

#endif

#if ATOF_TOM_VAN
#define white_space(c) ((c) == ' ' || (c) == '\t')
#define valid_digit(c) ((c) >= '0' && (c) <= '9')
double atof_tom(const char *p);
#endif

#if ATOF_ANGEL
#define  inicioNumeros 48
float CadenaAnumeroFlotantePersonalizada(char *c);
#endif

#ifndef xNULL
#define  xNULL 0
#endif

#ifndef xPI
#define  xPI 3.141592653589f
#endif

#ifndef xSMALLFLOAT
#define  xSMALLFLOAT 0.000000001f
#endif

#if defined(WIN32) || defined(_WIN32)
#pragma warning( disable : 4521 )
#pragma warning( disable : 4522 )
#endif

#ifndef Rad2Deg
#define Rad2Deg( r ) ((r) * (180.0f / xPI))
#endif

#ifndef Deg2Rad
#define Deg2Rad( d ) ((d) * (xPI / 180.0f))
#endif


#ifndef xmax
#define xmax(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef xmin
#define xmin(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#define USE_LEFT_HANDED 0

#if USE_LEFT_HANDED
#define XMatRotationX		XMatRotationXLH
#define XMatRotationY		XMatRotationYLH
#define XMatRotationZ		XMatRotationZLH
#define XMatViewLookAt		XMatViewLookAtLH
#define XMatPerspective		XMatPerspectiveLH
#define XMatOrtho			XMatOrthoLH
#define XMatRotationAxis	XMatRotationAxisLH
#define XMatOrtho			XMatOrthoLH
#define XVecTransform		XVecTransformLH
#define XVecTransformNormal	XVecTransformNormalLH
#else
#define XMatRotationX		XMatRotationXRH
#define XMatRotationY		XMatRotationYRH
#define XMatRotationZ		XMatRotationZRH
#define XMatViewLookAt		XMatViewLookAtRH
#define XMatPerspective		XMatPerspectiveRH
#define XMatOrtho			XMatOrthoRH
#define XMatRotationAxis	XMatRotationAxisRH
#define XMatOrtho			XMatOrthoRH
#define XVecTransform		XVecTransformRH
#define XVecTransformNormal	XVecTransformNormalRH
#endif
namespace hyperspace {

		struct XMATRIX44;
		struct XVECTOR2;
		struct XVECTOR3;

		void XMatMultiply(XMATRIX44 &, const XMATRIX44 &, const XMATRIX44 &);
		void XMatTranslation(XMATRIX44 &, const float &, const float &, const float &);
		void XMatTranslation(XMATRIX44 &, XVECTOR3&);
		void XMatScaling(XMATRIX44 &, const float &, const float &, const float &);
		void XMatRotationXLH(XMATRIX44 &, const float &);
		void XMatRotationXRH(XMATRIX44 &, const float &);
		void XMatRotationYLH(XMATRIX44 &, const float &);
		void XMatRotationYRH(XMATRIX44 &, const float &);
		void XMatRotationZLH(XMATRIX44 &, const float &);
		void XMatRotationZRH(XMATRIX44 &, const float &);
		void XMatTranspose(XMATRIX44 &,  XMATRIX44 );
		void XMatIdentity(XMATRIX44 &);
		void XMatViewLookAtLH(XMATRIX44 &, const XVECTOR3 &, const XVECTOR3 &, const XVECTOR3 &);
		void XMatViewLookAtRH(XMATRIX44 &, const XVECTOR3 &, const XVECTOR3 &, const XVECTOR3 &);
		void XMatPerspectiveLH(XMATRIX44 &, const float &, const float &, const float &, const float &);
		void XMatPerspectiveRH(XMATRIX44 &, const float &, const float &, const float &, const float &);
		void XMatOrthoLH(XMATRIX44 &, const float &, const float &, const float &, const float &);
		void XMatOrthoRH(XMATRIX44 &, const float &, const float &, const float &, const float &);
		void XMatRotationAxisRH(XMATRIX44 &, const XVECTOR3 &, const float &);
		void XMatRotationAxisLH(XMATRIX44 &, const XVECTOR3 &, const float &);

		void XVecTransformLH(XVECTOR3 &, const XVECTOR3 &, const XMATRIX44 &);
		void XVecTransformNormalLH(XVECTOR3 &, const XVECTOR3 &, const XMATRIX44 &);

		void XVecTransformRH(XVECTOR3 &, const XVECTOR3 &, const XMATRIX44 &);
		void XVecTransformNormalRH(XVECTOR3 &, const XVECTOR3 &, const XMATRIX44 &);

		void XVecDot(float &, const XVECTOR3 &, const XVECTOR3 &);
		void XVecLength(float &, const XVECTOR3 &v);
		void XVecCross(XVECTOR3 &, const XVECTOR3 &, const XVECTOR3 &);
		void XVec3Normalize(XVECTOR3 &, const XVECTOR3 &);
		


		typedef unsigned int	std_uint;

		struct XVECTOR2 {
			XVECTOR2() :x(0.0f), y(0.0f) {}
			XVECTOR2(const float *vf) :x(vf[0]), y(vf[1]) {}
			XVECTOR2(const XVECTOR2& v) :x(v.x), y(v.y) {}
			XVECTOR2(const float &xp, const float &yp) :x(xp), y(yp) {}

			XVECTOR2 & operator= (const XVECTOR2 & other) {
				this->x = other.x; this->y = other.y;
				return *this;
			}
			XVECTOR2 & operator= (XVECTOR2 & other) {
				this->x = other.x; this->y = other.y;
				return *this;
			}

			operator float* ();
			operator const float* () const;

			XVECTOR2& operator += (const XVECTOR2&);
			XVECTOR2& operator -= (const XVECTOR2&);
			XVECTOR2& operator *= (float);
			XVECTOR2& operator /= (float);

			XVECTOR2 operator + () const;
			XVECTOR2 operator - () const;

			XVECTOR2 operator + (const XVECTOR2&) const;
			XVECTOR2 operator - (const XVECTOR2&) const;
			XVECTOR2 operator * (float) const;
			XVECTOR2 operator / (float) const;

			bool operator == (const XVECTOR2&) const;
			bool operator != (const XVECTOR2&) const;

			void Normalize();
			float Length();

			union {
				float	 v[2];

				struct {
					float x;
					float y;
				};
			};
		};

		//Vector 3D

		struct _XVECTOR3 {
			float x, y, z;
		};

		struct XVECTOR3 {
			XVECTOR3() :x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
			XVECTOR3(const float *vf) :x(vf[0]), y(vf[1]), z(vf[2]) { w = 0.0f; }
			XVECTOR3(const XVECTOR3& v) :x(v.x), y(v.y), z(v.z), w(0.0f) {}
			XVECTOR3(const _XVECTOR3& v) :x(v.x), y(v.y), z(v.z), w(0.0f) {}
			XVECTOR3(const float &xp, const float &yp, const float &zp) :x(xp), y(yp), z(zp), w(0.0f){  }
			XVECTOR3(const float &xp, const float &yp, const float &zp,const float &wp) :x(xp), y(yp), z(zp), w(wp) { }

			XVECTOR3 & operator= (const XVECTOR3 & other) {
				this->x = other.x; this->y = other.y; this->z = other.z; this->w = other.w;
				return *this;
			}
			XVECTOR3 & operator= (XVECTOR3 & other) {
				this->x = other.x; this->y = other.y; this->z = other.z; this->w = other.w;
				return *this;
			}

			operator float* ();
			operator const float* () const;

			XVECTOR3& operator += (const XVECTOR3&);
			XVECTOR3& operator -= (const XVECTOR3&);
			XVECTOR3& operator *= (float);
			XVECTOR3& operator /= (float);

			XVECTOR3 operator + () const;
			XVECTOR3 operator - () const;

			XVECTOR3 operator + (const XVECTOR3&) const;
			XVECTOR3 operator - (const XVECTOR3&) const;
			XVECTOR3 operator * (float) const;
			XVECTOR3 operator / (float) const;

			XVECTOR3	   operator * (const XVECTOR3&);

			friend XVECTOR3 operator * (float, const struct XVECTOR3&);

			bool operator == (const XVECTOR3&) const;
			bool operator != (const XVECTOR3&) const;

			void Normalize();
			float Length();

			union {
				float	 v[4];

				struct {
					float x;
					float y;
					float z;
					float w;
				};

				struct {
					float r;
					float g;
					float b;
					float a;
				};
			};
		};

		struct XMATRIX44
		{
			XMATRIX44() :m11(1.0f), m12(0.0f), m13(0.0f), m14(0.0f),
						 m21(0.0f), m22(1.0f), m23(0.0f), m24(0.0f),
						 m31(0.0f), m32(0.0f), m33(1.0f), m34(0.0f),
						 m41(0.0f), m42(0.0f), m43(0.0f), m44(1.0f) {}
			XMATRIX44(const float* m) : m11(m[0]), m12(m[1]), m13(m[2]),   m14(m[3]),
										m21(m[4]), m22(m[5]), m23(m[6]),   m24(m[7]),
										m31(m[8]), m32(m[9]), m33(m[10]),  m34(m[11]),
										m41(m[12]),m42(m[13]),m43(m[14]),  m44(m[15]) {}
			XMATRIX44(const XMATRIX44& m) : m11(m.m11), m12(m.m12), m13(m.m13), m14(m.m14),
											m21(m.m21), m22(m.m22), m23(m.m23), m24(m.m24),
											m31(m.m31), m32(m.m32), m33(m.m33), m34(m.m34),
											m41(m.m41), m42(m.m42), m43(m.m43), m44(m.m44) {}
			XMATRIX44(  const float &m11, const float &m12, const float &m13, const float &m14,
						const float &m21, const float &m22, const float &m23, const float &m24,
						const float &m31, const float &m32, const float &m33, const float &m34,
						const float &m41, const float &m42, const float &m43, const float &m44)
				:   m11(m11), m12(m12), m13(m13), m14(m14),
					m21(m21), m22(m22), m23(m23), m24(m24),
					m31(m31), m32(m32), m33(m33), m34(m34),
					m41(m41), m42(m42), m43(m43), m44(m44) {}

			XMATRIX44 & operator= (const XMATRIX44 & other) {
				this->m11 = other.m11; this->m12 = other.m12; this->m13 = other.m13; this->m14 = other.m14;
				this->m21 = other.m21; this->m22 = other.m22; this->m23 = other.m23; this->m24 = other.m24;
				this->m31 = other.m31; this->m32 = other.m32; this->m33 = other.m33; this->m34 = other.m34;
				this->m41 = other.m41; this->m42 = other.m42; this->m43 = other.m43; this->m44 = other.m44;
				return *this;
			}
			XMATRIX44 & operator= (XMATRIX44 & other) {
				this->m11 = other.m11; this->m12 = other.m12; this->m13 = other.m13; this->m14 = other.m14;
				this->m21 = other.m21; this->m22 = other.m22; this->m23 = other.m23; this->m24 = other.m24;
				this->m31 = other.m31; this->m32 = other.m32; this->m33 = other.m33; this->m34 = other.m34;
				this->m41 = other.m41; this->m42 = other.m42; this->m43 = other.m43; this->m44 = other.m44;
				return *this;
			}

			XMATRIX44& operator *= (const XMATRIX44&);
			XMATRIX44& operator += (const XMATRIX44&);
			XMATRIX44& operator -= (const XMATRIX44&);
			XMATRIX44& operator *= (float);
			XMATRIX44& operator /= (float);

			XMATRIX44 operator + () const;
			XMATRIX44 operator - () const;

			operator float*();
			operator const float* () const;

			XMATRIX44 operator * (const XMATRIX44&) const;
			XMATRIX44 operator + (const XMATRIX44&) const;
			XMATRIX44 operator - (const XMATRIX44&) const;
			XMATRIX44 operator * (float) const;
			XMATRIX44 operator / (float) const;

			friend XMATRIX44 operator * (float, const XMATRIX44&);

			bool operator == (const XMATRIX44&) const;
			bool operator != (const XMATRIX44&) const;

			union
			{
				float m[4][4];

				struct {
					float        m11, m12, m13, m14;
					float        m21, m22, m23, m24;
					float        m31, m32, m33, m34;
					float        m41, m42, m43, m44;
				};

				float mat[16];
			};

			void Identity() {
				m[0][0] = 1.0f;
				m[0][1] = 0.0f;
				m[0][2] = 0.0f;
				m[0][3] = 0.0f;

				m[1][0] = 0.0f;
				m[1][1] = 1.0f;
				m[1][2] = 0.0f;
				m[1][3] = 0.0f;

				m[2][0] = 0.0f;
				m[2][1] = 0.0f;
				m[2][2] = 1.0f;
				m[2][3] = 0.0f;

				m[3][0] = 0.0f;
				m[3][1] = 0.0f;
				m[3][2] = 0.0f;
				m[3][3] = 1.0f;
			}

			void Zero() {
				m[0][0] = 0.0f;
				m[0][1] = 0.0f;
				m[0][2] = 0.0f;
				m[0][3] = 0.0f;

				m[1][0] = 0.0f;
				m[1][1] = 0.0f;
				m[1][2] = 0.0f;
				m[1][3] = 0.0f;

				m[2][0] = 0.0f;
				m[2][1] = 0.0f;
				m[2][2] = 0.0f;
				m[2][3] = 0.0f;

				m[3][0] = 0.0f;
				m[3][1] = 0.0f;
				m[3][2] = 0.0f;
				m[3][3] = 0.0f;
			}
		};

		struct XQUATERNION {
			XQUATERNION() :x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
			XQUATERNION(const float *vf) :x(vf[0]), y(vf[1]), z(vf[2]), w(vf[3]) {}
			XQUATERNION(const XVECTOR3& v) :x(v.x), y(v.y), z(v.z), w(v.w) {}
			XQUATERNION(const XQUATERNION& v) :x(v.x), y(v.y), z(v.z), w(v.w) {}
			XQUATERNION(const float &xp, const float &yp, const float &zp, const float &wp) :x(xp), y(yp), z(zp), w(wp) {}
			union {
				float	 v[4];

				struct {
					float x;
					float y;
					float z;
					float w;
				};
			};
		};

		inline void XMatMultiply(XMATRIX44 &mpout, const XMATRIX44 &mp1, const XMATRIX44 &mp2) {
			
			float a00 = mp1.m[0][0], a01 = mp1.m[0][1], a02 = mp1.m[0][2], a03 = mp1.m[0][3],
				a10 = mp1.m[1][0], a11 = mp1.m[1][1], a12 = mp1.m[1][2], a13 = mp1.m[1][3],
				a20 = mp1.m[2][0], a21 = mp1.m[2][1], a22 = mp1.m[2][2], a23 = mp1.m[2][3],
				a30 = mp1.m[3][0], a31 = mp1.m[3][1], a32 = mp1.m[3][2], a33 = mp1.m[3][3];

			float b00 = mp2.m[0][0], b01 = mp2.m[0][1], b02 = mp2.m[0][2], b03 = mp2.m[0][3],
				b10 = mp2.m[1][0], b11 = mp2.m[1][1], b12 = mp2.m[1][2], b13 = mp2.m[1][3],
				b20 = mp2.m[2][0], b21 = mp2.m[2][1], b22 = mp2.m[2][2], b23 = mp2.m[2][3],
				b30 = mp2.m[3][0], b31 = mp2.m[3][1], b32 = mp2.m[3][2], b33 = mp2.m[3][3];

			mpout.m[0][0] = a00*b00 + a01*b10 + a02*b20 + a03*b30;
			mpout.m[0][1] = a00*b01 + a01*b11 + a02*b21 + a03*b31;
			mpout.m[0][2] = a00*b02 + a01*b12 + a02*b22 + a03*b32;
			mpout.m[0][3] = a00*b03 + a01*b13 + a02*b23 + a03*b33;
			mpout.m[1][0] = a10*b00 + a11*b10 + a12*b20 + a13*b30;
			mpout.m[1][1] = a10*b01 + a11*b11 + a12*b21 + a13*b31;
			mpout.m[1][2] = a10*b02 + a11*b12 + a12*b22 + a13*b32;
			mpout.m[1][3] = a10*b03 + a11*b13 + a12*b23 + a13*b33;
			mpout.m[2][0] = a20*b00 + a21*b10 + a22*b20 + a23*b30;
			mpout.m[2][1] = a20*b01 + a21*b11 + a22*b21 + a23*b31;
			mpout.m[2][2] = a20*b02 + a21*b12 + a22*b22 + a23*b32;
			mpout.m[2][3] = a20*b03 + a21*b13 + a22*b23 + a23*b33;
			mpout.m[3][0] = a30*b00 + a31*b10 + a32*b20 + a33*b30;
			mpout.m[3][1] = a30*b01 + a31*b11 + a32*b21 + a33*b31;
			mpout.m[3][2] = a30*b02 + a31*b12 + a32*b22 + a33*b32;
			mpout.m[3][3] = a30*b03 + a31*b13 + a32*b23 + a33*b33;
		}

		inline void XMatTranslation(XMATRIX44 &mpout, XVECTOR3& v) {
			XMatIdentity(mpout);
			mpout.m[3][0] = v.x; mpout.m[3][1] = v.y; mpout.m[3][2] = v.z; mpout.m[3][3] = 1.0f;
		}

		inline void XMatTranslation(XMATRIX44 &mpout, XVECTOR3 v) {
			XMatIdentity(mpout);
			mpout.m[3][0] = v.x; mpout.m[3][1] = v.y; mpout.m[3][2] = v.z; mpout.m[3][3] = 1.0f;
		}

		inline void XMatTranslation(XMATRIX44 &mpout, const float &x, const float &y, const float &z) {
			XMatIdentity(mpout);
			mpout.m[3][0] = x; mpout.m[3][1] = y; mpout.m[3][2] = z; mpout.m[3][3] = 1.0f;
		}

		inline void XMatScaling(XMATRIX44 &mpout, const float &x, const float &y, const float &z) {
			mpout.m[0][0] *= x;
			mpout.m[1][1] *= y;
			mpout.m[2][2] *= z;
		}

		inline void XMatRotationXLH(XMATRIX44 &mpout, const float &rads) {
			mpout.m[1][1] = std::cos(rads);
			mpout.m[1][2] = std::sin(rads);
			mpout.m[2][1] = -std::sin(rads);
			mpout.m[2][2] = std::cos(rads);
		}

		inline void XMatRotationXRH(XMATRIX44 &mpout, const float &rads) {
			mpout.m[1][1] = std::cos(rads);
			mpout.m[1][2] = -std::sin(rads);
			mpout.m[2][1] = std::sin(rads);
			mpout.m[2][2] = std::cos(rads);
		}

		inline void XMatRotationYLH(XMATRIX44 &mpout, const float &rads) {
			mpout.m[0][0] = std::cos(rads);
			mpout.m[0][2] = -std::sin(rads);
			mpout.m[2][0] = std::sin(rads);
			mpout.m[2][2] = std::cos(rads);
		}

		inline void XMatRotationYRH(XMATRIX44 &mpout, const float &rads) {
			mpout.m[0][0] = std::cos(rads);
			mpout.m[0][2] = std::sin(rads);
			mpout.m[2][0] = -std::sin(rads);
			mpout.m[2][2] = std::cos(rads);
		}

		inline void XMatRotationZLH(XMATRIX44 &mpout, const float &rads) {
			mpout.m[0][0] = std::cos(rads);
			mpout.m[0][1] = std::sin(rads);
			mpout.m[1][0] = -std::sin(rads);
			mpout.m[1][1] = std::cos(rads);
		}

		inline void XMatRotationZRH(XMATRIX44 &mpout, const float &rads) {
			mpout.m[0][0] = std::cos(rads);
			mpout.m[0][1] = -std::sin(rads);
			mpout.m[1][0] = std::sin(rads);
			mpout.m[1][1] = std::cos(rads);
		}


		inline void XMatTranspose(XMATRIX44 &mpout, const XMATRIX44 mat) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					mpout.m[i][j] = mat.m[j][i];
				}
			}
		}

		inline void XMatIdentity(XMATRIX44 &mat) {
			mat.m[0][0] = 1.0f;
			mat.m[0][1] = 0.0f;
			mat.m[0][2] = 0.0f;
			mat.m[0][3] = 0.0f;

			mat.m[1][0] = 0.0f;
			mat.m[1][1] = 1.0f;
			mat.m[1][2] = 0.0f;
			mat.m[1][3] = 0.0f;

			mat.m[2][0] = 0.0f;
			mat.m[2][1] = 0.0f;
			mat.m[2][2] = 1.0f;
			mat.m[2][3] = 0.0f;

			mat.m[3][0] = 0.0f;
			mat.m[3][1] = 0.0f;
			mat.m[3][2] = 0.0f;
			mat.m[3][3] = 1.0f;
		}

		inline void XMatViewLookAtLH(XMATRIX44 &mpout, const XVECTOR3 &camPos, const XVECTOR3 &lookAt, const XVECTOR3 &upAxis) {
			XVECTOR3 zaxe = lookAt;
			zaxe -= camPos;
			zaxe.Normalize();

			XVECTOR3 xaxe;
			XVecCross(xaxe, upAxis, zaxe);
			xaxe.Normalize();

			XVECTOR3 yaxe;
			XVecCross(yaxe, zaxe, xaxe);

			mpout.m[0][0] = xaxe.x;
			mpout.m[0][1] = xaxe.y;
			mpout.m[0][2] = xaxe.z;
			XVecDot(mpout.m[0][3], xaxe, camPos);
			mpout.m[0][3] = -mpout.m[0][3];

			mpout.m[1][0] = yaxe.x;
			mpout.m[1][1] = yaxe.y;
			mpout.m[1][2] = yaxe.z;
			XVecDot(mpout.m[1][3], yaxe, camPos);
			mpout.m[1][3] = -mpout.m[1][3];

			mpout.m[2][0] = zaxe.x;
			mpout.m[2][1] = zaxe.y;
			mpout.m[2][2] = zaxe.z;
			XVecDot(mpout.m[2][3], zaxe, camPos);
			mpout.m[2][3] *= -mpout.m[2][3];

			mpout.m[3][0] = 0.0f;
			mpout.m[3][1] = 0.0f;
			mpout.m[3][2] = 0.0f;
			mpout.m[3][3] = 1.0f;
		}

		inline void XMatViewLookAtRH(XMATRIX44 &mpout, const XVECTOR3 &camPos, const XVECTOR3 &lookAt, const XVECTOR3 &upAxis) {
			XVECTOR3 zaxe = camPos;
			zaxe -= lookAt;
			zaxe.Normalize();

			XVECTOR3 xaxe;
			XVecCross(xaxe, upAxis, zaxe);
			xaxe.Normalize();

			XVECTOR3 yaxe;
			XVecCross(yaxe, zaxe, xaxe);

			mpout.m[0][0] = xaxe.x;
			mpout.m[0][1] = xaxe.y;
			mpout.m[0][2] = xaxe.z;
			XVecDot(mpout.m[0][3], xaxe, camPos);

			mpout.m[1][0] = yaxe.x;
			mpout.m[1][1] = yaxe.y;
			mpout.m[1][2] = yaxe.z;
			XVecDot(mpout.m[1][3], yaxe, camPos);

			mpout.m[2][0] = zaxe.x;
			mpout.m[2][1] = zaxe.y;
			mpout.m[2][2] = zaxe.z;
			XVecDot(mpout.m[2][3], zaxe, camPos);

			mpout.m[3][0] = 0.0f;
			mpout.m[3][1] = 0.0f;
			mpout.m[3][2] = 0.0f;
			mpout.m[3][3] = 1.0f;
		}


		inline void XMatPerspectiveLH(XMATRIX44 &mpout, const float &FOV, const float &Aspect, const float &NearPlane, const float &FarPlane) {
			float x, y;
			float ang = FOV / 2.0f;

			y = cos(ang) / sin(ang) ;
			x = y / Aspect;

			mpout.m[0][0] = x;
			mpout.m[1][1] = y;
			mpout.m[2][2] = FarPlane / (FarPlane - NearPlane);
			mpout.m[2][3] = 1.0;
			mpout.m[3][2] = -(NearPlane*FarPlane) / (FarPlane - NearPlane);

		}

		inline void XMatPerspectiveRH(XMATRIX44 &mpout, const float &FOV, const float &Aspect, const float &NearPlane, const float &FarPlane)
		{
			float x, y;
			float ang = FOV / 2.0f;

			y = cos(ang) / sin(ang) ;
			x = y / Aspect;

			mpout.m[0][0] = x;
			mpout.m[1][1] = y;
			mpout.m[2][2] = FarPlane / (NearPlane - FarPlane);
			mpout.m[2][3] = -1.0f;
			mpout.m[3][2] = (NearPlane*FarPlane) / (NearPlane - FarPlane);
		}

		inline void XMatOrthoLH(XMATRIX44 &m, const float &w, const float &h, const float &zn, const float &zf) {
			m.m[0][0] = 2.0f / w;
			m.m[1][1] = 2.0f / h;
			m.m[2][2] = 1.0f / (zf - zn);
			m.m[2][3] = -zn / (zf - zn);
			m.m[3][3] = 1.0f;
		}

		inline void XMatOrthoRH(XMATRIX44 &m, const float &w, const float &h, const float &zn, const float &zf) {
			m.m[0][0] = 2.0f / w;
			m.m[1][1] = 2.0f / h;
			m.m[2][2] = 1.0f / (zn - zf);
			m.m[2][3] = zn / (zn - zf);
			m.m[3][3] = 1.0f;
		}

		inline void XMatRotationAxisRH(XMATRIX44 &mpout, const XVECTOR3 &axis, const float &angle) {
			float cosangle = std::cos(angle);
			float sinangle = std::sin(angle);
			float A = 1.0f - cosangle;

			mpout.m[0][0] = A*axis.x*axis.x + cosangle;
			mpout.m[1][0] = A*axis.x*axis.y + sinangle*axis.z;
			mpout.m[2][0] = A*axis.x*axis.z - sinangle*axis.y;
			mpout.m[3][0] = 0.0f;

			mpout.m[0][1] = A*axis.x*axis.y - sinangle*axis.z;
			mpout.m[1][1] = A*axis.y*axis.y + cosangle;
			mpout.m[2][1] = A*axis.y*axis.z + sinangle*axis.x;
			mpout.m[3][1] = 0.0f;

			mpout.m[0][2] = A*axis.x*axis.z + sinangle*axis.y;
			mpout.m[1][2] = A*axis.y*axis.z - sinangle*axis.x;
			mpout.m[2][2] = A*axis.z*axis.z + cosangle;
			mpout.m[3][2] = 0.0f;

			mpout.m[0][3] = mpout.m[1][3] = mpout.m[2][3] = 0.0f; mpout.m[3][3] = 1.0f;
		}

		inline void XMatRotationAxisLH(XMATRIX44 &mpout, const XVECTOR3 &axis, const float &angle) {
			float cosangle = std::cos(angle);
			float sinangle = std::sin(angle);
			float A = 1.0f - cosangle;

			mpout.m[0][0] = A*axis.x*axis.x + cosangle;
			mpout.m[1][0] = A*axis.x*axis.y - sinangle*axis.z;
			mpout.m[2][0] = A*axis.x*axis.z + sinangle*axis.y;
			mpout.m[3][0] = 0.0f;

			mpout.m[0][1] = A*axis.x*axis.y + sinangle*axis.z;
			mpout.m[1][1] = A*axis.y*axis.y + cosangle;
			mpout.m[2][1] = A*axis.y*axis.z - sinangle*axis.x;
			mpout.m[3][1] = 0.0f;

			mpout.m[0][2] = A*axis.x*axis.z - sinangle*axis.y;
			mpout.m[1][2] = A*axis.y*axis.z + sinangle*axis.x;
			mpout.m[2][2] = A*axis.z*axis.z + cosangle;
			mpout.m[3][2] = 0.0f;

			mpout.m[0][3] = mpout.m[1][3] = mpout.m[2][3] = 0.0f; mpout.m[3][3] = 1.0f;
		}

		inline void XVecTransformLH(XVECTOR3 &vpout, const XVECTOR3 &v, const XMATRIX44 &mat) {
			vpout.x = v.x*mat.m[0][0] + v.y*mat.m[1][0] + v.z*mat.m[2][0] + 1.0f*mat.m[3][0];
			vpout.y = v.x*mat.m[0][1] + v.y*mat.m[1][1] + v.z*mat.m[2][1] + 1.0f*mat.m[3][1];
			vpout.z = v.x*mat.m[0][2] + v.y*mat.m[1][2] + v.z*mat.m[2][2] + 1.0f*mat.m[3][2];
			vpout.w = v.x*mat.m[0][3] + v.y*mat.m[1][3] + v.z*mat.m[2][3] + 1.0f*mat.m[3][3];
		}

		inline void XVecTransformNormalLH(XVECTOR3 &vpout, const XVECTOR3 &v, const XMATRIX44 &mat) {
			vpout.x = v.x*mat.m[0][0] + v.y*mat.m[1][0] + v.z*mat.m[2][0];
			vpout.y = v.x*mat.m[0][1] + v.y*mat.m[1][1] + v.z*mat.m[2][1];
			vpout.z = v.x*mat.m[0][2] + v.y*mat.m[1][2] + v.z*mat.m[2][2];
			vpout.w = 1.0f;
		}

		inline void XVecTransformRH(XVECTOR3 &vpout, const XVECTOR3 &v, const XMATRIX44 &mat) {
			vpout.x = v.x*mat.m[0][0] + v.y*mat.m[0][1] + v.z*mat.m[0][2] + 1.0f*mat.m[0][3];
			vpout.y = v.x*mat.m[1][0] + v.y*mat.m[1][1] + v.z*mat.m[1][2] + 1.0f*mat.m[1][3];
			vpout.z = v.x*mat.m[2][0] + v.y*mat.m[2][1] + v.z*mat.m[2][2] + 1.0f*mat.m[2][3];
			vpout.w = v.x*mat.m[3][0] + v.y*mat.m[3][1] + v.z*mat.m[3][2] + 1.0f*mat.m[3][3];
		}

		inline void XVecTransformNormalRH(XVECTOR3 &vpout, const XVECTOR3 &v, const XMATRIX44 &mat) {
			vpout.x = v.x*mat.m[0][0] + v.y*mat.m[0][1] + v.z*mat.m[0][2];
			vpout.y = v.x*mat.m[1][0] + v.y*mat.m[1][1] + v.z*mat.m[1][2];
			vpout.z = v.x*mat.m[2][0] + v.y*mat.m[2][1] + v.z*mat.m[2][2];
			vpout.w = 1.0f;
		}

		inline void XVecDot(float &f, const XVECTOR3 &v1, const XVECTOR3 &v2) {
			f = v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
		}

		inline void XVecLength(float &f, const XVECTOR3 &v) {
			f = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
		}

		inline void XVecCross(XVECTOR3 &vpout, const XVECTOR3 &lv, const XVECTOR3 &rv) {
			vpout.x = lv.y*rv.z - lv.z*rv.y;
			vpout.y = lv.z*rv.x - lv.x*rv.z;
			vpout.z = lv.x*rv.y - lv.y*rv.x;
		}

		inline void XVec3Normalize(XVECTOR3 &pOut, const XVECTOR3 &pV) {
			float v = 1.0f / sqrtf(pV.x * pV.x + pV.y * pV.y + pV.z * pV.z);
			pOut.x = pV.x * v;
			pOut.y = pV.y * v;
			pOut.z = pV.z * v;
		}

		inline XVECTOR2& XVECTOR2::operator += (const XVECTOR2& v)
		{
			x += v.x;
			y += v.y;
			return *this;
		}

		inline XVECTOR2& XVECTOR2::operator -= (const XVECTOR2& v)
		{
			x -= v.x;
			y -= v.y;
			return *this;
		}

		inline XVECTOR2& XVECTOR2::operator *= (float f)
		{
			x *= f;
			y *= f;
			return *this;
		}

		inline XVECTOR2& XVECTOR2::operator /= (float f)
		{
			x /= f;
			y /= f;
			return *this;
		}

		inline XVECTOR2 XVECTOR2::operator + () const
		{
			return *this;
		}

		inline XVECTOR2 XVECTOR2::operator - () const
		{
			return XVECTOR2(-x, -y);
		}

		inline XVECTOR2 XVECTOR2::operator + (const XVECTOR2& v) const
		{
			return XVECTOR2(x + v.x, y + v.y);
		}

		inline XVECTOR2 XVECTOR2::operator - (const XVECTOR2& v) const
		{
			return XVECTOR2(x - v.x, y - v.y);
		}

		inline XVECTOR2 XVECTOR2::operator * (float f) const
		{
			return XVECTOR2(x*f, y*f);
		}

		inline XVECTOR2 XVECTOR2::operator / (float f) const
		{
			return XVECTOR2(x / f, y / f);
		}

		inline bool XVECTOR2::operator == (const XVECTOR2& v) const
		{
			return x == v.x && y == v.y;
		}

		inline bool XVECTOR2::operator != (const XVECTOR2& v) const
		{
			return x != v.x || y != v.y;
		}

		inline void XVECTOR2::Normalize()
		{
			float mod = sqrt(x*x + y*y);
			x /= mod;
			y /= mod;
		}

		inline XVECTOR2::operator float* ()
		{
			return (float *)&x;
		}

		inline XVECTOR2::operator const float* () const
		{
			return (const float *)&x;
		}

		inline float XVECTOR2::Length()
		{
			return sqrt(x*x + y*y);
		}

		inline XVECTOR3::operator float* ()
		{
			return (float *)&x;
		}

		inline XVECTOR3::operator const float* () const
		{
			return (const float *)&x;
		}

		inline XVECTOR3& XVECTOR3::operator += (const XVECTOR3& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		inline XVECTOR3& XVECTOR3::operator -= (const XVECTOR3& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}

		inline XVECTOR3& XVECTOR3::operator *= (float f)
		{
			x *= f;
			y *= f;
			z *= f;
			return *this;
		}

		inline XVECTOR3& XVECTOR3::operator /= (float f)
		{
			x /= f;
			y /= f;
			z /= f;
			return *this;
		}


		inline XVECTOR3 XVECTOR3::operator + () const
		{
			return *this;
		}

		inline XVECTOR3 XVECTOR3::operator - () const
		{
			return	XVECTOR3(-x, -y, -z);
		}

		inline XVECTOR3 operator * (float f, const struct XVECTOR3& v)
		{
			return XVECTOR3(f * v.x, f * v.y, f * v.z);
		}

		inline XVECTOR3 XVECTOR3::operator * (const XVECTOR3& v)
		{
			return XVECTOR3((this->x*v.x) , (this->y*v.y) , (this->z*v.z));
		}

		inline XVECTOR3 XVECTOR3::operator + (const XVECTOR3& v) const
		{
			return XVECTOR3(x + v.x, y + v.y, z + v.z);
		}

		inline XVECTOR3 XVECTOR3::operator - (const XVECTOR3& v) const
		{
			return XVECTOR3(x - v.x, y - v.y, z - v.z);
		}

		inline XVECTOR3 XVECTOR3::operator * (float f) const
		{
			return XVECTOR3(x*f, y*f, z*f);
		}

		inline XVECTOR3 XVECTOR3::operator / (float f) const
		{
			return XVECTOR3(x / f, y / f, z / f);
		}

		inline bool XVECTOR3::operator == (const XVECTOR3& v) const
		{
			return (x == v.x && y == v.y && z == v.z);
		}

		inline bool XVECTOR3::operator != (const XVECTOR3& v) const
		{
			return (x != v.x || y != v.y || z != v.z);
		}

		inline void XVECTOR3::Normalize()
		{
			float mod = sqrt(x*x + y*y + z*z);
			x /= mod;
			y /= mod;
			z /= mod;
		}

		inline float XVECTOR3::Length()
		{
			return sqrt(x*x + y*y + z*z);
		}

		inline XMATRIX44& XMATRIX44::operator *= (const XMATRIX44& mb)
		{
			float a00 = this->m[0][0], a01 = this->m[0][1], a02 = this->m[0][2], a03 = this->m[0][3],
				a10 = this->m[1][0], a11 = this->m[1][1], a12 = this->m[1][2], a13 = this->m[1][3],
				a20 = this->m[2][0], a21 = this->m[2][1], a22 = this->m[2][2], a23 = this->m[2][3],
				a30 = this->m[3][0], a31 = this->m[3][1], a32 = this->m[3][2], a33 = this->m[3][3];

			float b00 = mb.m[0][0], b01 = mb.m[0][1], b02 = mb.m[0][2], b03 = mb.m[0][3],
				b10 = mb.m[1][0], b11 = mb.m[1][1], b12 = mb.m[1][2], b13 = mb.m[1][3],
				b20 = mb.m[2][0], b21 = mb.m[2][1], b22 = mb.m[2][2], b23 = mb.m[2][3],
				b30 = mb.m[3][0], b31 = mb.m[3][1], b32 = mb.m[3][2], b33 = mb.m[3][3];

			this->m[0][0] = a00*b00 + a01*b10 + a02*b20 + a03*b30;
			this->m[0][1] = a00*b01 + a01*b11 + a02*b21 + a03*b31;
			this->m[0][2] = a00*b02 + a01*b12 + a02*b22 + a03*b32;
			this->m[0][3] = a00*b03 + a01*b13 + a02*b23 + a03*b33;
			this->m[1][0] = a10*b00 + a11*b10 + a12*b20 + a13*b30;
			this->m[1][1] = a10*b01 + a11*b11 + a12*b21 + a13*b31;
			this->m[1][2] = a10*b02 + a11*b12 + a12*b22 + a13*b32;
			this->m[1][3] = a10*b03 + a11*b13 + a12*b23 + a13*b33;
			this->m[2][0] = a20*b00 + a21*b10 + a22*b20 + a23*b30;
			this->m[2][1] = a20*b01 + a21*b11 + a22*b21 + a23*b31;
			this->m[2][2] = a20*b02 + a21*b12 + a22*b22 + a23*b32;
			this->m[2][3] = a20*b03 + a21*b13 + a22*b23 + a23*b33;
			this->m[3][0] = a30*b00 + a31*b10 + a32*b20 + a33*b30;
			this->m[3][1] = a30*b01 + a31*b11 + a32*b21 + a33*b31;
			this->m[3][2] = a30*b02 + a31*b12 + a32*b22 + a33*b32;
			this->m[3][3] = a30*b03 + a31*b13 + a32*b23 + a33*b33;
			return *this;
		}

		inline XMATRIX44& XMATRIX44::operator += (const XMATRIX44& mb)
		{
			m11 += mb.m11; m12 += mb.m12; m13 += mb.m13; m14 += mb.m14;
			m21 += mb.m21; m22 += mb.m22; m23 += mb.m23; m24 += mb.m24;
			m31 += mb.m31; m32 += mb.m32; m33 += mb.m33; m34 += mb.m34;
			m41 += mb.m41; m42 += mb.m42; m43 += mb.m43; m44 += mb.m44;
			return *this;
		}

		inline XMATRIX44& XMATRIX44::operator -= (const XMATRIX44& mb)
		{
			m11 -= mb.m11; m12 -= mb.m12; m13 -= mb.m13; m14 -= mb.m14;
			m21 -= mb.m21; m22 -= mb.m22; m23 -= mb.m23; m24 -= mb.m24;
			m31 -= mb.m31; m32 -= mb.m32; m33 -= mb.m33; m34 -= mb.m34;
			m41 -= mb.m41; m42 -= mb.m42; m43 -= mb.m43; m44 -= mb.m44;
			return *this;
		}

		inline XMATRIX44::operator float* ()
		{
			return (float *)&m11;
		}


		inline XMATRIX44::operator const float* () const
		{
			return (const float *)&m11;
		}

		inline XMATRIX44& XMATRIX44::operator *= (float f)
		{
			m11 *= f; m12 *= f; m13 *= f; m14 *= f;
			m21 *= f; m22 *= f; m23 *= f; m24 *= f;
			m31 *= f; m32 *= f; m33 *= f; m34 *= f;
			m41 *= f; m42 *= f; m43 *= f; m44 *= f;
			return *this;
		}

		inline XMATRIX44& XMATRIX44::operator /= (float f)
		{
			m11 /= f; m12 /= f; m13 /= f; m14 /= f;
			m21 /= f; m22 /= f; m23 /= f; m24 /= f;
			m31 /= f; m32 /= f; m33 /= f; m34 /= f;
			m41 /= f; m42 /= f; m43 /= f; m44 /= f;
			return *this;
		}

		inline XMATRIX44 XMATRIX44::operator + () const
		{
			return *this;
		}

		inline XMATRIX44 XMATRIX44::operator - () const
		{
			return XMATRIX44(-m11, -m12, -m13, -m14,
				-m21, -m22, -m23, -m24,
				-m31, -m32, -m33, -m34,
				-m41, -m42, -m43, -m44);
		}

		inline XMATRIX44 XMATRIX44::operator * (const XMATRIX44& mb) const
		{
			XMATRIX44	tmp;

			float a00 = this->m[0][0], a01 = this->m[0][1], a02 = this->m[0][2], a03 = this->m[0][3],
				a10 = this->m[1][0], a11 = this->m[1][1], a12 = this->m[1][2], a13 = this->m[1][3],
				a20 = this->m[2][0], a21 = this->m[2][1], a22 = this->m[2][2], a23 = this->m[2][3],
				a30 = this->m[3][0], a31 = this->m[3][1], a32 = this->m[3][2], a33 = this->m[3][3];

			float b00 = mb.m[0][0], b01 = mb.m[0][1], b02 = mb.m[0][2], b03 = mb.m[0][3],
				b10 = mb.m[1][0], b11 = mb.m[1][1], b12 = mb.m[1][2], b13 = mb.m[1][3],
				b20 = mb.m[2][0], b21 = mb.m[2][1], b22 = mb.m[2][2], b23 = mb.m[2][3],
				b30 = mb.m[3][0], b31 = mb.m[3][1], b32 = mb.m[3][2], b33 = mb.m[3][3];

			tmp.m[0][0] = a00*b00 + a01*b10 + a02*b20 + a03*b30;
			tmp.m[0][1] = a00*b01 + a01*b11 + a02*b21 + a03*b31;
			tmp.m[0][2] = a00*b02 + a01*b12 + a02*b22 + a03*b32;
			tmp.m[0][3] = a00*b03 + a01*b13 + a02*b23 + a03*b33;
			tmp.m[1][0] = a10*b00 + a11*b10 + a12*b20 + a13*b30;
			tmp.m[1][1] = a10*b01 + a11*b11 + a12*b21 + a13*b31;
			tmp.m[1][2] = a10*b02 + a11*b12 + a12*b22 + a13*b32;
			tmp.m[1][3] = a10*b03 + a11*b13 + a12*b23 + a13*b33;
			tmp.m[2][0] = a20*b00 + a21*b10 + a22*b20 + a23*b30;
			tmp.m[2][1] = a20*b01 + a21*b11 + a22*b21 + a23*b31;
			tmp.m[2][2] = a20*b02 + a21*b12 + a22*b22 + a23*b32;
			tmp.m[2][3] = a20*b03 + a21*b13 + a22*b23 + a23*b33;
			tmp.m[3][0] = a30*b00 + a31*b10 + a32*b20 + a33*b30;
			tmp.m[3][1] = a30*b01 + a31*b11 + a32*b21 + a33*b31;
			tmp.m[3][2] = a30*b02 + a31*b12 + a32*b22 + a33*b32;
			tmp.m[3][3] = a30*b03 + a31*b13 + a32*b23 + a33*b33;

			return tmp;
		}

		inline XMATRIX44 XMATRIX44::operator + (const XMATRIX44& mat) const
		{
			return XMATRIX44(m11 + mat.m11, m12 + mat.m12, m13 + mat.m13, m14 + mat.m14,
				m21 + mat.m21, m22 + mat.m22, m23 + mat.m23, m24 + mat.m24,
				m31 + mat.m31, m32 + mat.m32, m33 + mat.m33, m34 + mat.m34,
				m41 + mat.m41, m42 + mat.m42, m43 + mat.m43, m44 + mat.m44);
		}

		inline XMATRIX44 XMATRIX44::operator - (const XMATRIX44& mat) const
		{
			return XMATRIX44(m11 - mat.m11, m12 - mat.m12, m13 - mat.m13, m14 - mat.m14,
				m21 - mat.m21, m22 - mat.m22, m23 - mat.m23, m24 - mat.m24,
				m31 - mat.m31, m32 - mat.m32, m33 - mat.m33, m34 - mat.m34,
				m41 - mat.m41, m42 - mat.m42, m43 - mat.m43, m44 - mat.m44);
		}

		inline XMATRIX44 operator * (float f, const XMATRIX44& mat)
		{
			return XMATRIX44(f * mat.m11, f * mat.m12, f * mat.m13, f * mat.m14,
				f * mat.m21, f * mat.m22, f * mat.m23, f * mat.m24,
				f * mat.m31, f * mat.m32, f * mat.m33, f * mat.m34,
				f * mat.m41, f * mat.m42, f * mat.m43, f * mat.m44);
		}

		inline XMATRIX44 XMATRIX44::operator * (float f) const
		{
			return XMATRIX44(m11*f, m12*f, m13*f, m14*f,
				m21*f, m22*f, m23*f, m24*f,
				m31*f, m32*f, m33*f, m34*f,
				m41*f, m42*f, m43*f, m44*f);
		}

		inline XMATRIX44 XMATRIX44::operator / (float f) const
		{
			return XMATRIX44(m11 / f, m12 / f, m13 / f, m14 / f,
				m21 / f, m22 / f, m23 / f, m24 / f,
				m31 / f, m32 / f, m33 / f, m34 / f,
				m41 / f, m42 / f, m43 / f, m44 / f);
		}

		inline bool XMATRIX44::operator == (const XMATRIX44& mat) const
		{
			return 0 == memcmp(this, &mat, sizeof(XMATRIX44));
		}
		inline bool XMATRIX44::operator != (const XMATRIX44& mat) const
		{
			return 0 != memcmp(this, &mat, sizeof(XMATRIX44));
		}

}

#endif


