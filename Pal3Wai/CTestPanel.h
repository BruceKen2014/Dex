


#pragma once

#include "../DexBase/CEventHandler.h"
#include "../widget/DexWidgetImage.h"
#include "../widget/DexWidgetButton.h"
#include "../widget/DexWidgetImageSequence.h"
#include "../widget/DexWidgetCheckBtn.h"
#include "../widget/DexWidgetEditBox.h"
class CDexWidgetButton;
class CDexWidgetImageSequence;
class CDexWidgetLabel;
class CTestPanel:public CEventHandler
{
protected:
	CDexWidgetImage*  m_backImage;
	CDexWidgetImage*  m_image1;
	CDexWidgetImage*  m_image2;
	CDexWidgetButton* m_btn1;
	CDexWidgetButton* m_btn2;
	CDexWidgetLabel*  m_label;
	CDexWidgetLabel*  m_label2;
	CDexWidgetEditBox* m_editBox;
	CDexWidgetImageSequence* m_imagesequence;
	CDexWidgetCheckBtn*  m_checkBtn1;
	//CDexWidgetScrollBarV*  m_scrollV;
	int16 g_iMouseLX, g_iMouseLY;
	bool  g_bInTriangle;

	CDexWidget* m_focusWidget;

	DexPoint g_PntA;
	DexPoint g_PntB;
	DexPoint g_PntC;
public:
	CTestPanel();
	virtual ~CTestPanel();

public:
	virtual bool Init();
	virtual void Render() ;
	virtual void Update(int delta);
	virtual void ShutDown();

public:
	void OnMouseLDown(int mouse_x, int mouse_y);
	void OnClickBtn(CEventHandler* sys, CDexObject* object, stEvent event);
	void OnClickBtn2(CEventHandler* sys,CDexObject* object,stEvent event);
	void OnEditTextChange(CEventHandler* sys,CDexObject* object,stEvent event);
	void OnEditTextUpdate(CEventHandler* sys,CDexObject* object,stEvent event);
};
