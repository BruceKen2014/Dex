
#include "DexBase/DexLog.h"
#include "IDexState.h"

//==================IDexStateTransition=================
std::vector<stArgs> IDexStateTransition::s_vecEventArgs;
IDexStateTransition::IDexStateTransition(IDexState* state)
{
	m_transitionState = state;
}

IDexStateTransition::~IDexStateTransition()
{
}
void IDexStateTransition::S_SetArgs(const std::vector<stArgs>& vecArgs)
{
	s_vecEventArgs.clear();
	s_vecEventArgs = vecArgs;
}
std::vector<stArgs> IDexStateTransition::S_GetArgs()
{
	return s_vecEventArgs;
}
IDexState* IDexStateTransition::getNextState()
{
	 return m_transitionState;
}
void IDexStateTransition::PushArgs(const stArgs& args)
{
	m_vecEventArgs.push_back(args);
}
std::vector<stArgs> IDexStateTransition::GetArgs()
{
	return m_vecEventArgs;
}
//==================IDexState=================
IDexState::IDexState()
{

}
IDexState::~IDexState()
{
	for(std::list<IDexStateTransition*>::iterator ite = m_listTrans.begin(); ite !=m_listTrans.end();)
	{
		if(*ite != NULL)
		{
			delete (*ite);
			*ite = NULL;
		}
		ite = m_listTrans.erase(ite);
	}
}

bool IDexState::AddTranstion(IDexStateTransition* tran)
{
	bool exist = false;
	foreach(std::list<IDexStateTransition*>, ite, m_listTrans)
	{
		if((*ite)->getType() == tran->getType())
		{
			dex_log_current_f_l(log_allert);
			DexLog::getSingleton()->Log(log_allert, "试图添加一个已经存在的state transition!");
			exist =  true;
			break;
		}
	}
	if(!exist)
	{
		m_listTrans.push_back(tran);
	}
	return !exist;
}

IDexState* IDexState::Update()
{
	IDexState* nextState = NULL;
	foreach(std::list<IDexStateTransition*>, ite, m_listTrans)
	{
		if((*ite)->IsTrue())
		{
			IDexStateTransition::S_SetArgs((*ite)->GetArgs());
			nextState =  (*ite)->getNextState();
			break;
		}
	}
	return nextState;
}
//==============IDexStateMachine=============
IDexStateMachine::IDexStateMachine()
{
	m_currState = NULL;
}

IDexStateMachine::~IDexStateMachine()
{
	for(std::list<IDexState*>::iterator ite = m_listStates.begin(); ite !=m_listStates.end();)
	{
		if(*ite != NULL)
		{
			delete (*ite);
			*ite = NULL;
		}
		ite = m_listStates.erase(ite);
	}
}

bool IDexStateMachine::AddState(IDexState* state)
{
	bool exist = false;
	foreach(std::list<IDexState*>, ite, m_listStates)
	{
		if((*ite)->getType() == state->getType())
		{
			dex_log_current_f_l(log_allert);
			DexLog::getSingleton()->Log(log_allert, "试图添加一个已经存在的state!");
			exist =  true;
			break;
		}
	}
	if(!exist)
	{
		m_listStates.push_back(state);
		//if(m_currState == NULL)
		//	m_currState = state;
	}
	return !exist;
}

IDexState* IDexStateMachine::getState(string type)
{
	foreach(std::list<IDexState*>, ite, m_listStates)
	{
		if((*ite)->getType() == type)
		{
			return (*ite);
		}
	}
	return NULL;
}

bool IDexStateMachine::setCurrState(IDexState* state)
{
	DEX_ENSURE_B(state);
	foreach(std::list<IDexState*>, ite, m_listStates)
	{
		if((*ite) == state)
		{
			if(m_currState != NULL)
			{
				m_currState->OnLeaveState(state->getType());
				state->OnEnterState(m_currState->getType());
			}
			else
			{
				state->OnEnterState("");
			}
			m_currState = state;
			return true;
		}
	}
	return false;
}

bool IDexStateMachine::setCurrState(string type)
{
	foreach(std::list<IDexState*>, ite, m_listStates)
	{
		if((*ite) != NULL && (*ite)->getType() == type)
		{
			if(m_currState != NULL)
			{
				m_currState->OnLeaveState(type);
				(*ite)->OnEnterState(m_currState->getType());
			}
			else
			{
				(*ite)->OnEnterState("");
			}
			m_currState = (*ite);
			return true;
		}
	}
	return false;
}

bool IDexStateMachine::Update()
{
	if(m_currState != NULL)
	{
		IDexState* nextState = m_currState->Update();
		if(nextState != NULL)
		{
			setCurrState(nextState);
		}
	}
	return Super::Update(DexTime::getSingleton()->getDeltaTime());
}
