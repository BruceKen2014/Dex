
#include "../state/DexGameEngine.h"
#include "../DexBase/DexVertexDeclDx9.h"
#include "../DexBase/DexLog.h"
#include "../DexBase/DexDefine.h"
#include "../DexBase/DexRenderDirectx9.h"
#include "../Source/CTexture.h"
#include "DexShaderHlslSkinMeshVertexToJoint.h"
#include "DexShaderHlslSkinMesh.h"
#include "DexSkinMesh.h"


DexShaderHlslSkinMeshVertexToJoint::DexShaderHlslSkinMeshVertexToJoint()
{
	LPD3DXBUFFER	pbuff = NULL;
	HRESULT hr = D3DXCreateEffectFromFile(DexGameEngine::getEngine()->GetDevice(), "shader\\skinMeshVertexToJoint.fx", 
		NULL, NULL, D3DXSHADER_DEBUG, NULL, &pFxEffect, &pbuff);
	if (pbuff)
	{
		DexLog::getSingleton()->BeginLog();
		DexLog::getSingleton()->Log(log_error, (char*)(pbuff->GetBufferPointer()));
		pbuff->Release();
	}
	if (FAILED(hr))
	{
		DexLog::getSingleton()->BeginLog();
		DexLog::getSingleton()->Log(log_error, "DexShaderHlslSkinMesh D3DXCreateEffectFromFile µ÷ÓÃÊ§°Ü£¡");
		DexLog::getSingleton()->EndLog();
	}
	else
	{
		WVPMatrixHandle = pFxEffect->GetParameterByName(0, "WVPMarix");
		JointMatrixHandle = pFxEffect->GetParameterByName(0, "JointsMatrix");
		TechHandle = pFxEffect->GetTechniqueByName("T0");
		//JointInvertMatrixHandle = pFxEffect->GetParameterByName(0, "JointsMatrixInvert");
	}
	D3DVERTEXELEMENT9 decl[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		{ 0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 36, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
		{ 0, 40, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
		//{ 0, 60, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
		//{ 0, 64, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
		D3DDECL_END()
	};
	m_pDeclaration = new DexVertexDeclDx9(DexGameEngine::getEngine()->GetDevice(), decl);
}

DexShaderHlslSkinMeshVertexToJoint::~DexShaderHlslSkinMeshVertexToJoint()
{
}


void DexShaderHlslSkinMeshVertexToJoint::Render()
{
	DEX_ENSURE(m_pTarget != nullptr && m_pTarget->getType() == DexSkinMesh::getClassType());
	DexSkinMesh* skinMesh = (DexSkinMesh*)m_pTarget;
	pFxEffect->SetMatrix(WVPMatrixHandle, &skinMesh->matWVP);
	pFxEffect->SetMatrixArray(JointMatrixHandle, skinMesh->jointsMatrix, DexSkinMesh::sGetMaxJointCount());

	DexGameEngine::getEngine()->setDexVertexDecl(m_pDeclaration);
	pFxEffect->SetTechnique(TechHandle);
	DUInt32 pass = 0;
	pFxEffect->Begin(&pass, 0);
	for (size_t i = 0; i < skinMesh->m_vecMeshs.size(); ++i)
	{
		if (skinMesh->m_vecMeshs[i] == NULL)
			continue;
		for (DUInt32 p = 0; p < pass; ++p)
		{
			pFxEffect->BeginPass(p);
			DexGameEngine::getEngine()->DrawPrimitive(DexPT_LINELIST, skinMesh->m_vecMeshs[i]->GetVertexToJointBuffer(),
				skinMesh->m_vecMeshs[i]->GetVertexToJointBufferCount(), skinMesh->m_vecMeshs[i]->GetVertexToJointIndiceBuffer(),
				skinMesh->m_vecMeshs[i]->GetVertexToJointIndiceCount() / 2, sizeof(DexSkinMesh::stMeshVertex));
			pFxEffect->EndPass();
		}
	}
	pFxEffect->End();
}