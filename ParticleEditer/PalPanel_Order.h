
#ifndef _PAL_PANEL_ORDER_H
#define _PAL_PANEL_ORDER_H

#include "PalPanelInterface.h"

class CDexWidgetButton;
class CDexWidgetImage;

class PalPanel_Order: public PalPanelInterface
{
	Dex_DeclareClass(PalPanel_Order,0)
protected:
	CDexWidgetImage*  m_pImgOrderBack;
	CDexWidgetButton* m_pBtnAttack; //��
	CDexWidgetButton* m_pBtnDefence;//��
	CDexWidgetButton* m_pBtnSkill;  //��
 	CDexWidgetButton* m_pBtnSkill2; //��
	CDexWidgetButton* m_pBtnItem;   //��
	CDexWidgetButton* m_pBtnEscape;  //��

public:
	PalPanel_Order();
	virtual ~PalPanel_Order();

public:
	virtual bool Initialize();

public:
	virtual void setVisible(bool visible);
	virtual bool getVisible();
	virtual bool Update(int delta);
	virtual bool Render();
	void OnClickAttack(CEventHandler* sys, CDexObject* object,stEvent event); 
	void OnClickSkill(CEventHandler* sys, CDexObject* object,stEvent event);
};
#endif