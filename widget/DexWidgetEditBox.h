

#ifndef DEXWIDGET_EDITBOX_H
#define DEXWIDGET_EDITBOX_H


#include "DexWidget.h"


class CDexWidgetEditBox: public CDexWidget
{
protected:
	stUiImage*  m_BackImage; //����򱳾�ͼƬ
	int16       m_iOffsetX;  //��һ���ַ���Xƫ��
	int16       m_iOffsetY;  //��һ���ַ���Yƫ�� 
	int16       m_iCursorTickSpeed; //���tick speed ms �� 500ms ��ʾ����� �������ذ�������ʾ����
	bool        m_bShowCursor; //�Ƿ��ֵ������ʾ
	int16       m_iTickInterval;  //������˸interval
	
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
	void  SetStartOffset(int16 iOffsetX, int16 iOffsetY);
	void  SetStartOffsetX(int16 iOffsetX);
	void  SetStartOffsetY(int16 iOffsetY);
	int16 GetStartOffsetX();
	int16 GetStartOffsetY();
	string GetEditText();
	void SetText(string text);
	void  ClearText();
	void SetBackImage(char* filename, const DexRect& rect = DexRect(), bool mirro = false);
public:
	virtual void OnKeyChar(stEvent event);
};

#endif