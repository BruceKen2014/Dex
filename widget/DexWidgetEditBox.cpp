

#include "../Source/CTexture.h"
#include "../DexBase/CDexDraw2D.h"
#include "../DexBase/DexLog.h"

#include "DexWidgetEditBox.h"
#include "CDexUiSrcMgr.h"
#include "DexWidgetContainer.h"
#include "DexDesktop.h"


DexWidgetEditBox::DexWidgetEditBox()
{
	m_type = widget_editbox;
	m_iOffsetX = 0;
	m_iOffsetY = 0;
	m_iCursorTickSpeed = 500;
	m_iTickInterval = 0;
	m_iTickInterval = 0;
	m_bShowCursor = true;
	m_BackImage = new stUiImage;
	m_iInputCharIndex = 0;
	m_iTextSize = 15;
	m_textColor = DEXCOLOR_BLACK;
}
DexWidgetEditBox::~DexWidgetEditBox()
{

}

void DexWidgetEditBox::ShutDown()
{
	_SafeDelete(m_BackImage);
	DexWidget::ShutDown();
}

bool DexWidgetEditBox::Update(int delta)
{
	DEX_ENSURE_B(DexWidget::Update(delta));
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

void DexWidgetEditBox::RenderThis()
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
	if (getDesktop()->getSelectWgt() == this)
	{
		if (m_bShowCursor)
			retText.insert(m_iInputCharIndex, 1, '|');
		else
			retText.insert(m_iInputCharIndex, 1, ' ');
	}

	get2DDrawer()->SetTextSize(m_iTextSize);
	get2DDrawer()->SetTextColor(m_textColor);
	get2DDrawer()->DrawString(m_rect.left + m_iOffsetX, m_rect.top + m_iOffsetY, (TCHAR*)retText.c_str());
}
void DexWidgetEditBox::Enable(bool child)
{
}

void DexWidgetEditBox::Disable(bool child)
{
}

void DexWidgetEditBox::SetStartOffset(DInt16 iOffsetX, DInt16 iOffsetY)
{
	m_iOffsetX = iOffsetX;
	m_iOffsetY = iOffsetY;
}

void DexWidgetEditBox::SetStartOffsetX(DInt16 iOffsetX)
{
	m_iOffsetX = iOffsetX;
}

void DexWidgetEditBox::SetStartOffsetY(DInt16 iOffsetY)
{
	m_iOffsetY = iOffsetY;
}

DInt16 DexWidgetEditBox::GetStartOffsetX()
{
	return m_iOffsetX;
}

DInt16 DexWidgetEditBox::GetStartOffsetY()
{
	return m_iOffsetY;
}

string DexWidgetEditBox::GetEditText()
{
	return m_strText;
}


void DexWidgetEditBox::SetTextSize(DInt16 size)
{
	m_iTextSize = size;
}

void DexWidgetEditBox::SetTextColor(const DexColor& color)
{
	m_textColor = color;
}

void DexWidgetEditBox::SetText(string text)
{
	m_strText = text;
	m_iInputCharIndex = m_strText.length();
}

void DexWidgetEditBox::ClearText()
{
	m_strText.clear();
	m_iInputCharIndex = 0;
}
void DexWidgetEditBox::SetBackImage(char* filename, const DexRect& rect, bool mirro)
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

void DexWidgetEditBox::OnKeyChar(stEvent event)
{
	stArgs ch = event.back_args();
	if(ch.i_member == EVK_BACK)
	{
		if(m_strText.size() != 0)
		{
			//是否是汉字（好像是汉字的话 就是由两个负数编码的？）
			if (m_strText.at(m_iInputCharIndex - 1) < 0 && m_strText.size() >= 2)
			{//
				m_iInputCharIndex -= 2;
				m_strText.erase(m_iInputCharIndex - 1, 2); //-2 再 -1
			}
			else
				m_strText.erase(--m_iInputCharIndex, 1); //删除光标之前的字符
		}
		m_DisTextChange.Handle(this, event);
	}
	else if(ch.i_member == EVK_ENTER)
	{
		m_DisTextUpdate.Handle(this, event);
	}
	else
	{
		m_strText.insert(m_iInputCharIndex++, 1, (TCHAR)ch.i_member);
		m_DisTextChange.Handle(this, event);
	}
}

void DexWidgetEditBox::OnKeyDown(stEvent event)
{
	stArgs ch = event.back_args();
	if (ch.i_member == EVK_LEFT)
	{
		m_iInputCharIndex = --m_iInputCharIndex < 0 ? 0 : m_iInputCharIndex;
	}
	else if (ch.i_member == EVK_RIGHT)
	{
		m_iInputCharIndex = ++m_iInputCharIndex > m_strText.length() ? m_strText.length() : m_iInputCharIndex;
	}
	m_DisMiscKey.Handle(this, event);
}