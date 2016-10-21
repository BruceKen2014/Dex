#ifndef _DEX_TEXTURE_DX9_H
#define _DEX_TEXTURE_DX9_H

#include "DexTexture.h"
class DexTextureDx9 :public DexTexture
{
public:
	DexTextureDx9();
	virtual ~DexTextureDx9();
protected:
	virtual void ShutDown();   
public:
	virtual bool LoadTexture(DString filename, const DexColor& transcolor = DEXCOLOR_BLACK);
};
#endif

