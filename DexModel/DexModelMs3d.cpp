
#include <fstream>
#include "../state/DexGameEngine.h"
#include "../Source/CTexture.h"
#include "DexModelMs3d.h"

DexModelMs3d::DexModelMs3d()
{
	m_pMeshes = NULL;
	m_pMaterials = NULL;
	m_pTriangles = NULL;
	m_pVertices = NULL;
	m_textureList = NULL;
}

DexModelMs3d::~DexModelMs3d()
{
	_SafeDeleteArr(m_pMeshes);
	_SafeDeleteArr(m_pMaterials);
	_SafeDeleteArr(m_pTriangles);
	_SafeDeleteArr(m_pVertices);
	_SafeDeleteArr(m_textureList);
}

bool DexModelMs3d::Update(int delta)
{
	return DexModelBase::Update(delta);
}
void _temp_set_color_value(D3DCOLORVALUE& value, float* f)
{
	value.r = f[0];
	value.g = f[1];
	value.b = f[2];
	value.a = f[3];
}
bool DexModelMs3d::Render()
{
	D3DXMatrixTranslation(&g_transMatrix, 0, 0, 0);
	DexGameEngine::getEngine()->GetDevice()->SetTransform(D3DTS_WORLD, &g_transMatrix);
	DexGameEngine::getEngine()->GetDevice()->SetFVF(FVF_XYZ_DIFF_TEX1);
	// Draw by group
	for (int i = 0; i < m_numMeshes; i++)
	{
		int materialIndex = m_pMeshes[i].m_materialIndex;
		D3DMATERIAL9 dexMaterial;
		_temp_set_color_value(dexMaterial.Ambient, m_pMaterials[materialIndex].m_ambient);
		_temp_set_color_value(dexMaterial.Diffuse, m_pMaterials[materialIndex].m_diffuse);
		_temp_set_color_value(dexMaterial.Specular, m_pMaterials[materialIndex].m_specular);
		_temp_set_color_value(dexMaterial.Emissive, m_pMaterials[materialIndex].m_emissive);
		dexMaterial.Power = m_pMaterials[materialIndex].m_shininess;
		DexGameEngine::getEngine()->GetDevice()->SetMaterial(&dexMaterial);
		DexGameEngine::getEngine()->GetDevice()->SetTexture(0, m_textureList[i].GetTexPt());
		for (int j = 0; j < m_pMeshes[i].m_numTriangles; j++)
		{
			int triangleIndex = m_pMeshes[i].m_pTriangleIndices[j];
			const Triangle* pTri = &m_pTriangles[triangleIndex];

			for (int k = 0; k < 3; ++k)
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
	return DexModelBase::Render();
}

bool DexModelMs3d::LoadModel(const char* filename)
{
	ifstream inputFile( filename, ios::in | ios::binary | ios::_Nocreate );
	if ( inputFile.fail())
		return false;	// "Couldn't open the model file."

	inputFile.seekg( 0, ios::end );
	long fileSize = inputFile.tellg();
	inputFile.seekg( 0, ios::beg );

	byte *pBuffer = new byte[fileSize];
	inputFile.read((char*) pBuffer, fileSize );
	inputFile.close();

	const byte *pPtr = pBuffer;
	MS3DHeader *pHeader = ( MS3DHeader* )pPtr;
	pPtr += sizeof( MS3DHeader );

	if ( strncmp( pHeader->m_ID, "MS3D000000", 10 ) != 0 )
		return false; // "Not a valid Milkshape3D model file."

	if ( pHeader->m_version < 3 || pHeader->m_version > 4 )
		return false; // "Unhandled file version. Only Milkshape3D Version 1.3 and 1.4 is supported." );

	int nVertices = *( int16* )pPtr; 
	m_numVertices = nVertices;
	m_pVertices = new Vertex[nVertices];
	pPtr += sizeof( int16 );

	int i;
	for ( i = 0; i < nVertices; i++ )
	{
		MS3DVertex *pVertex = ( MS3DVertex* )pPtr;
		m_pVertices[i].m_boneID = pVertex->m_boneID;
		memcpy(&m_pVertices[i].m_location, pVertex->m_vertex, sizeof( m_pVertices[i].m_location ));
		pPtr += sizeof( MS3DVertex );
	}

	int nTriangles = *( int16* )pPtr;
	m_numTriangles = nTriangles;
	m_pTriangles = new Triangle[nTriangles];
	pPtr += sizeof( int16 );

	for ( i = 0; i < nTriangles; i++ )
	{
		MS3DTriangle *pTriangle = ( MS3DTriangle* )pPtr;
		int vertexIndices[3] = { pTriangle->m_vertexIndices[0], pTriangle->m_vertexIndices[1], pTriangle->m_vertexIndices[2] };
		float t[3] = { 1.0f-pTriangle->m_t[0], 1.0f-pTriangle->m_t[1], 1.0f-pTriangle->m_t[2] };
		memcpy( m_pTriangles[i].m_vertexNormals, pTriangle->m_vertexNormals, sizeof( float )*3*3 );
		memcpy( m_pTriangles[i].m_s, pTriangle->m_s, sizeof( float )*3 );
		memcpy( m_pTriangles[i].m_t, t, sizeof( float )*3 );
		memcpy( m_pTriangles[i].m_vertexIndices, vertexIndices, sizeof( int )*3 );
		pPtr += sizeof( MS3DTriangle );
	}

	int nGroups = *( int16* )pPtr;
	m_numMeshes = nGroups;
	m_pMeshes = new Mesh[nGroups];
	pPtr += sizeof( int16 );
	for ( i = 0; i < nGroups; i++ )
	{
		pPtr += sizeof( byte );	// flags
		pPtr += 32;				// name

		int16 nTriangles = *( int16* )pPtr;
		pPtr += sizeof( int16 );
		int *pTriangleIndices = new int[nTriangles];
		for ( int j = 0; j < nTriangles; j++ )
		{
			pTriangleIndices[j] = *( int16* )pPtr;
			pPtr += sizeof( int16 );
		}

		char materialIndex = *( char* )pPtr;
		pPtr += sizeof( char );

		m_pMeshes[i].m_materialIndex = materialIndex;
		m_pMeshes[i].m_numTriangles = nTriangles;
		m_pMeshes[i].m_pTriangleIndices = pTriangleIndices;
	}

	int nMaterials = *( int16* )pPtr;
	m_numMaterials = nMaterials;
	m_pMaterials = new Material[nMaterials];
	pPtr += sizeof( int16 );
	m_textureList = new CDexTex[nMaterials];
	for ( i = 0; i < nMaterials; i++ )
	{
		MS3DMaterial *pMaterial = ( MS3DMaterial* )pPtr;
		memcpy( m_pMaterials[i].m_ambient, pMaterial->m_ambient, sizeof( float )*4 );
		memcpy( m_pMaterials[i].m_diffuse, pMaterial->m_diffuse, sizeof( float )*4 );
		memcpy( m_pMaterials[i].m_specular, pMaterial->m_specular, sizeof( float )*4 );
		memcpy( m_pMaterials[i].m_emissive, pMaterial->m_emissive, sizeof( float )*4 );
		m_pMaterials[i].m_shininess = pMaterial->m_shininess;
		m_pMaterials[i].m_pTextureFilename = new char[strlen( pMaterial->m_texture )+1];
		strcpy( m_pMaterials[i].m_pTextureFilename, pMaterial->m_texture );
		pPtr += sizeof( MS3DMaterial );
		m_textureList[i].LoadTex(m_pMaterials[i].m_pTextureFilename);
	}
	delete[] pBuffer;
	return true;
}