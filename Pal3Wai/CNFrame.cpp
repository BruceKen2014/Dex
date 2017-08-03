

#include "CNFrame.h"

CNFrame::CNFrame()
{
	m_device = NULL; 
	for(int i= 0; i < PIC_COUNT; i++)
		m_pic[i] = NULL;
	m_width = 0;
	m_height = 0;
	m_posX = 0;
	m_posY = 0;
	m_horizonScale = 1.0f;
	m_verticalScale = 1.0f;
	m_render = true;
	m_successRes = false;
}

CNFrame::CNFrame(LPDIRECT3DDEVICE9 device)
{
	m_device = device; 
	for(int i= 0; i < PIC_COUNT; i++)
		m_pic[i] = NULL;
	m_width = 0;
	m_height = 0;
	m_posX = 0;
	m_posY = 0;
	m_horizonScale = 1.0f;
	m_verticalScale = 1.0f;
	m_render = true;
	m_successRes = false;
}
CNFrame::~CNFrame()
{
	ShutDown();
}

bool CNFrame::CheckRes()
{
	for(int i = 0 ; i < PIC_COUNT; i++)
		if(m_pic[i] == NULL)
			return false;
	return true;
}
bool CNFrame::Check()
{
	return (CheckRes() && m_device);
}

void CNFrame::ShutDown()
{
	for(int i = 0; i < PIC_COUNT; i++)
	    _SafeDelete(m_pic[i]);
}

void CNFrame::Draw()
{
	if(!m_render)
		return;
	if(!Check())
		return;
	m_pic[TOP_LEFT]->Draw(m_device, m_posX, m_posY);
	m_pic[BOTTOM_LEFT]->Draw(m_device, m_posX, m_posY + m_height - m_pic[BOTTOM_LEFT]->GetHeight());
	m_pic[TOP_RIGHT]->Draw(m_device, m_posX + m_width - m_pic[TOP_RIGHT]->GetWidth(), m_posY);
	m_pic[BOTTOM_RIGHT]->Draw(m_device, m_posX + m_width - m_pic[TOP_RIGHT]->GetWidth(), m_posY + m_height - m_pic[BOTTOM_LEFT]->GetHeight());
	m_pic[LEFT]->Draw(m_device, m_posX, m_posY + m_pic[TOP_LEFT]->GetHeight());
	m_pic[RIGHT]->Draw(m_device, m_posX + m_width - m_pic[TOP_RIGHT]->GetWidth(), m_posY + m_pic[TOP_LEFT]->GetHeight());
	m_pic[TOP]->Draw(m_device, m_posX + m_pic[TOP_LEFT]->GetWidth(), m_posY);
	m_pic[BOTTOM]->Draw(m_device, m_posX + m_pic[TOP_LEFT]->GetWidth(),  m_posY + m_height - m_pic[BOTTOM_LEFT]->GetHeight());
	m_pic[CENTER]->Draw(m_device,  m_posX + m_pic[TOP_LEFT]->GetWidth(),  m_posY + m_pic[TOP_LEFT]->GetHeight());
}

bool CNFrame::LoadTexture(DInt16 index, char* filename)
{
	switch(index)
	{
		case TOP_LEFT:
			{
				m_pic[TOP_LEFT] = new CTextureEx();
				if(!m_pic[TOP_LEFT]->Load(m_device, filename))
					return false;
				break;
			}
		case TOP:
			{
				m_pic[TOP] = new CTextureEx();
				if(!m_pic[TOP]->Load(m_device, filename))
					return false;
				break;
			}
		case TOP_RIGHT:
			{
				m_pic[TOP_RIGHT] = new CTextureEx();
				if(!m_pic[TOP_RIGHT]->Load(m_device, filename))
					return false;
				break;
			}
		case LEFT:
			{
				m_pic[LEFT] = new CTextureEx();
				if(!m_pic[LEFT]->Load(m_device, filename))
					return false;
				break;
			}
		case CENTER:
			{
				m_pic[CENTER] = new CTextureEx();
				if(!m_pic[CENTER]->Load(m_device, filename))
					return false;
				break;
			}
		case RIGHT:
			{
				m_pic[RIGHT] = new CTextureEx();
				if(!m_pic[RIGHT]->Load(m_device, filename))
					return false;
				break;
			}
		case BOTTOM_LEFT:
			{
				m_pic[BOTTOM_LEFT] = new CTextureEx();
				if(!m_pic[BOTTOM_LEFT]->Load(m_device, filename))
					return false;
				break;
			}
		case BOTTOM:
			{
				m_pic[BOTTOM] = new CTextureEx();
				if(!m_pic[BOTTOM]->Load(m_device, filename))
					return false;
				break;
			}
		case BOTTOM_RIGHT:
			{
				m_pic[BOTTOM_RIGHT] = new CTextureEx();
				if(!m_pic[BOTTOM_RIGHT]->Load(m_device, filename))
					return false;
				break;
			}
		default:
			return false;
	}
	m_successRes = true;;
	return true;
}
void CNFrame::SetPos(DInt16 x, DInt16 y)
{
	m_posX = x;
	m_posY = y;
}
D3DXVECTOR2 CNFrame::GetPos()
{
	D3DXVECTOR2 v ;
	v.x = (float)m_posX;
	v.y = (float)m_posY;
	return v;
}

void CNFrame::SetSize(DInt16 width, DInt16 height)
{
	m_width = width;
	m_height = height;
	if(!CheckRes())
		return;
	//¼ÆËãËõ·ÅÁ¿
	float temp = m_height - m_pic[TOP_LEFT]->GetHeight() - m_pic[BOTTOM_LEFT]->GetHeight();
	m_verticalScale = temp/m_pic[LEFT]->GetHeight();
	temp = m_width - m_pic[TOP_LEFT]->GetWidth() - m_pic[TOP_RIGHT]->GetWidth();
	m_horizonScale = temp/m_pic[TOP]->GetWidth();
	
	m_pic[LEFT]->SetScaleY(m_verticalScale);
	m_pic[RIGHT]->SetScaleY(m_verticalScale);

	m_pic[TOP]->SetScaleX(m_horizonScale);
	m_pic[BOTTOM]->SetScaleX(m_horizonScale);

	m_pic[CENTER]->SetScale(m_horizonScale, m_verticalScale);
}
D3DXVECTOR2 CNFrame::GetSize()
{
	D3DXVECTOR2 v ;
	v.x = (float)m_width;
	v.y = (float)m_height;
	return v;
}

void CNFrame::SetRender(bool b)
{
	m_render = b;
}

bool CNFrame::GetRender()
{
	return m_render;
}


