

#pragma once
#include "../DexBase/typedefine.h"
#include "../DexBase/DexObject.h"

class DexWidget;
//����
class CDexDesktop:public DexObject
{
protected:
	DexWidget*  m_dragingWgt;  //������ק�Ŀؼ� һ����select һ����focus
	
	//select wdiget���ܺ�focus����ͬһwidget�����ȵ��һ��label����ô���label��select��Ȼ������ƶ���
	//һ��image���棬��ʱfocus��image������δselect���image
	DexWidget*  m_selectWidget; 
	DexWidget*  m_focusWidget; //focus ָ�����������ͣ��Ŀ��
	DexWidget*  m_inputWidget;
	DexWidget*  m_pRootWidget;
	bool         m_uiEvent;      //�Ƿ���mouse�¼��ɴ���

	DInt64        m_lastKeyTime;  //�ϴδ�������Ϣ��ʱ��

public:
	bool m_bRenderRect; //�Ƿ���Ⱦui��Χ���ο�
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