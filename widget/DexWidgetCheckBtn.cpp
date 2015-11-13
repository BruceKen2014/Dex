

#include "../Source/CTexture.h"
#include "../DexBase/CDexDraw2D.h"
#include "../DexBase/DexLog.h"

#include "DexWidgetCheckBtn.h"
#include "CDexUiSrcMgr.h"
#include "DexWidgetContainer.h"
#include "DexDesktop.h"


CDexWidgetCheckBtn::CDexWidgetCheckBtn()
{
	m_checkTex = new stUiImage;
	m_uCheckTex = new stUiImage;
	m_checked = false;
	m_type = widget_check_button;
	ModifyFlag(Add_Flag, mouse_l_up);
}
CDexWidgetCheckBtn::~CDexWidgetCheckBtn()
{

}

void CDexWidgetCheckBtn::ShutDown()
{
	_SafeDelete(m_checkTex);
	_SafeDelete(m_uCheckTex);
	CDexWidget::ShutDown();
}
bool CDexWidgetCheckBtn::OnMouseLUp(stEvent event)
{
	bool ret = CDexWidget::OnMouseLUp(event);
	m_checked = m_checked ? 0 : 1;
	return ret;
}

bool CDexWidgetCheckBtn::Update(int delta)
{
	DEX_ENSURE_B(CDexWidget::Update(delta));
	return true;
}

void CDexWidgetCheckBtn::RenderThis()
{
	DEX_ENSURE(m_checkTex && m_checkTex->m_tex && m_uCheckTex &&  m_uCheckTex->m_tex );
	stUiImage* temp = NULL;
	if(m_checked)
		temp = m_checkTex;
	else
		temp = m_uCheckTex;
	D3DXVECTOR2 v1, v2, v3, v4;
	// v1 v2
	// v4 v3
	v1.x = (float)temp->m_srcArea.left/(float)temp->m_tex->GetWidth();
	v1.y = (float)temp->m_srcArea.top /(float)temp->m_tex->GetHeight();
	v3.x = (float)temp->m_srcArea.right/(float)temp->m_tex->GetWidth();
	v3.y = (float)temp->m_srcArea.bottom /(float)temp->m_tex->GetHeight();
	v2.x = v3.x;
	v2.y = v1.y;
	v4.x = v1.x;
	v4.y = v3.y;
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

	DexRectF rect = GetArea();
	get2DDrawer()->SetScale(rect.right - rect.left, rect.bottom - rect.top);
	get2DDrawer()->SetTrans(rect.left, rect.top);
	get2DDrawer()->SetAlpha(m_alpha);
	get2DDrawer()->Draw(temp->m_tex->GetTexPt());
}

bool CDexWidgetCheckBtn::GetChecked()
{
	return m_checked;
}

void CDexWidgetCheckBtn::SetChecked(bool checked)
{
	m_checked = checked;
}
void CDexWidgetCheckBtn::SetCheckTex(string texname, const DexRect& rect /* = DexRect */, bool mirro /* = false */)
{
	m_checkTex->m_tex = getUiSrcMgrSingleton()->getUiTexFactory()->FindTex(texname.c_str());
	if(m_checkTex->m_tex == NULL)
	{
		getLog()->BeginLog();
		getLog()->Log(log_allert, "CDexWidgetImageSequence::AddImageµ÷ÓÃÊ§°Ü£ºÎ´ÕÒµ½Í¼Æ¬%s", texname.c_str());
		getLog()->EndLog();
		return;
	}

	m_checkTex->m_texname = texname;
	if(rect.right == 0)
	{
		m_checkTex->m_srcArea = DexRect(0,0,m_checkTex->m_tex->GetWidth(), m_checkTex->m_tex->GetHeight());
	}
	else
		m_checkTex->m_srcArea = rect;
	m_checkTex->m_Mirro = mirro;
}

void CDexWidgetCheckBtn::SetUnCheckTex(string texname, const DexRect& rect /* = DexRect */, bool mirro /* = false */)
{
	m_uCheckTex->m_tex = getUiSrcMgrSingleton()->getUiTexFactory()->FindTex(texname.c_str());
	if(m_uCheckTex->m_tex == NULL)
	{
		getLog()->BeginLog();
		getLog()->Log(log_allert, "CDexWidgetImageSequence::AddImageµ÷ÓÃÊ§°Ü£ºÎ´ÕÒµ½Í¼Æ¬%s", texname.c_str());
		getLog()->EndLog();
		return;
	}
	m_uCheckTex->m_texname = texname;
	if(rect.right == 0)
	{
		m_uCheckTex->m_srcArea = DexRect(0,0,m_uCheckTex->m_tex->GetWidth(), m_uCheckTex->m_tex->GetHeight());
	}
	else
		m_uCheckTex->m_srcArea = rect;
	m_uCheckTex->m_Mirro = mirro;
}