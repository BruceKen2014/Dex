
#ifndef _DEX_STL_VECTOR_H
#define _DEX_STL_VECTOR_H

#include <vector>
#include "DexType.h"

//type
#define DVector	std::vector		
typedef	DVector<DUInt8>	 VectorUInt8;
typedef DVector<DUInt16>	 VectorUInt16;
typedef DVector<DUDInt32>	 VectorDUDInt32;
typedef DVector<DUInt64>	 VectorUInt64;

typedef DVector<DInt8>	 VectorInt8;
typedef DVector<DInt16>	 VectorInt16;
typedef DVector<DInt32>	 VectorDInt32;
typedef DVector<DInt64>	 VectorInt64;

typedef DVector<DFloat32>	 VectorFloat32;
typedef DVector<DFloat64>	 VectorFloat64;

#endif