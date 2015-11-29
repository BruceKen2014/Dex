

#include <fstream>
#include "../DexBase/DexDefine.h"
#include "DexModelMs3dLoader.h"
#include "DexSkinMesh.h"

/*
MS3D STRUCTURES
*/

// byte-align structures
#ifdef _MSC_VER
#	pragma pack( push, 1 )  
#	define PACK_STRUCT
#elif defined( __GNUC__ )
#	define PACK_STRUCT	__attribute__((packed))
#endif

// File header
struct MS3DHeader
{
	char m_ID[10];
	int32 m_version;
} PACK_STRUCT;

// Vertex information
struct MS3DVertex
{
	int8 m_flags;
	float m_vertex[3];
	char m_boneID;
	int8 m_refCount;
} PACK_STRUCT;

// Triangle information
struct MS3DTriangle
{
	int16 m_flags;
	int16 m_vertexIndices[3];
	float m_vertexNormals[3][3];
	float m_s[3], m_t[3];
	int8 m_smoothingGroup;
	int8 m_groupIndex;
} PACK_STRUCT;

// Material information
struct MS3DMaterial
{
	char m_name[32];
	float m_ambient[4];
	float m_diffuse[4];
	float m_specular[4];
	float m_emissive[4];
	float m_shininess;	// 0.0f - 128.0f
	float m_transparency;	// 0.0f - 1.0f
	int8 m_mode;	// 0, 1, 2 is unused now
	char m_texture[128];
	char m_alphamap[128];
} PACK_STRUCT;

//	Joint information
struct MS3DJoint
{
	int8 m_flags;
	char m_name[32];
	char m_parentName[32];
	float m_rotation[3];
	float m_translation[3];
	int16 m_numRotationKeyframes;
	int16 m_numTranslationKeyframes;
} PACK_STRUCT;

// Keyframe data
struct MS3DKeyframe
{
	float m_time;
	float m_parameter[3];
} PACK_STRUCT;

// Default alignment
#ifdef _MSC_VER
#	pragma pack(pop)
#endif

DexModelMs3dLoader::DexModelMs3dLoader()
{

}
DexModelMs3dLoader::~DexModelMs3dLoader()
{

}

