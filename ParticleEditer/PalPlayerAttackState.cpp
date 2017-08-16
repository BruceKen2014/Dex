
#include "DexBase/typedefine.h"
#include "DexBase/DexLog.h"
#include "PalFightHead.h"
#include "PalPlayer.h"
#include "PalSkill.h"
#include "PalStateBattleMain.h"
#include "PalPlayerAttackStateTransition.h"
#include "PalPlayerAttackState.h"


//================================================================

PalPlayerAttackStateMachine::PalPlayerAttackStateMachine(PalPlayer* p)
{
	m_player = p;
}
PalPlayerAttackStateMachine::~PalPlayerAttackStateMachine()
{
}

//bool PalPlayerAttackStateMachine::AddState(PalPlayerAttackState* state)
//{
//	bool exist = false;
//	foreach(std::list<PalPlayerAttackState*>, ite, m_listStates)
//	{
//		if((*ite)->getType() == state->getType())
//		{
//			exist =  true;
//		}
//	}
//	if(!exist)
//	{
//		m_listStates.push_back(state);
//		if(m_currState == NULL)
//			m_currState = state;
//	}
//	return !exist;
//}
//
//PalPlayerAttackState* PalPlayerAttackStateMachine::getState(string type)
//{
//	foreach(std::list<PalPlayerAttackState*>, ite, m_listStates)
//	{
//		if((*ite)->getType() == type)
//		{
//			return (*ite);
//		}
//	}
//	return NULL;
//}
//
//bool PalPlayerAttackStateMachine::SetCurrState(PalPlayerAttackState* state)
//{
//	foreach(std::list<PalPlayerAttackState*>, ite, m_listStates)
//	{
//		if((*ite) == state)
//		{
//			m_currState->OnLeaveState(state->getType());
//			state->OnEnterState(m_currState->getType());
//			m_currState = state;
//			return true;
//		}
//	}
//	return false;
//}
//
//bool PalPlayerAttackStateMachine::setCurrState(string type)
//{
//	foreach(std::list<PalPlayerAttackState*>, ite, m_listStates)
//	{
//		if((*ite)->getType() == type)
//		{
//			m_currState->OnLeaveState(type);
//			(*ite)->OnEnterState(m_currState->getType());
//			m_currState = (*ite);
//			return true;
//		}
//	}
//	return false;
//}
//
//void PalPlayerAttackStateMachine::Update()
//{
//	if(m_currState != NULL)
//	{
//		PalPlayerAttackState* nextState = m_currState->Update();
//		if(nextState != NULL)
//		{
//			SetCurrState(nextState);
//		}
//	}
//}
PalPlayerAttackState::PalPlayerAttackState(PalPlayer* p)
{
	player = p;
}

PalPlayerAttackState::~PalPlayerAttackState()
{
}
IDexState* PalPlayerAttackState::Update()
{
	return IDexState::Update();
}
//////////////
PalPlayerAttackStateSpawn::PalPlayerAttackStateSpawn(PalPlayer* p):PalPlayerAttackState(p)
{
}
PalPlayerAttackStateSpawn::~PalPlayerAttackStateSpawn()
{

}

void PalPlayerAttackStateSpawn::OnEnterState(std::string preType)
{
	
}

void PalPlayerAttackStateSpawn::OnLeaveState(std::string nextType)
{
}

IDexState* PalPlayerAttackStateSpawn::Update()
{
	return PalPlayerAttackState::Update();
}

//////////////
PalPlayerAttackStateMarching::PalPlayerAttackStateMarching(PalPlayer* p):PalPlayerAttackState(p)
{
}
PalPlayerAttackStateMarching::~PalPlayerAttackStateMarching()
{

}

void PalPlayerAttackStateMarching::OnEnterState(std::string preType)
{
	player->getAniModel()->SetAnimation(player->getStandIndex());
}

void PalPlayerAttackStateMarching::OnLeaveState(std::string nextType)
{
}

