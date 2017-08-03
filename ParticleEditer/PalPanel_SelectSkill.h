
#ifndef _PAL_PANEL_SELECT_SKILL_H
#define _PAL_PANEL_SELECT_SKILL_H

#include "PalPanelInterface.h"

class CDexWidgetButton;
class CDexWidgetImage;
class CDexWidgetCheckBtn;
class CDexWidgetLabel;
class PalPanel_SelectSkill: public PalPanelInterface
{
	//根据技能 法术和 物品的不同对widget信息进行不同的显示
	Dex_DeclareClass(PalPanel_SelectSkill, PalPanelInterface,0)
protected:
	CDexWidgetImage*  m_pImgOrderBack;
	CDexWidgetCheckBtn* m_pCheckBtnWater;    //水
	CDexWidgetCheckBtn* m_pCheckBtnFire;	 //火
	CDexWidgetCheckBtn* m_pCheckBtnThunder;  //雷 
	CDexWidgetCheckBtn* m_pCheckBtnWind;	 //风
	CDexWidgetCheckBtn* m_pCheckBtnSoil;	 //土

	CDexWidgetLabel*    m_pLabelTop;
	//CDexWidgetButton* m_pBtnAttack; //攻

public:
	PalPanel_SelectSkill();
	virtual ~PalPanel_SelectSkill();

public:
	virtual bool Initialize();
public:
	virtual void setVisible(bool visible);
	virtual bool getVisible();
	virtual bool Update(int delta);
	virtual void OnTimer(const stTimer& timer);
	virtual bool Render();
	void OnClickSkillWater(CEventHandler* sys, CDexObject* object,stEvent event); 
	void OnClickSkillFire(CEventHandler* sys, CDexObject* object,stEvent event); 
	void OnClickSkillThunder(CEventHandler* sys, CDexObject* object,stEvent event); 
	void OnClickSkillWind(CEventHandler* sys, CDexObject* object,stEvent event); 
	void OnClickSkillSoil(CEventHandler* sys, CDexObject* object,stEvent event); 
	void OnClickSelectSkill(CEventHandler* sys, CDexObject* object,stEvent event);
	void OnSelectSkill(int skill_id);
};
#endif