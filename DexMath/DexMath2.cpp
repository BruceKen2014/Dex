

#include "DexMath2.h"
#include "DexMath.h"
bool DexMath2::Equal(const D3DXVECTOR3& dxVec, const DexVector3& dexVec)
{
	return DexMath::Equal(dxVec.x, dexVec.x) && DexMath::Equal(dxVec.y, dexVec.y) 
		&& DexMath::Equal(dxVec.z, dexVec.z);
}