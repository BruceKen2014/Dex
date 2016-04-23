
#ifndef _DEX_SHADER_HLSL_SKINMESH_H
#define _DEX_SHADER_HLSL_SKINMESH_H

#include "../DexBase/IDexShaderHlsl.h"
class DexSkinMesh;
class DexShaderHlslSkinMesh:public IDexShaderHlsl
{
protected:
	D3DXHANDLE	 WVPMatrixHandle;
	D3DXHANDLE	 Tex0Handle;
	D3DXHANDLE	 TechHandle;
	D3DXHANDLE   VertexBlendFlag;
	D3DXHANDLE	 JointMatrixHandle;
	D3DXHANDLE   m_bLightEffect;
	D3DXHANDLE   m_bLightEnable;
	D3DXHANDLE   m_biPointLightCount;
	D3DXHANDLE   m_bPointData;
	D3DXHANDLE   m_ambientColor;
	D3DXHANDLE   m_material;
public:
	DexShaderHlslSkinMesh();
	virtual ~DexShaderHlslSkinMesh();

public:
	virtual void Render();
};
#endif