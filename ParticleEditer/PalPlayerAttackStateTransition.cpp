
#include "PalPlayer.h"
#include "PalFightHead.h"
#include "PalSkill.h"
#include "PalStateBattleMain.h"
#include "PalPlayerAttackState.h"
#include "PalPlayerAttackStateTransition.h"

PalPlayerAttackStateTransition::PalPlayerAttackStateTransition(PalPlayerAttackState* state, PalPlayer* p)
	:IDexStateTransition(state)
{
	player = p;
}

PalPlayerAttackStateTransition::~PalPlayerAttackStateTransition()
{
}

/////////////
PalPlayerAttackStateTransitionMarchComplete::PalPlayerAttackStateTransitionMarchComplete(PalPlayerAttackState* state, PalPlayer* p)
	:PalPlayerAttackStateTransition(state,p)
{
}

PalPlayerAttackStateTransitionMarchComplete::~PalPlayerAttackStateTransitionMarchComplete()
{
}

bool PalPlayerAttackStateTransitionMarchComplete::IsTrue()
{
	DEX_ENSURE_B(m_transitionState != NULL && player != NULL);
	float value = player->getHeadFlag()->getValue();
	bool ret = value >= 1.0f;
	if(ret)
	{
		PalGameStateBattleMain::getBattleMain()->getSkillContent()->CreateNewContent();
	}
	return ret;
}
/////////////
PalPlayerAttackStateTransitionWait::PalPlayerAttackStateTransitionWait(PalPlayerAttackState* state, PalPlayer* p)
	:PalPlayerAttackStateTransition(state,p)
{
}

PalPlayerAttackStateTransitionWait::~PalPlayerAttackStateTransitionWait()
{
}

bool PalPlayerAttackStateTransitionWait::IsTrue()
{
	TlistPlayer players =  PalGameStateBattleMain::getBattleMain()->getPlayers(EPT_ROLE);
	{
		foreach(TlistPlayer, ite, players)
		{
			if((*ite)->getAIMachine()->getCurrState()->getType() == PalPlayerAttackStateSelectSkill::getClassType())
				return true;
			if((*ite)->getAIMachine()->getCurrState()->getType() == PalPlayerAttackStateSelectTarget::getClassType())
				return true;
		}
	}
	players =  PalGameStateBattleMain::getBattleMain()->getPlayers(EPT_ENEMY);
	{
		foreach(TlistPlayer, ite, players)
		{
			if((*ite)->getAIMachine()->getCurrState()->getType() == PalPlayerAttackStateSelectSkill::getClassType())
				return true;
			if((*ite)->getAIMachine()->getCurrState()->getType() == PalPlayerAttackStateSelectTarget::getClassType())
				return true;
		}
	}
	return false;
}
/////////////
PalPlayerAttackStateTransitionWaitOK::PalPlayerAttackStateTransitionWaitOK(PalPlayerAttackState* state, PalPlayer* p)
	:PalPlayerAttackStateTransition(state,p)
{
}

PalPlayerAttackStateTransitionWaitOK::~PalPlayerAttackStateTransitionWaitOK()
{
}

bool PalPlayerAttackStateTransitionWaitOK::IsTrue()
{
	TlistPlayer players =  PalGameStateBattleMain::getBattleMain()->getPlayers(EPT_ROLE);
	{
		foreach(TlistPlayer, ite, players)
		{
			if((*ite)->getAIMachine()->getCurrState()->getType() == PalPlayerAttackStateSelectSkill::getClassType())
				return false;
			if((*ite)->getAIMachine()->getCurrState()->getType() == PalPlayerAttackStateSelectTarget::getClassType())
				return false;
		}
	}
	players =  PalGameStateBattleMain::getBattleMain()->getPlayers(EPT_ENEMY);
	{
		foreach(TlistPlayer, ite, players)
		{
			if((*ite)->getAIMachine()->getCurrState()->getType() == PalPlayerAttackStateSelectSkill::getClassType())
				return false;
			if((*ite)->getAIMachine()->getCurrState()->getType() == PalPlayerAttackStateSelectTarget::getClassType())
				return false;
		}
	}
	return true;
}

/////////////
PalPlayerAttackStateTransitionSelectSkillOK::PalPlayerAttackStateTransitionSelectSkillOK(PalPlayerAttackState* state, PalPlayer* p)
	:PalPlayerAttackStateTransition(state,p)
{
}

PalPlayerAttackStateTransitionSelectSkillOK::~PalPlayerAttackStateTransitionSelectSkillOK()
{
}

bool PalPlayerAttackStateTransitionSelectSkillOK::IsTrue()
{
	bool ret =  (player!= NULL && player->getCurrSkill() != NULL);
	if(ret)
	{
		PalGameStateBattleMain::getBattleMain()->getSkillContent()->SetContent(player,player->getCurrSkill());
	}
	return ret;
}

/////////////
PalPlayerAttackStateTransitionSelectTargetOK::PalPlayerAttackStateTransitionSelectTargetOK(PalPlayerAttackState* state, PalPlayer* p)
	:PalPlayerAttackStateTransition(state,p)
{
}

PalPlayerAttackStateTransitionSelectTargetOK::~PalPlayerAttackStateTransitionSelectTargetOK()
{
}

bool PalPlayerAttackStateTransitionSelectTargetOK::IsTrue()
{
	bool ret = (player!= NULL && player->getTargets().size() != 0);
	return ret;
}

