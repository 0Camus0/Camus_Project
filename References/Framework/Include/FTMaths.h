/*=============================================================================
FTMaths.h 	: Math classes and global functions
Revision history:
* Created by Camus
=============================================================================*/

#ifndef FTMATHS_H
#define FTMATHS_H

#include <math.h>
#include <string.h>


#define STDNULL	0
#define STD_PI  3.141592654f
#define SMALLNUMB 0.000000001f
#define Rad2Deg( r ) ((r) * (180.0f / STD_PI))
#define Deg2Rad( d ) ((d) * (STD_PI / 180.0f))



typedef struct _stdFloat16{

	_stdFloat16(){ val = 0; }
	_stdFloat16(float f);
	_stdFloat16(unsigned short v);

 	operator float () const;

	_stdFloat16& operator = (unsigned short f);
 	_stdFloat16& operator = (float f);
 	_stdFloat16& operator = (_stdFloat16 f16);
 
 	_stdFloat16& operator - ();
 
 	_stdFloat16& operator += (float f);
 	_stdFloat16& operator -= (float f);
 	_stdFloat16& operator *= (float f);
 	_stdFloat16& operator /= (float f);

  	_stdFloat16& operator += (_stdFloat16 f16);
  	_stdFloat16& operator -= (_stdFloat16 f16);
  	_stdFloat16& operator *= (_stdFloat16 f16);
  	_stdFloat16& operator /= (_stdFloat16 f16);
// 
 	_stdFloat16 operator + (float f);
 	_stdFloat16 operator - (float f);
 	_stdFloat16 operator * (float f);
 	_stdFloat16 operator / (float f);

 	_stdFloat16 operator + (_stdFloat16 f16);
 	_stdFloat16 operator - (_stdFloat16 f16);
 	_stdFloat16 operator * (_stdFloat16 f16);
 	_stdFloat16 operator / (_stdFloat16 f16);

	unsigned short val;
}float16_legacy;

typedef unsigned int	std_uint;

typedef struct _STDMat
{
	_STDMat(){
		for (int i = 0; i < 16 ; i++)
			mat[i] = 0.0f;
	}

	union
	{
		float m[4][4];

		struct {
			float        a1, a2, a3, a4;
			float        b1, b2, b3, b4;
			float        c1, c2, c3, c4;
			float        d1, d2, d3, d4;
		};

		float mat[16];
	};
} STDMat;

typedef struct _STDVector
{
	union{
		float	 v[3];
			
		struct{	float x;
				float y;
				float z;
		};
	};
} STDVector;

//Vector 2D
typedef struct _STDVector2
{
	union{
		float	 v[2];

		struct{	float x;
		float y;
		};
	};
} STDVector2;


struct STDVECTOR2 : public STDVector2
{
	STDVECTOR2();
	STDVECTOR2( const float *vf );
	STDVECTOR2( const STDVector& v);
	STDVECTOR2( float x, float y );

	operator float* ();
	operator const float* () const;

	STDVECTOR2& operator += ( const STDVECTOR2& );
	STDVECTOR2& operator -= ( const STDVECTOR2& );
	STDVECTOR2& operator *= ( float );
	STDVECTOR2& operator /= ( float );

	STDVECTOR2 operator + () const;
	STDVECTOR2 operator - () const;

	STDVECTOR2 operator + ( const STDVECTOR2& ) const;
	STDVECTOR2 operator - ( const STDVECTOR2& ) const;
	STDVECTOR2 operator * ( float ) const;
	STDVECTOR2 operator / ( float ) const;

	bool operator == ( const STDVECTOR2& ) const;
	bool operator != ( const STDVECTOR2& ) const;

	void Normalize();
	float Length();

};

//Vector 3D

struct STDVECTOR3 : public STDVector
{
	STDVECTOR3();
	STDVECTOR3( const float *vf );
	STDVECTOR3( const STDVector& v);
	STDVECTOR3( float x, float y, float z );

	operator float* ();
	operator const float* () const;

	STDVECTOR3& operator += ( const STDVECTOR3& );
	STDVECTOR3& operator -= ( const STDVECTOR3& );
	STDVECTOR3& operator *= ( float );
	STDVECTOR3& operator /= ( float );

