/*=============================================================================
xMaths.h 	: Math classes and global functions
Revision history:
* Created by Camus
* Renamed to xMath 24 Ago 12 - Camus
=============================================================================*/

#ifndef xMATHS_H
#define xMATHS_H

#if defined(WIN32) || defined(_WIN32)
#pragma warning( disable : 4521 )
#pragma warning( disable : 4522 )
#endif


#include <math.h>
#include <string.h>

static const unsigned short xNULL = 0;
static const float xPI = 3.1415926535897932384626433832795f;
static const float xSMALLFLOAT = 0.000000001f;

#define Rad2Deg( r ) ((r) * (180.0f / xPI))
#define Deg2Rad( d ) ((d) * (xPI / 180.0f))


#ifndef xmax
#define xmax(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef xmin
#define xmin(a,b)            (((a) < (b)) ? (a) : (b))
#endif

struct XMATRIX44;
struct XVECTOR2;
struct XVECTOR3;

void XMatMultiply(XMATRIX44 &, const XMATRIX44 &, const XMATRIX44 &);
void XMatTranslation(XMATRIX44 &,const float &,const float &,const float &);
void XMatScaling(XMATRIX44 &,const float &,const float &,const float &);
void XMatRotationXLH(XMATRIX44 &,const float &);
void XMatRotationXRH(XMATRIX44 &,const float &);
void XMatRotationYLH(XMATRIX44 &,const float &);
void XMatRotationYRH(XMATRIX44 &,const float &);
void XMatRotationZLH(XMATRIX44 &,const float &);
void XMatRotationZRH(XMATRIX44 &,const float &);
void XMatTranspose(XMATRIX44 &,const XMATRIX44 &);
void XMatIdentity(XMATRIX44 &);
void XMatViewLookAtLH(XMATRIX44 &,const XVECTOR3 &,const XVECTOR3 &,const XVECTOR3 &);
void XMatViewLookAtRH(XMATRIX44 &,const XVECTOR3 &,const XVECTOR3 &,const XVECTOR3 &);
void XMatPerspectiveLH(XMATRIX44 &,const float &,const float &,const float &,const float &);
void XMatPerspectiveRH(XMATRIX44 &,const float &,const float &,const float &,const float &);
void XMatRotationAxisRH(XMATRIX44 &,const XVECTOR3 &,const float &);
void XMatRotationAxisLH(XMATRIX44 &,const XVECTOR3 &,const float &);

void XVecDot(float &,const XVECTOR3 &,const XVECTOR3 &);
void XVecLength(float &,const XVECTOR3 &v);
void XVecCross(XVECTOR3 &,const XVECTOR3 &,const XVECTOR3 &);
void XVec3Normalize( XVECTOR3 &, const XVECTOR3 &);
void XVecTransform(XVECTOR3 &,const XVECTOR3 &,const XMATRIX44 &);
void XVecTransformNormal(XVECTOR3 &,const XVECTOR3 &,const XMATRIX44 &);

typedef struct xFloat16{

	xFloat16(){ val = 0; }
	xFloat16(float f);
	xFloat16(unsigned short v);

 	operator float () const;

	xFloat16& operator = (unsigned short f);
 	xFloat16& operator = (float f);
 	xFloat16& operator = (xFloat16 f16);

 	xFloat16& operator - ();

 	xFloat16& operator += (float f);
 	xFloat16& operator -= (float f);
 	xFloat16& operator *= (float f);
 	xFloat16& operator /= (float f);

  	xFloat16& operator += (xFloat16 f16);
  	xFloat16& operator -= (xFloat16 f16);
  	xFloat16& operator *= (xFloat16 f16);
  	xFloat16& operator /= (xFloat16 f16);
//
 	xFloat16 operator + (float f);
 	xFloat16 operator - (float f);
 	xFloat16 operator * (float f);
 	xFloat16 operator / (float f);

 	xFloat16 operator + (xFloat16 f16);
 	xFloat16 operator - (xFloat16 f16);
 	xFloat16 operator * (xFloat16 f16);
 	xFloat16 operator / (xFloat16 f16);

	unsigned short val;
}float16;

typedef unsigned int	std_uint;

struct XVECTOR2{
	XVECTOR2():x(0.0f),y(0.0f){}
	XVECTOR2( const float *vf ):x(vf[0]),y(vf[1]){}
	XVECTOR2( const XVECTOR2& v):x(v.x),y(v.y){}
	XVECTOR2( const float &xp,const float &yp ):x(xp),y(yp){}

	XVECTOR2 & operator= (const XVECTOR2 & other){
		this->x = other.x; this->y = other.y;
		return *this;
	}
	XVECTOR2 & operator= (XVECTOR2 & other){
		this->x = other.x; this->y = other.y;
		return *this;
	}

	operator float* ();
	operator const float* () const;

	XVECTOR2& operator += ( const XVECTOR2& );
	XVECTOR2& operator -= ( const XVECTOR2& );
	XVECTOR2& operator *= ( float );
	XVECTOR2& operator /= ( float );

	XVECTOR2 operator + () const;
	XVECTOR2 operator - () const;

	XVECTOR2 operator + ( const XVECTOR2& ) const;
	XVECTOR2 operator - ( const XVECTOR2& ) const;
	XVECTOR2 operator * ( float ) const;
	XVECTOR2 operator / ( float ) const;

	bool operator == ( const XVECTOR2& ) const;
	bool operator != ( const XVECTOR2& ) const;

	void Normalize();
	float Length();

	union{
		float	 v[2];

		struct{	float x;
		float y;
		};
	};
};

//Vector 3D

struct _XVECTOR3{
	float x,y,z;
};

