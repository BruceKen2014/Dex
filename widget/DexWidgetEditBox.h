

#ifndef DEXWIDGET_EDITBOX_H
#define DEXWIDGET_EDITBOX_H


#include "DexWidget.h"


class CDexWidgetEditBox: public CDexWidget
{
protected:
	stUiImage*  m_BackImage; //输入框背景图片
	DInt16       m_iOffsetX;  //第一个字符的X偏移
	DInt16       m_iOffsetY;  //第一个字符的Y偏移 
	DInt16       m_iCursorTickSpeed; //光标tick speed ms 如 500ms 显示半秒后 接着隐藏半秒再显示半秒
	bool        m_bShowCursor; //是否轮到光标显示
	DInt16       m_iTickInterval;  //光标的闪烁interval
	
	string m_strText;
	virtual void ShutDown();

public:
	CEventDispather  m_DisTextChange;
	CEventDispather  m_DisTextUpdate;
public:
	CDexWidgetEditBox();
	~CDexWidgetEditBox();

public:
	virtual bool Update(int delta);
	virtual void RenderThis();
	virtual void Enable(bool child);
	virtual void Disable(bool child);

public:
	void  SetStartOffset(DInt16 iOffsetX, DInt16 iOffsetY);
	void  SetStartOffsetX(DInt16 iOffsetX);
	void  SetStartOffsetY(DInt16 iOffsetY);
	DInt16 GetStartOffsetX();
	DInt16 GetStartOffsetY();
	string GetEditText();
	void SetText(string text);
	void  ClearText();
	void SetBackImage(char* filename, const DexRect& rect = DexRect(), bool mirro = false);
public:
	virtual void OnKeyChar(stEvent event);
};

#endif