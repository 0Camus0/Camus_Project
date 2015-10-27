#include "../include/xMaths.h"
/*
 xFloat16::xFloat16(unsigned short v){
	val = v;
 }

 xFloat16::xFloat16(float f){

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

 xFloat16::operator float () const{

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

 xFloat16& xFloat16::operator = (float f){
	*this = xFloat16(f); return *this;
 }

 xFloat16& xFloat16::operator = (xFloat16 f16){
	val = f16.val;  return *this;
 }

 xFloat16& xFloat16::operator - (){
	  xFloat16 f; f.val = val ^ 0x8000; return f;
 }

 xFloat16& xFloat16::operator += (float f){
	*this = xFloat16(float(*this) + f); return *this;
 }

 xFloat16& xFloat16::operator -= (float f){
	*this = xFloat16(float(*this) - f); return *this;
 }

 xFloat16& xFloat16::operator *= (float f){
	*this = xFloat16(float(*this) * f); return *this;
 }

 xFloat16& xFloat16::operator /= (float f){
	*this = xFloat16(float(*this) / f); return *this;
 }

 xFloat16& xFloat16::operator += (xFloat16 f16){
    *this = xFloat16(float(*this) + float(f16)); return *this;
 }

 xFloat16& xFloat16::operator -= (xFloat16 f16){
    *this = xFloat16(float(*this) - float(f16)); return *this;
 }

 xFloat16& xFloat16::operator *= (xFloat16 f16){
	*this = xFloat16(float(*this) * float(f16)); return *this;
 }

 xFloat16& xFloat16::operator /= (xFloat16 f16){
	*this = xFloat16(float(*this) / float(f16)); return *this;
 }

 xFloat16 xFloat16::operator + (float f){
	return xFloat16(float(*this) + f);
 }

 xFloat16 xFloat16::operator - (float f){
	return xFloat16(float(*this) - f);
  }

 xFloat16 xFloat16::operator * (float f){
	return xFloat16(float(*this) * f);
 }

 xFloat16 xFloat16::operator / (float f){
	return xFloat16(float(*this) / f);
 }

 xFloat16 xFloat16::operator + (xFloat16 f16){
	return  xFloat16(float(*this) + float(f16));
 }

 xFloat16 xFloat16::operator - (xFloat16 f16){
	return  xFloat16(float(*this) - float(f16));
 }

 xFloat16 xFloat16::operator * (xFloat16 f16){
	return  xFloat16(float(*this) * float(f16));
 }

 xFloat16 xFloat16::operator / (xFloat16 f16){
	return  xFloat16(float(*this) / float(f16));
 }

*/