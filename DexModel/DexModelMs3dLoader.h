/*
DexEngine& ms3d model loader
*/
#ifndef _DEX_MODEL_LOADER_MS3D_H
#define _DEX_MODEL_LOADER_MS3D_H
#include "DexModelLoader.h"


class DexModelMs3dLoader :public IDexModelLoader
{
public:
	DexModelMs3dLoader();
	virtual ~DexModelMs3dLoader();

public:
	virtual DexModelBase* LoadModel(const char* filename, int32 flag);
};

#endif 