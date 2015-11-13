
/*
*DexEngine Vector3
*create by Bruce
*Date:2015.11.7
*/
#ifndef _DEX_MATH_VECTOR3_H
#define _DEX_MATH_VECTOR3_H

#include "DexMath.h"
class DexVector3
{
public:
	float x, y, z;

public:
	DexVector3();
	DexVector3(float _x, float _y, float _z);
	~DexVector3();
public:
	float Length();
public:
	DexVector3& operator = (const DexVector3& vector3);
	bool operator == (const DexVector3& vector3)const;
	bool operator != (const DexVector3& vector3)const;
	DexVector3 operator + (const DexVector3& vector3)const;
	DexVector3 operator - (const DexVector3& vector3)const;
	DexVector3 operator * (float _value)const;
	DexVector3 operator / (float _value)const;

	DexVector3& operator += (const DexVector3& vector3);
	DexVector3& operator -= (const DexVector3& vector3);
	DexVector3& operator *= (float _value);
	DexVector3& operator /= (float _value);
public:
	static void Normalize(DexVector3& vector3);

};
DexVector3 operator* (float _value, const DexVector3& vector3);
inline DexVector3& DexVector3::operator = (const DexVector3& vector3)
{
	x = vector3.x;
	y = vector3.y;
	z = vector3.z;
}
inline bool DexVector3::operator == (const DexVector3& vector3)const
{
	return DexMath::Abs(x-vector3.x) < 0.0001f
		&& DexMath::Abs(y-vector3.y) < 0.0001f
		&& DexMath::Abs(z-vector3.z) < 0.0001f;
}
inline bool DexVector3::operator != (const DexVector3& vector3)const
{
	return DexMath::Abs(x-vector3.x) > 0.0001f
		|| DexMath::Abs(y-vector3.y) > 0.0001f
		|| DexMath::Abs(z-vector3.z) > 0.0001f;
}
inline DexVector3 DexVector3::operator + (const DexVector3& vector3)const
{
	DexVector3 ret;
	ret.x = x + vector3.x;
	ret.y = y + vector3.y;
	ret.z = z + vector3.z;
}
inline DexVector3 DexVector3::operator - (const DexVector3& vector3)const
{
	DexVector3 ret;
	ret.x = x - vector3.x;
	ret.y = y - vector3.y;
	ret.z = z - vector3.z;
}
inline DexVector3 DexVector3::operator * (float _value)const
{
	DexVector3 ret;
	ret.x = x * _value;
	ret.y = y * _value;
	ret.z = z * _value;
}
inline DexVector3 DexVector3::operator / (float _value)const
{
	DexVector3 ret;
	ret.x = x / _value;
	ret.y = y / _value;
	ret.z = z / _value;
}

inline DexVector3& DexVector3::operator += (const DexVector3& vector3)
{
	x = x + vector3.x;
	y = y + vector3.y;
	z = z + vector3.z;
	return *this;
}
inline DexVector3& DexVector3::operator -= (const DexVector3& vector3)
{
	x = x - vector3.x;
	y = y - vector3.y;
	z = z - vector3.z;
	return *this;
}
inline DexVector3& DexVector3::operator *= (float _value)
{
	x = x * _value;
	y = y * _value;
	z = z * _value;
	return *this;
}
inline DexVector3& DexVector3::operator /= (float _value)
{
	x = x / _value;
	y = y / _value;
	z = z / _value;
	return *this;
}

inline float DexVector3::Length()
{
	return DexMath::Sqrt(x*x + y*y + z*z);
}

#endif