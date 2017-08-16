
#include "DexBase/CInput.h"
#include "DexStateCommonEvent.h"

/////////////
DexStateCommontEvent_AlwaysTrue::DexStateCommontEvent_AlwaysTrue(IDexState* state)
	:IDexStateTransition(state)
{
}

DexStateCommontEvent_AlwaysTrue::~DexStateCommontEvent_AlwaysTrue()
{
}

bool DexStateCommontEvent_AlwaysTrue::IsTrue()
{
	return true;
}

/////////////
DexStateCommontEvent_MouseRButtonUp::DexStateCommontEvent_MouseRButtonUp(IDexState* state)
	:IDexStateTransition(state)
{
}

DexStateCommontEvent_MouseRButtonUp::~DexStateCommontEvent_MouseRButtonUp()
{
}

bool DexStateCommontEvent_MouseRButtonUp::IsTrue()
{
	bool ret = CInputSystem::GetInputSingleton().RightButtonUp();
	return ret;
}

/////////////
DexStateCommontEvent_KeyUp::DexStateCommontEvent_KeyUp(DUInt32 iKeyId,IDexState* state)
	:IDexStateTransition(state)
{
	m_iKeyId = iKeyId;
}

DexStateCommontEvent_KeyUp::~DexStateCommontEvent_KeyUp()
{
}

bool DexStateCommontEvent_KeyUp::IsTrue()
{
	bool ret = CInputSystem::GetInputSingleton().KeyUp(m_iKeyId);
	return ret;
}