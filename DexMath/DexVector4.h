
/*
*DexEngine Vector4
*create by Bruce
*Date:2016.1.28
*/
#ifndef _DEX_MATH_VECTOR4_H
#define _DEX_MATH_VECTOR4_H

#include <memory.h>
#include "DexMath.h"
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
template<typename T>
inline DexVector4T<T>::DexVector4T()
{
	memset(m, 0, sizeof(T)*4);
}

template<typename T>
inline DexVector4T<T>::DexVector4T(const DexVector4T<T>& vector4)
{
	memcpy(m, vector4.m, sizeof(vector4));
}

template<typename T>
inline DexVector4T<T>::DexVector4T(T _x, T _y, T _z, T _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

template<typename T>
inline DexVector4T<T>::DexVector4T(T* value)
{
	memcpy(m, value, sizeof(T) * 4);
}

template<typename T>
inline DexVector4T<T>::~DexVector4T()
{

}

template<typename T>
inline DexVector4T<T> DexVector4T<T>::Normalize()
{
	float length = Length();
	if (length < 0.00001f)
	{
		return *this;
	}
	x = x / length;
	y = y / length;
	z = z / length;
	w = w / length;
	return *this;
}
template<typename T>
inline DexVector4T<T> DexVector4T<T>::Set(T* value)
{
	memcpy(m, value, sizeof(T) * 4);
	return *this;
}

template<typename T>
inline DexVector4T<T> DexVector4T<T>::Set(const DexVector4T<T>& vector4)
{
	memcpy(m, vector4.m, sizeof(vector4));
	return *this;
}

template<typename T>
inline DexVector4T<T> DexVector4T<T>::Set(const T& _x, const T& _y, const T& _z, const T& _w)
{
	x = _x;
	y = _y;
	z = _z;
	x = _w;
	return *this;
}

template<typename T>
inline DexVector4T<T>& DexVector4T<T>::operator = (const DexVector4T<T>& vector4)
{
	x = vector4.x;
	y = vector4.y;
	z = vector4.z;
	w = vector4.w;
	return *this;
}
template<typename T>
inline bool DexVector4T<T>::operator == (const DexVector4T<T>& vector4)const
{
	return DexMath::Equal(x, vector4.x)
		&& DexMath::Equal(y, vector4.y)
		&& DexMath::Equal(z, vector4.z)
		&& DexMath::Equal(w, vector4.w);
}
template<typename T>
inline bool DexVector4T<T>::operator != (const DexVector4T<T>& vector4)const
{
	return !DexMath::Equal(x, vector4.x)
		|| !DexMath::Equal(y, vector4.y)
		|| !DexMath::Equal(z, vector4.z)
		|| !DexMath::Equal(w, vector4.w);
}
template<typename T>
inline DexVector4T<T> DexVector4T<T>::operator + (const DexVector4T<T>& vector4)const
{
	DexVector4T<T> ret;
	ret.x = x + vector4.x;
	ret.y = y + vector4.y;
	ret.z = z + vector4.z;
	ret.w = w + vector4.w;
	return ret;
}
template<typename T>
inline DexVector4T<T> DexVector4T<T>::operator - (const DexVector4T<T>& vector4)const
{
	DexVector4T<T> ret;
	ret.x = x - vector3.x;
	ret.y = y - vector3.y;
	ret.z = z - vector3.z;
	ret.w = w - vector3.w;
	return ret;
}
template<typename T>
inline DexVector4T<T> DexVector4T<T>::operator * (float _value)const
{
	DexVector4T<T> ret;
	ret.x = x * _value;
	ret.y = y * _value;
	ret.z = z * _value;
	ret.w = w * _value;
	return ret;
}
template<typename T>
inline DexVector4T<T> DexVector4T<T>::operator / (float _value)const
{
	DexVector4T<T> ret;
	ret.x = x / _value;
	ret.y = y / _value;
	ret.z = z / _value; 
	ret.w = w / _value;
	return ret;
}

template<typename T>
inline DexVector4T<T>& DexVector4T<T>::operator += (const DexVector4T<T>& vector4)
{
	x = x + vector4.x;
	y = y + vector4.y;
	z = z + vector4.z;
	w = w + vector4.w;
	return *this;
}
template<typename T>
inline DexVector4T<T>& DexVector4T<T>::operator -= (const DexVector4T<T>& vector4)
{
	x = x - vector4.x;
	y = y - vector4.y;
	z = z - vector4.z;
	w = w - vector4.w;
	return *this;
}
template<typename T>
inline DexVector4T<T>& DexVector4T<T>::operator *= (float _value)
{
	x = x * _value;
	y = y * _value;
	z = z * _value;
	w = w * _value;
	return *this;
}
template<typename T>
inline DexVector4T<T>& DexVector4T<T>::operator /= (float _value)
{
	x = x / _value;
	y = y / _value;
	z = z / _value;
	w = w / _value;
	return *this;
}

template<typename T>
inline T DexVector4T<T>::Length() const
{
	return DexMath::Sqrt(x*x + y*y + z*z + w*w);
}

template<typename T>
inline T DexVector4T<T>::LengthSq() const
{
	return x*x + y*y + z*z + w*w;
}

template<typename T>
inline DexVector4T<T> DexVector4T<T>::Invert()
{
	x *= (T)(-1);
	y *= (T)(-1);
	z *= (T)(-1);
	w *= (T)(-1);
	return *this;
}


template<typename T>
inline DexVector4T<T> DexVector4T<T>::GetInvert()	const
{
	DexVector4T<T> ret;
	ret.x = x* (T)(-1);
	ret.y = y* (T)(-1);
	ret.z = z* (T)(-1);
	ret.w = w* (T)(-1);
	return ret;
}


template<typename T>
inline bool DexVector4T<T>::operator>(const DexVector4T<T>& vector4) const
{
	return x > vector4.x &&
		y > vector4.y &&
		z > vector4.z &&
		w > vector4.w;
}
template<typename T>
inline bool DexVector4T<T>::operator>=(const DexVector4T<T>& vector4) const
{
	return x >= vector4.x &&
		y >= vector4.y &&
		z >= vector4.z &&
		w >= vector4.w;
}

template<typename T>
inline bool DexVector4T<T>::operator<(const DexVector4T<T>& vector4) const
{
	return x < vector4.x &&
		y < vector4.y &&
		z < vector4.z &&
		w < vector4.w;
}

template<typename T>
inline bool DexVector4T<T>::operator<=(const DexVector4T<T>& vector4) const
{
	return x <= vector4.x &&
		y <= vector4.y &&
		z <= vector4.z &&
		w <= vector4.w;
}

typedef DexVector4T<float>  DexVector4;
typedef DexVector4T<int32>  DexVector4_int32;
#endif