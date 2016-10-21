#ifndef _DEX_TEXTURE_NULL_H
#define _DEX_TEXTURE_NULL_H

#include "DexTexture.h"
class DexTextureNull :public DexTexture
{
public:
	DexTextureNull();
	virtual ~DexTextureNull();
protected:
	virtual void ShutDown();
public:
	virtual bool LoadTexture(DString filename, const DexColor& transcolor = DEXCOLOR_BLACK);
};
#endif

