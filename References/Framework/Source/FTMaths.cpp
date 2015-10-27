#include "../Include/FTMaths.h"
 
 _stdFloat16::_stdFloat16(unsigned short v){
	val = v;
 }

 _stdFloat16::_stdFloat16(float f){

	 unsigned int x = *reinterpret_cast<unsigned int*>(&f);

	 register int e = (x >> 23) & 0x000000ff;
	 register int s = (x >> 16) & 0x00008000;
	 register int m =  x & 0x007fffff;

	 e = e - 127;
	 if (e == 128) {
		 val = s | (31 << 10) | (m >> 13);
	 } else if (e > 15) {
		 val = s | (31 << 10);
	 } else if (e > -15) {
		 val = s | ((e+15) << 10) | ((m + 0x00001000) >> 13);
	 } else if (e > -25) {
		 m |= 0x00800000;
		 e = -14 - e; 
		 m >>= e; 
		 m += (1 << 12);
		 m >>= 13;
		 val = s | m;
	 } else {
		 val = s;
	 }
 }

 _stdFloat16::operator float () const{

	 register int s =  val & 0x8000;
	 register int e = (val & 0x7c00) >> 10;
	 register int m =  val & 0x03ff;
	 
	 unsigned int x;
		
	 s <<= 16;
	 if (e == 31) {
		 e = 255 << 23;
		 m <<= 13;
		 x = s | e | m;
	 } else if (e > 0) {
		 e = e + (127 - 15);
		 e <<= 23;
		 m <<= 13;
		 x = s | e | m;
	 } else if (m == 0) {
		 x = s;
	 } else {
		 x = m;
		 x = s | (x - (24 << 23));
	 }

	 return *reinterpret_cast<float*>(&x);
 }

 _stdFloat16& _stdFloat16::operator = (float f){
	*this = _stdFloat16(f); return *this;
 }

 _stdFloat16& _stdFloat16::operator = (_stdFloat16 f16){
	val = f16.val;  return *this;
 } 

 _stdFloat16& _stdFloat16::operator - (){
	  _stdFloat16 f; f.val = val ^ 0x8000; return f;
 }

 _stdFloat16& _stdFloat16::operator += (float f){
	*this = _stdFloat16(float(*this) + f); return *this;
 }

 _stdFloat16& _stdFloat16::operator -= (float f){
	*this = _stdFloat16(float(*this) - f); return *this;
 }

 _stdFloat16& _stdFloat16::operator *= (float f){
	*this = _stdFloat16(float(*this) * f); return *this; 
 }

 _stdFloat16& _stdFloat16::operator /= (float f){
	*this = _stdFloat16(float(*this) / f); return *this;
 } 

 _stdFloat16& _stdFloat16::operator += (_stdFloat16 f16){
    *this = _stdFloat16(float(*this) + float(f16)); return *this;
 }

 _stdFloat16& _stdFloat16::operator -= (_stdFloat16 f16){
    *this = _stdFloat16(float(*this) - float(f16)); return *this;
 }

 _stdFloat16& _stdFloat16::operator *= (_stdFloat16 f16){
	*this = _stdFloat16(float(*this) * float(f16)); return *this;
 }

 _stdFloat16& _stdFloat16::operator /= (_stdFloat16 f16){
	*this = _stdFloat16(float(*this) / float(f16)); return *this;
 }

 _stdFloat16 _stdFloat16::operator + (float f){
	return _stdFloat16(float(*this) + f);  
 }

 _stdFloat16 _stdFloat16::operator - (float f){
	return _stdFloat16(float(*this) - f);  
  }

 _stdFloat16 _stdFloat16::operator * (float f){ 
	return _stdFloat16(float(*this) * f);  
 }

 _stdFloat16 _stdFloat16::operator / (float f){
	return _stdFloat16(float(*this) / f);  
 } 

 _stdFloat16 _stdFloat16::operator + (_stdFloat16 f16){
	return  _stdFloat16(float(*this) + float(f16));
 }

 _stdFloat16 _stdFloat16::operator - (_stdFloat16 f16){
	return  _stdFloat16(float(*this) - float(f16));
 }

 _stdFloat16 _stdFloat16::operator * (_stdFloat16 f16){
	return  _stdFloat16(float(*this) * float(f16));
 }

 _stdFloat16 _stdFloat16::operator / (_stdFloat16 f16){
	return  _stdFloat16(float(*this) / float(f16));
 }
