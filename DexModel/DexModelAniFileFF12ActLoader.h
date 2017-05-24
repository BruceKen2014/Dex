/*
读取FF12的.act 动画文件
*/
#ifndef _DEX_MODEL_ANIFILE_FF12ACT_LOADER_H
#define _DEX_MODEL_ANIFILE_FF12ACT_LOADER_H

#include "DexModelAniFileLoader.h"

class DexModelAniFileFF12ActLoader:public IDexModelAniFileLoader
{
protected:
public:
	DexModelAniFileFF12ActLoader();
	virtual ~DexModelAniFileFF12ActLoader();

public:
	virtual bool		  SupportType(const char* fileType);
	virtual bool		  LoadAnimationFile(DexModelBase* pModel, const char* filename);
};
#endif