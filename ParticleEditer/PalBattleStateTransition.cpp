
#include "PalPlayer.h"
#include "PalStateBattleMain.h"
#include "PalBattleStateTransition.h"

PalBattleStateTransition::PalBattleStateTransition(IDexState* state)
	:IDexStateTransition(state)
{
}

PalBattleStateTransition::~PalBattleStateTransition()
{
}

//======================================
PalBattleStateTransitionSelectTargetOK::PalBattleStateTransitionSelectTargetOK(IDexState* state)
	:PalBattleStateTransition(state)
{
}

PalBattleStateTransitionSelectTargetOK::~PalBattleStateTransitionSelectTargetOK()
{
}

bool PalBattleStateTransitionSelectTargetOK::IsTrue()
{
	PalPlayer* player =  PalGameStateBattleMain::getBattleMain()->getCurrOperationPlayer();
	return (player != NULL && player->getTargets().size() != 0);
}

//======================================
PalBattleStateTransitionPlayerMarchComplete::PalBattleStateTransitionPlayerMarchComplete(IDexState* state)
	:PalBattleStateTransition(state)
{
}

PalBattleStateTransitionPlayerMarchComplete::~PalBattleStateTransitionPlayerMarchComplete()
{
}

bool PalBattleStateTransitionPlayerMarchComplete::IsTrue()
{
	return PalGameStateBattleMain::getBattleMain()->getCurrOperationPlayer() != NULL;
}

//======================================
PalBattleStateTransitionCancelSkill::PalBattleStateTransitionCancelSkill(IDexState* state)
	:PalBattleStateTransition(state)
{
}

PalBattleStateTransitionCancelSkill::~PalBattleStateTransitionCancelSkill()
{
}

bool PalBattleStateTransitionCancelSkill::IsTrue()
{
	PalPlayer* player =  PalGameStateBattleMain::getBattleMain()->getCurrOperationPlayer();
	DEX_ENSURE_B(player != NULL);
	PalSkill* skill = player->getCurrSkill();
	return skill == NULL;
}
//======================================
PalBattleStateTransitionSelectSkillOK::PalBattleStateTransitionSelectSkillOK(IDexState* state)
	:PalBattleStateTransition(state)
{
}

PalBattleStateTransitionSelectSkillOK::~PalBattleStateTransitionSelectSkillOK()
{
}

bool PalBattleStateTransitionSelectSkillOK::IsTrue()
{
	PalPlayer* player =  PalGameStateBattleMain::getBattleMain()->getCurrOperationPlayer();
	return (player != NULL && player->getCurrSkill() != NULL);
}