STDVECTOR2::STDVECTOR2()
{
	x = y = 0.0f;
}

STDVECTOR2::STDVECTOR2( const float *vf )
{
	x = vf[0]; y = vf[1]; 
}

STDVECTOR2::STDVECTOR2( const STDVector& v)
{
	x = v.x; y = v.y; 
}

STDVECTOR2::STDVECTOR2( float vx, float vy )
{
	x =vx; y = vy;
}

STDVECTOR2& STDVECTOR2::operator += ( const STDVECTOR2& v)
{
	x+=v.x;
	y+=v.y;
	return *this;
}

STDVECTOR2& STDVECTOR2::operator -= ( const STDVECTOR2& v)
{
	x-=v.x;
	y-=v.y;
	return *this;
}

STDVECTOR2& STDVECTOR2::operator *= ( float f )
{
	x*=f;
	y*=f;
	return *this;
}

STDVECTOR2& STDVECTOR2::operator /= ( float f)
{
	x/=f;
	y/=f;
	return *this;
}

STDVECTOR2 STDVECTOR2::operator + () const
{
	return *this;
}

STDVECTOR2 STDVECTOR2::operator - () const
{
	return STDVECTOR2(-x,-y);
}

STDVECTOR2 STDVECTOR2::operator + ( const STDVECTOR2& v) const
{
	return STDVECTOR2(x+v.x,y+v.y);
}

STDVECTOR2 STDVECTOR2::operator - ( const STDVECTOR2& v) const
{
	return STDVECTOR2(x-v.x,y-v.y);
}

STDVECTOR2 STDVECTOR2::operator * ( float f ) const
{
	return STDVECTOR2(x*f,y*f);
}

STDVECTOR2 STDVECTOR2::operator / ( float f) const
{
	return STDVECTOR2(x/f,y/f);
}

bool STDVECTOR2::operator == ( const STDVECTOR2& v) const
{
	return x == v.x && y == v.y;
}

bool STDVECTOR2::operator != ( const STDVECTOR2& v) const
{
	return x != v.x || y != v.y;
}

void STDVECTOR2::Normalize()
{
	float mod = sqrt(x*x + y*y);
	x/=mod;
	y/=mod;
}

STDVECTOR2::operator float* ()
{
	return (float *) &x;
}

STDVECTOR2::operator const float* () const
{
	return (const float *) &x;
}

float STDVECTOR2::Length()
{
	return sqrt(x*x + y*y);
}

STDVECTOR3::STDVECTOR3()
{
	x = y = z = 0.0f;
}

STDVECTOR3::STDVECTOR3( const float *vf )
{
	x = vf[0]; y = vf[1]; z = vf[2];
}

STDVECTOR3::STDVECTOR3( const STDVector& v )
{
	x = v.x; y = v.y; z = v.z;
}

STDVECTOR3::STDVECTOR3( float vx, float vy, float vz )
{
	x =vx; y = vy; z = vz;
}

STDVECTOR3::operator float* ()
{
	return (float *) &x;
}

STDVECTOR3::operator const float* () const
{
	return (const float *) &x;
}

STDVECTOR3& STDVECTOR3::operator += ( const STDVECTOR3& v)
{
	x+=v.x;
	y+=v.y;
	z+=v.z;
	return *this;
}

