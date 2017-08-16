
#ifndef _DEX_STL_VECTOR_H
#define _DEX_STL_VECTOR_H

#include <vector>
#include <algorithm>
#include "DexType.h"


//type
#define DVector	std::vector		
typedef	DVector<DUInt8>	 VectorUInt8;
typedef DVector<DUInt16>	 VectorUInt16;
typedef DVector<DUInt32>	 VectorDUDInt32;
typedef DVector<DUInt64>	 VectorUInt64;

typedef DVector<DInt8>	 VectorInt8;
typedef DVector<DInt16>	 VectorInt16;
typedef DVector<DInt32>	 VectorDInt32;
typedef DVector<DInt64>	 VectorInt64;

typedef DVector<DFloat32>	 VectorFloat32;
typedef DVector<DFloat64>	 VectorFloat64;

template<typename T>
//s2b:small to bigger,DTRUE:从小到大排序 DFALSE:从大到小排序
inline void DVector_Sort(DVector<T>& vec, DBool s2b=DTRUE)
{//使用lambda表达式进行排序
	std::stable_sort(vec.begin(), vec.end(), [s2b](const T& a, const T& b)-> DBool {return s2b ? (a < b) : (a > b); });
}

#endif