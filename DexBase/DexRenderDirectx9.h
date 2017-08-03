
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

public:
	virtual bool InitShader();

public:
	//for固定管线，需要设置FVF
	virtual void DrawPrimitive(DexPrimitivetType type, const void* vertexs, DInt32 vertexCount, const void* indices, DInt32 primitiveCount, DInt32 fvf, DInt32 stridesize);
	//for shader，不需要设置FVF
	virtual void DrawPrimitive(DexPrimitivetType type, const void* vertexs, DInt32 vertexCount, const void* indices, DInt32 primitiveCount, DInt32 stridesize);
};
#endif