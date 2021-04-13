
/*
*DexEngine Vector3
*create by Bruce
*Date:2015.11.7
*/
#ifndef _DEX_MATH_VECTOR3_H
#define _DEX_MATH_VECTOR3_H

#include <memory.h>
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
	DexVector3T<T>	Invert();
	DexVector3T<T>  GetInvert()const;
	DexVector3T<T>  Set(T* value);//一个3元素数组
	DexVector3T<T>  Set(const DexVector3T<T>& vector3);
	DexVector3T<T>  Set(const T& _x, const T& _y, const T& _z);
public:
	//for vector
	T Length() const;
	T LengthSq() const; //length square
	DexVector3T<T>  Cross(const DexVector3T<T>& vector3) const;
	static DexVector3T<T>  S_Cross(const DexVector3T<T>& v1, const DexVector3T<T>& v2);
	T				Dot(const DexVector3T<T>& vector3) const;
	static T  S_Dot(const DexVector3T<T>& v1, const DexVector3T<T>& v2);
	T		  GetRadian(const DexVector3T<T> vector) const; //取得与vector之间的夹角，单位弧度
	DexVector3T<T>  Normalize();
	//for point
	T Distance(const DexVector3T<T>& Point)const;
	static T S_Distance(const DexVector3T<T>& Point1, const DexVector3T<T>& Point2) ;
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

template<typename T1, typename T>
DexVector3T<T> operator* (T1 _value, const DexVector3T<T>& vector3)
{
	DexVector3T<T> ret;
	ret.x = (T)(vector3.x *  _value);
	ret.y = (T)(vector3.y *  _value);
	ret.z = (T)(vector3.z *  _value);
	return ret;
}

extern template class DexVector3T<float>;
extern template class DexVector3T<DInt32>;
typedef DexVector3T<float>  DexVector3;
typedef DexVector3T<DInt32>  DexVector3_DInt32;
#endif