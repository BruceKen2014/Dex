
#ifndef _PAL_PANEL_ORDER_H
#define _PAL_PANEL_ORDER_H

#include "widget/DexPanelInterface.h"

class CDexWidgetButton;
class DexWidgetImage;

class PalPanel_Order: public DexPanelInterface
{
	Dex_DeclareClass(PalPanel_Order, DexPanelInterface,0)
protected:
	DexWidgetImage*  m_pImgOrderBack;
	CDexWidgetButton* m_pBtnAttack; //π•
	CDexWidgetButton* m_pBtnDefence;//∑¿
	CDexWidgetButton* m_pBtnSkill;  // ı
 	CDexWidgetButton* m_pBtnSkill2; //ºº
	CDexWidgetButton* m_pBtnItem;   //ŒÔ
	CDexWidgetButton* m_pBtnEscape;  //Ã”

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
	void OnClickAttack(CEventHandler* sys, DexObject* object,stEvent event); 
	void OnClickSkill(CEventHandler* sys, DexObject* object,stEvent event);
};
#endif