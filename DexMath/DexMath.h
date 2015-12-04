
/*
*DexEngine数学函数调用接口
*create by Bruce
*Date:2015.11.7
*/

#ifndef _DEX_MATH_H_
#define _DEX_MATH_H_
#include "../DexBase/DexType.h"
class DexMath
{
public:
	static float32 Sqrt(float32 _value);
	static float32 Abs(float32 _value);
	static bool    Equal(float value1, float value2, float pre = 1e-04f);
	static bool    Equal(int32 value1, int32 value2);
	static float32 Acos(float32 Radian);
	static float64 Acos(float64 Radian);

	static float32 Cosf(float32 radian);
	static float32 Sinf(float32 radian);
};
#endif