

#ifndef DEXWIDGET_EDITBOX_H
#define DEXWIDGET_EDITBOX_H


#include "DexWidget.h"


class CDexWidgetEditBox: public CDexWidget
{
protected:
	stUiImage*  m_BackImage; //����򱳾�ͼƬ
	DInt16       m_iOffsetX;  //��һ���ַ���Xƫ��
	DInt16       m_iOffsetY;  //��һ���ַ���Yƫ�� 
	DInt16       m_iCursorTickSpeed; //���tick speed ms �� 500ms ��ʾ����� �������ذ�������ʾ����
	bool        m_bShowCursor; //�Ƿ��ֵ������ʾ
	DInt16       m_iTickInterval;  //������˸interval
	
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