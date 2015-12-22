

#include <math.h>
#include "DexMath.h"
#include "DexVector3.h"

float DexMath::Sqrt(float _value)
{
	return sqrt(_value);
}

float DexMath::Abs(float _value)
{
	return abs(_value);
}

bool DexMath::Equal(float value1, float value2, float pre/* = 1e-04f */)
{
	pre = 0.000000001f;
	return abs(value1 - value2) < pre;
}

bool DexMath::Equal(int32 value1, int32 value2)
{
	return value1 == value2;
}

float32 DexMath::Acos(float32 Radian)
{
	return acos(Radian);
}

float64 DexMath::Acos(float64 Radian)
{
	return acos(Radian);
}

float32 DexMath::Cosf(float32 radian)
{
	return cosf(radian);
}

float32 DexMath::Sinf(float32 radian)
{
	return sinf(radian);
}