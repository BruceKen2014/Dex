

#pragma once
#include "DexWidget.h"


class CDexTex;
class CDexWidgetButton;
//��ֱ������
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

	
	//m_iTotalValue:���ֵ ��������һ������ ��100�� ������ֻ����ʾ10�� ��m_iTotalValueΪ 100/10 - 1 = 9
	//                     �����45��       ��m_iTotalValueΪ 45/10 - 1 = 3.5
	//�߶�ͬ�� ���500 ��ֻ����ʾ200 ��m_iTotalValueΪ 500/200 -1 = 1.5
	float     m_fTotalValue;  
	float     m_fCurrValue;
	DInt16     m_iLength;   //����
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