
#include "DexModelBase.h"


DexModelBase::DexModelBase()
{
	m_iLightFlag = 0;
}
void DexModelBase::SetLightFlag(DUInt8 flag)
{
	m_iLightFlag = flag;
}

DexModelBase::~DexModelBase()
{
}

bool DexModelBase::Update(int delta)
{
	return DexObject::Update(delta);
}

bool DexModelBase::Render()
{
	return true;
}
