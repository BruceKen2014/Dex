

#include "../Source/CTexture.h"
#include "../DexBase/CDexDraw2D.h"
#include "../DexBase/DexLog.h"

#include "DexWidgetEditBox.h"
#include "CDexUiSrcMgr.h"
#include "DexWidgetContainer.h"
#include "DexDesktop.h"


CDexWidgetEditBox::CDexWidgetEditBox()
{
	m_type = widget_editbox;
	m_iOffsetX = 0;
	m_iOffsetY = 0;
	m_iCursorTickSpeed = 500;
	m_iTickInterval = 0;
	m_iTickInterval = 0;
	m_bShowCursor = true;
	m_BackImage = new stUiImage;
}
CDexWidgetEditBox::~CDexWidgetEditBox()
{

}

void CDexWidgetEditBox::ShutDown()
{
	_SafeDelete(m_BackImage);
	CDexWidget::ShutDown();
}

bool CDexWidgetEditBox::Update(int delta)
{
	DEX_ENSURE_B(CDexWidget::Update(delta));
	m_iTickInterval += delta;
	if(m_iTickInterval > m_iCursorTickSpeed)
	{
		if(m_bShowCursor)
			m_bShowCursor = false;
		else
			m_bShowCursor = true;
		m_iTickInterval = 0;
	}
	return true;
}

void CDexWidgetEditBox::RenderThis()
{
	if(m_BackImage != NULL && m_BackImage->m_tex != NULL)
	{
		D3DXVECTOR2 v1, v2, v3, v4;
		// v1 v2
		// v4 v3
		v1.x = (float)m_BackImage->m_srcArea.left/(float)m_BackImage->m_tex->GetWidth();
		v1.y = (float)m_BackImage->m_srcArea.top /(float)m_BackImage->m_tex->GetHeight();
		v3.x = (float)m_BackImage->m_srcArea.right/(float)m_BackImage->m_tex->GetWidth();
		v3.y = (float)m_BackImage->m_srcArea.bottom /(float)m_BackImage->m_tex->GetHeight();
		v2.x = v3.x;
		v2.y = v1.y;
		v4.x = v1.x;
		v4.y = v3.y;
		if(m_BackImage->m_Mirro)
		{
			float coor = v2.x;
			v2.x = v3.x = v1.x;
			v1.x = v4.x = coor;
		}
		get2DDrawer()->ResetMatrix();
		get2DDrawer()->SetScale(m_rect.right - m_rect.left, m_rect.bottom - m_rect.top);
		get2DDrawer()->SetUV(v1, v2, v3, v4);
		get2DDrawer()->SetAlpha(m_alpha);
		get2DDrawer()->SetRotate(m_rotate);
		get2DDrawer()->SetTrans(m_rect.left, m_rect.top);
	
		get2DDrawer()->Draw(m_BackImage->m_tex->GetTexPt());
	}

		//文字
	string retText = m_strText;
	if(m_bShowCursor && getDesktop()->getFocusWgt() == this)
		retText.append(1, '|');
	get2DDrawer()->DrawString(m_rect.left + m_iOffsetX, m_rect.top + m_iOffsetY, (TCHAR*)retText.c_str());
}
void CDexWidgetEditBox::Enable(bool child)
{
}

void CDexWidgetEditBox::Disable(bool child)
{
}

void CDexWidgetEditBox::SetStartOffset(DInt16 iOffsetX, DInt16 iOffsetY)
{
	m_iOffsetX = iOffsetX;
	m_iOffsetY = iOffsetY;
}

void CDexWidgetEditBox::SetStartOffsetX(DInt16 iOffsetX)
{
	m_iOffsetX = iOffsetX;
}

void CDexWidgetEditBox::SetStartOffsetY(DInt16 iOffsetY)
{
	m_iOffsetY = iOffsetY;
}

DInt16 CDexWidgetEditBox::GetStartOffsetX()
{
	return m_iOffsetX;
}

DInt16 CDexWidgetEditBox::GetStartOffsetY()
{
	return m_iOffsetY;
}

string CDexWidgetEditBox::GetEditText()
{
	return m_strText;
}

void CDexWidgetEditBox::SetText(string text)
{
	m_strText = text;
}

void CDexWidgetEditBox::ClearText()
{
	m_strText.clear();
}
void CDexWidgetEditBox::SetBackImage(char* filename, const DexRect& rect, bool mirro)
{
	m_BackImage->m_tex = getUiSrcMgrSingleton()->getUiTexFactory()->FindTex(filename);
	if(m_BackImage->m_tex == NULL)
	{
		DexLog::getSingleton()->BeginLog();
		DexLog::getSingleton()->Log(log_allert, "CDexWidgetEditBox::SetBackImage调用失败：未找到图片%s", filename);
		DexLog::getSingleton()->EndLog();
		return;
	}
	if(rect.right == 0)
	{
		m_BackImage->m_srcArea = DexRect(0,0,m_BackImage->m_tex->GetWidth(), m_BackImage->m_tex->GetHeight());
	}
	else
		m_BackImage->m_srcArea = rect;
	m_BackImage->m_Mirro = mirro;
}

void CDexWidgetEditBox::OnKeyChar(stEvent event)
{
	stArgs ch;
	event.pop_args(ch);
	if(ch.i_member == EVK_BACK)
	{
		if(m_strText.size() != 0)
		{
			//最后一个字是是否是汉字（好像是汉字的话 就是由两个负数编码的？）
			if(m_strText.at(m_strText.size() - 1) < 0)
			{//
				if(m_strText.size() >= 2)
					m_strText.erase(m_strText.size()-2, 2);
			}
			else
				m_strText.erase(m_strText.size()-1, 1);
		}
		m_DisTextChange.Handle(this, stEvent());
	}
	else if(ch.i_member == EVK_ENTER)
	{
		m_DisTextUpdate.Handle(this, stEvent());
	}
	else
	{
		m_strText.append(1, (TCHAR)ch.i_member);
		m_DisTextChange.Handle(this, stEvent());
	}
}