struct XVECTOR3{
	XVECTOR3():x(0.0f),y(0.0f),z(0.0f),w(0.0f){}
	XVECTOR3( const float *vf ):x(vf[0]),y(vf[1]),z(vf[2]){w=0.0f;}
	XVECTOR3( const XVECTOR3& v):x(v.x),y(v.y),z(v.z),w(0.0f){}
	XVECTOR3( const _XVECTOR3& v):x(v.x),y(v.y),z(v.z),w(0.0f){}
	XVECTOR3( const float &xp,const float &yp,const float &zp ):x(xp),y(yp),z(zp){w=0.0f;}

	XVECTOR3 & operator= (const XVECTOR3 & other){
		this->x = other.x; this->y = other.y; this->z = other.z; this->w = other.w;
		return *this;
	}
	XVECTOR3 & operator= (XVECTOR3 & other){
		this->x = other.x; this->y = other.y; this->z = other.z; this->w = other.w;
		return *this;
	}

	operator float* ();
	operator const float* () const;

	XVECTOR3& operator += ( const XVECTOR3& );
	XVECTOR3& operator -= ( const XVECTOR3& );
	XVECTOR3& operator *= ( float );
	XVECTOR3& operator /= ( float );

	XVECTOR3 operator + () const;
	XVECTOR3 operator - () const;

	XVECTOR3 operator + ( const XVECTOR3& ) const;
	XVECTOR3 operator - ( const XVECTOR3& ) const;
	XVECTOR3 operator * ( float ) const;
	XVECTOR3 operator / ( float ) const;

	float	   operator * ( const XVECTOR3& );

	friend XVECTOR3 operator * ( float, const struct XVECTOR3& );

	bool operator == ( const XVECTOR3& ) const;
	bool operator != ( const XVECTOR3& ) const;

	void Normalize();
	float Length();

	union{
		float	 v[4];

		struct{	float x;
		float y;
		float z;
		float w;
		};

		struct{	float r;
		float g;
		float b;
		float a;
		};
	};
};

struct XMATRIX44
{
	XMATRIX44() :a1(1.0f), b1(0.0f), c1(0.0f), d1(0.0f),
		a2(0.0f), b2(1.0f), c2(0.0f), d2(0.0f),
		a3(0.0f), b3(0.0f), c3(1.0f), d3(0.0f),
		a4(0.0f), b4(0.0f), c4(0.0f), d4(1.0f) {}
	XMATRIX44(const float* m) : a1(m[0]), b1(m[1]), c1(m[2]),   d1(m[3]),
		a2(m[4]), b2(m[5]), c2(m[6]),   d2(m[7]),
		a3(m[8]), b3(m[9]), c3(m[10]),  d3(m[11]),
		a4(m[12]),b4(m[13]),c4(m[14]),  d4(m[15]) {}
	XMATRIX44(const XMATRIX44& m) : a1(m.a1), b1(m.b1), c1(m.c1), d1(m.d1),
		a2(m.a2), b2(m.b2), c2(m.c2), d2(m.d2),
		a3(m.a3), b3(m.b3), c3(m.c3), d3(m.d3),
		a4(m.a4), b4(m.b4), c4(m.c4), d4(m.d4) {}
	XMATRIX44(const float &a1, const float &a2, const float &a3, const float &a4,
		const float &b1, const float &b2, const float &b3, const float &b4,
		const float &c1, const float &c2, const float &c3, const float &c4,
		const float &d1, const float &d2, const float &d3, const float &d4)
		:   a1(a1), b1(b1), c1(c1), d1(d1),
		a2(a2), b2(b2), c2(c2), d2(d2),
		a3(a3), b3(b3), c3(c3), d3(d3),
		a4(a4), b4(b4), c4(c4), d4(d4) {}

	/*
	XMATRIX44():a1(1.0f),a2(0.0f),a3(0.0f),a4(0.0f),
				b1(0.0f),b2(1.0f),b3(0.0f),b4(0.0f),
				c1(0.0f),c2(0.0f),c3(1.0f),c4(0.0f),
				d1(0.0f),d2(0.0f),d3(0.0f),d4(1.0f){}
	XMATRIX44( const float* m ):	a1(m[0]),a2(m[1]),a3(m[2]),a4(m[3]),
									b1(m[4]),b2(m[5]),b3(m[6]),b4(m[7]),
									c1(m[8]),c2(m[9]),c3(m[10]),c4(m[11]),
									d1(m[12]),d2(m[13]),d3(m[14]),d4(m[15]){}
	XMATRIX44( const XMATRIX44& m):	a1(m.a1),a2(m.a2),a3(m.a3),a4(m.a4),
									b1(m.b1),b2(m.b2),b3(m.b3),b4(m.b4),
									c1(m.c1),c2(m.c2),c3(m.c3),c4(m.c4),
									d1(m.d1),d2(m.d2),d3(m.d3),d4(m.d4){}
	XMATRIX44(  const float &a1,const float &a2,const float &a3,const float &a4,
				const float &b1,const float &b2,const float &b3,const float &b4,
				const float &c1,const float &c2,const float &c3,const float &c4,
				const float &d1,const float &d2,const float &d3,const float &d4 )
				:	a1(a1),a2(a2),a3(a3),a4(a4),
					b1(b1),b2(b2),b3(b3),b4(b4),
					c1(c1),c2(c2),c3(c3),c4(c4),
					d1(d1),d2(d2),d3(d3),d4(d4){}
*/
	XMATRIX44 & operator= (const XMATRIX44 & other){
		this->a1 = other.a1; this->a2 = other.a2; this->a3 = other.a3; this->a4 = other.a4;
		this->b1 = other.b1; this->b2 = other.b2; this->b3 = other.b3; this->b4 = other.b4;
		this->c1 = other.c1; this->c2 = other.c2; this->c3 = other.c3; this->c4 = other.c4;
		this->d1 = other.d1; this->d2 = other.d2; this->d3 = other.d3; this->d4 = other.d4;
		return *this;
	}
	XMATRIX44 & operator= (XMATRIX44 & other){
		this->a1 = other.a1; this->a2 = other.a2; this->a3 = other.a3; this->a4 = other.a4;
		this->b1 = other.b1; this->b2 = other.b2; this->b3 = other.b3; this->b4 = other.b4;
		this->c1 = other.c1; this->c2 = other.c2; this->c3 = other.c3; this->c4 = other.c4;
		this->d1 = other.d1; this->d2 = other.d2; this->d3 = other.d3; this->d4 = other.d4;
		return *this;
	}

