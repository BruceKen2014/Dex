

#ifndef DEXWIDGET_EDITBOX_H
#define DEXWIDGET_EDITBOX_H


#include "DexWidget.h"


class DexWidgetEditBox: public DexWidget
{
protected:
	stUiImage*   m_BackImage; //输入框背景图片
	DexColor     m_textColor;
	DInt16		 m_iTextSize;
	DInt16       m_iOffsetX;  //第一个字符的X偏移
	DInt16       m_iOffsetY;  //第一个字符的Y偏移 
	DInt16       m_iCursorTickSpeed; //光标tick speed ms 如 500ms 显示半秒后 接着隐藏半秒再显示半秒
	DInt16       m_iTickInterval;  //光标的闪烁interval
	DInt16		 m_iInputCharIndex; //输入字符的索引
	string		 m_strText;
	bool         m_bShowCursor; //是否轮到光标显示
	virtual void ShutDown();

public:
	CEventDispather  m_DisTextChange;
	CEventDispather  m_DisTextUpdate;
	CEventDispather  m_DisMiscKey; //其他按键输入
public:
	DexWidgetEditBox();
	~DexWidgetEditBox();

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
	void SetTextSize(DInt16 size);
	void SetTextColor(const DexColor& color);
	void SetText(string text);
	void  ClearText();
	void SetBackImage(char* filename, const DexRect& rect = DexRect(), bool mirro = false);
public:
	virtual void OnKeyChar(stEvent event);
	virtual void OnKeyDown(stEvent event);
};

#endif