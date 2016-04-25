
#ifndef _DEX_MODEL_LOADER_H
#define _DEX_MODEL_LOADER_H


class DexModelBase;
class IDexModelLoader
{
public:
	IDexModelLoader(){};
	virtual ~IDexModelLoader(){};

public:
	virtual DexModelBase* LoadModel(const char* filename) = 0;
};
#endif