STDVECTOR3& STDVECTOR3::operator -= ( const STDVECTOR3& v)
{
	x-=v.x;
	y-=v.y;
	z-=v.z;
	return *this;
}

STDVECTOR3& STDVECTOR3::operator *= ( float f )
{
	x*=f;
	y*=f;
	z*=f;
	return *this;
}

STDVECTOR3& STDVECTOR3::operator /= ( float f)
{
	x/=f;
	y/=f;
	z/=f;
	return *this;
}


STDVECTOR3 STDVECTOR3::operator + () const
{
	return *this;
}

STDVECTOR3 STDVECTOR3::operator - () const
{
	return	STDVECTOR3(-x,-y,-z);
}

STDVECTOR3 operator * ( float f, const struct STDVECTOR3& v )
{
	return STDVECTOR3(f * v.x, f * v.y, f * v.z);
}

float STDVECTOR3::operator * ( const STDVECTOR3& v )
{
	return ((this->x*v.x) + (this->y*v.y) + (this->z*v.z));
}

STDVECTOR3 STDVECTOR3::operator + ( const STDVECTOR3& v ) const
{
	return STDVECTOR3(x+v.x,y+v.y,z+v.z);
}

STDVECTOR3 STDVECTOR3::operator - ( const STDVECTOR3& v ) const
{
	return STDVECTOR3(x-v.x,y-v.y,z-v.z);
}

STDVECTOR3 STDVECTOR3::operator * ( float f) const
{
	return STDVECTOR3(x*f,y*f,z*f);
}

STDVECTOR3 STDVECTOR3::operator / ( float f ) const
{
	return STDVECTOR3(x/f,y/f,z/f);
}

bool STDVECTOR3::operator == ( const STDVECTOR3& v) const
{
	return (x == v.x && y == v.y && z == v.z);
}

bool STDVECTOR3::operator != ( const STDVECTOR3& v) const
{
	return (x != v.x || y != v.y || z != v.z);
}

void STDVECTOR3::Normalize()
{
	float mod = sqrt(x*x + y*y + z*z);
	x/=mod;
	y/=mod;
	z/=mod;
}

float STDVECTOR3::Length()
{
	return sqrt(x*x + y*y + z*z);
}

STDMATRIX::STDMATRIX()
{
	a1 = a2 = a3 = a4 = b1 = b2 = b3 = b4 = c1 = c2 = c3 = c4 = d1 = d2 = d3 = d4 = 0.0f;
}

STDMATRIX::STDMATRIX( const float* m)
{
	if(!m) return;

	memcpy(&a1, m, sizeof(STDMATRIX));
}

STDMATRIX::STDMATRIX( const STDMATRIX& m)
{
	memcpy(&a1, &m, sizeof(STDMATRIX));
}

STDMATRIX::STDMATRIX( float a1,float a2,float a3,float a4,
				   float b1,float b2,float b3,float b4,
				   float c1,float c2,float c3,float c4,
				   float d1,float d2,float d3,float d4 )
{
	this->a1 = a1; this->a2 = a2; this->a3 = a3; this->a4 = a4;
	this->b1 = b1; this->b2 = b2; this->b3 = b3; this->b4 = b4;
	this->c1 = c1; this->c2 = c2; this->c3 = c3; this->c4 = c4;
	this->d1 = d1; this->d2 = d2; this->d3 = d3; this->d4 = d4;
}

STDMATRIX& STDMATRIX::operator *= ( const STDMATRIX& mb)
{
	MatMultiply(this,this,&mb);
	return *this;
}

STDMATRIX& STDMATRIX::operator += ( const STDMATRIX& mb)
{
	a1 += mb.a1; a2 += mb.a2; a3 += mb.a3; a4 += mb.a4;
	b1 += mb.b1; b2 += mb.b2; b3 += mb.b3; b4 += mb.b4;
	c1 += mb.c1; c2 += mb.c2; c3 += mb.c3; c4 += mb.c4;
	d1 += mb.d1; d2 += mb.d2; d3 += mb.d3; d4 += mb.d4;
	return *this;
}

