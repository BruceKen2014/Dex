

#ifndef _DEX_SHADER_H
#define _DEX_SHADER_H

#include "DexDVector.h"
#include "DexColor.h"
#include "DexLight.h"
#include "DexMaterial.h"
#include "IDexVertexDecl.h"
#include "CDexObject.h"


class IDexShader:public CDexObject
{
	Dex_DeclareClass(IDexShader, CDexObject, 0)
protected:
	DUInt8	m_iLightFlag; //ตฦนโ
	//DexMaterial m_Material;
	DexVector4 m_colorAmbient; //rgba

	CDexObject* m_pTarget;
	IDexVertexDecl* m_pDeclaration;
	int			m_iPointLightCount;
	stDexPointLight m_arrPointLights[DEXRENDER_LIGHT_MAX_COUNT];
	stDexDirectionLight m_directionLight;
public:
	IDexShader();
	virtual ~IDexShader();

public:
	//void SetMaterial(const DexMaterial& material);
public:
	void SetTarget(CDexObject* obj){ m_pTarget = obj;}
	virtual void Render() = 0;
};
#endif