IDexState* PalPlayerAttackStateMarching::Update()
{
	DEX_ENSURE_P(player != NULL);
	float process = player->getHeadFlag()->getValue();
	process += player->getPlayerData()->speed;
	process = process > 1.0f ? 1.0f : process;
	player->getHeadFlag()->setValue(process);
	return PalPlayerAttackState::Update();
}
//////////////
PalPlayerAttackStateWait::PalPlayerAttackStateWait(PalPlayer* p):PalPlayerAttackState(p)
{
}
PalPlayerAttackStateWait::~PalPlayerAttackStateWait()
{

}

void PalPlayerAttackStateWait::OnEnterState(std::string preType)
{
}

void PalPlayerAttackStateWait::OnLeaveState(std::string nextType)
{
}

IDexState* PalPlayerAttackStateWait::Update()
{
	DEX_ENSURE_P(player != NULL);
	return PalPlayerAttackState::Update();
}

//////////////
PalPlayerAttackStateReady::PalPlayerAttackStateReady(PalPlayer* p):PalPlayerAttackState(p)
{
}
PalPlayerAttackStateReady::~PalPlayerAttackStateReady()
{

}

void PalPlayerAttackStateReady::OnEnterState(std::string preType)
{
	
}

void PalPlayerAttackStateReady::OnLeaveState(std::string nextType)
{
}
IDexState* PalPlayerAttackStateReady::Update()
{
	return PalPlayerAttackState::Update();
}

//////////////
PalPlayerAttackStateSelectTarget::PalPlayerAttackStateSelectTarget(PalPlayer* p):PalPlayerAttackState(p)
{
}
PalPlayerAttackStateSelectTarget::~PalPlayerAttackStateSelectTarget()
{

}

void PalPlayerAttackStateSelectTarget::OnEnterState(std::string preType)
{
	player->clearAttackTarget();
}

IDexState* PalPlayerAttackStateSelectTarget::Update()
{
	if(player->getAuto() && player->getTargets().size() == 0)
	{
		DInt8 select_enemy = 1;
		EPlayerType self_type = player->getPlayerData()->m_PlayerType;
		if(select_enemy)
		{
			TlistPlayer players;
			if(self_type == EPT_ENEMY)
			{
				players = PalGameStateBattleMain::getBattleMain()->getPlayers(EPT_ROLE);
			}
			else
			{
				players = PalGameStateBattleMain::getBattleMain()->getPlayers(EPT_ENEMY);
			}
			if(players.size()==0)
			{
				DexLog::getSingleton()->Log(log_error, "select target error! enemy size is 0!");
			}
			else
			{
				PalPlayer* target = players.front();
				player->AddTarget(target);
			}
		}
	}
	
	return PalPlayerAttackState::Update();
}


void PalPlayerAttackStateSelectTarget::OnLeaveState(std::string nextType)
{
}

//////////////
PalPlayerAttackStateSelectSkill::PalPlayerAttackStateSelectSkill(PalPlayer* p):PalPlayerAttackState(p)
{
}
PalPlayerAttackStateSelectSkill::~PalPlayerAttackStateSelectSkill()
{

}

void PalPlayerAttackStateSelectSkill::OnEnterState(std::string preType)
{
	player->setCurrSkill(NULL);
	if(!player->getAuto())
	{
		PalGameStateBattleMain::getBattleMain()->setCurrOperationPlayer(player);
	}
}

IDexState* PalPlayerAttackStateSelectSkill::Update()
{
	if(player->getAuto() && player->getCurrSkill()==NULL)
	{
		player->setCurrSkill(player->getSkillContainer()->getSlashSkill());
	}
	return PalPlayerAttackState::Update();
}

void PalPlayerAttackStateSelectSkill::OnLeaveState(std::string nextType)
{
}

//////////////
PalPlayerAttackStateRunToTarget::PalPlayerAttackStateRunToTarget(PalPlayer* p):PalPlayerAttackState(p)
{
	run_count = 0;
	run_time = 2000;
}
PalPlayerAttackStateRunToTarget::~PalPlayerAttackStateRunToTarget()
{

}

bool PalPlayerAttackStateRunToTarget::getRunToTarget()
{
	return run_count >= run_time;
}
void PalPlayerAttackStateRunToTarget::OnEnterState(std::string preType)
{
	run_count = 0;
	player->getAniModel()->SetAnimation(player->getRunIndex());
	TlistPlayer targets = player->getTargets();
	player->FaceTo((targets.front())->getStandOriginPosition());
	player->getHeadFlag()->setValue(0.0f);
}

