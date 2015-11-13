
#ifndef _I_DEX_STATE_COMMON_EVENT_H
#include "IDexState.h"

////////////////////////always true
class DexStateCommontEvent_AlwaysTrue: public IDexStateTransition
{
	Dex_DeclareClass(DexStateCommontEvent_AlwaysTrue,0)
public:
	DexStateCommontEvent_AlwaysTrue(IDexState* state);
	virtual ~DexStateCommontEvent_AlwaysTrue();
protected:

public:
	virtual bool IsTrue();
};
////////////////////////key up event
class DexStateCommontEvent_KeyUp: public IDexStateTransition
{
	Dex_DeclareClass(DexStateCommontEvent_KeyUp, 0)
protected:
	uint32 m_iKeyId;
public:
	DexStateCommontEvent_KeyUp(uint32 iKeyId,IDexState* state);
	virtual ~DexStateCommontEvent_KeyUp();
public:
	virtual bool IsTrue();
};
////////////////////////mouse button event
class DexStateCommontEvent_MouseRButtonUp: public IDexStateTransition
{
	Dex_DeclareClass(DexStateCommontEvent_MouseRButtonUp, 0)
public:
	DexStateCommontEvent_MouseRButtonUp(IDexState* state);
	virtual ~DexStateCommontEvent_MouseRButtonUp();
public:
	virtual bool IsTrue();
};


#define _I_DEX_STATE_COMMON_EVENT_H
#endif