DexModelBase* DexModelMs3dLoader::LoadModel(const char* filename)
{
	//TODO 把model加入object工程，采用query机制
	DexSkinMesh* skinMesh = new DexSkinMesh();
	ifstream inputFile(filename, ios::in | ios::binary | ios::_Nocreate);
	if (inputFile.fail())
	{
		delete skinMesh;
		return NULL;	
	}
	inputFile.seekg(0, ios::end);
	long fileSize = inputFile.tellg();
	inputFile.seekg(0, ios::beg);

	byte *pBuffer = new byte[fileSize];
	inputFile.read((char*)pBuffer, fileSize);
	inputFile.close();

	const byte *pPtr = pBuffer;
	MS3DHeader *pHeader = (MS3DHeader*)pPtr;
	pPtr += sizeof(MS3DHeader);
	if (strncmp(pHeader->m_ID, "MS3D000000", 10) != 0)
	{
		delete skinMesh;
		return NULL;
	}// "Not a valid Milkshape3D model file."

	if (pHeader->m_version < 3 || pHeader->m_version > 4)
	{
		delete skinMesh;
		return NULL;
	}// "Unhandled file version. Only Milkshape3D Version 1.3 and 1.4 is supported." );

	int nVertices = *(int16*)pPtr;  //顶点个数
	pPtr += sizeof(int16);

	int i;
	for (i = 0; i < nVertices; i++)
	{
		//目前尚未支持ms3d boneid
		MS3DVertex *pVertex = (MS3DVertex*)pPtr;
		DexVector3 pos(pVertex->m_vertex);
		skinMesh->AddVertex(pos);
		pPtr += sizeof(MS3DVertex);
	}
	int nTriangles = *(int16*)pPtr; //三角形个数
	struct Triangle
	{
		float m_vertexNormals[3][3];
		float m_s[3], m_t[3];
		int m_vertexIndices[3];
	};
	Triangle* Triangles = new Triangle[nTriangles];
	pPtr += sizeof(int16);
	for (i = 0; i < nTriangles; i++)
	{
		MS3DTriangle *pTriangle = (MS3DTriangle*)pPtr;
		int vertexIndices[3] = { pTriangle->m_vertexIndices[0], pTriangle->m_vertexIndices[1], pTriangle->m_vertexIndices[2] };
		float t[3] = { 1.0f - pTriangle->m_t[0], 1.0f - pTriangle->m_t[1], 1.0f - pTriangle->m_t[2] };
		
		//对于我们的skinmesh而言，后面只需要indice，所以这里只记录indice
		//memcpy(Triangles[i].m_vertexNormals, pTriangle->m_vertexNormals, sizeof(float)* 3 * 3);
		//memcpy(Triangles[i].m_s, pTriangle->m_s, sizeof(float)* 3);
		//memcpy(Triangles[i].m_t, t, sizeof(float)* 3);
		memcpy(Triangles[i].m_vertexIndices, vertexIndices, sizeof(int)* 3);
		//这里可能会对一些公用顶点进行重复设置
		skinMesh->SetVertexNormal(pTriangle->m_vertexIndices[0], pTriangle->m_vertexNormals[0]);
		skinMesh->SetVertexUv(pTriangle->m_vertexIndices[0], pTriangle->m_s[0], t[0]);

		skinMesh->SetVertexNormal(pTriangle->m_vertexIndices[1], pTriangle->m_vertexNormals[1]);
		skinMesh->SetVertexUv(pTriangle->m_vertexIndices[1], pTriangle->m_s[1], t[1]);

		skinMesh->SetVertexNormal(pTriangle->m_vertexIndices[2], pTriangle->m_vertexNormals[2]);
		skinMesh->SetVertexUv(pTriangle->m_vertexIndices[2], pTriangle->m_s[2], t[2]);
		pPtr += sizeof(MS3DTriangle);
	}

	int nGroups = *(int16*)pPtr; //mesh数量
	pPtr += sizeof(int16);
	for (i = 0; i < nGroups; i++)
	{
		pPtr += sizeof(byte);	// flags
		pPtr += 32;				// name
		int16 nTriangles = *(int16*)pPtr; //这个mesh有多少个三角形
		pPtr += sizeof(int16);
		int *pTriangleIndices = new int[nTriangles]; //三角形索引

		int32* indices = new int32[nTriangles * 3];
		int32 indice_index = 0;
		DexSkinMesh::DexMesh* mesh =  skinMesh->AddMesh(i); //新建mesh
		for (int j = 0; j < nTriangles; j++)
		{//解析这个mesh所用到的所有三角形的顶点索引
			int triangleIndex = *(int16*)pPtr;
			pPtr += sizeof(int16);
			indices[indice_index++] = Triangles[triangleIndex].m_vertexIndices[0];
			indices[indice_index++] = Triangles[triangleIndex].m_vertexIndices[1];
			indices[indice_index++] = Triangles[triangleIndex].m_vertexIndices[2];
		}
		skinMesh->SetIndices(i, indices, nTriangles * 3);
		char materialIndex = *(char*)pPtr;
		pPtr += sizeof(char);
		mesh->materialId = materialIndex;
		mesh->textureId = materialIndex;
		delete[] indices;
		delete[] pTriangleIndices;
	}

	int nMaterials = *(int16*)pPtr;
	pPtr += sizeof(int16);
	for (i = 0; i < nMaterials; i++)
	{
		MS3DMaterial *pMaterial = (MS3DMaterial*)pPtr;
		DexMaterial material;
		material.diffuse.Set(pMaterial->m_diffuse[0], pMaterial->m_diffuse[1], pMaterial->m_diffuse[2],
			pMaterial->m_diffuse[3]);
		material.ambient.Set(pMaterial->m_ambient[0], pMaterial->m_ambient[1], pMaterial->m_ambient[2],
			pMaterial->m_ambient[3]);
		material.specular.Set(pMaterial->m_specular[0], pMaterial->m_specular[1], pMaterial->m_specular[2],
			pMaterial->m_specular[3]);
		material.emissive.Set(pMaterial->m_emissive[0], pMaterial->m_emissive[1], pMaterial->m_emissive[2],
			pMaterial->m_emissive[3]);
		material.power = pMaterial->m_shininess;
		//还有几个material目前还没支持
		char* filename = (char*) malloc(strlen(pMaterial->m_texture) + 1);
		strcpy(filename, pMaterial->m_texture);
		skinMesh->AddMaterial(material);
		skinMesh->AddTexture(filename);
		pPtr += sizeof(MS3DMaterial);
		_SafeFree(filename);
	}
	delete[] Triangles;
	delete[] pBuffer;
	skinMesh->CalculateVertex();
	return skinMesh;
}