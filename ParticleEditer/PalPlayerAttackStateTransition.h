#ifndef _PAL_PLAYER_ATTACK_STATE_TRANSITION_H
#define _PAL_PLAYER_ATTACK_STATE_TRANSITION_H
#include "DexBase/DexObject.h"
#include "IDexState.h"

class PalPlayer;
class PalPlayerAttackState;
/////======================state transition========================================
class PalPlayerAttackStateTransition: public IDexStateTransition
{
	Dex_DeclareClass(PalPlayerAttackStateTransition, IDexStateTransition,0)
public:
	PalPlayerAttackStateTransition(PalPlayerAttackState* state, PalPlayer* p);
	virtual ~PalPlayerAttackStateTransition();
protected:
	PalPlayer*           player;

public:
	virtual bool IsTrue() = 0;

};
////////////////////////
class PalPlayerAttackStateTransitionMarchComplete: public PalPlayerAttackStateTransition
{//自己进度完成后进入选择技能状态
	Dex_DeclareClass(PalPlayerAttackStateTransitionMarchComplete, PalPlayerAttackStateTransition, 0)
public:
	PalPlayerAttackStateTransitionMarchComplete(PalPlayerAttackState* state, PalPlayer* p);
	virtual ~PalPlayerAttackStateTransitionMarchComplete();
protected:

public:
	virtual bool IsTrue();
};
////////////////////////
class PalPlayerAttackStateTransitionWait: public PalPlayerAttackStateTransition
{//别人正在操作则进入等待状态（如我方一个角色正在选择技能或者选择目标）
	Dex_DeclareClass(PalPlayerAttackStateTransitionWait, PalPlayerAttackStateTransition, 0)
public:
	PalPlayerAttackStateTransitionWait(PalPlayerAttackState* state, PalPlayer* p);
	virtual ~PalPlayerAttackStateTransitionWait();
protected:

public:
	virtual bool IsTrue();
};
////////////////////////
class PalPlayerAttackStateTransitionWaitOK: public PalPlayerAttackStateTransition
{
	Dex_DeclareClass(PalPlayerAttackStateTransitionWaitOK, PalPlayerAttackStateTransition,0)
public:
	PalPlayerAttackStateTransitionWaitOK(PalPlayerAttackState* state, PalPlayer* p);
	virtual ~PalPlayerAttackStateTransitionWaitOK();
protected:

public:
	virtual bool IsTrue();
};
////////////////////////
class PalPlayerAttackStateTransitionSelectSkillOK: public PalPlayerAttackStateTransition
{
	Dex_DeclareClass(PalPlayerAttackStateTransitionSelectSkillOK, PalPlayerAttackStateTransition,0)
public:
	PalPlayerAttackStateTransitionSelectSkillOK(PalPlayerAttackState* state, PalPlayer* p);
	virtual ~PalPlayerAttackStateTransitionSelectSkillOK();
protected:

public:
	virtual bool IsTrue();
};
////////////////////////
class PalPlayerAttackStateTransitionSelectTargetOK: public PalPlayerAttackStateTransition
{
	Dex_DeclareClass(PalPlayerAttackStateTransitionSelectTargetOK, PalPlayerAttackStateTransition,0)
public:
	PalPlayerAttackStateTransitionSelectTargetOK(PalPlayerAttackState* state, PalPlayer* p);
	virtual ~PalPlayerAttackStateTransitionSelectTargetOK();
protected:

public:
	virtual bool IsTrue();
};
////////////////////////
class PalPlayerAttackStateTransitionSelectTargetOK_NearSkill: public PalPlayerAttackStateTransition
{
	Dex_DeclareClass(PalPlayerAttackStateTransitionSelectTargetOK_NearSkill, PalPlayerAttackStateTransition, 0)
public:
	PalPlayerAttackStateTransitionSelectTargetOK_NearSkill(PalPlayerAttackState* state, PalPlayer* p);
	virtual ~PalPlayerAttackStateTransitionSelectTargetOK_NearSkill();
protected:

public:
	virtual bool IsTrue();
};
////////////////////////
class PalPlayerAttackStateTransitionSelectTargetOK_FarSkill: public PalPlayerAttackStateTransition
{
	Dex_DeclareClass(PalPlayerAttackStateTransitionSelectTargetOK_FarSkill, PalPlayerAttackStateTransition, 0)
public:
	PalPlayerAttackStateTransitionSelectTargetOK_FarSkill(PalPlayerAttackState* state, PalPlayer* p);
	virtual ~PalPlayerAttackStateTransitionSelectTargetOK_FarSkill();
protected:

public:
	virtual bool IsTrue();
};

////////////////////////
class PalPlayerAttackStateTransitionRun2TargetOK: public PalPlayerAttackStateTransition
{
	Dex_DeclareClass(PalPlayerAttackStateTransitionRun2TargetOK, PalPlayerAttackStateTransition,0)
public:
	PalPlayerAttackStateTransitionRun2TargetOK(PalPlayerAttackState* state, PalPlayer* p);
	virtual ~PalPlayerAttackStateTransitionRun2TargetOK();
protected:

public:
	virtual bool IsTrue();
};

////////////////////////
class PalPlayerAttackStateTransitionPlayAttackAniOK: public PalPlayerAttackStateTransition
{
	Dex_DeclareClass(PalPlayerAttackStateTransitionPlayAttackAniOK, PalPlayerAttackStateTransition,0)
public:
	PalPlayerAttackStateTransitionPlayAttackAniOK(PalPlayerAttackState* state, PalPlayer* p);
	virtual ~PalPlayerAttackStateTransitionPlayAttackAniOK();
protected:

public:
	virtual bool IsTrue();
};

////////////////////////
class PalPlayerAttackStateTransitionFarSkillComplete: public PalPlayerAttackStateTransition
{
	Dex_DeclareClass(PalPlayerAttackStateTransitionFarSkillComplete, PalPlayerAttackStateTransition,0)
public:
	PalPlayerAttackStateTransitionFarSkillComplete(PalPlayerAttackState* state, PalPlayer* p);
	virtual ~PalPlayerAttackStateTransitionFarSkillComplete();
protected:

public:
	virtual bool IsTrue();
};
////////////////////////
class PalPlayerAttackStateTransitionNearSkillComplete: public PalPlayerAttackStateTransition
{
	Dex_DeclareClass(PalPlayerAttackStateTransitionNearSkillComplete, PalPlayerAttackStateTransition,0)
public:
	PalPlayerAttackStateTransitionNearSkillComplete(PalPlayerAttackState* state, PalPlayer* p);
	virtual ~PalPlayerAttackStateTransitionNearSkillComplete();
protected:

public:
	virtual bool IsTrue();
};

////////////////////////
class PalPlayerAttackStateTransitionRunBackOK: public PalPlayerAttackStateTransition
{
	Dex_DeclareClass(PalPlayerAttackStateTransitionRunBackOK, PalPlayerAttackStateTransition,0)
public:
	PalPlayerAttackStateTransitionRunBackOK(PalPlayerAttackState* state, PalPlayer* p);
	virtual ~PalPlayerAttackStateTransitionRunBackOK();
protected:

public:
	virtual bool IsTrue();
};









#endif