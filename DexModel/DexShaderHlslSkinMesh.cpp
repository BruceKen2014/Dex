
#include "../state/DexGameEngine.h"
#include "../DexBase/DexVertexDeclDx9.h"
#include "../DexBase/DexLog.h"
#include "../DexBase/DexDefine.h"
#include "../DexBase/DexRenderDirectx9.h"
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
		m_handleWVPMatrix = pFxEffect->GetParameterByName(0, "WVPMarix");
		m_handleTex0 = pFxEffect->GetParameterByName(0, "Tex0");
		m_handleTech = pFxEffect->GetTechniqueByName("T0");
		m_handleLightEnable = pFxEffect->GetParameterByName(0, "g_bLightEnable");
		m_handleAmbientEnable = pFxEffect->GetParameterByName(0, "g_bAmbientEnable");
		m_handlePointLightEnable = pFxEffect->GetParameterByName(0, "g_bPointLightEnable");
		m_handleDirectionLightEnable = pFxEffect->GetParameterByName(0, "g_bDirectionLightEnable");
		m_handlePointLightCount = pFxEffect->GetParameterByName(0, "g_iLightCountPoint");
		m_handlePointLightData = pFxEffect->GetParameterByName(0, "g_LightPoints");
		m_handleDirectionLightData = pFxEffect->GetParameterByName(0, "g_LightDirection");
		m_handleAmbientColor = pFxEffect->GetParameterByName(0, "g_ambientColor");
		m_handleMaterial = pFxEffect->GetParameterByName(0, "g_material");
		m_handleJointMatrix = pFxEffect->GetParameterByName(0, "JointsMatrix");
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

