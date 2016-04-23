

#ifndef _I_DEX_RENDER_H_
#define _I_DEX_RENDER_H_

#include "DexLight.h"
#include "DexMaterial.h"
#include "DexPrimitive.h"
#include "DexColor.h"
#include "IDexShader.h"
#include "DexSTLVector.h"
#define DEXRENDER_LIGHT_AMBIENT   (1 << 0)
#define DEXRENDER_LIGHT_POINT	  (1 << 1)
#define DEXRENDER_LIGHT_DIRECTION (1 << 2)
class IDexRender
{
protected:
	char	m_iLightFlag; //灯光
	DexMaterial m_Material;
	DexColor m_colorAmbient;
	Vector<stDexPointLight> m_vecPointLights;
	IDexShader* m_pShader;
public:
	IDexRender();
	virtual ~IDexRender();

public:
	void SetMaterial(const DexMaterial& material);
	void SetLightFlag(char flag);
	void SetAmbient(const DexColor ambient);
	void SetShader(IDexShader* shader);
	virtual void RenderShader();


	//for固定管线，需要设置FVF
	//virtual void DrawPrimitive(DexPrimitivetType type, const void* vertexs, int32 vertexCount, const void* indices, int32 primitiveCount, int32 fvf, int32 stridesize) = 0;
	//for shader，不需要设置FVF
	//virtual void DrawPrimitive(DexPrimitivetType type, const void* vertexs, int32 vertexCount, const void* indices, int32 primitiveCount, int32 stridesize) = 0;
};
#endif 