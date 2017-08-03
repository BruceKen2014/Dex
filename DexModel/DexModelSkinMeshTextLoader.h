/*
DexEngine& dex model Loader
加载dex engine自己的模型数据，文本格式
*/

#ifndef _DEX_MODEL_LOADER_DEX_MODEL_TEXT_H
#define _DEX_MODEL_LOADER_DEX_MODEL_TEXT_H

#include "DexModelLoader.h"
#include "..\DexBase\DexMem.h"
#include "DexSkinMesh.h"

class DexModelSkinMeshTextLoader :public IDexModelLoader
{
public:
	DexModelSkinMeshTextLoader();
	virtual ~DexModelSkinMeshTextLoader();

public:
	virtual bool		  SupportType(const char* fileType);
	virtual DexModelBase* LoadModel(const char* filename, DInt32 flag);
	virtual bool		  SaveModel(DexSkinMesh* pSkinMesh, const char* filename, DInt32 flag);
protected:
	void ReadMaterials(DexMem* pMem, DexSkinMesh* pSkinMesh, char* pBuffer);
	void ReadTextures(DexMem* pMem, DexSkinMesh* pSkinMesh, char* pBuffer);
	void ReadJoints(DexMem* pMem, DexSkinMesh* pSkinMesh, char* pBuffer);
	void ReadVertex(DexMem* pMem, DVector<DexSkinMesh::stMeshVertex>& vertexs, char* pBuffer);
	void ReadMeshes(DexMem* pMem, DexSkinMesh* pSkinMesh, DVector<DexSkinMesh::stMeshVertex>& vertexs, char* pBuffer);
	void ReadRenderFlag(DexMem* pMem, DexSkinMesh* pSkinMesh, char* pBuffer);
	void ReadLightFlag(DexMem* pMem, DexSkinMesh* pSkinMesh, char* pBuffer);
};

#endif