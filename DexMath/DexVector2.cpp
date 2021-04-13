
#include <vcruntime_string.h>
#include "DexVector2.h"

template class DexVector2T<float>;
template class DexVector2T<DInt32>;

template<typename T>
inline DexVector2T<T>::DexVector2T()
{
	memset(m, 0, sizeof(T) * 2);
}

template<typename T>
inline DexVector2T<T>::DexVector2T(const DexVector2T<T>& vector2) :DexVector2T(vector2.x, vector2.y)
{}

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
inline DexVector2T<T> DexVector2T<T>::Normalize()
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
inline DexVector2T<T> DexVector2T<T>::Set(T* value)
{
	memcpy(m, value, sizeof(T) * 2);
	return *this;
}

template<typename T>
inline DexVector2T<T> DexVector2T<T>::Set(const DexVector2T<T>& vector2)
{
	memcpy(m, vector2.m, sizeof(vector2));
	return *this;
}

template<typename T>
inline DexVector2T<T> DexVector2T<T>::Set(const T& _x, const T& _y)
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
	return *this;
}
template<typename T>
inline bool DexVector2T<T>::operator == (const DexVector2T<T>& vector2)const
{
	return DexMath::Equal(x, vector2.x)
		&& DexMath::Equal(y, vector2.y);
}
template<typename T>
inline bool DexVector2T<T>::operator != (const DexVector2T<T>& vector2)const
{
	return !DexMath::Equal(x, vector2.x)
		|| !DexMath::Equal(y, vector2.y);
}
template<typename T>
inline DexVector2T<T> DexVector2T<T>::operator + (const DexVector2T<T>& vector2)const
{
	DexVector2T<T> ret;
	ret.x = x + vector2.x;
	ret.y = y + vector2.y;
	return ret;
}
template<typename T>
inline DexVector2T<T> DexVector2T<T>::operator - (const DexVector2T<T>& vector2)const
{
	DexVector2T<T> ret;
	ret.x = x - vector2.x;
	ret.y = y - vector2.y;
	return ret;
}
template<typename T>
inline DexVector2T<T> DexVector2T<T>::operator * (float _value)const
{
	DexVector2T<T> ret;
	ret.x = x * _value;
	ret.y = y * _value;
	return ret;
}
template<typename T>
inline DexVector2T<T> DexVector2T<T>::operator / (float _value)const
{
	DexVector2T<T> ret;
	ret.x = x / _value;
	ret.y = y / _value;
	return ret;
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
	return DexMath::Sqrt(x*x + y * y);
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
		y > vector2.y;
}
template<typename T>
inline bool DexVector2T<T>::operator>=(const DexVector2T<T>& vector2) const
{
	return x >= vector2.x &&
		y >= vector2.y;
}

template<typename T>
inline bool DexVector2T<T>::operator<(const DexVector2T<T>& vector2) const
{
	return x < vector2.x &&
		y < vector2.y;
}

template<typename T>
inline bool DexVector2T<T>::operator<=(const DexVector2T<T>& vector2) const
{
	return x <= vector2.x &&
		y <= vector2.y;
}

template<typename T1, typename T>
DexVector2T<T> operator* (T1 _value, const DexVector2T<T>& vector2)
{
	DexVector2T<T> ret;
	ret.x = (T)(vector2.x *  _value);
	ret.y = (T)(vector2.y *  _value);
	return ret;
}

