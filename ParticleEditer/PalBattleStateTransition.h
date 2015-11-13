
#ifndef _PAL_BATTLE_STATE_TRANSITION_H
#define _PAL_BATTLE_STATE_TRANSITION_H

#include "DexBase/CDexObject.h"

#include "IDexState.h"
/////======================state transition========================================
class PalBattleStateTransition: public IDexStateTransition
{
	Dex_DeclareClass(PalBattleStateTransition,0)
public:
	PalBattleStateTransition(IDexState* state);
	virtual ~PalBattleStateTransition();
public:
	virtual bool IsTrue() = 0;
};
/////======================
class PalBattleStateTransitionSelectTargetOK: public PalBattleStateTransition
{
	Dex_DeclareClass(PalBattleStateTransitionSelectTargetOK,0)
public:
	PalBattleStateTransitionSelectTargetOK(IDexState* state);
	virtual ~PalBattleStateTransitionSelectTargetOK();

public:
	virtual bool IsTrue();
};
/////======================
class PalBattleStateTransitionCancelSkill: public PalBattleStateTransition
{
	Dex_DeclareClass(PalBattleStateTransitionCancelSkill,0)
public:
	PalBattleStateTransitionCancelSkill(IDexState* state);
	virtual ~PalBattleStateTransitionCancelSkill();

public:
	virtual bool IsTrue();
};
/////======================
class PalBattleStateTransitionPlayerMarchComplete: public PalBattleStateTransition
{
	Dex_DeclareClass(PalBattleStateTransitionPlayerMarchComplete,0)
public:
	PalBattleStateTransitionPlayerMarchComplete(IDexState* state);
	virtual ~PalBattleStateTransitionPlayerMarchComplete();

public:
	virtual bool IsTrue();
};

/////======================
class PalBattleStateTransitionSelectSkillOK: public PalBattleStateTransition
{
	Dex_DeclareClass(PalBattleStateTransitionSelectSkillOK,0)
public:
	PalBattleStateTransitionSelectSkillOK(IDexState* state);
	virtual ~PalBattleStateTransitionSelectSkillOK();

public:
	virtual bool IsTrue();
};
#endif