/////////////
PalPlayerAttackStateTransitionSelectTargetOK_NearSkill::PalPlayerAttackStateTransitionSelectTargetOK_NearSkill(PalPlayerAttackState* state, PalPlayer* p)
	:PalPlayerAttackStateTransition(state,p)
{
}

PalPlayerAttackStateTransitionSelectTargetOK_NearSkill::~PalPlayerAttackStateTransitionSelectTargetOK_NearSkill()
{
}

bool PalPlayerAttackStateTransitionSelectTargetOK_NearSkill::IsTrue()
{
	DEX_ENSURE_B(player);
	DEX_ENSURE_B((player->getTargets().size()) != 0);
	PalSkill* skill = player->getCurrSkill();
	return (skill != NULL && skill->getSkillData()->attackType == space_skill::PSAAT_TARGET);
}

/////////////
PalPlayerAttackStateTransitionSelectTargetOK_FarSkill::PalPlayerAttackStateTransitionSelectTargetOK_FarSkill(PalPlayerAttackState* state, PalPlayer* p)
	:PalPlayerAttackStateTransition(state,p)
{
}

PalPlayerAttackStateTransitionSelectTargetOK_FarSkill::~PalPlayerAttackStateTransitionSelectTargetOK_FarSkill()
{
}

bool PalPlayerAttackStateTransitionSelectTargetOK_FarSkill::IsTrue()
{
	DEX_ENSURE_B(player);
	DEX_ENSURE_B((player->getTargets().size()) != 0);
	PalSkill* skill = player->getCurrSkill();
	return (skill != NULL && skill->getSkillData()->attackType == space_skill::PSAAT_NOTMOVE);
}

/////////////
PalPlayerAttackStateTransitionRun2TargetOK::PalPlayerAttackStateTransitionRun2TargetOK(PalPlayerAttackState* state, PalPlayer* p)
	:PalPlayerAttackStateTransition(state,p)
{
}

PalPlayerAttackStateTransitionRun2TargetOK::~PalPlayerAttackStateTransitionRun2TargetOK()
{
}

bool PalPlayerAttackStateTransitionRun2TargetOK::IsTrue()
{
	DEX_ENSURE_B(player);
	PalPlayerAttackState* base = (PalPlayerAttackState*)player->getAIMachine()->getCurrState();
	DEX_ENSURE_B(base->getType() == PalPlayerAttackStateRunToTarget::getClassType());
	PalPlayerAttackStateRunToTarget* state = (PalPlayerAttackStateRunToTarget*) base;
	return state->getRunToTarget();
}

/////////////
PalPlayerAttackStateTransitionPlayAttackAniOK::PalPlayerAttackStateTransitionPlayAttackAniOK(PalPlayerAttackState* state, PalPlayer* p)
	:PalPlayerAttackStateTransition(state,p)
{
}

PalPlayerAttackStateTransitionPlayAttackAniOK::~PalPlayerAttackStateTransitionPlayAttackAniOK()
{
}

bool PalPlayerAttackStateTransitionPlayAttackAniOK::IsTrue()
{
	DEX_ENSURE_B(player);
	DEX_ENSURE_B(player->getAttackAnimationTimeCount() >= player->getAttackAnimationStartTime());
	return true;
}

/////////////
PalPlayerAttackStateTransitionFarSkillComplete::PalPlayerAttackStateTransitionFarSkillComplete(PalPlayerAttackState* state, PalPlayer* p)
	:PalPlayerAttackStateTransition(state,p)
{
}

PalPlayerAttackStateTransitionFarSkillComplete::~PalPlayerAttackStateTransitionFarSkillComplete()
{
}

bool PalPlayerAttackStateTransitionFarSkillComplete::IsTrue()
{
	DEX_ENSURE_B(player);
	PalSkill* skill = player->getCurrSkill();
	DEX_ENSURE_B(skill != NULL && skill->getSkillData()->attackType == space_skill::PSAAT_NOTMOVE);
	return player->completeSkill();
}
/////////////
PalPlayerAttackStateTransitionNearSkillComplete::PalPlayerAttackStateTransitionNearSkillComplete(PalPlayerAttackState* state, PalPlayer* p)
	:PalPlayerAttackStateTransition(state,p)
{
}

PalPlayerAttackStateTransitionNearSkillComplete::~PalPlayerAttackStateTransitionNearSkillComplete()
{
}

bool PalPlayerAttackStateTransitionNearSkillComplete::IsTrue()
{
	DEX_ENSURE_B(player);
	PalSkill* skill = player->getCurrSkill();
	DEX_ENSURE_B(skill != NULL && skill->getSkillData()->attackType == space_skill::PSAAT_TARGET);
	return player->completeSkill();
}

/////////////
PalPlayerAttackStateTransitionRunBackOK::PalPlayerAttackStateTransitionRunBackOK(PalPlayerAttackState* state, PalPlayer* p)
	:PalPlayerAttackStateTransition(state,p)
{
}

PalPlayerAttackStateTransitionRunBackOK::~PalPlayerAttackStateTransitionRunBackOK()
{
}

bool PalPlayerAttackStateTransitionRunBackOK::IsTrue()
{
	DEX_ENSURE_B(player);
	PalPlayerAttackState* base = (PalPlayerAttackState*)player->getAIMachine()->getCurrState();
	DEX_ENSURE_B(base->getType() == PalPlayerAttackStateRunBack::getClassType());
	PalPlayerAttackStateRunBack* state = (PalPlayerAttackStateRunBack*) base;
	return state->getRunBack();
}









