#include "DexWidgetProgressBar.h"
#include "..\DexBase\CDexDraw2D.h"
#include "CDexUiSrcMgr.h"




DexWidgetProgressBar::DexWidgetProgressBar()
{
	m_pBackImage = DexNull;
	m_pFrontImage = DexNull;
	m_eProgressType = EPBT_L2R;
	m_fProgress = 0.0f;
}
DexWidgetProgressBar::~DexWidgetProgressBar()
{

}

void DexWidgetProgressBar::ShutDown()
{
	m_pBackImage = DexNull;
	m_pFrontImage = DexNull;
	CDexWidget::ShutDown();
}
void DexWidgetProgressBar::CalculateImageUv(DexVector2& uv0, DexVector2& uv1, DexVector2& uv2, DexVector2& uv3, const DexRectF& rect, CDexTex* pImage)
{
	uv0.x = (float)rect.left / (float)pImage->GetWidth();
	uv0.y = (float)rect.top / (float)pImage->GetHeight();
	uv2.x = (float)rect.right / (float)pImage->GetWidth();
	uv2.y = (float)rect.bottom / (float)pImage->GetHeight();
	uv1.x = uv2.x;
	uv1.y = uv0.y;
	uv3.x = uv0.x;
	uv3.y = uv2.y;
}
bool DexWidgetProgressBar::Update(int delta)
{
	DEX_ENSURE_B(CDexWidget::Update(delta));
	m_rectProgress = m_rect;
	if (m_eProgressType == EPBT_L2R)
		m_rectProgress.right = m_rectProgress.left + m_rect.getWidth() * m_fProgress;
	else if (m_eProgressType == EPBT_B2T)
		m_rectProgress.top = m_rectProgress.bottom - m_rect.getHeight() * m_fProgress;
	return true;
}

void DexWidgetProgressBar::RenderThis()
{
	//uv0 uv1
	//uv3 uv2
	DexVector2 uv0, uv1, uv2, uv3;
	if (m_pBackImage != DexNull)
	{
		get2DDrawer()->ResetMatrix();
		CalculateImageUv(uv0, uv1, uv2, uv3, m_rectBackImageSrcArea, m_pBackImage);
		if (m_bClipByParent)
		{
			ReCalculateUV(uv0, uv1, uv2, uv3, m_rect, m_rectAfterClip);
			get2DDrawer()->SetUV(uv0, uv1, uv2, uv3);
		}
		else
		{
			get2DDrawer()->SetUV(uv0, uv1, uv2, uv3);
		}
		DexRectF rect = GetArea();
		get2DDrawer()->SetScale(rect.right - rect.left, rect.bottom - rect.top);
		get2DDrawer()->SetAlpha(m_alpha);
		get2DDrawer()->SetRotate(m_rotate);
		int xpos = rect.left;
		int ypos = rect.top;
		get2DDrawer()->SetTrans(xpos, ypos);
		get2DDrawer()->Draw(m_pBackImage->GetTexPt());
	}
	if (m_pFrontImage != DexNull)
	{
		get2DDrawer()->ResetMatrix();
		CalculateImageUv(uv0, uv1, uv2, uv3,m_rectFrontImageSrcArea, m_pFrontImage);
		if (m_eProgressType == EPBT_L2R)
		{
			DFloat32 uv_length = uv2.x - uv0.x;
			uv2.x = uv0.x + m_fProgress * uv_length;
			uv1.x = uv2.x;
			//计算进度条的裁减，要以当前进度的rect进行裁减
			DexRectF realRect = m_rectProgress;
			if (m_bClipByParent && m_father != DexNull)
			{
				realRect = m_rectProgress.Clip(m_father->GetArea());
				ReCalculateUV(uv0, uv1, uv2, uv3, m_rectProgress, realRect);
				get2DDrawer()->SetUV(uv0, uv1, uv2, uv3);
			}
			else
			{
				get2DDrawer()->SetUV(uv0, uv1, uv2, uv3);
			}
			get2DDrawer()->SetScale(realRect.getWidth(), realRect.getHeight());
			get2DDrawer()->SetAlpha(m_alpha);
			get2DDrawer()->SetRotate(m_rotate);
			get2DDrawer()->SetTrans(m_rectProgress.left, m_rectProgress.top);
			get2DDrawer()->Draw(m_pFrontImage->GetTexPt());
		}
		else if (m_eProgressType == EPBT_B2T)
		{
		}

	}
}

CDexTex* DexWidgetProgressBar::getBackImage()
{
	return m_pBackImage;
}

CDexTex* DexWidgetProgressBar::getFrontImage()
{
	return m_pFrontImage;
}

void DexWidgetProgressBar::SetProgressBarType(EProgressBarType type)
{
	m_eProgressType = type;
}

void DexWidgetProgressBar::SetProgress(DFloat32 fProgress)
{
	m_fProgress = fProgress;
	DEX_CLIP(m_fProgress, 0.0f, 1.0f);
}
void DexWidgetProgressBar::SetProgressImage(DString backImageName, const DexRectF& backSrcRect, DString FrontImageName, const DexRectF& FrontSrcRect)
{
	if (backImageName.size() == 0)
		m_pBackImage = DexNull;
	else
		m_pBackImage = getUiSrcMgrSingleton()->getUiTexFactory()->FindTex(backImageName.c_str());
	if (m_pBackImage != DexNull)
	{
		m_rect = DexRectF(0, 0, m_pBackImage->GetWidth(), m_pBackImage->GetHeight());
		if (backSrcRect.getWidth() == 0)
			m_rectBackImageSrcArea = m_rect;
		else
			m_rectBackImageSrcArea = backSrcRect;
	}
	m_pFrontImage = getUiSrcMgrSingleton()->getUiTexFactory()->FindTex(FrontImageName.c_str());
	if (FrontSrcRect.getWidth() == 0)
		m_rectFrontImageSrcArea = DexRectF(0, 0, m_pFrontImage->GetWidth(), m_pFrontImage->GetHeight());
	else
		m_rectFrontImageSrcArea = FrontSrcRect;
}