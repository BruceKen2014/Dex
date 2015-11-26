
/*
*DexEngine Vector2
*create by Bruce
*Date:2015.11.26
*/
#ifndef _DEX_MATH_VECTOR2_H
#define _DEX_MATH_VECTOR2_H

#include "DexMath.h"
template<typename T>
class DexVector2T
{
public:
	union
	{
		struct
		{
			T x, y;
		};
		T m[2];
	};


public:
	DexVector2T();
	DexVector2T(const DexVector2T<T>& vector2);
	DexVector2T(T _x, T _y);
	DexVector2T(T* value); //一个2元素数组
	~DexVector2T<T>();
public:
	T Length();
	T				Dot(const DexVector2T<T>& vector2) const;
	DexVector2T<T>& Normalize();
	DexVector2T<T>& Set(T* value);//一个3元素数组
	DexVector2T<T>& Set(const DexVector2T<T>& vector2);
	DexVector2T<T>& Set(const T& _x, const T& _y);
public:
	DexVector2T<T>& operator = (const DexVector2T<T>& vector2);
	bool operator == (const DexVector2T<T>& vector2)const;
	bool operator != (const DexVector2T<T>& vector2)const;
	DexVector2T<T> operator + (const DexVector2T<T>& vector2)const;
	DexVector2T<T> operator - (const DexVector2T<T>& vector2)const;
	DexVector2T<T> operator * (float _value)const;
	DexVector2T<T> operator / (float _value)const;

	DexVector2T<T>& operator += (const DexVector2T<T>& vector2);
	DexVector2T<T>& operator -= (const DexVector2T<T>& vector2);
	DexVector2T<T>& operator *= (float _value);
	DexVector2T<T>& operator /= (float _value);

	bool operator > (const DexVector2T<T>& vector2) const;
	bool operator >=(const DexVector2T<T>& vector2) const;
	bool operator < (const DexVector2T<T>& vector2) const;
	bool operator <=(const DexVector2T<T>& vector2) const;


};
template<typename T>
inline DexVector2T<T>::DexVector2T()
{
	memset(m, 0, sizeof(T) * 2);
}

template<typename T>
inline DexVector2T<T>::DexVector2T(const DexVector2T<T>& vector2)
{
	memcpy(m, vector2.m, sizeof(vector2));
}

template<typename T>
inline DexVector2T<T>::DexVector2T(T _x, T _y)
{
	x = _x;
	y = _y;
}

template<typename T>
inline DexVector2T<T>::DexVector2T(T* value)
{
	memcpy(m, value, sizeof(T) * 2);
}

template<typename T>
inline DexVector2T<T>::~DexVector2T()
{

}

template<typename T>
inline DexVector2T<T>& DexVector2T<T>::Normalize()
{
	float length = Length();
	if (length < 0.00001f)
	{
		return *this;
	}
	x = x / length;
	y = y / length;
	return *this;
}
template<typename T>
inline DexVector2T<T>& DexVector2T<T>::Set(T* value)
{
	memcpy(m, value, sizeof(T) * 2);
	return *this;
}

template<typename T>
inline DexVector2T<T>& DexVector2T<T>::Set(const DexVector2T<T>& vector2)
{
	memcpy(m, vector2.m, sizeof(vector2));
	return *this;
}

template<typename T>
inline DexVector2T<T>& DexVector2T<T>::Set(const T& _x, const T& _y)
{
	x = _x;
	y = _y;
	return *this;
}

template<typename T>
inline DexVector2T<T>& DexVector2T<T>::operator = (const DexVector2T<T>& vector2)
{
	x = vector2.x;
	y = vector2.y;
}
template<typename T>
inline bool DexVector2T<T>::operator == (const DexVector2T<T>& vector2)const
{
	return DexMath::equal(x, vector2.x)
		&& DexMath::equal(y, vector2.y);
}
template<typename T>
inline bool DexVector2T<T>::operator != (const DexVector2T<T>& vector2)const
{
	return !DexMath::equal(x, vector2.x)
		|| !DexMath::equal(y, vector2.y);
}
template<typename T>
inline DexVector2T<T> DexVector2T<T>::operator + (const DexVector2T<T>& vector2)const
{
	DexVector2T<T> ret;
	ret.x = x + vector2.x;
	ret.y = y + vector2.y;
}
template<typename T>
inline DexVector2T<T> DexVector2T<T>::operator - (const DexVector2T<T>& vector2)const
{
	DexVector2T<T> ret;
	ret.x = x - vector2.x;
	ret.y = y - vector2.y;
}
template<typename T>
inline DexVector2T<T> DexVector2T<T>::operator * (float _value)const
{
	DexVector2T<T> ret;
	ret.x = x * _value;
	ret.y = y * _value;
}
template<typename T>
inline DexVector2T<T> DexVector2T<T>::operator / (float _value)const
{
	DexVector2T<T> ret;
	ret.x = x / _value;
	ret.y = y / _value;
}

template<typename T>
inline DexVector2T<T>& DexVector2T<T>::operator += (const DexVector2T<T>& vector2)
{
	x = x + vector2.x;
	y = y + vector2.y;
	return *this;
}
template<typename T>
inline DexVector2T<T>& DexVector2T<T>::operator -= (const DexVector2T<T>& vector2)
{
	x = x - vector2.x;
	y = y - vector2.y;
	return *this;
}
template<typename T>
inline DexVector2T<T>& DexVector2T<T>::operator *= (float _value)
{
	x = x * _value;
	y = y * _value;
	return *this;
}
template<typename T>
inline DexVector2T<T>& DexVector2T<T>::operator /= (float _value)
{
	x = x / _value;
	y = y / _value;
	return *this;
}

template<typename T>
inline T DexVector2T<T>::Length()
{
	return DexMath::Sqrt(x*x + y*y);
}

template<typename T>
inline T DexVector2T<T>::Dot(const DexVector2T<T>& vector2) const
{
	T ret = x * vector2.x + y * vector2.y;
	return ret;
}


template<typename T>
inline bool DexVector2T<T>::operator>(const DexVector2T<T>& vector2) const
{
	return x > vector2.x &&
		y > vector2.y &&;
}
template<typename T>
inline bool DexVector2T<T>::operator>=(const DexVector2T<T>& vector2) const
{
	return x >= vector2.x &&
		y >= vector2.y &&;
}

template<typename T>
inline bool DexVector2T<T>::operator<(const DexVector2T<T>& vector2) const
{
	return x < vector2.x &&
		y < vector2.y &&;
}

template<typename T>
inline bool DexVector2T<T>::operator<=(const DexVector2T<T>& vector2) const
{
	return x <= vector2.x &&
		y <= vector2.y &&;
}

template<typename T1, typename T>
DexVector2T<T> operator* (T1 _value, const DexVector2T<T>& vector2)
{
	DexVector2T<T> ret;
	ret.x = vector2.x *  T1;
	ret.y = vector2.y *  T1;
	return ret;
}
typedef DexVector2T<float>  DexVector2;
typedef DexVector2T<int32>  DexVector2_int32;
#endif