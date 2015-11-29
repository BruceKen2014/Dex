
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
	T Length();
	DexVector3T<T>  Cross(const DexVector3T<T>& vector3) const;
	T				Dot(const DexVector3T<T>& vector3) const;
	void			Invert();
	DexVector3T<T>  GetInvert()const;
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
inline DexVector3T<T>& DexVector3T<T>::operator = (const DexVector3T<T>& vector3)
{
	x = vector3.x;
	y = vector3.y;
	z = vector3.z;
	return *this;
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
	return ret;
}
template<typename T>
inline DexVector3T<T> DexVector3T<T>::operator - (const DexVector3T<T>& vector3)const
{
	DexVector3T<T> ret;
	ret.x = x - vector3.x;
	ret.y = y - vector3.y;
	ret.z = z - vector3.z;
	return ret;
}
template<typename T>
inline DexVector3T<T> DexVector3T<T>::operator * (float _value)const
{
	DexVector3T<T> ret;
	ret.x = x * _value;
	ret.y = y * _value;
	ret.z = z * _value;
	return ret;
}
template<typename T>
inline DexVector3T<T> DexVector3T<T>::operator / (float _value)const
{
	DexVector3T<T> ret;
	ret.x = x / _value;
	ret.y = y / _value;
	ret.z = z / _value;
	return ret;
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
inline T DexVector3T<T>::Length()
{
	return DexMath::Sqrt(x*x + y*y + z*z);
}

template<typename T>
inline DexVector3T<T> DexVector3T<T>::Cross(const DexVector3T<T>& vector3) const
{
	/*
		0	1	2
	O	i	j	k	
	1	x1	y1	z1
	2	x2	y2	z2
	算x时去掉i列， x3 = (-1)^(0+0) * (y1*z2 - y2*z1) = y1z2 - y2z1; //i处在第0行第0列，故符号位(-1)^(0+0)=0为正
	算y时去掉j列， y3 = (-1)^(0+1) * (x1*z2 - x2*z1) = -(x1z2 - x2z1); //j处在第0行第1列，故符号位(-1)^(0+1)=1为负
	算z时去掉k列， z3 = (-1)^(0+2) * (x1*y2 - x2*y1) = x1y2 - x2y1; //k处在第0行第2列，故符号位(-1)^(0+2)=0为正
	*/
	T new_x = y*vector3.z - vector3.y *z;
	T new_y = -(x*vector3.z - vector3.x * z);
	T new_z = x*vector3.y - vector3.x*y;
	return DexVector3T<T>(new_x, new_y, new_z);
}

template<typename T>
inline T DexVector3T<T>::Dot(const DexVector3T<T>& vector3) const
{
	T ret =x * vector3.x + y * vector3.y + z * vector3.z;
	return ret;
}


template<typename T>
inline void DexVector3T<T>::Invert()
{
	x *= (T)(-1);
	y *= (T)(-1);
	z *= (T)(-1);
}


template<typename T>
inline DexVector3T<T> DexVector3T<T>::GetInvert()	const
{
	DexVector3T<T> ret;
	ret.x = x* (T)(-1);
	ret.y = y* (T)(-1);
	ret.z = z* (T)(-1);
	return ret;
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

template<typename T1, typename T>
DexVector3T<T> operator* (T1 _value, const DexVector3T<T>& vector3)
{
	DexVector3T<T> ret;
	ret.x = (T)(vector3.x *  _value);
	ret.y = (T)(vector3.y *  _value);
	ret.z = (T)(vector3.z *  _value);
	return ret;
}
typedef DexVector3T<float>  DexVector3;
typedef DexVector3T<int32>  DexVector3_int32;
#endif