
/*
��ȡDex �Զ����ģ�Ͷ����ļ����ı���ʽ
*/
#ifndef _DEX_MODEL_ANIFILE_TEXT_LOADER_H
#define _DEX_MODEL_ANIFILE_TEXT_LOADER_H

#include "DexModelAniFileLoader.h"

class DexModelAniFileTextLoader :public IDexModelAniFileLoader
{

public:
	virtual bool		  SupportType(const char* fileType);
	virtual bool		  LoadAnimationFile(DexModelBase* pModel, const char* filename);
public:
	DexModelAniFileTextLoader();
	virtual ~DexModelAniFileTextLoader();

protected:
	void ReadAnimationTime(DexMem* pMem, DexSkinMesh* pSkinMesh, char* pBuffer);
	void ReadJointAnimation(DexMem* pMem, DexSkinMesh* pSkinMesh, char* pBuffer);

};
#endif