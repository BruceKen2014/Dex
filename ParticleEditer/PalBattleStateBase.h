
#ifndef _PALGAMESTATE_BATTLE_BASE_H
#define _PALGAMESTATE_BATTLE_BASE_H
#include "DexBase/CDexObject.h"

#include "IDexState.h"
class PalBattleStateBase;
class PalBattleStateMachine: public IDexStateMachine
{
	Dex_DeclareClass(PalBattleStateMachine,0)
public:
	PalBattleStateMachine();
	virtual ~PalBattleStateMachine();
public:
	virtual bool Update();
	virtual bool Render();
	virtual void  KeyDown(){};
	virtual void  MouseLDown(int xPos, int yPos);
	virtual void  MouseLUp(int xPos, int yPos);
	virtual void  MouseRUp(int xPos, int yPos);
public:
	PalBattleStateBase* getCurrState() { return (PalBattleStateBase*)m_currState;};
};
class PalBattleStateBase:public IDexState
{
	Dex_DeclareClass(PalBattleStateBase,0)
protected:
public:
	PalBattleStateBase();
	virtual ~PalBattleStateBase();
public:
	virtual bool  Render(); //这只是战斗的逻辑处理，不应该有任何渲染任务，渲染任务交给主状态的scene处理
	virtual void  KeyDown();
	virtual void  MouseLDown(int xPos, int yPos){};
	virtual void  MouseLUp(int xPos, int yPos){};
	virtual void  MouseRUp(int xPos, int yPos){};
};

class PalBattleStateMoveCamera: public PalBattleStateBase
{
	Dex_DeclareClass(PalBattleStateMoveCamera,0)

public:
	PalBattleStateMoveCamera();
	virtual ~PalBattleStateMoveCamera();

	virtual IDexState* Update();
	virtual bool Render(); 
	virtual void OnEnterState(std::string preType);
	virtual void OnLeaveState(std::string nextType);
	virtual void KeyDown();
};
//====================================
class PalBattleStateLabelMove: public PalBattleStateBase
{
	Dex_DeclareClass(PalBattleStateLabelMove,0)

public:
	PalBattleStateLabelMove();
	virtual ~PalBattleStateLabelMove();

	virtual IDexState* Update();
	virtual bool Render(); 
	virtual void OnEnterState(std::string preType);
	virtual void OnLeaveState(std::string nextType);
	virtual void KeyDown();
};
//====================================
class PalBattleStateSelectSkill: public PalBattleStateBase
{
	Dex_DeclareClass(PalBattleStateSelectSkill,0)

public:
	PalBattleStateSelectSkill();
	virtual ~PalBattleStateSelectSkill();

	virtual IDexState* Update();
	virtual bool Render(); 
	virtual void OnEnterState(std::string preType);
	virtual void OnLeaveState(std::string nextType);
	virtual void KeyDown();
	virtual void MouseLUp(int xPos, int yPos);
	virtual void MouseRUp(int xPos, int yPos);
};

//====================================
class PalBattleStateSelectTarget: public PalBattleStateBase
{
	Dex_DeclareClass(PalBattleStateSelectTarget,0)

public:
	PalBattleStateSelectTarget();
	virtual ~PalBattleStateSelectTarget();

	virtual IDexState* Update();
	virtual bool  Render(); 
	virtual void  OnEnterState(std::string preType);
	virtual void  OnLeaveState(std::string nextType);
	virtual void  KeyDown();
	virtual void  MouseLDown(int xPos, int yPos);
	virtual void  MouseLUp(int xPos, int yPos);
	virtual void  MouseRUp(int xPos, int yPos);


};

#endif