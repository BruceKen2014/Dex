

#ifndef _PAL_PLAYER_ATTACK_STATE_H
#define _PAL_PLAYER_ATTACK_STATE_H
#include "DexBase/CDelegate.h"
#include "DexBase/CDexObject.h"

#include "IDexState.h"

class PalPlayer;
class PalPlayerAttackState;

/////======================state========================================
class PalPlayerAttackStateMachine: public IDexStateMachine
{
	Dex_DeclareClass(PalPlayerAttackStateMachine,0)
public:
	PalPlayerAttackStateMachine(PalPlayer* p);
	virtual ~PalPlayerAttackStateMachine();

protected:
	PalPlayer* m_player;
};
class PalPlayerAttackState: public IDexState
{
	Dex_DeclareClass(PalPlayerAttackState,0)
	friend class PalPlayerAttackStateTransition;
protected:
	PalPlayer* player;
public:
	PalPlayerAttackState(PalPlayer* p);
	virtual ~PalPlayerAttackState();

public:
	virtual IDexState* Update();
};
//出生
class PalPlayerAttackStateSpawn: public PalPlayerAttackState
{
	Dex_DeclareClass(PalPlayerAttackStateSpawn,0)
public:
	PalPlayerAttackStateSpawn(PalPlayer* p);
	virtual ~PalPlayerAttackStateSpawn();

public:
	virtual void OnEnterState(std::string preType);
	virtual void OnLeaveState(std::string nextType);
	virtual IDexState* Update();
};

//头像正在运动当中
class PalPlayerAttackStateMarching: public PalPlayerAttackState
{
	Dex_DeclareClass(PalPlayerAttackStateMarching,0)
public:
	PalPlayerAttackStateMarching(PalPlayer* p);
	virtual ~PalPlayerAttackStateMarching();

public:
	virtual void OnEnterState(std::string preType);
	virtual void OnLeaveState(std::string nextType);
	virtual IDexState* Update();
};

//头像等待，如自己在施法的过程中
class PalPlayerAttackStateWait: public PalPlayerAttackState
{
	Dex_DeclareClass(PalPlayerAttackStateWait,0)
public:
	PalPlayerAttackStateWait(PalPlayer* p);
	virtual ~PalPlayerAttackStateWait();

public:
	virtual void OnEnterState(std::string preType);
	virtual void OnLeaveState(std::string nextType);
	virtual IDexState* Update();
};


//class PalPlayerAttackStateWait 头像暂停状态 如外部发生一定条件所有头像暂停 如玩家在选择攻击指令时 
//所有头像暂停 选择完之后再重新进入marching状态

//头像运动到最右边，进入准备攻击状态
class PalPlayerAttackStateReady: public PalPlayerAttackState
{
	Dex_DeclareClass(PalPlayerAttackStateReady,0)
public:
	PalPlayerAttackStateReady(PalPlayer* p);
	virtual ~PalPlayerAttackStateReady();

public:
	virtual void OnEnterState(std::string preType);
	virtual void OnLeaveState(std::string nextType);
	virtual IDexState* Update();
};

//选择攻击目标
class PalPlayerAttackStateSelectTarget: public PalPlayerAttackState
{
	Dex_DeclareClass(PalPlayerAttackStateSelectTarget,0)
public:
	PalPlayerAttackStateSelectTarget(PalPlayer* p);
	virtual ~PalPlayerAttackStateSelectTarget();

public:
	virtual void OnEnterState(std::string preType);
	virtual void OnLeaveState(std::string nextType);
	virtual IDexState* Update();
};

//选择技能 可攻击地方 可治疗己方 可选择远程或者近程技能
class PalPlayerAttackStateSelectSkill: public PalPlayerAttackState
{
	Dex_DeclareClass(PalPlayerAttackStateSelectSkill,0)
public:
	PalPlayerAttackStateSelectSkill(PalPlayer* p);
	virtual ~PalPlayerAttackStateSelectSkill();

public:
	virtual void OnEnterState(std::string preType);
	virtual void OnLeaveState(std::string nextType);
	virtual IDexState* Update();
};

//如果是近程技能 进入跑到目标身边状态
class PalPlayerAttackStateRunToTarget: public PalPlayerAttackState
{
	Dex_DeclareClass(PalPlayerAttackStateRunToTarget,0)
protected:
	int run_time;
	int run_count;
public:
	PalPlayerAttackStateRunToTarget(PalPlayer* p);
	virtual ~PalPlayerAttackStateRunToTarget();

public:
	bool getRunToTarget();
	virtual void OnEnterState(std::string preType);
	virtual void OnLeaveState(std::string nextType);
	virtual IDexState* Update();
};
//播放技能animation   
class PalPlayerAttackStatePlayAttackAnimation: public PalPlayerAttackState
{
	friend class PalPlayerAttackStateTransitionPlayAttackAniOK;
	Dex_DeclareClass(PalPlayerAttackStatePlayAttackAnimation,0)
public:
	PalPlayerAttackStatePlayAttackAnimation(PalPlayer* p);
	virtual ~PalPlayerAttackStatePlayAttackAnimation();

public:
	virtual void OnEnterState(std::string preType);
	virtual void OnLeaveState(std::string nextType);
	virtual IDexState* Update();
};

//如果是近程技能 攻击后进入跑回状态
class PalPlayerAttackStateRunBack: public PalPlayerAttackState
{
	Dex_DeclareClass(PalPlayerAttackStateRunBack,0)
protected:
	int run_time;
	int run_count;
public:
	PalPlayerAttackStateRunBack(PalPlayer* p);
	virtual ~PalPlayerAttackStateRunBack();

public:
	bool getRunBack();
	virtual void OnEnterState(std::string preType);
	virtual void OnLeaveState(std::string nextType);
	virtual IDexState* Update();
};

//技能释放状态
class PalPlayerAttackStateSkillPlaying: public PalPlayerAttackState
{
	Dex_DeclareClass(PalPlayerAttackStateSkillPlaying,0)
public:
	PalPlayerAttackStateSkillPlaying(PalPlayer* p);
	virtual ~PalPlayerAttackStateSkillPlaying();

public:
	virtual void OnEnterState(std::string preType);
	virtual void OnLeaveState(std::string nextType);
	virtual IDexState* Update();
};
#endif