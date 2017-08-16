
#ifndef _PAL_PANEL_SELECT_SKILL_H
#define _PAL_PANEL_SELECT_SKILL_H

#include "widget/DexPanelInterface.h"

class CDexWidgetButton;
class DexWidgetImage;
class CDexWidgetCheckBtn;
class CDexWidgetLabel;
class PalPanel_SelectSkill: public DexPanelInterface
{
	//���ݼ��� ������ ��Ʒ�Ĳ�ͬ��widget��Ϣ���в�ͬ����ʾ
	Dex_DeclareClass(PalPanel_SelectSkill, DexPanelInterface,0)
protected:
	DexWidgetImage*  m_pImgOrderBack;
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
	void OnClickSkillWater(CEventHandler* sys, DexObject* object,stEvent event); 
	void OnClickSkillFire(CEventHandler* sys, DexObject* object,stEvent event); 
	void OnClickSkillThunder(CEventHandler* sys, DexObject* object,stEvent event); 
	void OnClickSkillWind(CEventHandler* sys, DexObject* object,stEvent event); 
	void OnClickSkillSoil(CEventHandler* sys, DexObject* object,stEvent event); 
	void OnClickSelectSkill(CEventHandler* sys, DexObject* object,stEvent event);
	void OnSelectSkill(int skill_id);
};
#endif