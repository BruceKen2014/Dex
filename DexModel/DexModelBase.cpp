
#include "DexModelBase.h"


DexModelBase::DexModelBase()
{
	m_bLightFlag = true;
}
void DexModelBase::SetLightFlag(bool enable)
{
	m_bLightFlag = enable;
}

DexModelBase::~DexModelBase()
{
}

bool DexModelBase::Update(int delta)
{
	return CDexObject::Update(delta);
}

bool DexModelBase::Render()
{
	return true;
}
