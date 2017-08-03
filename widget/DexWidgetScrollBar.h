

#pragma once
#include "DexWidget.h"


class CDexTex;
class CDexWidgetButton;
//竖直滚动条
class CDexWidgetScrollBarV: public CDexWidget
{
protected:
	stUiImage  m_backImage;
	CDexWidgetButton* m_pTopBtn;
	CDexWidgetButton* m_pBottomBtn;
	CDexWidgetButton* m_pHandleBtn;

	CEventDispather  m_ClickTopBtn;
	CEventDispather  m_ClickBottomBtn;
	CEventDispather  EDScroll;

	
	//m_iTotalValue:最大值 如左面是一串数据 共100行 但窗口只能显示10行 则m_iTotalValue为 100/10 - 1 = 9
	//                     如果是45行       则m_iTotalValue为 45/10 - 1 = 3.5
	//高度同理 如高500 但只能显示200 则m_iTotalValue为 500/200 -1 = 1.5
	float     m_fTotalValue;  
	float     m_fCurrValue;
	DInt16     m_iLength;   //长度
protected:
	virtual void ShutDown();
	
protected:
	void OnClickTopBtn(CDexObject* object, stEvent event);
	void OnClickBottomBtn(CDexObject* object,stEvent event);
	void OnDragHandleBtn(CDexObject* object,stEvent event);
	void OnResize(CDexObject* object,stEvent event);

public:
	CDexWidgetScrollBarV();
	~CDexWidgetScrollBarV();

public:
	virtual void Create();
	virtual bool Update(int delta);
	virtual void RenderThis();
	virtual void Enable(bool child);
	virtual void Disable(bool child);

	void SetLength(DInt16 length);


public:
	void SetTotalValue(float value);
	float GetCurrValue() {return m_fCurrValue;};
	void SetBackImage(string dexname, const DexRect& src_rect);
	CDexWidgetButton* GetTopBtn()	 { return m_pTopBtn;}; 
	CDexWidgetButton* GetBottomBtn() { return m_pBottomBtn;}; 
	CDexWidgetButton* GetHandleBtn() { return m_pHandleBtn;}; 
};