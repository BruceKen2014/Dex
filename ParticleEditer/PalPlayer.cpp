

#include "DexBase/DexObjectFactory.h"
#include "DexBase/DexCollideObject.h"
#include "DexStateCommonEvent.h"
#include "PalPlayerAttackStateTransition.h"
#include "PalPlayerAttackState.h"
#include "PalSkill.h"
#include "PalPlayer.h"
#include "CDefineXAni.h"


//==========================================================================
PalPlayer::PalPlayer()
{
	m_bAuto = true;
	m_data.m_PlayerType = EPT_ENEMY;
	m_iAttackAnimationTimeCount = 0;
	m_pSkillContainer = new CPalSkillContainer;

	m_AiMachine = new PalPlayerAttackStateMachine(this);
	PalPlayerAttackStateSpawn* spawn= new PalPlayerAttackStateSpawn(this);
	PalPlayerAttackStateMarching* marching = new PalPlayerAttackStateMarching(this);
	PalPlayerAttackStateWait* wait = new PalPlayerAttackStateWait(this);
	PalPlayerAttackStateReady* ready = new PalPlayerAttackStateReady(this);
	PalPlayerAttackStateSelectTarget* selectTarget = new PalPlayerAttackStateSelectTarget(this);
	PalPlayerAttackStateSelectSkill* selectSkill = new PalPlayerAttackStateSelectSkill(this);
	PalPlayerAttackStateRunToTarget* runToTarget = new PalPlayerAttackStateRunToTarget(this);
	PalPlayerAttackStatePlayAttackAnimation*  playAttAnimation = new PalPlayerAttackStatePlayAttackAnimation(this);
	PalPlayerAttackStateRunBack* runBack = new PalPlayerAttackStateRunBack(this);
	PalPlayerAttackStateSkillPlaying* skillPlaying = new PalPlayerAttackStateSkillPlaying(this);
	//after run back then marching
	spawn->AddTranstion(new DexStateCommontEvent_AlwaysTrue(marching));
	marching->AddTranstion(new PalPlayerAttackStateTransitionWait(wait, this));
	wait->AddTranstion(new PalPlayerAttackStateTransitionWaitOK(marching, this));
	marching->AddTranstion(new PalPlayerAttackStateTransitionMarchComplete(ready, this));
	ready->AddTranstion(new DexStateCommontEvent_AlwaysTrue(selectSkill));//现在到了之后立马选择
	selectSkill->AddTranstion(new PalPlayerAttackStateTransitionSelectSkillOK(selectTarget, this));
		DexStateCommontEvent_MouseRButtonUp* transition_cancelSkill = new DexStateCommontEvent_MouseRButtonUp(selectSkill);
		transition_cancelSkill->PushArgs(stArgs(PalArgs_PlayerCancelSkill));
	selectTarget->AddTranstion(transition_cancelSkill);
	selectTarget->AddTranstion(new PalPlayerAttackStateTransitionSelectTargetOK_NearSkill(runToTarget, this));
	selectTarget->AddTranstion(new PalPlayerAttackStateTransitionSelectTargetOK_FarSkill(playAttAnimation, this));
	runToTarget->AddTranstion(new PalPlayerAttackStateTransitionRun2TargetOK(playAttAnimation, this));
	playAttAnimation->AddTranstion(new PalPlayerAttackStateTransitionPlayAttackAniOK(skillPlaying, this));
	skillPlaying->AddTranstion(new PalPlayerAttackStateTransitionNearSkillComplete(runBack, this));
	skillPlaying->AddTranstion(new PalPlayerAttackStateTransitionFarSkillComplete(marching, this));
	runBack->AddTranstion(new PalPlayerAttackStateTransitionRunBackOK(marching, this));
	m_AiMachine->AddState(spawn);
	m_AiMachine->AddState(marching);
	m_AiMachine->AddState(wait);
	m_AiMachine->AddState(ready);
	m_AiMachine->AddState(selectTarget);
	m_AiMachine->AddState(selectSkill);
	m_AiMachine->AddState(runToTarget);
	m_AiMachine->AddState(playAttAnimation);
	m_AiMachine->AddState(runBack);
	m_AiMachine->AddState(skillPlaying);
}

PalPlayer::~PalPlayer()
{
}


void PalPlayer::UpdateAttackAnimationTimeCount()
{
	if(m_iAttackAnimationTimeCount < m_attackAnimationTime)
	{
		m_iAttackAnimationTimeCount += DexTime::getSingleton()->getDeltaTime();
		if(m_iAttackAnimationTimeCount > m_attackAnimationTime)
		{
			getAniModel()->SetAnimation(m_standIndex);
		}
	}
}
void PalPlayer::clearAttackTarget()
{
	m_listTargets.clear();
}
void PalPlayer::AddTarget(PalPlayer* target)
{
	m_listTargets.push_back(target);
}

void PalPlayer::Reset()
{

}

D3DXVECTOR3 PalPlayer::getTopPosition()
{
	D3DXVECTOR3 pos(0,0,0);
	if(m_pCollideObject != NULL)
	{
		if(m_pCollideObject->getType() == DexCollideObjectSphere::getClassType())
		{
			DexCollideObjectSphere* sphere = (DexCollideObjectSphere*)m_pCollideObject;
			stBall ball= sphere->getBall();
			memcpy(&pos, &ball.m_center, sizeof(D3DXVECTOR3));
			D3DXVECTOR3 up(0,1,0);
			pos += ball.m_radius * up;
		}
	}
	return pos;
}

bool PalPlayer::IsLive()
{
	return m_data.hp > 0;
}

bool PalPlayer::UpdatePlayerData(int delta)
{
	if(m_AiMachine != NULL)
	{
		m_AiMachine->Update();
	}
	return true;
}

void PalPlayer::ApplyDamage(int hp)
{
	m_data.hp -= hp;
	m_data.hp = m_data.hp < 0 ? 0: m_data.hp;
}

void PalPlayer::ApplyHeal(int hp)
{
	m_data.hp += hp;
	m_data.hp = m_data.hp > m_data.hpMax ? m_data.hpMax: m_data.hp;
}
