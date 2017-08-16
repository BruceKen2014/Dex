

#include "PalPanel_SelectSkill.h"



#include "widget/DexWidgetImage.h"
#include "widget/DexWidgetButton.h"
#include "widget/DexWidgetCheckBtn.h"
#include "widget/DexWidgetLabel.h"
#include "widget/DexWidgetFactory.h"
#include "widget/DexWidgetEditBox.h"
#include "DexBase/typedefine.h"
#include "DexBase/DexLog.h"

#include "PalPlayer.h"
#include "PalSkill.h"
#include "PalStateBattleMain.h"

#define Test_Timer 1001
PalPanel_SelectSkill::PalPanel_SelectSkill()
{
	AddTimer(Test_Timer, 1000);
}
PalPanel_SelectSkill::~PalPanel_SelectSkill()
{
	UI_DROP_REF(m_pImgOrderBack);

	UI_DROP_REF(m_pCheckBtnWater);
	UI_DROP_REF(m_pCheckBtnFire);
	UI_DROP_REF(m_pCheckBtnThunder);
	UI_DROP_REF(m_pCheckBtnWind);
	UI_DROP_REF(m_pCheckBtnSoil);

	UI_DROP_REF(m_pLabelTop);
}

bool PalPanel_SelectSkill::Initialize()
{
	m_pImgOrderBack = (DexWidgetImage* )getWidgetFactory()->useWidget("skill_back");

	m_pCheckBtnWater = (CDexWidgetCheckBtn* )getWidgetFactory()->useWidget("skill_back.skill_water");
	m_pCheckBtnFire = (CDexWidgetCheckBtn* )getWidgetFactory()->useWidget("skill_back.skill_fire");
	m_pCheckBtnThunder = (CDexWidgetCheckBtn* )getWidgetFactory()->useWidget("skill_back.skill_thunder");
	m_pCheckBtnWind = (CDexWidgetCheckBtn* )getWidgetFactory()->useWidget("skill_back.skill_wind");
	m_pCheckBtnSoil = (CDexWidgetCheckBtn* )getWidgetFactory()->useWidget("skill_back.skill_soil");
	m_pCheckBtnSoil->getArgs().i_member = 100;
	m_pLabelTop = (CDexWidgetLabel*)getWidgetFactory()->useWidget("skill_back.top_name");

	//m_pLabelTop->SetTexBackColor(DEXCOLOR_GREEN);

	m_pCheckBtnWater->SetChecked(true);
	m_pCheckBtnWater->m_MouseLUp +=  UI_DELEGATE(this, PalPanel_SelectSkill, OnClickSkillWater);
	m_pCheckBtnFire->m_MouseLUp +=  UI_DELEGATE(this, PalPanel_SelectSkill, OnClickSkillFire);
	m_pCheckBtnThunder->m_MouseLUp +=  UI_DELEGATE(this, PalPanel_SelectSkill, OnClickSkillThunder);
	m_pCheckBtnWind->m_MouseLUp +=  UI_DELEGATE(this, PalPanel_SelectSkill, OnClickSkillWind);
	m_pCheckBtnSoil->m_MouseLUp +=  UI_DELEGATE(this, PalPanel_SelectSkill, OnClickSkillSoil);
	return true;
}

void PalPanel_SelectSkill::setVisible(bool visible)
{
	DEX_ENSURE(m_pImgOrderBack);
	m_pImgOrderBack->setVisible(visible);
}

bool PalPanel_SelectSkill::getVisible()
{
	DEX_ENSURE_B(m_pImgOrderBack);
	return m_pImgOrderBack->GetVisible();
}

bool PalPanel_SelectSkill::Update(int delta)
{
	//int delta = DexTime::getSingleton()->getDeltaTime();
	m_pImgOrderBack->Update(delta);
	//CDexWidgetLabel*label =(CDexWidgetLabel*) getWidgetFactory()->createWidget(widget_label, "testLabel");
	//UI_DROP_REF(label);
	//CDexWidgetImage* image =(CDexWidgetImage*) getWidgetFactory()->createWidget(widget_image, "testImage");
	//UI_DROP_REF(image);
	//CDexWidgetButton* button =(CDexWidgetButton*) getWidgetFactory()->createWidget(widget_button, "testButton");
	//UI_DROP_REF(button);
	//CDexWidgetCheckBtn* chk_button =(CDexWidgetCheckBtn*) getWidgetFactory()->createWidget(widget_check_button, "testCheckButton");
	//UI_DROP_REF(chk_button);
	//CDexWidgetEditBox* eidtbox =(CDexWidgetEditBox*) getWidgetFactory()->createWidget(widget_editbox, "testEditBox");
	//UI_DROP_REF(eidtbox);
	//eidtbox->getArgs().i_member = 100;
	return DexPanelInterface::Update(delta);
}

