
#ifndef _I_DEX_STATE_H
#define _I_DEX_STATE_H
#include <vector>
#include "DexBase/DexObject.h"
class IDexState;
class IDexStateTransition:public DexObject
{
	Dex_DeclareClass(IDexStateTransition, DexObject, 0)
protected:
	IDexState* m_transitionState;
	std::vector<stArgs> m_vecEventArgs; //属于事件本身的参数
	//状态机里面一个事件发生时记录的参数
	static std::vector<stArgs> s_vecEventArgs; 

public:
	IDexStateTransition(IDexState* state);
	virtual ~IDexStateTransition();
public:
	static void S_SetArgs(const std::vector<stArgs>& vecArgs);
	static std::vector<stArgs> S_GetArgs();
public:
	virtual bool IsTrue() = 0;
	virtual IDexState* getNextState();
	void PushArgs(const stArgs& args);
	std::vector<stArgs> GetArgs();
};
class IDexState:public DexObject
{
	Dex_DeclareClass(IDexState, DexObject,0)
protected:
	std::list<IDexStateTransition*> m_listTrans;
public:
	IDexState();
	virtual ~IDexState();
public:
	virtual void OnEnterState(std::string preType) = 0;
	virtual void OnLeaveState(std::string nextType) = 0;
	virtual bool AddTranstion(IDexStateTransition* tran);//外部new内部delete
	virtual IDexState* Update();
};

class IDexStateMachine: public DexObject
{
	Dex_DeclareClass(IDexStateMachine, DexObject,0)
public:
	IDexStateMachine();
	virtual ~IDexStateMachine();
protected:
	std::list<IDexState*> m_listStates;
	IDexState* m_currState;
public:
	bool AddState(IDexState* state);
	IDexState* getState(string type);
	IDexState* getCurrState() { return m_currState;};
	bool setCurrState(IDexState* state);
	bool setCurrState(string type);
	virtual bool Update();
};
#endif