
#ifndef _DEX_SHADER_HLSL_SKINMESH_VERTEXNORMAL_H
#define _DEX_SHADER_HLSL_SKINMESH_VERTEXNORMAL_H

#include "../DexBase/IDexShaderHlsl.h"
class DexSkinMesh;
class DexShaderHlslSkinMeshVertexNormal :public IDexShaderHlsl
{
	Dex_DeclareClass(DexShaderHlslSkinMeshVertexNormal, IDexShaderHlsl,0)
protected:
	D3DXHANDLE	 WVPMatrixHandle;
	D3DXHANDLE	 JointMatrixHandle;
	D3DXHANDLE	 TechHandle;
public:
	DexShaderHlslSkinMeshVertexNormal();
	virtual ~DexShaderHlslSkinMeshVertexNormal();

public:
	virtual void Render();
};
#endif