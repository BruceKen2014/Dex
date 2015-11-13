
#include "../state/DexGameEngine.h"
#include "../Source/CTexture.h"
#include "DexModelBase.h"


DexModelBase::DexModelBase()
{
	m_pMeshes = NULL;
	m_pMaterials = NULL;
	m_pTriangles = NULL;
	m_pVertices = NULL;
	m_textureList = NULL;
}

DexModelBase::~DexModelBase()
{
	_SafeDeleteArr(m_pMeshes);
	_SafeDeleteArr(m_pMaterials);
	_SafeDeleteArr(m_pTriangles);
	_SafeDeleteArr(m_pVertices);
	_SafeDeleteArr(m_textureList);
}

bool DexModelBase::Update(int delta)
{
	return CDexObject::Update(delta);
}

void _temp_set_color_value(D3DCOLORVALUE& value, float* f)
{
	value.r = f[0];
	value.g = f[1];
	value.b = f[2];
	value.a = f[3];
}
bool DexModelBase::Render()
{
	D3DXMatrixTranslation(&g_transMatrix, 0, 0, 0);
	DexGameEngine::getEngine()->GetDevice()->SetTransform( D3DTS_WORLD, &g_transMatrix);
	DexGameEngine::getEngine()->GetDevice()->SetFVF(FVF_XYZ_DIFF_TEX1 );	  
	// Draw by group
	for ( int i = 0; i < m_numMeshes; i++ )
	{
		int materialIndex = m_pMeshes[i].m_materialIndex;
		D3DMATERIAL9 dexMaterial;
		_temp_set_color_value(dexMaterial.Ambient, m_pMaterials[materialIndex].m_ambient);
		_temp_set_color_value(dexMaterial.Diffuse, m_pMaterials[materialIndex].m_diffuse);
		_temp_set_color_value(dexMaterial.Specular, m_pMaterials[materialIndex].m_specular);
		_temp_set_color_value(dexMaterial.Emissive, m_pMaterials[materialIndex].m_emissive);
		dexMaterial.Power = m_pMaterials[materialIndex].m_shininess;
		DexGameEngine::getEngine()->GetDevice()->SetMaterial(&dexMaterial);
		DexGameEngine::getEngine()->GetDevice()->SetTexture(0,m_textureList[i].GetTexPt());
		for ( int j = 0; j < m_pMeshes[i].m_numTriangles; j++ )
		{
			int triangleIndex = m_pMeshes[i].m_pTriangleIndices[j];
			const Triangle* pTri = &m_pTriangles[triangleIndex];

			for(int k = 0 ;k < 3; ++k)
			{
				int index = pTri->m_vertexIndices[k];
				g_pVertexList1[k].m_pos.x = m_pVertices[index].m_location.x;
				g_pVertexList1[k].m_pos.y = m_pVertices[index].m_location.y;
				g_pVertexList1[k].m_pos.z = m_pVertices[index].m_location.z;
				g_pVertexList1[k].m_u = pTri->m_s[k];
				g_pVertexList1[k].m_v = pTri->m_t[k];
				g_pVertexList1[k].m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
			}

			DexGameEngine::getEngine()->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, g_pVertexList1, sizeof(stVertex1));
		}
	}
	return true;
}