void DexShaderHlslSkinMesh::Render()
{
	DEX_ENSURE(m_pTarget != nullptr);
	DexSkinMesh* skinMesh = (DexSkinMesh*)m_pTarget;
	pFxEffect->SetMatrix(m_handleWVPMatrix, &skinMesh->matWVP);
	pFxEffect->SetMatrixArray(m_handleJointMatrix, skinMesh->jointsMatrix, DexSkinMesh::sGetMaxJointCount());
	m_iPointLightCount = skinMesh->m_vecPointLight.size();
	m_iLightFlag = skinMesh->m_iLightFlag;
	if (m_iPointLightCount != 0)
		memcpy(m_arrPointLights, &skinMesh->m_vecPointLight[0], sizeof(stDexPointLight)*m_iPointLightCount);
	m_directionLight = skinMesh->m_directionLight;
	m_colorAmbient.x = skinMesh->m_ambientColor.r / 255.0f;
	m_colorAmbient.y = skinMesh->m_ambientColor.g / 255.0f;
	m_colorAmbient.z = skinMesh->m_ambientColor.b / 255.0f;
	m_colorAmbient.w = skinMesh->m_ambientColor.a / 255.0f;
	/*
	在shader中开启alphablend
	AlphaBlendEnable = true;
	SrcBlend = SrcAlpha;
	DestBlend = InvSrcAlpha;
	*/
	//这些属于固定管线的设置可以在shader中设置，所以这里不需要设置
	//D3DXMatrixIdentity(&DexGameEngine::getEngine()->g_worldMatrix);
	//DexGameEngine::getEngine()->GetDevice()->SetTransform(D3DTS_WORLD, &DexGameEngine::getEngine()->g_worldMatrix);
	//DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	DexGameEngine::getEngine()->GetDevice()->SetVertexDeclaration(((DexVertexDeclDx9*)m_pDeclaration)->m_pDecl);
	struct tempMaterial
	{
		DexVector4 diffuse;
		DexVector4 specular;
		DexVector4 ambient;
		DexVector4 emissive;
	};

	tempMaterial material;

	pFxEffect->SetFloatArray(m_handleAmbientColor, (float*)&m_colorAmbient, 4);
	pFxEffect->SetBool(m_handleLightEnable, m_iLightFlag & DEXRENDER_LIGHT_ENABLE);
	pFxEffect->SetBool(m_handleAmbientEnable, m_iLightFlag & DEXRENDER_LIGHT_AMBIENT);
	pFxEffect->SetBool(m_handlePointLightEnable, m_iLightFlag & DEXRENDER_LIGHT_POINT);
	pFxEffect->SetBool(m_handleDirectionLightEnable, m_iLightFlag & DEXRENDER_LIGHT_DIRECTION);
	pFxEffect->SetInt(m_handlePointLightCount, m_iPointLightCount);
	pFxEffect->SetRawValue(m_handlePointLightData, m_arrPointLights, 0, sizeof(stDexPointLight)* m_iPointLightCount);
	pFxEffect->SetRawValue(m_handleDirectionLightData, &m_directionLight, 0, sizeof(m_directionLight));

	pFxEffect->SetTechnique(m_handleTech);
	uint32 pass = 0;
	pFxEffect->Begin(&pass, 0);
	//for (int i = skinMesh->m_vecMeshs.size()-1; i >=0; --i)
	for (size_t i = 0; i < skinMesh->m_vecMeshs.size(); ++i)
	{
		if (skinMesh->m_vecMeshs[i] == NULL)
			continue;
		if (skinMesh->iHideMeshIndex == i)
			continue;
		if (DexGameEngine::getEngine()->GetRenderMode() == DexRenderMode_LINE)
		{
			if (skinMesh->m_vecMeshs[i]->GetLineIndiceCount() == 0)
			{
				skinMesh->m_vecMeshs[i]->CreateLineIndices();
			}
			pFxEffect->SetTexture(m_handleTex0, NULL);
			for (uint32 p = 0; p < pass; ++p)
			{
				pFxEffect->BeginPass(p);
				DexGameEngine::getEngine()->GetRender()->DrawPrimitive(DexPT_LINELIST, skinMesh->m_vecMeshs[i]->GetVertexBuffer(),
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
			{
				material.diffuse.x = skinMesh->m_vecMaterials[skinMesh->m_vecMeshs[i]->m_iMaterialId].diffuse.r / 255.0f;
				material.diffuse.y = skinMesh->m_vecMaterials[skinMesh->m_vecMeshs[i]->m_iMaterialId].diffuse.g / 255.0f;
				material.diffuse.z = skinMesh->m_vecMaterials[skinMesh->m_vecMeshs[i]->m_iMaterialId].diffuse.b / 255.0f;
				material.diffuse.w = skinMesh->m_vecMaterials[skinMesh->m_vecMeshs[i]->m_iMaterialId].diffuse.a / 255.0f;

				material.specular.x = skinMesh->m_vecMaterials[skinMesh->m_vecMeshs[i]->m_iMaterialId].specular.r / 255.0f;
				material.specular.y = skinMesh->m_vecMaterials[skinMesh->m_vecMeshs[i]->m_iMaterialId].specular.g / 255.0f;
				material.specular.z = skinMesh->m_vecMaterials[skinMesh->m_vecMeshs[i]->m_iMaterialId].specular.b / 255.0f;
				material.specular.w = skinMesh->m_vecMaterials[skinMesh->m_vecMeshs[i]->m_iMaterialId].specular.a / 255.0f;

				material.ambient.x = skinMesh->m_vecMaterials[skinMesh->m_vecMeshs[i]->m_iMaterialId].ambient.r / 255.0f;
				material.ambient.y = skinMesh->m_vecMaterials[skinMesh->m_vecMeshs[i]->m_iMaterialId].ambient.g / 255.0f;
				material.ambient.z = skinMesh->m_vecMaterials[skinMesh->m_vecMeshs[i]->m_iMaterialId].ambient.b / 255.0f;
				material.ambient.w = skinMesh->m_vecMaterials[skinMesh->m_vecMeshs[i]->m_iMaterialId].ambient.a / 255.0f;

				material.emissive.x = skinMesh->m_vecMaterials[skinMesh->m_vecMeshs[i]->m_iMaterialId].emissive.r / 255.0f;
				material.emissive.y = skinMesh->m_vecMaterials[skinMesh->m_vecMeshs[i]->m_iMaterialId].emissive.g / 255.0f;
				material.emissive.z = skinMesh->m_vecMaterials[skinMesh->m_vecMeshs[i]->m_iMaterialId].emissive.b / 255.0f;
				material.emissive.w = skinMesh->m_vecMaterials[skinMesh->m_vecMeshs[i]->m_iMaterialId].emissive.a / 255.0f;

				pFxEffect->SetRawValue(m_handleMaterial, &material, 0, sizeof(tempMaterial));
			}
			if (skinMesh->m_vecMeshs[i]->m_iTextureId != -1)
			{
				if (skinMesh->m_vecMeshs[i]->m_iTextureId < skinMesh->m_vecTextures.size() &&
					skinMesh->m_vecTextures[skinMesh->m_vecMeshs[i]->m_iTextureId] != nullptr)
					pFxEffect->SetTexture(m_handleTex0, skinMesh->m_vecTextures[skinMesh->m_vecMeshs[i]->m_iTextureId]->GetTexPt());
			}
			else
			{
				pFxEffect->SetTexture(0, NULL);
			}
			for (uint32 p = 0; p < pass; ++p)
			{
				pFxEffect->BeginPass(p);
				DexGameEngine::getEngine()->GetRender()->DrawPrimitive(DexPT_TRIANGLELIST, skinMesh->m_vecMeshs[i]->GetVertexBuffer(),
					skinMesh->m_vecMeshs[i]->GetVertexCount(), skinMesh->m_vecMeshs[i]->GetIndiceBuffer(),
					skinMesh->m_vecMeshs[i]->GetIndiceCount() / 3, sizeof(DexSkinMesh::stMeshVertex));
				pFxEffect->EndPass();
			}
		}
	}
	pFxEffect->End();
}