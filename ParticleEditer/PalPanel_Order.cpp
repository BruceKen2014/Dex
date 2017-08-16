

#include "widget/DexWidgetImage.h"
#include "widget/DexWidgetButton.h"
#include "widget/DexWidgetFactory.h"
#include "DexBase/typedefine.h"

#include "PalPlayer.h"
#include "PalSkill.h"
#include "PalStateBattleMain.h"
#include "PalPanel_SelectSkill.h"
#include "PalPanel_Order.h"

PalPanel_Order::PalPanel_Order()
{
	m_pImgOrderBack = NULL;
	m_pBtnAttack = NULL;
	m_pBtnDefence = NULL;
	m_pBtnSkill = NULL;
	m_pBtnSkill2 = NULL;
	m_pBtnItem = NULL;
	m_pBtnEscape = NULL;
}

PalPanel_Order::~PalPanel_Order()
{
	UI_DROP_REF(m_pImgOrderBack);

	UI_DROP_REF(m_pBtnAttack);
	UI_DROP_REF(m_pBtnSkill);
}

bool PalPanel_Order::Initialize()
{
	m_pImgOrderBack = (DexWidgetImage* )getWidgetFactory()->useWidget("order_back");
	m_pBtnAttack = (CDexWidgetButton* )getWidgetFactory()->useWidget("order_back.attack");
	m_pBtnSkill = (CDexWidgetButton* )getWidgetFactory()->useWidget("order_back.skill");
	m_pBtnAttack->m_MouseLUp +=  UI_DELEGATE(this, PalPanel_Order, OnClickAttack);
	m_pBtnSkill->m_MouseLUp +=  UI_DELEGATE(this, PalPanel_Order, OnClickSkill);
	return true;
}

void PalPanel_Order::setVisible(bool visible)
{
	DEX_ENSURE(m_pImgOrderBack);
	m_pImgOrderBack->setVisible(visible);
}

bool PalPanel_Order::getVisible()
{
	DEX_ENSURE_B(m_pImgOrderBack);
	return m_pImgOrderBack->GetVisible();
}

bool PalPanel_Order::Update(int delta)
{
	m_pImgOrderBack->Update(delta);
	return DexPanelInterface::Update(delta);
}

bool PalPanel_Order::Render()
{
	if(m_pImgOrderBack != NULL)
	{
		m_pImgOrderBack->Render();
	}
	return true;
}

void PalPanel_Order::OnClickAttack(CEventHandler* sys, DexObject* object,stEvent event)
{
	PalPlayer* player = PalGameStateBattleMain::getBattleMain()->getCurrOperationPlayer();
	DEX_ENSURE(player != NULL);
	player->setCurrSkill(player->getSkillContainer()->getSlashSkill());
}

void PalPanel_Order::OnClickSkill(CEventHandler* sys, DexObject* object,stEvent event)
{
	setVisible(false);
	PalGameStateBattleMain::getBattleMain()->setVisible(PalPanel_SelectSkill::getClassType(),true);
}