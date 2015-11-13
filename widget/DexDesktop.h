

#pragma once
#include "../DexBase/typedefine.h"
#include "../DexBase/CDexObject.h"

class CDexWidget;
//����
class CDexDesktop:public CDexObject
{
protected:
	CDexWidget*  m_dragingWgt;  //������ק�Ŀؼ� һ����select һ����focus
	
	//select wdiget���ܺ�focus����ͬһwidget�����ȵ��һ��label����ô���label��select��Ȼ������ƶ���
	//һ��image���棬��ʱfocus��image������δselect���image
	CDexWidget*  m_selectWidget; 
	CDexWidget*  m_focusWidget; //focus ָ�����������ͣ��Ŀ��
	CDexWidget*  m_inputWidget;
	CDexWidget*  m_pRootWidget;
	bool         m_uiEvent;      //�Ƿ���mouse�¼��ɴ���

	int64        m_lastKeyTime;  //�ϴδ�������Ϣ��ʱ��

public:
	bool m_bRenderRect; //�Ƿ���Ⱦui��Χ���ο�
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