
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
	union
	{
		struct  
		{
			T x, y, z;
		};
		T m[3];
	};
	

public:
	DexVector3T();
	DexVector3T(const DexVector3T<T>& vector3);
	DexVector3T(T _x, T _y, T _z);
	DexVector3T(T* value); //一个3元素数组
	~DexVector3T<T>();
public:
	float Length();
	DexVector3T<T>& Normalize();
	DexVector3T<T>& Set(T* value);//一个3元素数组
	DexVector3T<T>& Set(const DexVector3T<T>& vector3);
	DexVector3T<T>& Set(const T& _x, const T& _y, const T& _z);
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

	bool operator > (const DexVector3T<T>& vector3) const;
	bool operator >=(const DexVector3T<T>& vector3) const;
	bool operator < (const DexVector3T<T>& vector3) const;
	bool operator <=(const DexVector3T<T>& vector3) const;
	

};
template<typename T>
inline DexVector3T<T>::DexVector3T()
{
	memset(m, 0, sizeof(T)*3);
}

template<typename T>
inline DexVector3T<T>::DexVector3T(const DexVector3T<T>& vector3)
{
	memcpy(m, vector3.m, sizeof(vector3));
}

template<typename T>
inline DexVector3T<T>::DexVector3T(T _x, T _y, T _z)
{
	x = _x;
	y = _y;
	z = _z;
}

template<typename T>
inline DexVector3T<T>::DexVector3T(T* value)
{
	memcpy(m, value, sizeof(T) * 3);
}

template<typename T>
inline DexVector3T<T>::~DexVector3T()
{

}

template<typename T>
inline DexVector3T<T>& DexVector3T<T>::Normalize()
{
	float length = Length();
	if (length < 0.00001f)
	{
		return *this;
	}
	x = x / length;
	y = y / length;
	z = z / length;
	return *this;
}
template<typename T>
inline DexVector3T<T>& DexVector3T<T>::Set(T* value)
{
	memcpy(m, value, sizeof(T) * 3);
	return *this;
}

template<typename T>
inline DexVector3T<T>& DexVector3T<T>::Set(const DexVector3T<T>& vector3)
{
	memcpy(m, vector3.m, sizeof(vector3));
	return *this;
}

template<typename T>
inline DexVector3T<T>& DexVector3T<T>::Set(const T& _x, const T& _y, const T& _z)
{
	x = _x;
	y = _y;
	z = _z;
	return *this;
}

template<typename T>
inline DexVector3T<T> operator* (float _value, const DexVector3T<T>& vector3)
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

template<typename T>
inline bool DexVector3T<T>::operator>(const DexVector3T<T>& vector3) const
{
	return x > vector3.x &&
		   y > vector3.y &&
		   z > vector3.z;
}
template<typename T>
inline bool DexVector3T<T>::operator>=(const DexVector3T<T>& vector3) const
{
	return x >= vector3.x &&
		y >= vector3.y &&
		z >= vector3.z;
}

template<typename T>
inline bool DexVector3T<T>::operator<(const DexVector3T<T>& vector3) const
{
	return x < vector3.x &&
		y < vector3.y &&
		z < vector3.z;
}

template<typename T>
inline bool DexVector3T<T>::operator<=(const DexVector3T<T>& vector3) const
{
	return x <= vector3.x &&
		y <= vector3.y &&
		z <= vector3.z;
}

typedef DexVector3T<float>  DexVector3;
typedef DexVector3T<int32>  DexVector3_int32;
#endif