	XMATRIX44& operator *= ( const XMATRIX44& );
	XMATRIX44& operator += ( const XMATRIX44& );
	XMATRIX44& operator -= ( const XMATRIX44& );
	XMATRIX44& operator *= ( float );
	XMATRIX44& operator /= ( float );

	XMATRIX44 operator + () const;
	XMATRIX44 operator - () const;

	operator float*();
	operator const float* () const;

	XMATRIX44 operator * ( const XMATRIX44& ) const;
	XMATRIX44 operator + ( const XMATRIX44& ) const;
	XMATRIX44 operator - ( const XMATRIX44& ) const;
	XMATRIX44 operator * ( float ) const;
	XMATRIX44 operator / ( float ) const;

	friend XMATRIX44 operator * ( float, const XMATRIX44& );

	bool operator == ( const XMATRIX44& ) const;
	bool operator != ( const XMATRIX44& ) const;

	union
	{
		float m[4][4];

		struct {
		/*	float        a1, a2, a3, a4;
			float        b1, b2, b3, b4;
			float        c1, c2, c3, c4;
			float        d1, d2, d3, d4;*/
			float        a1, b1, c1, d1;
			float        a2, b2, c2, d2;
			float        a3, b3, c3, d3;
			float        a4, b4, c4, d4;
		};

		float mat[16];
	};