void PalPlayerAttackStateRunToTarget::OnLeaveState(std::string nextType)
{
}
IDexState* PalPlayerAttackStateRunToTarget::Update()
{
	D3DXVECTOR3 pos = player->getStandOriginPosition();
	D3DXVECTOR3 target_pos = (player->getTargets().front())->getStandOriginPosition();
	run_count += DexTime::getSingleton()->getDeltaTime();
	run_count = run_count > run_time ? run_time:run_count;
	float percent = float(run_count)/run_time;
	D3DXVECTOR3 new_pos = pos + (target_pos - pos)*percent;
	player->setPosition(new_pos);
	return PalPlayerAttackState::Update();
}

//////////////
PalPlayerAttackStatePlayAttackAnimation::PalPlayerAttackStatePlayAttackAnimation(PalPlayer* p):PalPlayerAttackState(p)
{
}
PalPlayerAttackStatePlayAttackAnimation::~PalPlayerAttackStatePlayAttackAnimation()
{

}

void PalPlayerAttackStatePlayAttackAnimation::OnEnterState(std::string preType)
{
	DEX_ENSURE(player);
	player->getAniModel()->SetAnimation(player->getAttackIndex());
	player->getHeadFlag()->setValue(0.0f);//如果是远程技能 不会再run to target时将进度置为0 这里要设置一次
}

void PalPlayerAttackStatePlayAttackAnimation::OnLeaveState(std::string nextType)
{
}
IDexState* PalPlayerAttackStatePlayAttackAnimation::Update()
{
	player->UpdateAttackAnimationTimeCount();
	return PalPlayerAttackState::Update();
}

//////////////
PalPlayerAttackStateRunBack::PalPlayerAttackStateRunBack(PalPlayer* p):PalPlayerAttackState(p)
{
	run_count = 0;
	run_time = 1000;
}
PalPlayerAttackStateRunBack::~PalPlayerAttackStateRunBack()
{

}

bool PalPlayerAttackStateRunBack::getRunBack()
{
	return run_count >= run_time;
}
void PalPlayerAttackStateRunBack::OnEnterState(std::string preType)
{
	player->FaceTo(player->getStandOriginPosition());
	run_count = 0;
	player->getAniModel()->SetAnimation(player->getRunIndex());
}

void PalPlayerAttackStateRunBack::OnLeaveState(std::string nextType)
{
	TlistPlayer targets = player->getTargets();
	player->FaceTo((targets.front())->getStandOriginPosition());
	player->getAniModel()->SetAnimation(player->getStandIndex());
}
IDexState* PalPlayerAttackStateRunBack::Update()
{
	D3DXVECTOR3 pos = player->getStandOriginPosition();
	D3DXVECTOR3 target_pos = (player->getTargets().front())->getStandOriginPosition();
	run_count += DexTime::getSingleton()->getDeltaTime();
	run_count = run_count > run_time ? run_time:run_count;
	float percent = float(run_count)/run_time;
	D3DXVECTOR3 new_pos = target_pos + (pos - target_pos)*percent;
	player->setPosition(new_pos);
	return PalPlayerAttackState::Update();
}


//////////////
PalPlayerAttackStateSkillPlaying::PalPlayerAttackStateSkillPlaying(PalPlayer* p):PalPlayerAttackState(p)
{
}
PalPlayerAttackStateSkillPlaying::~PalPlayerAttackStateSkillPlaying()
{

}

void PalPlayerAttackStateSkillPlaying::OnEnterState(std::string preType)
{
	player->skillBegin();
}
void PalPlayerAttackStateSkillPlaying::OnLeaveState(std::string nextType)
{
}
IDexState* PalPlayerAttackStateSkillPlaying::Update()
{
	//DEX_ENSURE_P(player);
	//PalSkill* skill = player->getCurrSkill();
	//DEX_ENSURE_P(skill != NULL);
	//skill->Update(DexTime::getSingleton()->getDeltaTime());
	return PalPlayerAttackState::Update();
}