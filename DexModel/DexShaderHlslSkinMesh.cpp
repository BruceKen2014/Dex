
#include "../state/DexGameEngine.h"
#include "../DexBase/DexVertexDeclDx9.h"
#include "../DexBase/DexLog.h"
#include "../DexBase/DexDefine.h"
#include "../Source/CTexture.h"
#include "DexShaderHlslSkinMesh.h"
#include "DexSkinMesh.h"


DexShaderHlslSkinMesh::DexShaderHlslSkinMesh()
{
	LPD3DXBUFFER	pbuff = NULL;
	HRESULT hr = D3DXCreateEffectFromFile(DexGameEngine::getEngine()->GetDevice(), "shader\\effect.fx", NULL, NULL, D3DXSHADER_DEBUG, NULL,
		&pFxEffect, &pbuff);
	if (pbuff)
	{
		getLog()->BeginLog();
		getLog()->Log(log_error, (char*)(pbuff->GetBufferPointer()));
		pbuff->Release();
	}
	if (FAILED(hr))
	{
		getLog()->BeginLog();
		getLog()->Log(log_error, "DexShaderHlslSkinMesh D3DXCreateEffectFromFile 调用失败！");
		getLog()->EndLog();
	}
	else
	{
		WVPMatrixHandle = pFxEffect->GetParameterByName(0, "WVPMarix");
		Tex0Handle = pFxEffect->GetParameterByName(0, "Tex0");
		TechHandle = pFxEffect->GetTechniqueByName("T0");
		VertexBlendFlag = pFxEffect->GetParameterByName(0, "VertexBlendFlag");
		m_bLightEffect = pFxEffect->GetParameterByName(0, "g_bLightEffect");
		m_bLightEnable = pFxEffect->GetParameterByName(0, "g_bLightEnable");
		m_biPointLightCount = pFxEffect->GetParameterByName(0, "g_iLightCountPoint");
		m_bPointData = pFxEffect->GetParameterByName(0, "g_LightPoints");
		m_ambientColor = pFxEffect->GetParameterByName(0, "g_ambientColor");
		m_material = pFxEffect->GetParameterByName(0, "g_material");
		JointMatrixHandle = pFxEffect->GetParameterByName(0, "JointsMatrix");
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

DexShaderHlslSkinMesh::~DexShaderHlslSkinMesh()
{
}

const int JointNumber = 40;
void DexShaderHlslSkinMesh::Render()
{
	DEX_ENSURE(m_pTarget != nullptr && m_pTarget->getType() == DexSkinMesh::getClassType());
	DexSkinMesh* skinMesh = (DexSkinMesh*)m_pTarget;
	pFxEffect->SetMatrix(WVPMatrixHandle, &skinMesh->matWVP);
	pFxEffect->SetMatrixArray(JointMatrixHandle, skinMesh->jointsMatrix, JointNumber);
	//得找一下如何在shader中开启alphablend
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//这些属于固定管线的设置可以在shader中设置，所以这里不需要设置
	//D3DXMatrixIdentity(&DexGameEngine::getEngine()->g_worldMatrix);
	//DexGameEngine::getEngine()->GetDevice()->SetTransform(D3DTS_WORLD, &DexGameEngine::getEngine()->g_worldMatrix);
	//DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	DexGameEngine::getEngine()->GetDevice()->SetVertexDeclaration(((DexVertexDeclDx9*)m_pDeclaration)->m_pDecl);
	int lighDSize = 0;
	struct tempPointLight
	{
		DexVector4  color;
		DexVector4  position;
		DexVector4  rangeAtte;
	};
	struct tempMaterial
	{
		DexVector4 diffuse;
		DexVector4 specular;
		DexVector4 ambient;
		DexVector4 emissive;
	};
	tempPointLight tLight;
	tLight.color = DexVector4(1.0f, 1.0f, 1.0f, 1.0f);
	DexVector3 vec = DexGameEngine::getEngine()->getCamera()->GetPosition();
	tLight.position = DexVector4(vec.x, vec.y, vec.z, 1.0f);
	tLight.rangeAtte = DexVector4(500.0f, 0.0f, 0.0f, 0.0f);
	tempPointLight tLight2;
	tLight2.color = DexVector4(0.0f, 1.0f, 0.0f, 1.0f);
	tLight2.position = DexVector4(0.0f, 0.0f, -100.0f, 1.0f);
	tLight2.rangeAtte = DexVector4(1000.0f, 0.0f, 0.0f, 0.0f);
	tempPointLight arrLight[2] = { tLight, tLight2 };

	DexVector4 ambientColor(0.2f, 0.2f, 0.2f, 1.0f);

	tempMaterial material;
	material.diffuse = DexVector4(1.0f, 1.0f, 1.0f, 1.0f);
	material.specular = DexVector4(1.0f, 1.0f, 1.0f, 1.0f);
	material.ambient = DexVector4(1.0f, 1.0f, 1.0f, 1.0f);
	material.emissive = DexVector4(0.0f, 0.0f, 0.0f, 1.0f);
	pFxEffect->SetRawValue(m_material, &material, 0, sizeof(tempMaterial));
	pFxEffect->SetRawValue(m_bPointData, arrLight, 0, sizeof(tempPointLight)* 2);
	pFxEffect->SetFloatArray(m_ambientColor, (float*)&ambientColor, 4);
	pFxEffect->SetInt(m_biPointLightCount, 2);
	pFxEffect->SetBool(m_bLightEffect, true);
	pFxEffect->SetBool(m_bLightEnable, true);
	pFxEffect->SetTechnique(TechHandle);
	pFxEffect->SetInt(VertexBlendFlag, SkinMeshVertexBlend_Blend);
	uint32 pass = 0;
	pFxEffect->Begin(&pass, 0);
	for (size_t i = 0; i < skinMesh->m_vecMeshs.size(); ++i)
	{
		if (skinMesh->m_vecMeshs[i] == NULL)
			continue;
		if (DexGameEngine::getEngine()->GetRenderMode() == DexRenderMode_LINE)
		{
			if (skinMesh->m_vecMeshs[i]->GetLineIndiceCount() == 0)
			{
				skinMesh->m_vecMeshs[i]->CreateLineIndices();
			}
			pFxEffect->SetTexture(Tex0Handle, NULL);
			for (uint32 p = 0; p < pass; ++p)
			{
				pFxEffect->BeginPass(p);
				DexGameEngine::getEngine()->DrawPrimitive(DexPT_LINELIST, skinMesh->m_vecMeshs[i]->GetVertexBuffer(),
					skinMesh->m_vecMeshs[i]->GetVertexCount(), skinMesh->m_vecMeshs[i]->GetLineIndiceBuffer(),
					skinMesh->m_vecMeshs[i]->GetLineIndiceCount() / 2, sizeof(DexSkinMesh::stMeshVertex));
				pFxEffect->EndPass();
			}
		}
		else if (DexGameEngine::getEngine()->GetRenderMode() == DexRenderMode_TRIANGLE)
		{
			if (skinMesh->m_vecMeshs[i]->GetLineIndiceCount() != 0)
			{
				skinMesh->m_vecMeshs[i]->DestroyLineIndices();
			}
			if (skinMesh->m_vecMeshs[i]->m_iMaterialId != -1)
				DexGameEngine::getEngine()->SetMaterial(skinMesh->m_vecMaterials[skinMesh->m_vecMeshs[i]->m_iMaterialId]);
			if (skinMesh->m_vecMeshs[i]->m_iTextureId != -1)
			{
				pFxEffect->SetTexture(Tex0Handle, skinMesh->m_vecTextures[skinMesh->m_vecMeshs[i]->m_iTextureId]->GetTexPt());
			}
			else
			{
				pFxEffect->SetTexture(0, NULL);
			}
			for (uint32 p = 0; p < pass; ++p)
			{
				pFxEffect->BeginPass(p);
				DexGameEngine::getEngine()->DrawPrimitive(DexPT_TRIANGLELIST, skinMesh->m_vecMeshs[i]->GetVertexBuffer(),
					skinMesh->m_vecMeshs[i]->GetVertexCount(), skinMesh->m_vecMeshs[i]->GetIndiceBuffer(),
					skinMesh->m_vecMeshs[i]->GetIndiceCount() / 3, sizeof(DexSkinMesh::stMeshVertex));
				pFxEffect->EndPass();
			}
		}
	}
	pFxEffect->End();
}