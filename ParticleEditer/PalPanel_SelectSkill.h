
#ifndef _PAL_PANEL_SELECT_SKILL_H
#define _PAL_PANEL_SELECT_SKILL_H

#include "PalPanelInterface.h"

class CDexWidgetButton;
class CDexWidgetImage;
class CDexWidgetCheckBtn;
class CDexWidgetLabel;
class PalPanel_SelectSkill: public PalPanelInterface
{
	//���ݼ��� ������ ��Ʒ�Ĳ�ͬ��widget��Ϣ���в�ͬ����ʾ
	Dex_DeclareClass(PalPanel_SelectSkill, PalPanelInterface,0)
protected:
	CDexWidgetImage*  m_pImgOrderBack;
	CDexWidgetCheckBtn* m_pCheckBtnWater;    //ˮ
	CDexWidgetCheckBtn* m_pCheckBtnFire;	 //��
	CDexWidgetCheckBtn* m_pCheckBtnThunder;  //�� 
	CDexWidgetCheckBtn* m_pCheckBtnWind;	 //��
	CDexWidgetCheckBtn* m_pCheckBtnSoil;	 //��

	CDexWidgetLabel*    m_pLabelTop;
	//CDexWidgetButton* m_pBtnAttack; //��

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