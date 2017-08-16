

#include "DexDesktop.h"
#include "DexWidget.h"
#include "../DexBase/DexLog.h"
#include "../DexBase/CDexDraw2D.h"


CDexDesktop::CDexDesktop()
{
	m_pRootWidget = NULL;
	m_dragingWgt = NULL;
	m_bRenderRect = true;
	m_focusWidget = NULL;
	m_inputWidget = NULL;
	m_selectWidget = NULL;
	get2DDrawer()->AddListener(this);
}
CDexDesktop::~CDexDesktop()
{

}

DexWidget* CDexDesktop::getDragingWgt()
{
	return m_dragingWgt;
}

void CDexDesktop::SetDragingWgt(DexWidget* wgt)
{
	m_dragingWgt = wgt;
}

DexWidget* CDexDesktop::getFocusWgt()
{
	return m_focusWidget;
}

void CDexDesktop::SetFocusWgt(DexWidget* wgt)
{
	DexLog::getSingleton()->BeginLog();
	if(m_focusWidget != NULL)
	{
		if(m_focusWidget != wgt)
		{
			DexLog::getSingleton()->Log(log_ok,"¿Ø¼þ%s lose focus!", m_focusWidget->GetName().c_str());
		}
	}
	m_focusWidget = wgt;
	if(m_focusWidget != NULL)
	{
		DexLog::getSingleton()->Log(log_ok,"¿Ø¼þ%s get focus!", m_focusWidget->GetName().c_str());
	}
	DexLog::getSingleton()->EndLog();
}

DexWidget* CDexDesktop::getSelectWgt()
{
	return m_selectWidget;
}

void CDexDesktop::SetSelectWgt(DexWidget* wgt)
{
	m_selectWidget = wgt;
}

DexWidget* CDexDesktop::getInputWgt()
{
	return m_inputWidget;
}

void CDexDesktop::SetInputWgt(DexWidget* wgt)
{
	m_inputWidget = wgt;
}

DexWidget* CDexDesktop::getRootWidget()
{
	return m_pRootWidget;
}
void CDexDesktop::SetRootWidget(DexWidget* wgt)
{
	m_pRootWidget = wgt;
}
bool CDexDesktop::getUiEvent()
{
	return m_uiEvent;
}

void CDexDesktop::SetUiEvent(bool b)
{
	m_uiEvent = b;
}

void CDexDesktop::OnKeyDown(char key_code)
{
	DEX_ENSURE(m_selectWidget);
	stEvent event;
	event.push_args(stArgs(key_code));
	m_selectWidget->OnKeyDown(event);
}

void CDexDesktop::OnKeyUp(char key_code)
{

}

void CDexDesktop::OnKeyChar(char ch)
{
	DEX_ENSURE(m_selectWidget);
	stEvent event;
	event.push_args(stArgs(ch));
	m_selectWidget->OnKeyChar(event);
}

CDexDesktop* getDesktop()
{
	static CDexDesktop *g_desktop = new CDexDesktop();
	return g_desktop;
}

bool CDexDesktop::Update(int delta)
{
	return true;
}

void CDexDesktop::Render()
{

}

bool CDexDesktop::OnEvent(stEvent event)
{
	if(event.id == CDexDraw2DOrth::g_moveCamera.id)
	{
		stArgs delta_y;event.pop_args(delta_y);
		stArgs delta_x;event.pop_args(delta_x);
		if(m_pRootWidget != NULL)
		{
			m_pRootWidget->OffSet(DexPointF(delta_x.f_member, delta_y.f_member));
		}
	}
	return false;
}