STDMATRIX& STDMATRIX::operator -= ( const STDMATRIX& mb)
{
	a1 -= mb.a1; a2 -= mb.a2; a3 -= mb.a3; a4 -= mb.a4;
	b1 -= mb.b1; b2 -= mb.b2; b3 -= mb.b3; b4 -= mb.b4;
	c1 -= mb.c1; c2 -= mb.c2; c3 -= mb.c3; c4 -= mb.c4;
	d1 -= mb.d1; d2 -= mb.d2; d3 -= mb.d3; d4 -= mb.d4;
	return *this;
}

STDMATRIX::operator float* ()
{
	return (float *) &a1;
}


STDMATRIX::operator const float* () const
{
	return (const float *) &a1;
}

STDMATRIX& STDMATRIX::operator *= ( float f)
{
	a1 *= f; a2 *= f; a3 *= f; a4 *= f;
	b1 *= f; b2 *= f; b3 *= f; b4 *= f;
	c1 *= f; c2 *= f; c3 *= f; c4 *= f;
	d1 *= f; d2 *= f; d3 *= f; d4 *= f;
	return *this;
}

STDMATRIX& STDMATRIX::operator /= ( float f)
{
	a1 /= f; a2 /= f; a3 /= f; a4 /= f;
	b1 /= f; b2 /= f; b3 /= f; b4 /= f;
	c1 /= f; c2 /= f; c3 /= f; c4 /= f;
	d1 /= f; d2 /= f; d3 /= f; d4 /= f;
	return *this;
}

STDMATRIX STDMATRIX::operator + () const
{
	return *this;
}

STDMATRIX STDMATRIX::operator - () const
{
	return STDMATRIX( -a1,-a2,-a3,-a4,
		-b1,-b2,-b3,-b4,
		-c1,-c2,-c3,-c4,
		-d1,-d2,-d3,-d4 );
}

STDMATRIX STDMATRIX::operator * ( const STDMATRIX& mb) const
{
	STDMATRIX	tmp;
	MatMultiply(&tmp,this,&mb);
	return tmp;
}

STDMATRIX STDMATRIX::operator + ( const STDMATRIX& mat) const
{
	return STDMATRIX( a1 + mat.a1,a2 + mat.a2,a3 + mat.a3,a4 + mat.a4,
		b1 + mat.b1,b2 + mat.b2,b3 + mat.b3,b4 + mat.b4,
		c1 + mat.c1,c2 + mat.c2,c3 + mat.c3,c4 + mat.c4,
		d1 + mat.d1,d2 + mat.d2,d3 + mat.d3,d4 + mat.d4 );
}

STDMATRIX STDMATRIX::operator - ( const STDMATRIX& mat) const
{
	return STDMATRIX( a1 - mat.a1,a2 - mat.a2,a3 - mat.a3,a4 - mat.a4,
		b1 - mat.b1,b2 - mat.b2,b3 - mat.b3,b4 - mat.b4,
		c1 - mat.c1,c2 - mat.c2,c3 - mat.c3,c4 - mat.c4,
		d1 - mat.d1,d2 - mat.d2,d3 - mat.d3,d4 - mat.d4 );
}

STDMATRIX operator * ( float f, const STDMATRIX& mat )
{
	return STDMATRIX(f * mat.a1, f * mat.a2, f * mat.a3, f * mat.a4,
					f * mat.b1, f * mat.b2, f * mat.b3, f * mat.b4,
					f * mat.c1, f * mat.c2, f * mat.c3, f * mat.c4,
					f * mat.d1, f * mat.d2, f * mat.d3, f * mat.d4);
}

