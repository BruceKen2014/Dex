


#include "../DexBase/CDexDraw2D.h"
#include "../DexBase/DexLog.h"
#include "../Source/CTexture.h"
#include "../sound/CSoundSystem.h"

#include "CDexUiSrcMgr.h"
#include "DexWidgetContainer.h"
#include "DexWidgetButton.h"
#include "DexDesktop.h"


//m_btn1 = (CDexWidgetButton* )getWidgetFactory()->createWidget(widget_button, "按钮子控件1");
//m_btn1->SetStateTex(btn_normal, "about.png");
//m_btn1->SetStateTexRect(btn_normal, DexRect(0,0,115,240));
//m_btn1->SetStateTex(btn_mouse_on, "about.png");
//m_btn1->SetStateTexRect(btn_mouse_on, DexRect(0,240,115,480));
//m_btn1->SetStateTex(btn_mouse_down, "about.png");
//m_btn1->SetStateTexRect(btn_mouse_down, DexRect(0,480,115,720));
//m_btn1->SetStateTex(btn_useless, "about.png");
//m_btn1->SetStateTexRect(btn_useless, DexRect(0,0,115,240));
//m_btn1->SetPos(DexPoint(200, 200));
//m_btn1->ModifyFlag(Add_Flag, DexGUI::mouse_r_drag);
//m_btn1->Resize(DexSize(150,60));
CDexWidgetButton::CDexWidgetButton()
{
	for(int i = 0 ; i < btn_count; i++)
		m_texs[i] = NULL;
	m_state = btn_normal;
	m_type = widget_button;
	ModifyFlag(Add_Flag, mouse_move);
	ModifyFlag(Add_Flag, mouse_l_down);
	ModifyFlag(Add_Flag, mouse_l_up);
}

CDexWidgetButton::~CDexWidgetButton()
{

}

void CDexWidgetButton::ShutDown()
{
	CDexWidget::ShutDown();
}
void CDexWidgetButton::OnMouseMoveIn(stEvent event)
{
	DEX_ENSURE(m_state != btn_useless);
	CDexWidget::OnMouseMoveIn(event);
	m_state = btn_mouse_on;
}
void CDexWidgetButton::OnMouseMoveOut(stEvent event)
{
	DEX_ENSURE(m_state != btn_useless);
	CDexWidget::OnMouseMoveOut(event);
	m_state = btn_normal;
}
bool CDexWidgetButton::OnMouseLUp(stEvent event)
{
	DEX_ENSURE_B(m_state != btn_useless);
	bool ret = false;
	stArgs posX, posY; 
	event.pop_args(posY);
	event.pop_args(posX);
	if(CheckPointIn(DexPoint(posX.i_member, posY.i_member)))
	{//如果在按钮区域内弹起
		m_MouseLUp.Handle(this, event);
		getDesktop()->SetUiEvent(false);
		if(m_state == btn_mouse_down)
			m_state = btn_mouse_on;
		DexLog::getSingleton()->BeginLog();
		DexLog::getSingleton()->Log(log_ok, "控件%s处理鼠标左键弹起事件", m_name.c_str());
		DexLog::getSingleton()->EndLog();
		ret = true;
	}
	else
	{
		if(m_state != btn_normal)
		{
			m_state = btn_normal;
			ret = true;
		}
	}
	return ret;
}
void CDexWidgetButton::OnMouseLDown(stEvent event)
{
	DEX_ENSURE(m_state != btn_useless);
	CDexWidget::OnMouseLDown(event);
	if(m_state != btn_mouse_down)
	{
		if(m_mouseDownSoundFilename != "")
			CSound::getSoundSingleton().Play((char*)m_mouseDownSoundFilename.c_str(), 1);
	}
	m_state = btn_mouse_down;

}

bool CDexWidgetButton::MouseLUpValid(stEvent event)
{
	DEX_ENSURE_B(m_state != btn_useless);
	DEX_ENSURE_B(getDesktop()->getUiEvent()); 
	DEX_ENSURE_B(m_visible);
	DEX_ENSURE_B(GetEable());
	DEX_ENSURE_B(m_flag & mouse_l_up);

	return true; //无论是否在按钮区域内，都没有关系
}
bool CDexWidgetButton::Update(int delta)
{
	DEX_ENSURE_B(CDexWidget::Update(delta));
	return true;
}

