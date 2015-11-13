

#pragma once
#include "../DexBase/typedefine.h"
#include "../DexBase/CDexObject.h"

class CDexWidget;
//桌面
class CDexDesktop:public CDexObject
{
protected:
	CDexWidget*  m_dragingWgt;  //正在拖拽的控件 一定被select 一定被focus
	
	//select wdiget可能和focus不是同一widget，如先点击一个label，那么这个label被select，然后鼠标移动到
	//一个image上面，此时focus了image，但并未select这个image
	CDexWidget*  m_selectWidget; 
	CDexWidget*  m_focusWidget; //focus 指的是鼠标所悬停的目标
	CDexWidget*  m_inputWidget;
	CDexWidget*  m_pRootWidget;
	bool         m_uiEvent;      //是否有mouse事件可处理

	int64        m_lastKeyTime;  //上次处理按键信息的时间

public:
	bool m_bRenderRect; //是否渲染ui包围矩形框
	CDexDesktop();
	~CDexDesktop();

public:
	bool Update(int delta);
	void Render();
	virtual bool OnEvent(stEvent event);
public:
	CDexWidget* getDragingWgt() ;
	void SetDragingWgt(CDexWidget* wgt);
	CDexWidget* getFocusWgt() ;
	void SetFocusWgt(CDexWidget* wgt);
	CDexWidget* getSelectWgt() ;
	void SetSelectWgt(CDexWidget* wgt);
	CDexWidget* getInputWgt() ;
	void SetInputWgt(CDexWidget* wgt);

	CDexWidget* getRootWidget();
	void SetRootWidget(CDexWidget* wgt);

	bool        getUiEvent();
	void	SetUiEvent(bool b);

	void OnKeyDown(char key_code);
	void OnKeyUp(char key_code);
	void OnKeyChar(char ch);
};

CDexDesktop* getDesktop();