STDMATRIX STDMATRIX::operator * ( float f ) const
{
	return STDMATRIX( a1*f,a2*f,a3*f,a4*f,
		b1*f,b2*f,b3*f,b4*f,
		c1*f,c2*f,c3*f,c4*f,
		d1*f,d2*f,d3*f,d4*f );
}

STDMATRIX STDMATRIX::operator / ( float f ) const
{
	return STDMATRIX( a1/f,a2/f,a3/f,a4/f,
		b1/f,b2/f,b3/f,b4/f,
		c1/f,c2/f,c3/f,c4/f,
		d1/f,d2/f,d3/f,d4/f );
}

bool STDMATRIX::operator == ( const STDMATRIX& mat ) const
{
	return 0 == memcmp(this, &mat, sizeof(STDMATRIX));
}
bool STDMATRIX::operator != ( const STDMATRIX& mat) const
{
	return 0 != memcmp(this, &mat, sizeof(STDMATRIX));
}


STDMATRIX MatMultiply(STDMATRIX *mpout, const STDMATRIX *mp1, const STDMATRIX *mp2)
{
	unsigned int i,j,k; i = j = k =0;

	STDMATRIX tmp;

	for(i = 0; i < 4; ++i){
		for(j = 0; j < 4; ++j){
			for(k = 0; k < 4; ++k){
				tmp.m[i][j] += mp1->m[i][k]*mp2->m[k][j];
			}}}
	if(mpout)
		*mpout = tmp;
	
	return tmp;
}

STDMATRIX MatTranslation(STDMATRIX *mpout,float x,float y,float z)
{
	STDMATRIX tmp;
	MatIdentity(&tmp);
	tmp.d1 = x; tmp.d2 = y; tmp.d3 = z;
	//tmp.a4 = x; tmp.b4 = y; tmp.c4 = z;
	if(mpout)
		*mpout = tmp;
	return tmp;
}

STDMATRIX MatScaling(STDMATRIX *mpout,float x,float y,float z)
{
	STDMATRIX tmp;
	MatIdentity(&tmp);
	tmp.a1 *= x;
	tmp.b2 *= y;
	tmp.c3 *= z;
	if(mpout)
		*mpout = tmp;
	return tmp;
}

STDMATRIX MatRotationX(STDMATRIX *mpout,float rads)
{
	STDMATRIX tmp;
	MatIdentity(&tmp);
	tmp.b2 = cos(rads);
	tmp.b3 = -sin(rads);
	tmp.c2 = sin(rads);
	tmp.c3 = cos(rads);
	if(mpout)
		*mpout = tmp;
	return tmp;
}

STDMATRIX MatRotationY(STDMATRIX *mpout,float rads)
{
	STDMATRIX tmp;
	MatIdentity(&tmp);
	tmp.a1 = cos(rads);
	tmp.a3 = sin(rads);
	tmp.c1 = -sin(rads);
	tmp.c3 = cos(rads);
	if(mpout)
		*mpout = tmp;
	return tmp;
}

STDMATRIX MatRotationZ(STDMATRIX *mpout,float rads)
{
	STDMATRIX tmp;
	MatIdentity(&tmp);
	tmp.a1 = cos(rads);
	tmp.a2 = -sin(rads);
	tmp.b1 = sin(rads);
	tmp.b2 = cos(rads);
	if(mpout)
		*mpout = tmp;
	return tmp;
}

STDMATRIX MatTranspose(STDMATRIX *mpout,const STDMATRIX *mat)
{
	STDMATRIX tmp;
	for (int i = 0; i < 4 ; i++){
		for (int j = 0; j < 4 ; j++){
			tmp.m[i][j] = mat->m[j][i];
		}}
	if(mpout)
		*mpout = tmp;
	return tmp;
}

STDMATRIX MatIdentity(STDMATRIX *mpout)
{
	STDMATRIX tmp;
	tmp.a1 = tmp.b2 = tmp.c3 = tmp.d4 = 1.0f;

	if(mpout)
		*mpout = tmp;
	return tmp;
}

