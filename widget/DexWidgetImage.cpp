

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
	m_pImg = DexNull;
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

void CDexWidgetImage::CalculateUv(DexVector2& uv0, DexVector2& uv1, DexVector2& uv2, DexVector2& uv3)
{
	uv0.x = (float)m_srcArea.left / (float)m_tex->GetWidth();
	uv0.y = (float)m_srcArea.top / (float)m_tex->GetHeight();
	uv2.x = (float)m_srcArea.right / (float)m_tex->GetWidth();
	uv2.y = (float)m_srcArea.bottom / (float)m_tex->GetHeight();
	uv1.x = uv2.x;
	uv1.y = uv0.y;
	uv3.x = uv0.x;
	uv3.y = uv2.y;
	if (m_Mirro)
	{
		float coor = uv1.x;
		uv1.x = uv2.x = uv0.x;
		uv0.x = uv3.x = coor;
	}
}

bool CDexWidgetImage::Update(int delta)
{
	DEX_ENSURE_B(CDexWidget::Update(delta));
	return true;
}

void CDexWidgetImage::RenderThis()
{
	DEX_ENSURE(m_tex);
	DexVector2 v1, v2, v3, v4;
	CalculateUv(v1, v2, v3, v4);
	get2DDrawer()->ResetMatrix();
	if(m_bClipByParent)
	{
		ReCalculateUV(v1, v2, v3, v4,m_rect,m_rectAfterClip);
		get2DDrawer()->SetUV(v1, v2, v3, v4);
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