
#ifndef _DEX_TYPE_H
#define _DEX_TYPE_H
#include "DexCompiler.h"

using DChar	   =    char;
using DUChar   =	unsigned char;
using DBool    =    bool;
using DByte	   =	unsigned char;
using DInt8    =	signed char  ;
using DUInt8   =	unsigned char;
using DInt16   =	signed short;
using DUInt16  =	unsigned short;
using DInt32   =	signed int;
using DUInt32  =	unsigned int;
using DInt64   =	signed __int64;
using DUInt64  =	unsigned __int64;
using DFloat32 =	float;
using DFloat64 =	double;

#define DTRUE  true
#define DFALS  false
#define DEXINFINITE            0xFFFFFFFF
#ifdef _DEX_PLATFORM_WIN32
using DEXWORD = DUInt32;
#else
using DEXWORD = DUInt64;
#endif

#ifdef _MSC_VER
	#if _MSC_VER >= 1600 //vs2010 and later
	#define  DexNull  nullptr
	#endif
#endif
#ifndef DexNull
#define DexNull NULL
#endif

#endif //_DEX_TYPE_H

