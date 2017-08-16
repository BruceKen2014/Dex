
#ifndef _PAL_PANEL_SELECT_SKILL_H
#define _PAL_PANEL_SELECT_SKILL_H

#include "widget/DexPanelInterface.h"

class CDexWidgetButton;
class DexWidgetImage;
class CDexWidgetCheckBtn;
class CDexWidgetLabel;
class PalPanel_SelectSkill: public DexPanelInterface
{
	//根据技能 法术和 物品的不同对widget信息进行不同的显示
	Dex_DeclareClass(PalPanel_SelectSkill, DexPanelInterface,0)
protected:
	DexWidgetImage*  m_pImgOrderBack;
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
	void OnClickSkillWater(CEventHandler* sys, DexObject* object,stEvent event); 
	void OnClickSkillFire(CEventHandler* sys, DexObject* object,stEvent event); 
	void OnClickSkillThunder(CEventHandler* sys, DexObject* object,stEvent event); 
	void OnClickSkillWind(CEventHandler* sys, DexObject* object,stEvent event); 
	void OnClickSkillSoil(CEventHandler* sys, DexObject* object,stEvent event); 
	void OnClickSelectSkill(CEventHandler* sys, DexObject* object,stEvent event);
	void OnSelectSkill(int skill_id);
};
#endif