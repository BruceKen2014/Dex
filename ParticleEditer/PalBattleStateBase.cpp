
#include "DexBase/DexCollideObject.h"
#include "state/DexGameEngine.h"
#include "PalStateBattleMain.h"
#include "PalPlayer.h"
#include "PalBattleStateBase.h"
#include "PalPanel_Order.h"
#include "PalPanel_SelectSkill.h"

PalBattleStateMachine::PalBattleStateMachine()
{

}
PalBattleStateMachine::~PalBattleStateMachine()
{

}
bool PalBattleStateMachine::Update()
{
	return IDexStateMachine::Update();
}

bool PalBattleStateMachine::Render()
{
	if(getCurrState() != NULL)
	{
		getCurrState()->Render();
	}
	return true;
}

void PalBattleStateMachine::MouseLDown(int xPos, int yPos)
{
	if(getCurrState() != NULL)
	{
		getCurrState()->MouseLDown(xPos, yPos);
	}
}
void PalBattleStateMachine::MouseLUp(int xPos, int yPos)
{
	if(getCurrState() != NULL)
	{
		getCurrState()->MouseLUp(xPos, yPos);
	}
}
void PalBattleStateMachine::MouseRUp(int xPos, int yPos)
{
	if(getCurrState() != NULL)
	{
		getCurrState()->MouseRUp(xPos, yPos);
	}
}
//================================================
PalBattleStateBase::PalBattleStateBase()
{
}

PalBattleStateBase::~PalBattleStateBase()
{
}

bool PalBattleStateBase::Render()
{
	return true;
}

void PalBattleStateBase::KeyDown()
{

}
//====================================================
PalBattleStateMoveCamera::PalBattleStateMoveCamera()
{

}
PalBattleStateMoveCamera::~PalBattleStateMoveCamera()
{

}
IDexState* PalBattleStateMoveCamera::Update()
{
	return IDexState::Update();
}
bool PalBattleStateMoveCamera::Render()
{
	return PalBattleStateBase::Render();
}
void PalBattleStateMoveCamera::OnEnterState(std::string preType)
{

}

void PalBattleStateMoveCamera::OnLeaveState(std::string nextType)
{

}
void PalBattleStateMoveCamera::KeyDown()
{
	PalBattleStateBase::KeyDown();
}


//====================================================
PalBattleStateLabelMove::PalBattleStateLabelMove()
{

}
PalBattleStateLabelMove::~PalBattleStateLabelMove()
{

}
IDexState* PalBattleStateLabelMove::Update()
{
	return IDexState::Update();
}
bool PalBattleStateLabelMove::Render()
{
	return PalBattleStateBase::Render();
}
void PalBattleStateLabelMove::OnEnterState(std::string preType)
{

}

void PalBattleStateLabelMove::OnLeaveState(std::string nextType)
{

}
void PalBattleStateLabelMove::KeyDown()
{
	PalBattleStateBase::KeyDown();
}

//====================================================
PalBattleStateSelectSkill::PalBattleStateSelectSkill()
{

}
PalBattleStateSelectSkill::~PalBattleStateSelectSkill()
{

}
IDexState* PalBattleStateSelectSkill::Update()
{
	return IDexState::Update();
}
bool PalBattleStateSelectSkill::Render()
{
	return PalBattleStateBase::Render();
}
void PalBattleStateSelectSkill::OnEnterState(std::string preType)
{
	PalPlayer* player = PalGameStateBattleMain::getBattleMain()->getCurrOperationPlayer();
	DEX_ENSURE(player != NULL);
	PalGameStateBattleMain::getBattleMain()->setVisible(PalPanel_Order::getClassType(), true);
}

void PalBattleStateSelectSkill::OnLeaveState(std::string nextType)
{
	PalGameStateBattleMain::getBattleMain()->setVisible(PalPanel_Order::getClassType(), false);
}
void PalBattleStateSelectSkill::KeyDown()
{
	PalBattleStateBase::KeyDown();
}

void PalBattleStateSelectSkill::MouseLUp(int xPos, int yPos)
{

}

void PalBattleStateSelectSkill::MouseRUp(int xPos, int yPos)
{
	if(!PalGameStateBattleMain::getBattleMain()->getVisible(PalPanel_Order::getClassType()))
		PalGameStateBattleMain::getBattleMain()->setVisible(PalPanel_Order::getClassType(), true);
	if(PalGameStateBattleMain::getBattleMain()->getVisible(PalPanel_SelectSkill::getClassType()))
		PalGameStateBattleMain::getBattleMain()->setVisible(PalPanel_SelectSkill::getClassType(), false);
}

//====================================================
PalBattleStateSelectTarget::PalBattleStateSelectTarget()
{

}
PalBattleStateSelectTarget::~PalBattleStateSelectTarget()
{

}
IDexState* PalBattleStateSelectTarget::Update()
{
	return IDexState::Update();
}
bool PalBattleStateSelectTarget::Render()
{
	return PalBattleStateBase::Render();
}
void PalBattleStateSelectTarget::OnEnterState(std::string preType)
{

}

void PalBattleStateSelectTarget::OnLeaveState(std::string nextType)
{

}
void PalBattleStateSelectTarget::KeyDown()
{
	PalBattleStateBase::KeyDown();
}

void PalBattleStateSelectTarget::MouseLDown(int xPos, int yPos)
{

}
void PalBattleStateSelectTarget::MouseLUp(int xPos, int yPos)
{
	PalPlayer* player = PalGameStateBattleMain::getBattleMain()->getCurrOperationPlayer();
	DEX_ENSURE(player);
	stRay ray;
	DexGameEngine::getEngine()->GetRay(xPos, yPos, ray);
	DexCollideObject* collide_object = NULL;
	{
		TlistPlayer players = PalGameStateBattleMain::getBattleMain()->getPlayers(EPT_ENEMY);
		foreach(TlistPlayer, ite, players)
		{
			collide_object = (*ite)->getCollideObject();
			if(collide_object->checkPick(ray))
			{
				player->AddTarget(*ite);
				PalGameStateBattleMain::getBattleMain()->setCurrOperationPlayer(NULL);
				break;
			}
		}
		player->AddTarget(players.front());
		PalGameStateBattleMain::getBattleMain()->setCurrOperationPlayer(NULL);
	}
}
void PalBattleStateSelectTarget::MouseRUp(int xPos, int yPos)
{

}









