
/*
*DexEngine Vector4
*create by Bruce
*Date:2016.1.28
*/
#ifndef _DEX_MATH_VECTOR4_H
#define _DEX_MATH_VECTOR4_H

#include <memory.h>
#include "DexMath.h"
#include "DexVector3.h"
template<typename T>
class DexVector4T
{
public:
	union
	{
		struct  
		{
			T x, y, z, w;
		};
		T m[4];
	};
	

public:
	DexVector4T();
	DexVector4T(const DexVector4T& vector4);
	DexVector4T(T _x, T _y, T _z, T w);
	DexVector4T(const DexVector3T<T>& vector3, T w);
	DexVector4T(T* value); //一个4元素数组
	~DexVector4T<T>();
public:
	DexVector4T	 Invert();
	DexVector4T  GetInvert()const;
	DexVector4T  Set(T* value);//一个4元素数组
	DexVector4T  Set(const DexVector4T& vector4);
	DexVector4T  Set(const T& _x, const T& _y, const T& _z, const T& _w);
public:
	T Length() const;
	T LengthSq() const; //length square
	DexVector4T  Normalize();
public:
	DexVector4T& operator = (const DexVector4T& vector4);
	bool operator == (const DexVector4T& vector4)const;
	bool operator != (const DexVector4T& vector4)const;
	DexVector4T operator + (const DexVector4T& vector4)const;
	DexVector4T operator - (const DexVector4T& vector4)const;
	DexVector4T operator * (float _value)const;
	DexVector4T operator / (float _value)const;

	DexVector4T& operator += (const DexVector4T& vector4);
	DexVector4T& operator -= (const DexVector4T& vector4);
	DexVector4T& operator *= (float _value);
	DexVector4T& operator /= (float _value);

	bool operator > (const DexVector4T& vector4) const;
	bool operator >=(const DexVector4T& vector4) const;
	bool operator < (const DexVector4T& vector4) const;
	bool operator <=(const DexVector4T& vector4) const;
	

};

extern template class DexVector4T<float>;
extern template class DexVector4T<DInt32>;
typedef DexVector4T<float>  DexVector4;
typedef DexVector4T<DInt32>  DexVector4_DInt32;
#endif