STDMATRIX MatViewLookAtLH(STDMATRIX *mpout,const STDVECTOR3 &camPos,const STDVECTOR3 &lookAt,const STDVECTOR3 &upAxis)
{
	STDVECTOR3 zaxe = lookAt - camPos;
	zaxe.Normalize();

	STDVECTOR3 xaxe;
	VecCross(&xaxe,upAxis,zaxe);
	xaxe.Normalize();

	STDVECTOR3 yaxe;
	VecCross(&yaxe,zaxe,xaxe);

	STDMATRIX tmp;

	tmp.a1 = xaxe.x;
	tmp.b1 = xaxe.y;
	tmp.c1 = xaxe.z;
	tmp.d1 = -VecDot(xaxe,camPos);

	tmp.a2 = yaxe.x;
	tmp.b2 = yaxe.y;
	tmp.c2 = yaxe.z;
	tmp.d2 = -VecDot(yaxe,camPos);

	tmp.a3 = zaxe.x;
	tmp.b3 = zaxe.y;
	tmp.c3 = zaxe.z;
	tmp.d3 = -VecDot(zaxe,camPos);

	tmp.a4 = 0.0f;
	tmp.b4 = 0.0f;
	tmp.c4 = 0.0f;
	tmp.d4 = 1.0f;

	if(mpout)
		*mpout = tmp;
	return tmp;
}

STDMATRIX MatViewLookAtRH(STDMATRIX *mpout,const STDVECTOR3 &camPos,const STDVECTOR3 &lookAt,const STDVECTOR3 &upAxis)
{
	STDVECTOR3 zaxe = camPos - lookAt;
	zaxe.Normalize();

	STDVECTOR3 xaxe;
	VecCross(&xaxe,upAxis,zaxe);
	xaxe.Normalize();

	STDVECTOR3 yaxe;
	VecCross(&yaxe,zaxe,xaxe);

	STDMATRIX tmp;

	tmp.a1 = xaxe.x;
	tmp.b1 = xaxe.y;
	tmp.c1 = xaxe.z;
	tmp.d1 = -VecDot(xaxe,camPos);

	tmp.a2 = yaxe.x;
	tmp.b2 = yaxe.y;
	tmp.c2 = yaxe.z;
	tmp.d2 = -VecDot(yaxe,camPos);

	tmp.a3 = zaxe.x;
	tmp.b3 = zaxe.y;
	tmp.c3 = zaxe.z;
	tmp.d3 = -VecDot(zaxe,camPos);

	tmp.a4 = 0.0f;
	tmp.b4 = 0.0f;
	tmp.c4 = 0.0f;
	tmp.d4 = 1.0f;

	if(mpout)
		*mpout = tmp;

	return tmp;
}


STDMATRIX MatPerspectiveLH(STDMATRIX* mpout,float FOV,float Aspect,float NearPlane,float FarPlane)
{
	STDMATRIX tmp;
	float x,y;

	y = 1.0f/tan(FOV/2.0f);
	x = y / Aspect;

	tmp.a1 = x;
	tmp.b2 = y;
	tmp.c3 = FarPlane/(FarPlane-NearPlane);
	tmp.d3 = -(NearPlane*FarPlane)/(FarPlane-NearPlane);
	tmp.c4 = 1.0f;

	if(mpout)
		*mpout = tmp;

	return tmp;
}

STDMATRIX MatPerspectiveRH(STDMATRIX* mpout,float FOV,float Aspect,float NearPlane,float FarPlane)
{
	STDMATRIX tmp;
	float x,y;

	y = 1.0f/tan(FOV/2.0f);
	x = y / Aspect;

	tmp.a1 = x;
	tmp.b2 = y;
	tmp.c3 = FarPlane/(NearPlane-FarPlane);
	tmp.d3 = (NearPlane*FarPlane)/(NearPlane-FarPlane);
	tmp.c4 = -1.0f;
	if(mpout)
		*mpout = tmp;
	return tmp;
}

