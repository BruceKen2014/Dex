
#include <vcruntime_string.h>
#include "DexVector3.h"


template class DexVector3T<float>;
template class DexVector3T<DInt32>;

template<typename T>
inline DexVector3T<T>::DexVector3T()
{
	memset(m, 0, sizeof(T) * 3);
}

template<typename T>
inline DexVector3T<T>::DexVector3T(const DexVector3T& vector3) :DexVector3T(vector3.x, vector3.y, vector3.z)
{}

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
inline DexVector3T<T> DexVector3T<T>::Normalize()
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
inline T DexVector3T<T>::Distance(const DexVector3T& Point) const
{
	DexVector3T<T> temp = Point - *this;
	return temp.Length();
}

template<typename T>
T DexVector3T<T>::S_Distance(const DexVector3T& Point1, const DexVector3T& Point2)
{
	return Point1.Distance(Point2);
}
template<typename T>
inline DexVector3T<T> DexVector3T<T>::Set(T* value)
{
	memcpy(m, value, sizeof(T) * 3);
	return *this;
}

template<typename T>
inline DexVector3T<T> DexVector3T<T>::Set(const DexVector3T& vector3)
{
	memcpy(m, vector3.m, sizeof(vector3));
	return *this;
}

template<typename T>
inline DexVector3T<T> DexVector3T<T>::Set(const T& _x, const T& _y, const T& _z)
{
	x = _x;
	y = _y;
	z = _z;
	return *this;
}

template<typename T>
inline DexVector3T<T>& DexVector3T<T>::operator = (const DexVector3T& vector3)
{
	x = vector3.x;
	y = vector3.y;
	z = vector3.z;
	return *this;
}
template<typename T>
inline bool DexVector3T<T>::operator == (const DexVector3T& vector3)const
{
	return DexMath::Equal(x, vector3.x)
		&& DexMath::Equal(y, vector3.y)
		&& DexMath::Equal(z, vector3.z);
}
template<typename T>
inline bool DexVector3T<T>::operator != (const DexVector3T& vector3)const
{
	return !DexMath::Equal(x, vector3.x)
		|| !DexMath::Equal(y, vector3.y)
		|| !DexMath::Equal(z, vector3.z);
}
template<typename T>
inline DexVector3T<T> DexVector3T<T>::operator + (const DexVector3T& vector3)const
{
	DexVector3T ret;
	ret.x = x + vector3.x;
	ret.y = y + vector3.y;
	ret.z = z + vector3.z;
	return ret;
}
template<typename T>
inline DexVector3T<T> DexVector3T<T>::operator - (const DexVector3T& vector3)const
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
inline DexVector3T<T>& DexVector3T<T>::operator += (const DexVector3T& vector3)
{
	x = x + vector3.x;
	y = y + vector3.y;
	z = z + vector3.z;
	return *this;
}
template<typename T>
inline DexVector3T<T>& DexVector3T<T>::operator -= (const DexVector3T& vector3)
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
inline T DexVector3T<T>::Length() const
{
	return DexMath::Sqrt(x*x + y * y + z * z);
}

template<typename T>
inline T DexVector3T<T>::LengthSq() const
{
	return x * x + y * y + z * z;
}

template<typename T>
inline DexVector3T<T> DexVector3T<T>::Cross(const DexVector3T& vector3) const
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
	T new_x = y * vector3.z - vector3.y *z;
	T new_y = -(x*vector3.z - vector3.x * z);
	T new_z = x * vector3.y - vector3.x*y;
	return DexVector3T<T>(new_x, new_y, new_z);
}

template<typename T>
DexVector3T<T> DexVector3T<T>::S_Cross(const DexVector3T& v1, const DexVector3T& v2)
{
	return v1.Cross(v2);
}

template<typename T>
inline T DexVector3T<T>::Dot(const DexVector3T& vector3) const
{
	T ret = x * vector3.x + y * vector3.y + z * vector3.z;
	return ret;
}

template<typename T>
T DexVector3T<T>::S_Dot(const DexVector3T& v1, const DexVector3T& v2)
{
	return v1.Dot(v2);
}

template<typename T>
T DexVector3T<T>::GetRadian(const DexVector3T& vector) const
{
	T dot = x * vector.x + y * vector.y + z * vector.z;
	T length1 = Length();
	T length2 = vector.Length();
	if (DexMath::Equal(length1, (T)0.0f) ||
		DexMath::Equal(length2, (T)0.0f))
		return (T)0;
	DFloat32 cosf = dot / (length1*length2);
	return DexMath::Acos(cosf);
}

template<typename T>
inline DexVector3T<T> DexVector3T<T>::Invert()
{
	x *= (T)(-1);
	y *= (T)(-1);
	z *= (T)(-1);
	return *this;
}


template<typename T>
inline DexVector3T<T> DexVector3T<T>::GetInvert()	const
{
	DexVector3T<T> ret;
	ret.x = x * (T)(-1);
	ret.y = y * (T)(-1);
	ret.z = z * (T)(-1);
	return ret;
}


template<typename T>
inline bool DexVector3T<T>::operator>(const DexVector3T& vector3) const
{
	return x > vector3.x &&
		y > vector3.y &&
		z > vector3.z;
}
template<typename T>
inline bool DexVector3T<T>::operator>=(const DexVector3T& vector3) const
{
	return x >= vector3.x &&
		y >= vector3.y &&
		z >= vector3.z;
}

template<typename T>
inline bool DexVector3T<T>::operator<(const DexVector3T& vector3) const
{
	return x < vector3.x &&
		y < vector3.y &&
		z < vector3.z;
}

template<typename T>
inline bool DexVector3T<T>::operator<=(const DexVector3T& vector3) const
{
	return x <= vector3.x &&
		y <= vector3.y &&
		z <= vector3.z;
}