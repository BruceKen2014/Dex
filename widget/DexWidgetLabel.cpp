

#include "../DexBase/CDexDraw2D.h"
#include "../state/DexGameEngine.h"
//#include "../DexBase/DexLog.h"

#include "DexWidgetLabel.h"
//#include "CDexUiSrcMgr.h"
//#include "DexWidgetContainer.h"
//#include "DexDesktop.h"


CDexWidgetLabel::CDexWidgetLabel()
{
	m_type = widget_label;
	m_iTextSize = 10;
	m_textColor = 0xffffffff;
	m_textBackColor.a = 0.0f;  //默认背景色透明
}
CDexWidgetLabel::~CDexWidgetLabel()
{

}

void CDexWidgetLabel::ShutDown()
{
	DexWidget::ShutDown();
}

bool CDexWidgetLabel::Update(int delta)
{
	DEX_ENSURE_B(DexWidget::Update(delta));
	return true;
}

void CDexWidgetLabel::RenderThis()
{
	//文字
	get2DDrawer()->SetTextSize(m_iTextSize);
	get2DDrawer()->SetTextColor(m_textColor);
	DexRectF rc = GetArea();
	//D3DXVECTOR3 CamPos = get2DDrawer()->GetCameraPos();
	//rc.left -= CamPos.x;rc.right -= CamPos.x;
	//rc.top += CamPos.y;rc.bottom += CamPos.y;
	//get2DDrawer()->DrawString(rc, (TCHAR*)m_strText.c_str());
	
	//DexGameEngine::getEngine()->GetDevice()->GetRenderTarget(0, &DexGameEngine::getEngine()->m_pBackSurface);
	DexGameEngine::getEngine()->GetDevice()->SetRenderTarget(0, DexGameEngine::getEngine()->m_pBufferFontTextureSurface);
	DexGameEngine::getEngine()->GetDevice()->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,getD3DColor(m_textBackColor),1.0f,0); 
	//DexGameEngine::getEngine()->GetDevice()->BeginScene();
	DexRectF tempRect;
	tempRect.left = 0.0f; tempRect.top = 0.0f;
	tempRect.right = m_rect.getWidth(); tempRect.bottom = m_rect.getHeight();
	get2DDrawer()->DrawString(tempRect,(TCHAR*)m_strText.c_str());
	//get2DDrawer()->DrawString(0,0,"label");
	//DexGameEngine::getEngine()->GetDevice()->EndScene();
	//D3DXSaveTextureToFile("11.jpg",D3DXIFF_JPG, DexGameEngine::getEngine()->m_pBufferFontTexture,NULL);
	D3DXVECTOR2 v1, v2, v3, v4;
	// v1 v2
	// v4 v3
	v1.x = 0.0f; v1.y = 0.0f;
	v3.x = m_rect.getWidth() / 512; v3.y = m_rect.getHeight()/512;
	v2.x = v3.x;
	v2.y = v1.y;
	v4.x = v1.x;
	v4.y = v3.y;
	DexGameEngine::getEngine()->GetDevice()->SetRenderTarget(0, DexGameEngine::getEngine()->m_pBackSurface);
	//DexGameEngine::getEngine()->GetDevice()->BeginScene();
	//DexGameEngine::getEngine()->GetDevice()->EndScene();
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
	//get2DDrawer()->SetUV(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));
	get2DDrawer()->SetAlpha(-1);
	get2DDrawer()->SetScale(rc.getWidth(), rc.getHeight());
	//get2DDrawer()->SetScale(256, 256);
	get2DDrawer()->SetRotate(0);
	get2DDrawer()->SetTrans(rc.left, rc.top);
	get2DDrawer()->Draw(DexGameEngine::getEngine()->m_pBufferFontTexture);
}
void CDexWidgetLabel::Enable(bool child)
{
	DexWidget::Enable(child);
}

void CDexWidgetLabel::Disable(bool child)
{
	DexWidget::Disable(child);
}
string CDexWidgetLabel::GetText()
{
	return m_strText;
}

void CDexWidgetLabel::SetText(string text)
{
	m_strText = text;
}