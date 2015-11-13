


#include "DexWidgetFactory.h"
#include "../DexBase/CDexDraw2D.h"
#include "../DexBase/DexLog.h"
#include "../Source/CTexture.h"

#include "DexWidgetButton.h"
#include "CDexUiSrcMgr.h"
#include "DexWidgetContainer.h"
#include "DexDesktop.h"
#include "DexWidgetScrollBar.h"


CDexWidgetScrollBarV::CDexWidgetScrollBarV()
{
	m_type = widget_scrollbar_v;
	m_fTotalValue = 0;
	m_fCurrValue = 0;
	m_pTopBtn = NULL;
	m_pBottomBtn = NULL;
	m_pHandleBtn = NULL;

	m_pTopBtn = (CDexWidgetButton* )getWidgetFactory()->createWidget(widget_button, "滚动条向上按钮");
	GetChildrenContainer()->AddWidgetToBack(m_pTopBtn);
	m_pTopBtn->m_MouseLUp += UI_DELEGATE(this, CDexWidgetScrollBarV, OnClickTopBtn);

	m_pBottomBtn = (CDexWidgetButton* )getWidgetFactory()->createWidget(widget_button, "滚动条向下按钮");
	GetChildrenContainer()->AddWidgetToBack(m_pBottomBtn);
	m_pBottomBtn->m_MouseLUp += UI_DELEGATE(this, CDexWidgetScrollBarV, OnClickBottomBtn);

	m_pHandleBtn = (CDexWidgetButton* )getWidgetFactory()->createWidget(widget_button, "滚动条中间按钮");
	m_pHandleBtn->ModifyFlag(Add_Flag, mouse_l_drag);
	GetChildrenContainer()->AddWidgetToBack(m_pHandleBtn);
	m_pHandleBtn->m_MouseLDrag += UI_DELEGATE(this, CDexWidgetScrollBarV, OnDragHandleBtn);
	ModifyFlag(Add_Flag, mouse_l_drag);
}

CDexWidgetScrollBarV::~CDexWidgetScrollBarV()
{

}

void CDexWidgetScrollBarV::ShutDown()
{
	UI_DROP_REF(m_pTopBtn);
	UI_DROP_REF(m_pBottomBtn);
	UI_DROP_REF(m_pHandleBtn);
}

bool CDexWidgetScrollBarV::Update(int delta)
{
	DEX_ENSURE_B(CDexWidget::Update(delta));
	if(m_pTopBtn != NULL)
		m_pTopBtn->Update(delta);
	if(m_pBottomBtn != NULL)
		m_pBottomBtn->Update(delta);
	if(m_pHandleBtn != NULL)
		m_pHandleBtn->Update(delta);
	return true;
}

void CDexWidgetScrollBarV::Create()
{
	if(m_pTopBtn != 0)
	{
		m_pTopBtn->Resize(DexSize(m_rect.getWidth(), m_rect.getWidth()));
		m_pTopBtn->SetPos(m_rect.left, m_rect.top);
	}
	if(m_pBottomBtn != 0)
	{
		m_pBottomBtn->Resize(DexSize(m_rect.getWidth(), m_rect.getWidth()));
		m_pBottomBtn->SetPos(m_rect.left, m_rect.bottom - m_rect.getWidth());
	}
	if(m_pHandleBtn != 0)
	{
		DexSize handle_size;
		handle_size.w = m_rect.getWidth();
		handle_size.h = (m_rect.getHeight() - m_pTopBtn->GetHeight() - m_pBottomBtn->GetHeight())*1/(m_fTotalValue+1);
		m_pHandleBtn->Resize(handle_size);
		m_pHandleBtn->SetPos(m_rect.left, m_rect.top + m_pTopBtn->GetHeight());
	}
	m_iLength = m_rect.bottom - m_rect.top;
}

void CDexWidgetScrollBarV::RenderThis()
{
	get2DDrawer()->SetRotate(m_rotate);
	//get2DDrawer()->DrawImage(&m_backImage, m_rect, m_alpha);
}
void CDexWidgetScrollBarV::OnClickTopBtn(CDexObject* object,stEvent event)
{

}

void CDexWidgetScrollBarV::OnClickBottomBtn(CDexObject* object,stEvent event)
{

}

void CDexWidgetScrollBarV::OnDragHandleBtn(CDexObject* object,stEvent event)
{
	DexRectF rect = m_pHandleBtn->GetArea();
	DexPointF pnt;
	if(rect.top < m_pTopBtn->GetArea().bottom)
		pnt.y = m_pTopBtn->GetArea().bottom;
	else if(rect.bottom > m_pBottomBtn->GetArea().top)
		pnt.y = m_pBottomBtn->GetArea().top - m_pHandleBtn->GetHeight();
	else
		pnt.y = rect.top;
	pnt.x = m_rect.left;  
	m_pHandleBtn->SetPos(pnt);

	m_fCurrValue = (pnt.y - m_pTopBtn->GetArea().bottom)/m_pHandleBtn->GetHeight();
}

void CDexWidgetScrollBarV::OnResize(CDexObject* object,stEvent event)
{

}
void CDexWidgetScrollBarV::Enable(bool child)
{

}

void CDexWidgetScrollBarV::Disable(bool child)
{

}

void CDexWidgetScrollBarV::SetLength(int16 length)
{
	m_iLength = length;
}

void CDexWidgetScrollBarV::SetTotalValue(float value)
{
	m_fTotalValue = value;
}

void CDexWidgetScrollBarV::SetBackImage(string dexname, const DexRect& src_rect)
{
	m_backImage.m_tex = getUiSrcMgrSingleton()->getUiTexFactory()->FindTex(dexname.c_str());
	m_backImage.m_srcArea = src_rect;
	m_backImage.m_texname = dexname;
	UI_ADD_REF(m_backImage.m_tex);
}