/*
*DexEngine数学函数调用接口
*create by Bruce
*Date:2015.12.15
*/

#ifndef _DEX_MATH2_H_
#define _DEX_MATH2_H_
#include <d3dx9math.h>
#include "DexVector3.h"
class DexMath2
{
public:
	static bool    Equal(const D3DXVECTOR3& dxVec, const DexVector3& dexVec);
};
#endif