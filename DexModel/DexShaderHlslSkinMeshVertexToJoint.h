
#ifndef _DEX_SHADER_HLSL_SKINMESH_VERTEXTOJOINT_H
#define _DEX_SHADER_HLSL_SKINMESH_VERTEXTOJOINT_H

#include "../DexBase/IDexShaderHlsl.h"
class DexSkinMesh;
class DexShaderHlslSkinMeshVertexToJoint :public IDexShaderHlsl
{
	Dex_DeclareClass(DexShaderHlslSkinMeshVertexToJoint, 0)
protected:
	D3DXHANDLE	 WVPMatrixHandle;
	D3DXHANDLE	 JointMatrixHandle;
	D3DXHANDLE	 TechHandle;
public:
	DexShaderHlslSkinMeshVertexToJoint();
	virtual ~DexShaderHlslSkinMeshVertexToJoint();

public:
	virtual void Render();
};
#endif