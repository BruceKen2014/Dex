
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
	DexVector2T<T>  Normalize();
	DexVector2T<T>  Set(T* value);//一个3元素数组
	DexVector2T<T>  Set(const DexVector2T<T>& vector2);
	DexVector2T<T>  Set(const T& _x, const T& _y);
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

extern template class DexVector2T<float>;
extern template class DexVector2T<DInt32>;

typedef DexVector2T<float>  DexVector2;
typedef DexVector2T<DInt32>  DexVector2_DInt32;
#endif