	void Identity(){
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

	void Zero(){
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

struct XQUATERNION{
	XQUATERNION():x(0.0f),y(0.0f),z(0.0f),w(0.0f){}
	XQUATERNION( const float *vf ):x(vf[0]),y(vf[1]),z(vf[2]),w(vf[3]){}
	XQUATERNION( const XVECTOR3& v):x(v.x),y(v.y),z(v.z),w(v.w){}
	XQUATERNION( const XQUATERNION& v):x(v.x),y(v.y),z(v.z),w(v.w){}
	XQUATERNION( const float &xp,const float &yp,const float &zp, const float &wp ):x(xp),y(yp),z(zp),w(wp){}

	/*XVECTOR3 & operator= (const XVECTOR3 & other){
		this->x = other.x; this->y = other.y; this->z = other.z; this->w = other.w;
		return *this;
	}
	XVECTOR3 & operator= (XVECTOR3 & other){
		this->x = other.x; this->y = other.y; this->z = other.z; this->w = other.w;
		return *this;
	}

	operator float* ();
	operator const float* () const;

	XVECTOR3& operator += ( const XVECTOR3& );
	XVECTOR3& operator -= ( const XVECTOR3& );
	XVECTOR3& operator *= ( float );
	XVECTOR3& operator /= ( float );

	XVECTOR3 operator + () const;
	XVECTOR3 operator - () const;

	XVECTOR3 operator + ( const XVECTOR3& ) const;
	XVECTOR3 operator - ( const XVECTOR3& ) const;
	XVECTOR3 operator * ( float ) const;
	XVECTOR3 operator / ( float ) const;

	float	   operator * ( const XVECTOR3& );

	friend XVECTOR3 operator * ( float, const struct XVECTOR3& );

	bool operator == ( const XVECTOR3& ) const;
	bool operator != ( const XVECTOR3& ) const;

	void Normalize();
	float Length();
*/
	union{
		float	 v[4];

		struct{	
		float w;
		float x;
		float y;
		float z;
		
		};
	};
};
#include <stdio.h>
inline void XMatMultiply(XMATRIX44 &mpout, const XMATRIX44 &mp1, const XMATRIX44 &mp2){
    float a00 = mp1.m[0][0], a01 = mp1.m[0][1], a02 = mp1.m[0][2], a03 = mp1.m[0][3],
	a10 = mp1.m[1][0], a11 = mp1.m[1][1], a12 = mp1.m[1][2], a13 = mp1.m[1][3],
	a20 = mp1.m[2][0], a21 = mp1.m[2][1], a22 = mp1.m[2][2], a23 = mp1.m[2][3],
	a30 = mp1.m[3][0], a31 = mp1.m[3][1], a32 = mp1.m[3][2], a33 = mp1.m[3][3];

	float b00 = mp2.m[0][0], b01 = mp2.m[0][1], b02 = mp2.m[0][2], b03 = mp2.m[0][3],
	b10 = mp2.m[1][0], b11 = mp2.m[1][1], b12 = mp2.m[1][2], b13 = mp2.m[1][3],
	b20 = mp2.m[2][0], b21 = mp2.m[2][1], b22 = mp2.m[2][2], b23 = mp2.m[2][3],
	b30 = mp2.m[3][0], b31 = mp2.m[3][1], b32 = mp2.m[3][2], b33 = mp2.m[3][3];

	mpout.m[0][0]  = a00*b00 + a01*b10 + a02*b20 + a03*b30;
	mpout.m[0][1]  = a00*b01 + a01*b11 + a02*b21 + a03*b31;
	mpout.m[0][2]  = a00*b02 + a01*b12 + a02*b22 + a03*b32;
	mpout.m[0][3]  = a00*b03 + a01*b13 + a02*b23 + a03*b33;
	mpout.m[1][0]  = a10*b00 + a11*b10 + a12*b20 + a13*b30;
	mpout.m[1][1]  = a10*b01 + a11*b11 + a12*b21 + a13*b31;
	mpout.m[1][2]  = a10*b02 + a11*b12 + a12*b22 + a13*b32;
	mpout.m[1][3]  = a10*b03 + a11*b13 + a12*b23 + a13*b33;
	mpout.m[2][0]  = a20*b00 + a21*b10 + a22*b20 + a23*b30;
	mpout.m[2][1]  = a20*b01 + a21*b11 + a22*b21 + a23*b31;
	mpout.m[2][2]  = a20*b02 + a21*b12 + a22*b22 + a23*b32;
	mpout.m[2][3]  = a20*b03 + a21*b13 + a22*b23 + a23*b33;
	mpout.m[3][0]  = a30*b00 + a31*b10 + a32*b20 + a33*b30;
	mpout.m[3][1]  = a30*b01 + a31*b11 + a32*b21 + a33*b31;
	mpout.m[3][2]  = a30*b02 + a31*b12 + a32*b22 + a33*b32;
	mpout.m[3][3]  = a30*b03 + a31*b13 + a32*b23 + a33*b33;
/*
	float a00 = mp1.m[0][0], a01 = mp1.m[0][1], a02 = mp1.m[0][2], a03 = mp1.m[0][3],
		a10 = mp1.m[1][0], a11 = mp1.m[1][1], a12 = mp1.m[1][2], a13 = mp1.m[1][3],
		a20 = mp1.m[2][0], a21 = mp1.m[2][1], a22 = mp1.m[2][2], a23 = mp1.m[2][3],
		a30 = mp1.m[3][0], a31 = mp1.m[3][1], a32 = mp1.m[3][2], a33 = mp1.m[3][3];

	float b00 = mp2.m[0][0], b01 = mp2.m[0][1], b02 = mp2.m[0][2], b03 = mp2.m[0][3],
		b10 = mp2.m[1][0], b11 = mp2.m[1][1], b12 = mp2.m[1][2], b13 = mp2.m[1][3],
		b20 = mp2.m[2][0], b21 = mp2.m[2][1], b22 = mp2.m[2][2], b23 = mp2.m[2][3],
		b30 = mp2.m[3][0], b31 = mp2.m[3][1], b32 = mp2.m[3][2], b33 = mp2.m[3][3];

	mpout.m[0][0]  = -1.0f*(a00*b02 + a01*b12 + a02*b22 + a03*b32);
	mpout.m[0][1]  = a00*b01 + a01*b11 + a02*b21 + a03*b31;
	mpout.m[0][2]  = a00*b00 + a01*b10 + a02*b20 + a03*b30;
	mpout.m[0][3]  = a00*b03 + a01*b13 + a02*b23 + a03*b33;

	mpout.m[1][0]  = -1.0f*(a10*b02 + a11*b12 + a12*b22 + a13*b32);
	mpout.m[1][1]  = a10*b01 + a11*b11 + a12*b21 + a13*b31;
	mpout.m[1][2]  = a10*b00 + a11*b10 + a12*b20 + a13*b30;
	mpout.m[1][3]  = a10*b03 + a11*b13 + a12*b23 + a13*b33;

	mpout.m[2][0]  = -1.0f*( a20*b02 + a21*b12 + a22*b22 + a23*b32);
	mpout.m[2][1]  = a20*b01 + a21*b11 + a22*b21 + a23*b31;
	mpout.m[2][2]  = a20*b00 + a21*b10 + a22*b20 + a23*b30;
	mpout.m[2][3]  = a20*b03 + a21*b13 + a22*b23 + a23*b33;

	mpout.m[3][0]  = -1.0f*(a30*b02 + a31*b12 + a32*b22 + a33*b32);
	mpout.m[3][1]  = a30*b01 + a31*b11 + a32*b21 + a33*b31;
	mpout.m[3][2]  = a30*b00 + a31*b10 + a32*b20 + a33*b30;
	mpout.m[3][3]  = a30*b03 + a31*b13 + a32*b23 + a33*b33;
*/



	

}

inline void XMatTranslation(XMATRIX44 &mpout,const float &x,const float &y,const float &z){
	XMatIdentity(mpout);
//	mpout.m[0][3] = x; mpout.m[1][3] = y; mpout.m[2][3] = z; mpout.m[3][3] = 1.0f;
	mpout.m[3][0] = x; mpout.m[3][1] = y; mpout.m[3][2] = z; mpout.m[3][3] = 1.0f;
}

inline void XMatScaling(XMATRIX44 &mpout,const float &x,const float &y,const float &z){
	mpout.m[0][0]*=x;
	mpout.m[1][1]*=y;
	mpout.m[2][2]*=z;
}

inline void XMatRotationXLH(XMATRIX44 &mpout,const float &rads){
	mpout.m[1][1] = cos(rads);
	mpout.m[1][2] = -sin(rads);
	mpout.m[2][1] = sin(rads);
	mpout.m[2][2] = cos(rads);
}

inline void XMatRotationXRH(XMATRIX44 &mpout,const float &rads){
	mpout.m[1][1] = cos(rads);
	mpout.m[1][2] = sin(rads);
	mpout.m[2][1] = -sin(rads);
	mpout.m[2][2] = cos(rads);
}

inline void XMatRotationYLH(XMATRIX44 &mpout,const float &rads){
	mpout.m[0][0] = cos(rads);
	mpout.m[0][2] = sin(rads);
	mpout.m[2][0] = -sin(rads);
	mpout.m[2][2] = cos(rads);
}

inline void XMatRotationYRH(XMATRIX44 &mpout,const float &rads){
	mpout.m[0][0] = cos(rads);
	mpout.m[0][2] = -sin(rads);
	mpout.m[2][0] = sin(rads);
	mpout.m[2][2] = cos(rads);
}

inline void XMatRotationZLH(XMATRIX44 &mpout,const float &rads){
	mpout.m[0][0] = cos(rads);
	mpout.m[0][1] = -sin(rads);
	mpout.m[1][0] = sin(rads);
	mpout.m[1][1] = cos(rads);
}

inline void XMatRotationZRH(XMATRIX44 &mpout,const float &rads){
	mpout.m[0][0] = cos(rads);
	mpout.m[0][1] = sin(rads);
	mpout.m[1][0] = -sin(rads);
	mpout.m[1][1] = cos(rads);
}


inline void XMatTranspose(XMATRIX44 &mpout,const XMATRIX44 &mat){
	for (int i = 0; i < 4 ; i++){
		for (int j = 0; j < 4 ; j++){
			mpout.m[i][j] = mat.m[j][i];
		}}
}

inline void XMatIdentity(XMATRIX44 &mat){
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

inline void XMatViewLookAtLH(XMATRIX44 &mpout,const XVECTOR3 &camPos,const XVECTOR3 &lookAt,const XVECTOR3 &upAxis){
	XVECTOR3 zaxe = lookAt;
	zaxe -= camPos;
	zaxe.Normalize();

	XVECTOR3 xaxe;
	XVecCross(xaxe,upAxis,zaxe);
	xaxe.Normalize();

	XVECTOR3 yaxe;
	XVecCross(yaxe,zaxe,xaxe);

	mpout.a1 = xaxe.x;
	mpout.b1 = xaxe.y;
	mpout.c1 = xaxe.z;
	XVecDot(mpout.d1,xaxe,camPos);
	mpout.d1*= -1.0f;

	mpout.a2 = yaxe.x;
	mpout.b2 = yaxe.y;
	mpout.c2 = yaxe.z;
	XVecDot(mpout.d2,yaxe,camPos);
	mpout.d2 *= -1.0f;

	mpout.a3 = zaxe.x;
	mpout.b3 = zaxe.y;
	mpout.c3 = zaxe.z;
	XVecDot(mpout.d3,zaxe,camPos);
	mpout.d3 *= -1.0f;

	mpout.a4 = 0.0f;
	mpout.b4 = 0.0f;
	mpout.c4 = 0.0f;
	mpout.d4 = 1.0f;
}

inline void XMatViewLookAtRH(XMATRIX44 &mpout,const XVECTOR3 &camPos,const XVECTOR3 &lookAt,const XVECTOR3 &upAxis){
	XVECTOR3 zaxe = camPos;
	zaxe-=lookAt;
	zaxe.Normalize();

	XVECTOR3 xaxe;
	XVecCross(xaxe,upAxis,zaxe);
	xaxe.Normalize();

	XVECTOR3 yaxe;
	XVecCross(yaxe,zaxe,xaxe);

	mpout.a1 = xaxe.x;
	mpout.b1 = xaxe.y;
	mpout.c1 = xaxe.z;
	XVecDot(mpout.d1,xaxe,camPos);
	mpout.d1 *= -1.0f;

	mpout.a2 = yaxe.x;
	mpout.b2 = yaxe.y;
	mpout.c2 = yaxe.z;
	XVecDot(mpout.d2,yaxe,camPos);
	mpout.d2 *= -1.0f;

	mpout.a3 = zaxe.x;
	mpout.b3 = zaxe.y;
	mpout.c3 = zaxe.z;
	XVecDot(mpout.d3,zaxe,camPos);
	mpout.d3 *= -1.0f;

	mpout.a4 = 0.0f;
	mpout.b4 = 0.0f;
	mpout.c4 = 0.0f;
	mpout.d4 = 1.0f;
}


inline void XMatPerspectiveLH(XMATRIX44 &mpout,const float &FOV,const float &Aspect,const float &NearPlane,const float &FarPlane){
	float x,y;

	y = 1.0f/tan(FOV/2.0f);
	x = y / Aspect;

	mpout.a1 = x;
	mpout.b2 = y;
	mpout.c3 = FarPlane/(FarPlane-NearPlane);
	mpout.d3 = -(NearPlane*FarPlane)/(FarPlane-NearPlane);
	mpout.c4 = 1.0f;
}

inline void XMatPerspectiveRH(XMATRIX44 &mpout,const float &FOV,const float &Aspect,const float &NearPlane,const float &FarPlane)
{
	float x,y;

	y = 1.0f/tan(FOV/2.0f);
	x = y / Aspect;

	mpout.a1 = x;
	mpout.b2 = y;
	mpout.c3 = FarPlane/(NearPlane-FarPlane);
	mpout.d3 = (NearPlane*FarPlane)/(NearPlane-FarPlane);
	mpout.c4 = -1.0f;
}

inline void XMatRotationAxisRH(XMATRIX44 &mpout,const XVECTOR3 &axis,const float &angle){
	float cosangle = cos(angle);
	float sinangle = sin(angle);
	float A = 1.0f - cosangle;

	mpout.a1 = A*axis.x*axis.x + cosangle;
	mpout.a2 = A*axis.x*axis.y + sinangle*axis.z;
	mpout.a3 = A*axis.x*axis.z - sinangle*axis.y;
	mpout.a4 = 0.0f;

	mpout.b1 = A*axis.x*axis.y - sinangle*axis.z;
	mpout.b2 = A*axis.y*axis.y + cosangle;
	mpout.b3 = A*axis.y*axis.z + sinangle*axis.x;
	mpout.b4 = 0.0f;

	mpout.c1 = A*axis.x*axis.z + sinangle*axis.y;
	mpout.c2 = A*axis.y*axis.z - sinangle*axis.x;
	mpout.c3 = A*axis.z*axis.z + cosangle;
	mpout.c4 = 0.0f;

	mpout.d1 = mpout.d2 = mpout.d3 = 0.0f; mpout.d4 = 1.0f;
}

inline void XMatRotationAxisLH(XMATRIX44 &mpout,const XVECTOR3 &axis,const float &angle){
	float cosangle = cos(angle);
	float sinangle = sin(angle);
	float A = 1.0f - cosangle;

	mpout.a1 = A*axis.x*axis.x + cosangle;
	mpout.a2 = A*axis.x*axis.y - sinangle*axis.z;
	mpout.a3 = A*axis.x*axis.z + sinangle*axis.y;
	mpout.a4 = 0.0f;

	mpout.b1 = A*axis.x*axis.y + sinangle*axis.z;
	mpout.b2 = A*axis.y*axis.y + cosangle;
	mpout.b3 = A*axis.y*axis.z - sinangle*axis.x;
	mpout.b4 = 0.0f;

	mpout.c1 = A*axis.x*axis.z - sinangle*axis.y;
	mpout.c2 = A*axis.y*axis.z + sinangle*axis.x;
	mpout.c3 = A*axis.z*axis.z + cosangle;
	mpout.c4 = 0.0f;

	mpout.d1 = mpout.d2 = mpout.d3 = 0.0f; mpout.d4 = 1.0f;
}

inline void XVecTransform(XVECTOR3 &vpout,const XVECTOR3 &v,const XMATRIX44 &mat){
/*	vpout.x = v.x*mat.a1 + v.y*mat.a2 + v.z*mat.a3 + 1.0f*mat.d1;
 	vpout.y = v.x*mat.b1 + v.y*mat.b2 + v.z*mat.b3 + 1.0f*mat.d2;
 	vpout.z = v.x*mat.c1 + v.y*mat.c2 + v.z*mat.c3 + 1.0f*mat.d3;
*/
	vpout.x = v.x*mat.m[0][0] + v.y*mat.m[1][0] + v.z*mat.m[2][0] + 1.0f*mat.m[3][0];
	vpout.y = v.x*mat.m[0][1] + v.y*mat.m[1][1] + v.z*mat.m[2][1] + 1.0f*mat.m[3][1];
	vpout.z = v.x*mat.m[0][2] + v.y*mat.m[1][2] + v.z*mat.m[2][2] + 1.0f*mat.m[3][2];
	vpout.w = v.x*mat.m[0][3] + v.y*mat.m[1][3] + v.z*mat.m[2][3] + 1.0f*mat.m[3][3];
}

inline void XVecTransformNormal(XVECTOR3 &vpout,const XVECTOR3 &v,const XMATRIX44 &mat){
/*	vpout.x = v.x*mat.a1 + v.y*mat.a2 + v.z*mat.a3;
	vpout.y = v.x*mat.b1 + v.y*mat.b2 + v.z*mat.b3;
	vpout.z = v.x*mat.c1 + v.y*mat.c2 + v.z*mat.c3;*/

	vpout.x = v.x*mat.m[0][0] + v.y*mat.m[1][0] + v.z*mat.m[2][0];
	vpout.y = v.x*mat.m[0][1] + v.y*mat.m[1][1] + v.z*mat.m[2][1];
	vpout.z = v.x*mat.m[0][2] + v.y*mat.m[1][2] + v.z*mat.m[2][2];
	vpout.w = 1.0F;
}

inline void XVecDot(float &f,const XVECTOR3 &v1,const XVECTOR3 &v2){
	f = v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

inline void XVecCross(XVECTOR3 &vpout,const XVECTOR3 &lv,const XVECTOR3 &rv){
    vpout.x = lv.y*rv.z - lv.z*rv.y;
    vpout.y = lv.z*rv.x - lv.x*rv.z;
    vpout.z = lv.x*rv.y - lv.y*rv.x;
}

inline void XVec3Normalize( XVECTOR3 &pOut, const XVECTOR3 &pV ){
	float v = 1.0f / sqrtf(pV.x * pV.x + pV.y * pV.y + pV.z * pV.z);
	pOut.x = pV.x * v;
	pOut.y = pV.y * v;
	pOut.z = pV.z * v;
}

inline XVECTOR2& XVECTOR2::operator += ( const XVECTOR2& v)
{
	x+=v.x;
	y+=v.y;
	return *this;
}

inline XVECTOR2& XVECTOR2::operator -= ( const XVECTOR2& v)
{
	x-=v.x;
	y-=v.y;
	return *this;
}

inline XVECTOR2& XVECTOR2::operator *= ( float f )
{
	x*=f;
	y*=f;
	return *this;
}

inline XVECTOR2& XVECTOR2::operator /= ( float f)
{
	x/=f;
	y/=f;
	return *this;
}

inline XVECTOR2 XVECTOR2::operator + () const
{
	return *this;
}

inline XVECTOR2 XVECTOR2::operator - () const
{
	return XVECTOR2(-x,-y);
}

inline XVECTOR2 XVECTOR2::operator + ( const XVECTOR2& v) const
{
	return XVECTOR2(x+v.x,y+v.y);
}

inline XVECTOR2 XVECTOR2::operator - ( const XVECTOR2& v) const
{
	return XVECTOR2(x-v.x,y-v.y);
}

inline XVECTOR2 XVECTOR2::operator * ( float f ) const
{
	return XVECTOR2(x*f,y*f);
}

inline XVECTOR2 XVECTOR2::operator / ( float f) const
{
	return XVECTOR2(x/f,y/f);
}

inline bool XVECTOR2::operator == ( const XVECTOR2& v) const
{
	return x == v.x && y == v.y;
}

inline bool XVECTOR2::operator != ( const XVECTOR2& v) const
{
	return x != v.x || y != v.y;
}

inline void XVECTOR2::Normalize()
{
	float mod = sqrt(x*x + y*y);
	x/=mod;
	y/=mod;
}

inline XVECTOR2::operator float* ()
{
	return (float *) &x;
}

inline XVECTOR2::operator const float* () const
{
	return (const float *) &x;
}

inline float XVECTOR2::Length()
{
	return sqrt(x*x + y*y);
}

inline XVECTOR3::operator float* ()
{
	return (float *) &x;
}

inline XVECTOR3::operator const float* () const
{
	return (const float *) &x;
}

inline XVECTOR3& XVECTOR3::operator += ( const XVECTOR3& v)
{
	x+=v.x;
	y+=v.y;
	z+=v.z;
	return *this;
}

inline XVECTOR3& XVECTOR3::operator -= ( const XVECTOR3& v)
{
	x-=v.x;
	y-=v.y;
	z-=v.z;
	return *this;
}

inline XVECTOR3& XVECTOR3::operator *= ( float f )
{
	x*=f;
	y*=f;
	z*=f;
	return *this;
}

inline XVECTOR3& XVECTOR3::operator /= ( float f)
{
	x/=f;
	y/=f;
	z/=f;
	return *this;
}


inline XVECTOR3 XVECTOR3::operator + () const
{
	return *this;
}

inline XVECTOR3 XVECTOR3::operator - () const
{
	return	XVECTOR3(-x,-y,-z);
}

inline XVECTOR3 operator * ( float f, const struct XVECTOR3& v )
{
	return XVECTOR3(f * v.x, f * v.y, f * v.z);
}

inline float XVECTOR3::operator * ( const XVECTOR3& v )
{
	return ((this->x*v.x) + (this->y*v.y) + (this->z*v.z));
}

inline XVECTOR3 XVECTOR3::operator + ( const XVECTOR3& v ) const
{
	return XVECTOR3(x+v.x,y+v.y,z+v.z);
}

inline XVECTOR3 XVECTOR3::operator - ( const XVECTOR3& v ) const
{
	return XVECTOR3(x-v.x,y-v.y,z-v.z);
}

inline XVECTOR3 XVECTOR3::operator * ( float f) const
{
	return XVECTOR3(x*f,y*f,z*f);
}

inline XVECTOR3 XVECTOR3::operator / ( float f ) const
{
	return XVECTOR3(x/f,y/f,z/f);
}

inline bool XVECTOR3::operator == ( const XVECTOR3& v) const
{
	return (x == v.x && y == v.y && z == v.z);
}

inline bool XVECTOR3::operator != ( const XVECTOR3& v) const
{
	return (x != v.x || y != v.y || z != v.z);
}

inline void XVECTOR3::Normalize()
{
	float mod = sqrt(x*x + y*y + z*z);
	x/=mod;
	y/=mod;
	z/=mod;
}

inline float XVECTOR3::Length()
{
	return sqrt(x*x + y*y + z*z);
}

inline XMATRIX44& XMATRIX44::operator *= ( const XMATRIX44& mb)
{
	float a00 = this->m[0][0], a01 = this->m[0][1], a02 = this->m[0][2], a03 = this->m[0][3],
		a10 = this->m[1][0], a11 = this->m[1][1], a12 = this->m[1][2], a13 = this->m[1][3],
		a20 = this->m[2][0], a21 = this->m[2][1], a22 = this->m[2][2], a23 = this->m[2][3],
		a30 = this->m[3][0], a31 = this->m[3][1], a32 = this->m[3][2], a33 = this->m[3][3];

	float b00 = mb.m[0][0], b01 = mb.m[0][1], b02 = mb.m[0][2], b03 = mb.m[0][3],
		b10 = mb.m[1][0], b11 = mb.m[1][1], b12 = mb.m[1][2], b13 = mb.m[1][3],
		b20 = mb.m[2][0], b21 = mb.m[2][1], b22 = mb.m[2][2], b23 = mb.m[2][3],
		b30 = mb.m[3][0], b31 = mb.m[3][1], b32 = mb.m[3][2], b33 = mb.m[3][3];

	this->m[0][0]  = a00*b00 + a01*b10 + a02*b20 + a03*b30;
	this->m[0][1]  = a00*b01 + a01*b11 + a02*b21 + a03*b31;
	this->m[0][2]  = a00*b02 + a01*b12 + a02*b22 + a03*b32;
	this->m[0][3]  = a00*b03 + a01*b13 + a02*b23 + a03*b33;
	this->m[1][0]  = a10*b00 + a11*b10 + a12*b20 + a13*b30;
	this->m[1][1]  = a10*b01 + a11*b11 + a12*b21 + a13*b31;
	this->m[1][2]  = a10*b02 + a11*b12 + a12*b22 + a13*b32;
	this->m[1][3]  = a10*b03 + a11*b13 + a12*b23 + a13*b33;
	this->m[2][0]  = a20*b00 + a21*b10 + a22*b20 + a23*b30;
	this->m[2][1]  = a20*b01 + a21*b11 + a22*b21 + a23*b31;
	this->m[2][2]  = a20*b02 + a21*b12 + a22*b22 + a23*b32;
	this->m[2][3]  = a20*b03 + a21*b13 + a22*b23 + a23*b33;
	this->m[3][0]  = a30*b00 + a31*b10 + a32*b20 + a33*b30;
	this->m[3][1]  = a30*b01 + a31*b11 + a32*b21 + a33*b31;
	this->m[3][2]  = a30*b02 + a31*b12 + a32*b22 + a33*b32;
	this->m[3][3]  = a30*b03 + a31*b13 + a32*b23 + a33*b33;
	return *this;
}

inline XMATRIX44& XMATRIX44::operator += ( const XMATRIX44& mb)
{
	a1 += mb.a1; a2 += mb.a2; a3 += mb.a3; a4 += mb.a4;
	b1 += mb.b1; b2 += mb.b2; b3 += mb.b3; b4 += mb.b4;
	c1 += mb.c1; c2 += mb.c2; c3 += mb.c3; c4 += mb.c4;
	d1 += mb.d1; d2 += mb.d2; d3 += mb.d3; d4 += mb.d4;
	return *this;
}

inline XMATRIX44& XMATRIX44::operator -= ( const XMATRIX44& mb)
{
	a1 -= mb.a1; a2 -= mb.a2; a3 -= mb.a3; a4 -= mb.a4;
	b1 -= mb.b1; b2 -= mb.b2; b3 -= mb.b3; b4 -= mb.b4;
	c1 -= mb.c1; c2 -= mb.c2; c3 -= mb.c3; c4 -= mb.c4;
	d1 -= mb.d1; d2 -= mb.d2; d3 -= mb.d3; d4 -= mb.d4;
	return *this;
}

inline XMATRIX44::operator float* ()
{
	return (float *) &a1;
}


inline XMATRIX44::operator const float* () const
{
	return (const float *) &a1;
}

inline XMATRIX44& XMATRIX44::operator *= ( float f)
{
	a1 *= f; a2 *= f; a3 *= f; a4 *= f;
	b1 *= f; b2 *= f; b3 *= f; b4 *= f;
	c1 *= f; c2 *= f; c3 *= f; c4 *= f;
	d1 *= f; d2 *= f; d3 *= f; d4 *= f;
	return *this;
}

inline XMATRIX44& XMATRIX44::operator /= ( float f)
{
	a1 /= f; a2 /= f; a3 /= f; a4 /= f;
	b1 /= f; b2 /= f; b3 /= f; b4 /= f;
	c1 /= f; c2 /= f; c3 /= f; c4 /= f;
	d1 /= f; d2 /= f; d3 /= f; d4 /= f;
	return *this;
}

inline XMATRIX44 XMATRIX44::operator + () const
{
	return *this;
}

inline XMATRIX44 XMATRIX44::operator - () const
{
	return XMATRIX44( -a1,-a2,-a3,-a4,
		-b1,-b2,-b3,-b4,
		-c1,-c2,-c3,-c4,
		-d1,-d2,-d3,-d4 );
}

inline XMATRIX44 XMATRIX44::operator * ( const XMATRIX44& mb) const
{
	XMATRIX44	tmp;
	XMatMultiply(tmp,*this,mb);
	return tmp;
}

inline XMATRIX44 XMATRIX44::operator + ( const XMATRIX44& mat) const
{
	return XMATRIX44( a1 + mat.a1,a2 + mat.a2,a3 + mat.a3,a4 + mat.a4,
		b1 + mat.b1,b2 + mat.b2,b3 + mat.b3,b4 + mat.b4,
		c1 + mat.c1,c2 + mat.c2,c3 + mat.c3,c4 + mat.c4,
		d1 + mat.d1,d2 + mat.d2,d3 + mat.d3,d4 + mat.d4 );
}

inline XMATRIX44 XMATRIX44::operator - ( const XMATRIX44& mat) const
{
	return XMATRIX44( a1 - mat.a1,a2 - mat.a2,a3 - mat.a3,a4 - mat.a4,
		b1 - mat.b1,b2 - mat.b2,b3 - mat.b3,b4 - mat.b4,
		c1 - mat.c1,c2 - mat.c2,c3 - mat.c3,c4 - mat.c4,
		d1 - mat.d1,d2 - mat.d2,d3 - mat.d3,d4 - mat.d4 );
}

inline XMATRIX44 operator * ( float f, const XMATRIX44& mat )
{
	return XMATRIX44(f * mat.a1, f * mat.a2, f * mat.a3, f * mat.a4,
					f * mat.b1, f * mat.b2, f * mat.b3, f * mat.b4,
					f * mat.c1, f * mat.c2, f * mat.c3, f * mat.c4,
					f * mat.d1, f * mat.d2, f * mat.d3, f * mat.d4);
}

inline XMATRIX44 XMATRIX44::operator * ( float f ) const
{
	return XMATRIX44( a1*f,a2*f,a3*f,a4*f,
		b1*f,b2*f,b3*f,b4*f,
		c1*f,c2*f,c3*f,c4*f,
		d1*f,d2*f,d3*f,d4*f );
}

inline XMATRIX44 XMATRIX44::operator / ( float f ) const
{
	return XMATRIX44( a1/f,a2/f,a3/f,a4/f,
		b1/f,b2/f,b3/f,b4/f,
		c1/f,c2/f,c3/f,c4/f,
		d1/f,d2/f,d3/f,d4/f );
}

inline bool XMATRIX44::operator == ( const XMATRIX44& mat ) const
{
	return 0 == memcmp(this, &mat, sizeof(XMATRIX44));
}
inline bool XMATRIX44::operator != ( const XMATRIX44& mat) const
{
	return 0 != memcmp(this, &mat, sizeof(XMATRIX44));
}



#endif