	STDVECTOR3 operator + () const;
	STDVECTOR3 operator - () const;

	STDVECTOR3 operator + ( const STDVECTOR3& ) const;
	STDVECTOR3 operator - ( const STDVECTOR3& ) const;
	STDVECTOR3 operator * ( float ) const;
	STDVECTOR3 operator / ( float ) const;

	STDVECTOR3	   operator * ( const STDVECTOR3& );

	friend STDVECTOR3 operator * ( float, const struct STDVECTOR3& );

	bool operator == ( const STDVECTOR3& ) const;
	bool operator != ( const STDVECTOR3& ) const;

	void Normalize();
	float Length();

};

struct STDMATRIX	: public STDMat
{
	STDMATRIX();
	STDMATRIX( const float* );
	STDMATRIX( const STDMATRIX& );
	STDMATRIX(   float a1,float a2,float a3,float a4,
		float b1,float b2,float b3,float b4,
		float c1,float c2,float c3,float c4,
		float d1,float d2,float d3,float d4 );

	STDMATRIX& operator *= ( const STDMATRIX& );
	STDMATRIX& operator += ( const STDMATRIX& );
	STDMATRIX& operator -= ( const STDMATRIX& );
	STDMATRIX& operator *= ( float );
	STDMATRIX& operator /= ( float );

	STDMATRIX operator + () const;
	STDMATRIX operator - () const;

	operator float*();
	operator const float* () const;

	STDMATRIX operator * ( const STDMATRIX& ) const;
	STDMATRIX operator + ( const STDMATRIX& ) const;
	STDMATRIX operator - ( const STDMATRIX& ) const;
	STDMATRIX operator * ( float ) const;
	STDMATRIX operator / ( float ) const;

	friend STDMATRIX operator * ( float, const STDMATRIX& );

	bool operator == ( const STDMATRIX& ) const;
	bool operator != ( const STDMATRIX& ) const;
};


STDMATRIX MatMultiply(STDMATRIX *mpout, const STDMATRIX *mp1, const STDMATRIX *mp2);
STDMATRIX MatTranslation(STDMATRIX *mpout,float x,float y,float z);
STDMATRIX MatScaling(STDMATRIX *mpout,float x,float y,float z);
STDMATRIX MatRotationX(STDMATRIX *mpout,float rads);
STDMATRIX MatRotationY(STDMATRIX *mpout,float rads);
STDMATRIX MatRotationZ(STDMATRIX *mpout,float rads);
STDMATRIX MatTranspose(STDMATRIX *mpout,const STDMATRIX *mat);
STDMATRIX MatIdentity(STDMATRIX *mpout);
STDMATRIX MatViewLookAtLH(STDMATRIX *mpout,const STDVECTOR3 &camPos,const STDVECTOR3 &lookAt,const STDVECTOR3 &upAxis);
STDMATRIX MatViewLookAtRH(STDMATRIX *mpout,const STDVECTOR3 &camPos,const STDVECTOR3 &lookAt,const STDVECTOR3 &upAxis);
STDMATRIX MatPerspectiveLH(STDMATRIX *mpout,float FOV,float Aspect,float NearPlae,float FarPlane);
STDMATRIX MatPerspectiveRH(STDMATRIX *mpout,float FOV,float Aspect,float NearPlae,float FarPlane);
STDMATRIX MatRotationAxis(STDMATRIX *mpout,const STDVECTOR3 &axis,float angle);

float       VecDot(const STDVECTOR3 &v1,const STDVECTOR3 &v2);
float	    VecLength(const STDVECTOR3 &v);
STDVECTOR3 Vec3Normalize( STDVECTOR3 *pOut, const STDVECTOR3 &pV );
STDVECTOR3 VecCross(STDVECTOR3 *vpout,const STDVECTOR3 &lv,const STDVECTOR3 &rv);
STDVECTOR3 VecTransform(STDVECTOR3 *vpout,const STDVECTOR3 &v,const STDMATRIX &mat);
STDVECTOR3 VecTransformNormal(STDVECTOR3 *vpout,const STDVECTOR3 &v,const STDMATRIX &mat);

#endif


