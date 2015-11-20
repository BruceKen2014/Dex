
/*
*DexEngine Vector3
*create by Bruce
*Date:2015.11.7
*/
#ifndef _DEX_MATH_VECTOR3_H
#define _DEX_MATH_VECTOR3_H

#include "DexMath.h"
template<typename T>
class DexVector3T
{
public:
	T x, y, z;

public:
	DexVector3T<T>();
	DexVector3T<T>(T _x, T _y, T _z);
	~DexVector3T<T>();
public:
	float Length();
public:
	DexVector3T<T>& operator = (const DexVector3T<T>& vector3);
	bool operator == (const DexVector3T<T>& vector3)const;
	bool operator != (const DexVector3T<T>& vector3)const;
	DexVector3T<T> operator + (const DexVector3T<T>& vector3)const;
	DexVector3T<T> operator - (const DexVector3T<T>& vector3)const;
	DexVector3T<T> operator * (float _value)const;
	DexVector3T<T> operator / (float _value)const;

	DexVector3T<T>& operator += (const DexVector3T<T>& vector3);
	DexVector3T<T>& operator -= (const DexVector3T<T>& vector3);
	DexVector3T<T>& operator *= (float _value);
	DexVector3T<T>& operator /= (float _value);
public:
	static void Normalize(DexVector3T<T>& vector3);

};
template<typename T>
DexVector3T<T>::DexVector3T()
{
	x = y = z = 0;
}

template<typename T>
DexVector3T<T>::DexVector3T(T _x, T _y, T _z)
{
	x = _x;
	y = _y;
	z = _z;
}

template<typename T>
DexVector3T<T>::~DexVector3T()
{

}
//static
template<typename T>
void DexVector3T<T>::Normalize(DexVector3T<T>& vector3)
{
	float length = vector3.Length();
	if (length < 0.00001f)
	{
		return;
	}
	vector3.x = vector3.x / length;
	vector3.y = vector3.y / length;
	vector3.z = vector3.z / length;
}

template<typename T>
DexVector3T<T> operator* (float _value, const DexVector3T<T>& vector3)
{
	DexVector3T<T> ret;
	ret.x = vector3.x *  _value;
	ret.y = vector3.y *  _value;
	ret.z = vector3.z *  _value;
	return ret;
}
template<typename T>
DexVector3T<T> operator* (float _value, const DexVector3T<T>& vector3);
template<typename T>
inline DexVector3T<T>& DexVector3T<T>::operator = (const DexVector3T<T>& vector3)
{
	x = vector3.x;
	y = vector3.y;
	z = vector3.z;
}
template<typename T>
inline bool DexVector3T<T>::operator == (const DexVector3T<T>& vector3)const
{
	return DexMath::equal(x, vector3.x)
		&& DexMath::equal(y, vector3.y)
		&& DexMath::equal(z, vector3.z);
}
template<typename T>
inline bool DexVector3T<T>::operator != (const DexVector3T<T>& vector3)const
{
	return !DexMath::equal(x, vector3.x)
		|| !DexMath::equal(y, vector3.y)
		|| !DexMath::equal(z, vector3.z);
}
template<typename T>
inline DexVector3T<T> DexVector3T<T>::operator + (const DexVector3T<T>& vector3)const
{
	DexVector3T<T> ret;
	ret.x = x + vector3.x;
	ret.y = y + vector3.y;
	ret.z = z + vector3.z;
}
template<typename T>
inline DexVector3T<T> DexVector3T<T>::operator - (const DexVector3T<T>& vector3)const
{
	DexVector3T<T> ret;
	ret.x = x - vector3.x;
	ret.y = y - vector3.y;
	ret.z = z - vector3.z;
}
template<typename T>
inline DexVector3T<T> DexVector3T<T>::operator * (float _value)const
{
	DexVector3T<T> ret;
	ret.x = x * _value;
	ret.y = y * _value;
	ret.z = z * _value;
}
template<typename T>
inline DexVector3T<T> DexVector3T<T>::operator / (float _value)const
{
	DexVector3T<T> ret;
	ret.x = x / _value;
	ret.y = y / _value;
	ret.z = z / _value;
}

template<typename T>
inline DexVector3T<T>& DexVector3T<T>::operator += (const DexVector3T<T>& vector3)
{
	x = x + vector3.x;
	y = y + vector3.y;
	z = z + vector3.z;
	return *this;
}
template<typename T>
inline DexVector3T<T>& DexVector3T<T>::operator -= (const DexVector3T<T>& vector3)
{
	x = x - vector3.x;
	y = y - vector3.y;
	z = z - vector3.z;
	return *this;
}
template<typename T>
inline DexVector3T<T>& DexVector3T<T>::operator *= (float _value)
{
	x = x * _value;
	y = y * _value;
	z = z * _value;
	return *this;
}
template<typename T>
inline DexVector3T<T>& DexVector3T<T>::operator /= (float _value)
{
	x = x / _value;
	y = y / _value;
	z = z / _value;
	return *this;
}

template<typename T>
inline float DexVector3T<T>::Length()
{
	return DexMath::Sqrt(x*x + y*y + z*z);
}

typedef DexVector3T<float>  DexVector3;
typedef DexVector3T<int32>  DexVector3_int32;
#endif