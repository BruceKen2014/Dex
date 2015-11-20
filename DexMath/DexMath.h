
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
	static float Sqrt(float _value);
	static float Abs(float _value);
	static bool  equal(float value1, float value2, float pre = 1e-04f);
	static bool  equal(int32 value1, int32 value2);
};
#endif