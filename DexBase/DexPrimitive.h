#ifndef _DEX_PRIMITIVE_H
#define _DEX_PRIMITIVE_H


typedef enum _DexPrimitivetType{
	DexPT_POINTLIST = 1,
	DexPT_LINELIST = 2,
	DexPT_LINESTRIP = 3,
	DexPT_TRIANGLELIST = 4,
	DexPT_TRIANGLESTRIP = 5,
	DexPT_TRIANGLEFAN = 6,
} DexPrimitivetType;

typedef enum _DexRenderMode{
	DexRenderMode_LINE = 1,
	DexRenderMode_TRIANGLE = 2,
	DexRenderMode_TOTAL = 6,
} DexRenderMode;

#endif