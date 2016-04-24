

#ifndef _I_DEX_RENDER_H_
#define _I_DEX_RENDER_H_


#include "DexPrimitive.h"
#include "IDexShader.h"
#include "DexDMap.h"
#include "DexString.h"

class IDexRender
{
protected:

	DMap<DString, IDexShader*> m_mapShaders;
	IDexShader* m_pShader;

public:
	IDexRender();
	virtual ~IDexRender();
public:
	virtual bool InitShader() = 0;
public:
	IDexShader* GetShader(DString shader);
	void SetShader(IDexShader* shader);
	virtual void RenderShader();


	//for固定管线，需要设置FVF
	virtual void DrawPrimitive(DexPrimitivetType type, const void* vertexs, int32 vertexCount, const void* indices, int32 primitiveCount, int32 fvf, int32 stridesize) = 0;
	//for shader，不需要设置FVF
	virtual void DrawPrimitive(DexPrimitivetType type, const void* vertexs, int32 vertexCount, const void* indices, int32 primitiveCount, int32 stridesize) = 0;
};
#endif 