void CDexWidgetButton::Enable(bool child)
{
	CDexWidget::Enable(child);
	m_state = btn_normal;
}

void CDexWidgetButton::Disable(bool child)
{
	CDexWidget::Disable(child);
	m_state = btn_useless;
}
void CDexWidgetButton::RenderThis()
{
	CDexTex* tex = m_texs[m_state]; 
	DexRectF rect = m_srcArea[m_state];
	if(tex == NULL) //如果别的tex为空 则使用normal
	{
		tex = m_texs[btn_normal];
		rect = m_srcArea[btn_normal];
	}
	DEX_ENSURE(tex != NULL && tex->GetTexPt() != NULL);
	D3DXVECTOR2 v1, v2, v3, v4;
	// v1 v2
	// v4 v3
	v1.x = (float)rect.left/(float)tex->GetWidth();
	v1.y = (float)rect.top /(float)tex->GetHeight();
	v3.x = (float)rect.right/(float)tex->GetWidth();
	v3.y = (float)rect.bottom /(float)tex->GetHeight();
	v2.x = v3.x;
	v2.y = v1.y;
	v4.x = v1.x;
	v4.y = v3.y;
	rect = GetArea();
	get2DDrawer()->ResetMatrix();
	if(m_bClipByParent)
	{
		float image_u_length = v3.x - v1.x;
		float image_v_length = v3.y - v1.y;
		D3DXVECTOR2 v1_clip(v1), v2_clip(v2), v3_clip(v3), v4_clip(v4);
		float real_width = m_rect.getWidth();
		float real_height = m_rect.getHeight();
		if(m_rectAfterClip.left > m_rect.left)
		{
			float clip_length = m_rectAfterClip.left - m_rect.left;
			float clip_rate = clip_length/real_width;
			float clip_u = image_u_length * clip_rate;
			v1_clip.x += clip_u;
		}
		if(m_rectAfterClip.top > m_rect.top)
		{
			float clip_length = m_rectAfterClip.top - m_rect.top;
			float clip_rate = clip_length/real_height;
			float clip_v = image_v_length * clip_rate;
			v1_clip.y += clip_v;
		}
		if(m_rectAfterClip.right < m_rect.right)
		{
			float clip_length = m_rect.right - m_rectAfterClip.right;
			float clip_rate = clip_length/real_width;
			float clip_u = image_u_length * clip_rate;
			v3_clip.x -= clip_u;
		}
		if(m_rectAfterClip.bottom < m_rect.bottom)
		{
			float clip_length =m_rect.bottom - m_rectAfterClip.bottom;
			float clip_rate = clip_length/real_height;
			float clip_v = image_v_length * clip_rate;
			v3_clip.y -= clip_v;
		}
		v2_clip.x = v3_clip.x;
		v2_clip.y = v1_clip.y;
		v4_clip.x = v1_clip.x;
		v4_clip.y = v3_clip.y;
		get2DDrawer()->SetUV(v1_clip, v2_clip, v3_clip, v4_clip);
	}
	else
	{
		get2DDrawer()->SetUV(v1, v2, v3, v4);
	}
	get2DDrawer()->SetScale(rect.right - rect.left, rect.bottom - rect.top);
	get2DDrawer()->SetAlpha(m_alpha);
	get2DDrawer()->SetTrans(rect.left, rect.top);
	get2DDrawer()->Draw(tex->GetTexPt());
}

void CDexWidgetButton::SetStateTex(EBtnState state, string tex)
{
	m_texs[state] = getUiSrcMgrSingleton()->getUiTexFactory()->FindTex(tex.c_str());
	DEX_ENSURE(m_texs[state]);
	m_texnames[state] = tex;
	//m_rect = DexRectF(0,0,m_texs[state]->GetWidth(), m_texs[state]->GetHeight());
	//m_srcArea[state] = m_rect;
}

void CDexWidgetButton::SetStateTexRect(EBtnState state, const DexRect& rect)
{
	m_srcArea[state].left = rect.left;
	m_srcArea[state].right = rect.right;
	m_srcArea[state].top = rect.top;
	m_srcArea[state].bottom = rect.bottom;
}
void CDexWidgetButton::SetStateTexRect(EBtnState state, const DexRectF& rect)
{
	m_srcArea[state] = rect;
}