STDMATRIX MatRotationAxis(STDMATRIX *mpout,const STDVECTOR3 &axis,float angle)
{
	STDVECTOR3 tVec;
	tVec = axis;
	tVec.Normalize();
	STDMATRIX  tMat;

	float cosangle = cos(angle);
	float sinangle = sin(angle);
	float A = 1.0f - cosangle;

	tMat.a1 = A*tVec.x*tVec.x + cosangle;
	tMat.a2 = A*tVec.x*tVec.y + sinangle*tVec.z;
	tMat.a3 = A*tVec.x*tVec.z - sinangle*tVec.y;
	tMat.a4 = 0.0f;

	tMat.b1 = A*tVec.x*tVec.y - sinangle*tVec.z;
	tMat.b2 = A*tVec.y*tVec.y + cosangle;
	tMat.b3 = A*tVec.y*tVec.z + sinangle*tVec.x;
	tMat.b4 = 0.0f;

	tMat.c1 = A*tVec.x*tVec.z + sinangle*tVec.y;
	tMat.c2 = A*tVec.y*tVec.z - sinangle*tVec.x;
	tMat.c3 = A*tVec.z*tVec.z + cosangle;
	tMat.c4 = 0.0f;

	tMat.d1 = tMat.d2 = tMat.d3 = 0.0f; tMat.d4 = 1.0f;
	if(mpout)
		*mpout = tMat;
	return tMat;

}

STDVECTOR3 VecTransform(STDVECTOR3 *vpout,const STDVECTOR3 &v,const STDMATRIX &mat)
{
	STDVECTOR3 tmp;
	tmp.x = v.x*mat.a1 + v.y*mat.a2 + v.z*mat.a3 + 1.0f*mat.d1;
	tmp.y = v.x*mat.b1 + v.y*mat.b2 + v.z*mat.b3 + 1.0f*mat.d2;
	tmp.z = v.x*mat.c1 + v.y*mat.c2 + v.z*mat.c3 + 1.0f*mat.d3;
	if(vpout)
	 *vpout = tmp;
	return tmp;
}

STDVECTOR3 VecTransformNormal(STDVECTOR3 *vpout,const STDVECTOR3 &v,const STDMATRIX &mat)
{
	STDVECTOR3 tmp;
	tmp.x = v.x*mat.a1 + v.y*mat.a2 + v.z*mat.a3;
	tmp.y = v.x*mat.b1 + v.y*mat.b2 + v.z*mat.b3;
	tmp.z = v.x*mat.c1 + v.y*mat.c2 + v.z*mat.c3;
	if(vpout)
		*vpout = tmp;
	return tmp;
}

float VecDot(const STDVECTOR3 &v1,const STDVECTOR3 &v2)
{
	return ((v1.x*v2.x) + (v1.y*v2.y) + (v1.z*v2.z));
}

STDVECTOR3 VecCross(STDVECTOR3 *vpout,const STDVECTOR3 &lv,const STDVECTOR3 &rv)
{
	STDVECTOR3 tmp = STDVECTOR3( lv.y*rv.z - lv.z*rv.y,
								 lv.z*rv.x - lv.x*rv.z,
								 lv.x*rv.y - lv.y*rv.x );
	if(vpout)
		*vpout = tmp;
	return tmp;
}

STDVECTOR3 Vec3Normalize( STDVECTOR3 *pOut, const STDVECTOR3 &pV )
{
	float v = 1.0f / sqrtf(pV.x * pV.x + pV.y * pV.y + pV.z * pV.z);
	STDVECTOR3 tmp;
	tmp.x = pV.x * v;
	tmp.y = pV.y * v;
	tmp.z = pV.z * v;
	if(pOut)
		*pOut = tmp;
	return tmp;
}
