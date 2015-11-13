
#ifndef _PAL2GLOBAL_H
#define _PAL2GLOBAL_H

#include "state/DexGameState.h"

class CModelXAni;
class DexGlobalPal2: public DexGlobal
{
public:
	DexGlobalPal2();
	virtual ~DexGlobalPal2();

public:
	CModelXAni* g_pJingtian;
public:
	bool Init();
};
extern DexGlobalPal2* getGlobal();

#endif