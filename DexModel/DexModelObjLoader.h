
#ifndef _DEX_MODEL_LOADER_OBJ_H
#define _DEX_MODEL_LOADER_OBJ_H
#include "DexModelLoader.h"
#include "../DexMath/DexVector3.h"


class DexModelObjLoader :public IDexModelLoader
{
public:
	DexModelObjLoader();
	virtual ~DexModelObjLoader();


public:
	virtual DexModelBase* LoadModel(const char* filename);
private:
	void GetToken(int8*& pBuffer, int8* pOut, int8* pEnd);
	void MoveNextLine(int8*& pBuffer, int8* pEnd);
	
};

#endif 