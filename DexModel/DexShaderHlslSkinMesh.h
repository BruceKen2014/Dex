
#ifndef _DEX_SHADER_HLSL_SKINMESH_H
#define _DEX_SHADER_HLSL_SKINMESH_H

#include "../DexBase/IDexShaderHlsl.h"
class DexSkinMesh;
class DexShaderHlslSkinMesh:public IDexShaderHlsl
{
	Dex_DeclareClass(DexShaderHlslSkinMesh, 0)
protected:
	D3DXHANDLE	 m_handleWVPMatrix;
	D3DXHANDLE	 m_handleTex0;
	D3DXHANDLE	 m_handleTech;
	D3DXHANDLE	 m_handleJointMatrix;
	D3DXHANDLE   m_handleLightEnable;
	D3DXHANDLE   m_handleAmbientEnable;
	D3DXHANDLE   m_handlePointLightEnable;
	D3DXHANDLE   m_handleDirectionLightEnable;
	D3DXHANDLE   m_handlePointLightCount;
	D3DXHANDLE   m_handlePointLightData;
	D3DXHANDLE	 m_handleDirectionLightData;
	D3DXHANDLE   m_handleAmbientColor;
	D3DXHANDLE   m_handleMaterial;
public:
	DexShaderHlslSkinMesh();
	virtual ~DexShaderHlslSkinMesh();

public:
	virtual void Render();
};
#endif