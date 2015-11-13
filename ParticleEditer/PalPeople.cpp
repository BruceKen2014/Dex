
#include "DexBase/DexObjectFactory.h"
#include "DexBase/DexSceneObjectModel.h"
#include "CDefineXAni.h"
#include "PalPeople.h"

CPalPeople::CPalPeople()
{
	m_fScaleInit = 1.0f;
	m_fScaleCurr = 1.0f;
	m_standIndex = -1;
	m_walkIndex = -1;
	m_runIndex = -1;
	m_attackIndex = -1;
	m_dieIndex = -1;
	m_attackAnimationStartTime = 500;
	m_attackAnimationTime = 1000;
}

CPalPeople::~CPalPeople()
{
	//CDexObjectFactroy::getObjectFactory()->allocateObject((CDexObject*)m_pObject);//將模型收回

}

void CPalPeople::SetIndex(CPalPeople* people, int modelId)
{
	DEX_ENSURE(people != NULL);
	switch(modelId)
	{
	case ID_JINGTIAN:
		{
			people->m_standIndex = JINGTIAN_STAND;
			people->m_runIndex = JINTTIAN_RUN;
			people->m_attackIndex = JINGTIAN_ATTACK;
			people->m_dieIndex = 0;
			break;
		}
	case ID_PANDA:
		{
			people->m_standIndex = PANDA_STAND;
			people->m_runIndex = PANDA_RUN;
			people->m_attackIndex = PANDA_ATTACK_TWO;
			people->m_dieIndex = PANDA_DIE;
			break;
		}
	case ID_BEAR:
		{
			people->m_standIndex = BEAR_STAND;
			people->m_runIndex = BEAR_WALK;
			people->m_attackIndex = BEAR_ATTACK;
			people->m_dieIndex = BEAR_DIE;
			break;
		}
	case ID_HUNTER:
		{
			people->m_standIndex = HUNTER_LOOK;
			people->m_runIndex = HUNTER_RUN;
			people->m_attackIndex = HUNTER_ATTACK;
			people->m_dieIndex = HUNTER_DIE;
			break;
		}
	}
}
void CPalPeople::Reset()
{

}

//bool CPalPeople::Update(int delta)
//{
//	DEX_ENSURE_B(CDexObject::Update(delta));
//	return true;
//}
