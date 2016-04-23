
/*
DirectX9 Render
*/
#ifndef _DEX_RENDER_DIRECTX9_H
#define _DEX_RENDER_DIRECTX9_H

#include "IDexRender.h"
class DexRenderDirectX9:public IDexRender
{
public:
	DexRenderDirectX9();
	virtual ~DexRenderDirectX9();
	//for固定管线，需要设置FVF
	virtual void DrawPrimitive(DexPrimitivetType type, const void* vertexs, int32 vertexCount, const void* indices, int32 primitiveCount, int32 fvf, int32 stridesize);
	//for shader，不需要设置FVF
	virtual void DrawPrimitive(DexPrimitivetType type, const void* vertexs, int32 vertexCount, const void* indices, int32 primitiveCount, int32 stridesize);
};
#endif