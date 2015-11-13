

#ifndef DEXWIDGET_LABEL_H
#define DEXWIDGET_LABEL_H


#include "DexWidget.h"


class CDexWidgetLabel: public CDexWidget
{
protected:
	int16    m_iTextSize;
	string m_strText;
	DexColor m_textColor;	  //文字颜色
	DexColor m_textBackColor; //文字背景色 
	virtual void ShutDown();
public:
	CDexWidgetLabel();
	virtual ~CDexWidgetLabel();

public:
	virtual bool Update(int delta);
	virtual void RenderThis();
	virtual void Enable(bool child);
	virtual void Disable(bool child);

public:
	string GetText();
	void SetText(string text);
	DexColor GetTextColor() const{ m_textColor;}
	DexColor GetTexBackColor()const { m_textBackColor;}
	void SetTextColor(const DexColor& color)  { m_textColor = color;}
	void SetTexBackColor(const DexColor& color) { m_textBackColor = color;}
	
	int16 GetTextSize() const { return m_iTextSize;}
	void SetTextSize(int16 size) { m_iTextSize = size;}
	
	
};

#endif