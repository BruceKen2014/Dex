

#include "../Source/CTexture.h"
#include "../DexBase/CDexDraw2D.h"
#include "../DexBase/DexLog.h"

#include "CDexUiSrcMgr.h"
#include "DexWidgetContainer.h"
#include "DexWidgetImage.h"

//m_backImage = (CDexWidgetImage* )getWidgetFactory()->createWidget(widget_image, "±³¾°Í¼Æ¬1");
//m_backImage->SetTexName("YuanLeft.png");
//m_backImage->SetMirro(true);
//m_backImage->SetPos(DexPoint(200, 200));
//m_backImage->SetTexRes(DexRect(0,0,340,450));
//m_backImage->Resize(DexSize(300,100));
//m_backImage->OpenMask(0);
//m_backImage->SetInitAlpha(100);
//m_backImage->SetMaskColor(0,0,255,100);
//m_backImage->ModifyFlag(Add_Flag, DexGUI::mouse_r_drag);
CDexWidgetImage::CDexWidgetImage()
{
	m_tex = NULL;
	m_Mirro = false;
	m_type = widget_image;
}

CDexWidgetImage::~CDexWidgetImage()
{
}

void CDexWidgetImage::ShutDown()
{
	CDexWidget::ShutDown();
}

bool CDexWidgetImage::Update(int delta)
{
	DEX_ENSURE_B(CDexWidget::Update(delta));
	return true;
}

void CDexWidgetImage::RenderThis()
{
	DEX_ENSURE(m_tex);
	D3DXVECTOR2 v1, v2, v3, v4;
	// v1 v2
	// v4 v3
	v1.x = (float)m_srcArea.left/(float)m_tex->GetWidth();
	v1.y = (float)m_srcArea.top /(float)m_tex->GetHeight();
	v3.x = (float)m_srcArea.right/(float)m_tex->GetWidth();
	v3.y = (float)m_srcArea.bottom /(float)m_tex->GetHeight();
	v2.x = v3.x;
	v2.y = v1.y;
	v4.x = v1.x;
	v4.y = v3.y;
	if(m_Mirro)
	{
		float coor = v2.x;
		v2.x = v3.x = v1.x;
		v1.x = v4.x = coor;
	}
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
	get2DDrawer()->SetAlpha(m_alpha);
	get2DDrawer()->SetRotate(m_rotate);
	int xpos = rect.left;
	int ypos = rect.top;
	get2DDrawer()->SetTrans(xpos, ypos);
	
	get2DDrawer()->Draw(m_tex->GetTexPt());
}
void CDexWidgetImage::SetTexName(string texname)
{
	m_texname = texname;
	m_tex = getUiSrcMgrSingleton()->getUiTexFactory()->FindTex(m_texname.c_str());
	DEX_ENSURE(m_tex);
	m_rect = DexRectF(0,0,m_tex->GetWidth(), m_tex->GetHeight());
	m_srcArea = m_rect;
}
void CDexWidgetImage::SetTexRes(const DexRectF& rect)
{
	m_srcArea = rect;
}
void CDexWidgetImage::SetTexRes(const DexRect& rect)
{
	m_srcArea.left = rect.left;
	m_srcArea.right = rect.right;
	m_srcArea.top = rect.top;
	m_srcArea.bottom = rect.bottom;
}

void CDexWidgetImage::SetMirro(bool mirro)
{
	m_Mirro = mirro;
}