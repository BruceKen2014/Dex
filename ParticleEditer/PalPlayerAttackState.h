

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
//����
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

//ͷ�������˶�����
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

//ͷ��ȴ������Լ���ʩ���Ĺ�����
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


//class PalPlayerAttackStateWait ͷ����ͣ״̬ ���ⲿ����һ����������ͷ����ͣ �������ѡ�񹥻�ָ��ʱ 
//����ͷ����ͣ ѡ����֮�������½���marching״̬

//ͷ���˶������ұߣ�����׼������״̬
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

//ѡ�񹥻�Ŀ��
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

//ѡ���� �ɹ����ط� �����Ƽ��� ��ѡ��Զ�̻��߽��̼���
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

//����ǽ��̼��� �����ܵ�Ŀ�����״̬
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
//���ż���animation   
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

//����ǽ��̼��� ����������ܻ�״̬
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

//�����ͷ�״̬
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