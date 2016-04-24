
#ifndef _DEX_STL_VECTOR_H
#define _DEX_STL_VECTOR_H

#include <vector>
#include "DexType.h"

//type
#define DVector	std::vector		
typedef	DVector<uint8>	 VectorUInt8;
typedef DVector<uint16>	 VectorUInt16;
typedef DVector<uint32>	 VectorUInt32;
typedef DVector<uint64>	 VectorUInt64;

typedef DVector<int8>	 VectorInt8;
typedef DVector<int16>	 VectorInt16;
typedef DVector<int32>	 VectorInt32;
typedef DVector<int64>	 VectorInt64;

typedef DVector<float32>	 VectorFloat32;
typedef DVector<float64>	 VectorFloat64;

#endif