
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
	static DFloat32 Sqrt(DFloat32 _value);
	static DFloat32 Abs(DFloat32 _value);
	static bool    Equal(float value1, float value2, float pre = 1e-07f);
	static bool    Equal(DInt32 value1, DInt32 value2);
	
	static DFloat32 Acos(DFloat32 Radian);    
	static DFloat64 Acos(DFloat64 Radian);

	static DFloat32 Cosf(DFloat32 radian);
	static DFloat32 Sinf(DFloat32 radian);
	
};
#endif