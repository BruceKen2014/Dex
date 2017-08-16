

#ifndef DEXWIDGET_EDITBOX_H
#define DEXWIDGET_EDITBOX_H


#include "DexWidget.h"


class DexWidgetEditBox: public DexWidget
{
protected:
	stUiImage*   m_BackImage; //����򱳾�ͼƬ
	DexColor     m_textColor;
	DInt16		 m_iTextSize;
	DInt16       m_iOffsetX;  //��һ���ַ���Xƫ��
	DInt16       m_iOffsetY;  //��һ���ַ���Yƫ�� 
	DInt16       m_iCursorTickSpeed; //���tick speed ms �� 500ms ��ʾ����� �������ذ�������ʾ����
	DInt16       m_iTickInterval;  //������˸interval
	DInt16		 m_iInputCharIndex; //�����ַ�������
	string		 m_strText;
	bool         m_bShowCursor; //�Ƿ��ֵ������ʾ
	virtual void ShutDown();

public:
	CEventDispather  m_DisTextChange;
	CEventDispather  m_DisTextUpdate;
	CEventDispather  m_DisMiscKey; //������������
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