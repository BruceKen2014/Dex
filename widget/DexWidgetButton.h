


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
	DexRectF   m_srcArea[btn_count]; //纹理中的区域

	EBtnState m_state;

protected:
	virtual void ShutDown();

protected:
	virtual void OnMouseMoveIn(stEvent event);
	virtual void OnMouseMoveOut(stEvent event);
	virtual bool OnMouseLUp(stEvent event);
	virtual void OnMouseLDown(stEvent event);
	
	//需要重载，如果鼠标在按钮上按下，然后移动到按钮外面放开，则CDexWidget会判断鼠标弹起无效，不执行OnMouseLUp方法
	//这样会造成按钮一直处于按下状态
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
	//一个按钮可以只有一副图片，即一般、高亮、按下、无效都是同一资源
	void SetStateTex(EBtnState state, string tex);
	void SetStateTexRect(EBtnState state, const DexRectF& rect);
	void SetStateTexRect(EBtnState state, const DexRect& rect);
};