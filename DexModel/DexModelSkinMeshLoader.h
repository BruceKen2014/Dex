/*
DexEngine& dex model Loader
加载dex engine自己的模型数据，二进制格式
*/

#ifndef _DEX_MODEL_LOADER_DEX_MODEL_H
#define _DEX_MODEL_LOADER_DEX_MODEL_H

#include "DexModelLoader.h"
#include "..\DexBase\DexMem.h"
#include "DexSkinMesh.h"

class DexModelSkinMeshLoader :public IDexModelLoader
{
public:
	DexModelSkinMeshLoader();
	virtual ~DexModelSkinMeshLoader();

public:
	virtual bool		  SupportType(const char* fileType);
	virtual DexModelBase* LoadModel(const char* filename, DInt32 flag);
	virtual bool		  SaveModel(DexSkinMesh* pSkinMesh, const char* filename, DInt32 flag);
	void WriteModel(DexModelBase* model, const char* filename);

protected:
	void WriteMesh(DexSkinMesh* pSkinMesh, DexMem& mem);
	void WriteJoint(DexSkinMesh::Joint* pJoint, DexMem& mem);
	void WriteMaterial(DexSkinMesh* pSkinMesh, DexMem& mem);

	void ReadMesh(DexSkinMesh* pSkinMesh, DexMem& mem);
	void ReadJoint(DexSkinMesh* pSkinMesh, DexMem& mem);
	void ReadMaterial(DexSkinMesh* pSkinMesh, DexMem& mem);
};

#endif