void PalPanel_SelectSkill::OnTimer(const stTimer& timer)
{
	switch (timer.id)
	{
	case Test_Timer:
		{
			//CDexWidgetLabel*label =(CDexWidgetLabel*) getWidgetFactory()->createWidget(widget_label, "testLabel");
			//UI_DROP_REF(label);
		}
		break;
	}
}

bool PalPanel_SelectSkill::Render()
{
	if(m_pImgOrderBack != NULL)
	{
		m_pImgOrderBack->Render();
	}
	return true;
}

void PalPanel_SelectSkill::OnClickSkillWater(CEventHandler* sys, DexObject* object,stEvent event)
{
	PalPanel_SelectSkill* pThis = (PalPanel_SelectSkill*)sys;
	bool checked = pThis->m_pCheckBtnWater->GetChecked();
	if(!checked)
	{
		pThis->m_pCheckBtnFire->SetChecked(false);
		pThis->m_pCheckBtnThunder->SetChecked(false);
		pThis->m_pCheckBtnWind->SetChecked(false);
		pThis->m_pCheckBtnSoil->SetChecked(false);
	}
}

void PalPanel_SelectSkill::OnClickSkillFire(CEventHandler* sys, DexObject* object,stEvent event)
{
	PalPanel_SelectSkill* pThis = (PalPanel_SelectSkill*)sys;
	bool checked = pThis->m_pCheckBtnFire->GetChecked();
	if(!checked)
	{
		pThis->m_pCheckBtnWater->SetChecked(false);
		pThis->m_pCheckBtnThunder->SetChecked(false);
		pThis->m_pCheckBtnWind->SetChecked(false);
		pThis->m_pCheckBtnSoil->SetChecked(false);
	}
}
void PalPanel_SelectSkill::OnClickSkillThunder(CEventHandler* sys, DexObject* object,stEvent event)
{
	PalPanel_SelectSkill* pThis = (PalPanel_SelectSkill*)sys;
	bool checked = pThis->m_pCheckBtnThunder->GetChecked();
	if(!checked)
	{
		pThis->m_pCheckBtnWater->SetChecked(false);
		pThis->m_pCheckBtnFire->SetChecked(false);
		pThis->m_pCheckBtnWind->SetChecked(false);
		pThis->m_pCheckBtnSoil->SetChecked(false);
	}
}
void PalPanel_SelectSkill::OnClickSkillWind(CEventHandler* sys, DexObject* object,stEvent event)
{
	PalPanel_SelectSkill* pThis = (PalPanel_SelectSkill*)sys;
	bool checked = pThis->m_pCheckBtnWind->GetChecked();
	if(!checked)
	{
		pThis->m_pCheckBtnWater->SetChecked(false);
		pThis->m_pCheckBtnFire->SetChecked(false);
		pThis->m_pCheckBtnThunder->SetChecked(false);
		pThis->m_pCheckBtnSoil->SetChecked(false);
	}
}

void PalPanel_SelectSkill::OnClickSkillSoil(CEventHandler* sys, DexObject* object,stEvent event)
{
	PalPanel_SelectSkill* pThis = (PalPanel_SelectSkill*)sys;
	bool checked = pThis->m_pCheckBtnSoil->GetChecked();
	if(!checked)
	{
		pThis->m_pCheckBtnWater->SetChecked(false);
		pThis->m_pCheckBtnFire->SetChecked(false);
		pThis->m_pCheckBtnThunder->SetChecked(false);
		pThis->m_pCheckBtnWind->SetChecked(false);
	}
}

void PalPanel_SelectSkill::OnClickSelectSkill(CEventHandler* sys, DexObject* object,stEvent event)
{
}

void PalPanel_SelectSkill::OnSelectSkill(int skill_id)
{
	switch(skill_id)
	{
	case 1:
		{
			break;
		}
	default:
		break;

	}
}

