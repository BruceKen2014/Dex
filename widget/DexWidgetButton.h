


#pragma once

#include "DexWidget.h"


enum EBtnState
{
	btn_normal,
	btn_mouse_on,
	btn_mouse_down,
	btn_useless,
	btn_count
};
class CDexTex;
class CDexWidgetButton: public CDexWidget
{
protected:
	string    m_texnames[btn_count];
	CDexTex*  m_texs[btn_count];
	DexRectF   m_srcArea[btn_count]; //�����е�����

	EBtnState m_state;

protected:
	virtual void ShutDown();

protected:
	virtual void OnMouseMoveIn(stEvent event);
	virtual void OnMouseMoveOut(stEvent event);
	virtual bool OnMouseLUp(stEvent event);
	virtual void OnMouseLDown(stEvent event);
	
	//��Ҫ���أ��������ڰ�ť�ϰ��£�Ȼ���ƶ�����ť����ſ�����CDexWidget���ж���굯����Ч����ִ��OnMouseLUp����
	//��������ɰ�ťһֱ���ڰ���״̬
	virtual bool MouseLUpValid(stEvent event); 
public:
	CDexWidgetButton();
	~CDexWidgetButton();

public:
	virtual bool Update(int delta);
	virtual void RenderThis();
	virtual void Enable(bool child);
	virtual void Disable(bool child);


public:
	//һ����ť����ֻ��һ��ͼƬ����һ�㡢���������¡���Ч����ͬһ��Դ
	void SetStateTex(EBtnState state, string tex);
	void SetStateTexRect(EBtnState state, const DexRectF& rect);
	void SetStateTexRect(EBtnState state, const DexRect& rect);
};