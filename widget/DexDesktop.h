

#pragma once
#include "../DexBase/typedefine.h"
#include "../DexBase/DexObject.h"

class DexWidget;
//桌面
class CDexDesktop:public DexObject
{
protected:
	DexWidget*  m_dragingWgt;  //正在拖拽的控件 一定被select 一定被focus
	
	//select wdiget可能和focus不是同一widget，如先点击一个label，那么这个label被select，然后鼠标移动到
	//一个image上面，此时focus了image，但并未select这个image
	DexWidget*  m_selectWidget; 
	DexWidget*  m_focusWidget; //focus 指的是鼠标所悬停的目标
	DexWidget*  m_inputWidget;
	DexWidget*  m_pRootWidget;
	bool         m_uiEvent;      //是否有mouse事件可处理

	DInt64        m_lastKeyTime;  //上次处理按键信息的时间

public:
	bool m_bRenderRect; //是否渲染ui包围矩形框
	CDexDesktop();
	~CDexDesktop();

public:
	bool Update(int delta);
	void Render();
	virtual bool OnEvent(stEvent event);
public:
	DexWidget* getDragingWgt() ;
	void SetDragingWgt(DexWidget* wgt);
	DexWidget* getFocusWgt() ;
	void SetFocusWgt(DexWidget* wgt);
	DexWidget* getSelectWgt() ;
	void SetSelectWgt(DexWidget* wgt);
	DexWidget* getInputWgt() ;
	void SetInputWgt(DexWidget* wgt);

	DexWidget* getRootWidget();
	void SetRootWidget(DexWidget* wgt);

	bool        getUiEvent();
	void	SetUiEvent(bool b);

	void OnKeyDown(char key_code);
	void OnKeyUp(char key_code);
	void OnKeyChar(char ch);
};

CDexDesktop* getDesktop();