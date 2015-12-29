
#ifndef _DEX_STL_VECTOR_H
#define _DEX_STL_VECTOR_H

#include <vector>
#include "DexType.h"

//type
#define Vector	std::vector		
typedef	Vector<uint8>	 VectorUInt8;
typedef Vector<uint16>	 VectorUInt16;
typedef Vector<uint32>	 VectorUInt32;
typedef Vector<uint64>	 VectorUInt64;

typedef Vector<int8>	 VectorInt8;
typedef Vector<int16>	 VectorInt16;
typedef Vector<int32>	 VectorInt32;
typedef Vector<int64>	 VectorInt64;

typedef Vector<float32>	 VectorFloat32;
typedef Vector<float64>	 VectorFloat64;

#endif