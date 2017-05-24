

#ifndef _DEX_MODEL_ANIFILE_LOADER_H
#define _DEX_MODEL_ANIFILE_LOADER_H

#include "..\DexBase\DexType.h"

class DexModelBase;
class IDexModelAniFileLoader
{
public:
	IDexModelAniFileLoader(){};
	virtual ~IDexModelAniFileLoader(){};

public:
	virtual bool		  SupportType(const char* fileType) = 0;
	virtual bool		  LoadAnimationFile(DexModelBase* pModel, const char* filename) = 0;
};
#endif