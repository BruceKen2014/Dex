/*
*DexEngine 模型处理基类
*create by Bruce
*Date:2015.11.7
*/

#ifndef _DEX_MODEL_BASE_H_
#define _DEX_MODEL_BASE_H_

#include "../DexBase/CDexObject.h"
#include "../DexMath/DexVector3.h"
class CDexTex;
class DexModelBase:public CDexObject
{
	Dex_DeclareClass(DexModelBase, 0)
public:
	struct Vertex
	{
		int8 m_boneID;	// for skeletal animation
		DexVector3 m_location;
	};
	struct Triangle
	{
		float m_vertexNormals[3][3];
		float m_s[3], m_t[3];
		int m_vertexIndices[3];
	};
	struct Material
	{
		float m_ambient[4], m_diffuse[4], m_specular[4], m_emissive[4];
		float m_shininess;
		int32 m_texture;
		char *m_pTextureFilename;
	};
	struct Mesh
	{
		int m_materialIndex;
		int m_numTriangles;
		int *m_pTriangleIndices;
	};
public:
	DexModelBase();
	virtual ~DexModelBase();

public:
	virtual bool Update(int delta);
	virtual bool Render();
	virtual bool LoadModel(const char* filename) = 0; //日后扩展从字节流中加载文件
protected:
	//	Meshes used
	int m_numMeshes;
	Mesh *m_pMeshes;

	//	Materials used
	int m_numMaterials;
	Material *m_pMaterials;

	//	Triangles used
	int m_numTriangles;
	Triangle *m_pTriangles;

	//	Vertices Used
	int m_numVertices;
	Vertex *m_pVertices;

	CDexTex* m_textureList;
};
#endif