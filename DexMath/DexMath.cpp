

#include <math.h>
#include "DexMath.h"
#include "DexVector3.h"

float DexMath::Sqrt(float _value)
{
	return sqrt(_value);
}

float DexMath::Abs(float _value)
{
	return fabsf(_value);
}

bool DexMath::Equal(float value1, float value2, float pre/* = 1e-04f */)
{
	pre = 0.000000001f;
	return fabsf(value1 - value2) < pre;
}

bool DexMath::Equal(DInt32 value1, DInt32 value2)
{
	return value1 == value2;
}

DFloat32 DexMath::Acos(DFloat32 Radian)
{
	return acos(Radian);
}

DFloat64 DexMath::Acos(DFloat64 Radian)
{
	return acos(Radian);
}

DFloat32 DexMath::Cosf(DFloat32 radian)
{
	return cosf(radian);
}

DFloat32 DexMath::Sinf(DFloat32 radian)
{
	return sinf(radian);
}