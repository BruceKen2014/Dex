

#include "CTextureMove.h"


CTextureMove::CTextureMove(LPDIRECT3DDEVICE9 device):CTextureEx()
{
	m_device = device;
	m_startX = m_startY = 0;
	m_endX = m_endY = 0;
	m_currX = m_currY = 0;
	m_speed = 0;
	m_complete = false;
	m_alphaFlag = CTEXTUREMOVE_NONE;
}

CTextureMove::~CTextureMove()
{
	ShutDown();
}

void CTextureMove::ShutDown()
{
	m_device = NULL;
	CTextureEx::ShutDown();
}

void CTextureMove::Update()
{
	if(m_complete)
		return;
	if(m_startX < m_endX)
	{
		m_currX += m_speed;
	}
	else if(m_startX > m_endX)
	{
		m_currX -= m_speed;
	}

	if(m_startY < m_endY)
	{
		m_currY += m_speed;
	}
	else if(m_startY < m_endY)
	{
		m_currY -= m_speed;
	}

	if((abs(m_currX - m_endX) <= m_speed) && (abs(m_currY - m_endY) <= m_speed) )
		m_complete = true;
}

void CTextureMove::Render()
{
	if(m_device == NULL)
		return;
	switch(m_alphaFlag)
	{
	case CTEXTUREMOVE_NONE:
		break;
	case CTEXTUREMOVE_ZTH:
		{
			if(m_crAlpha < 255)
				m_crAlpha++;
			break;
		}
	case CTEXTUREMOVE_HTZ:
		{
			if(m_crAlpha > 0)
				m_crAlpha--;
			break;
		}
	default:
		break;
	}
	SetAlpha(m_crAlpha);
	CTextureEx::Draw(m_device, m_currX, m_currY);
}

void CTextureMove::SetStartPos(TSHORT x, TSHORT y)
{
	m_startX = x;
	m_startY = y;
	m_currX = m_startX;
	m_currY = m_startY;
}

void CTextureMove::SetEndPos(TSHORT x, TSHORT y)
{
	m_endX = x;
	m_endY = y;
}

void CTextureMove::SetSpeed(float speed)
{
	m_speed = speed;
}

void CTextureMove::SetAlphaFlag(int alphaflag)
{
	m_alphaFlag = alphaflag;
	if(m_alphaFlag == CTEXTUREMOVE_HTZ)
	{
		m_crAlpha = 255;
	}
	else if(m_alphaFlag == CTEXTUREMOVE_ZTH)
	{
		m_crAlpha = 0;
	}
}

void CTextureMove::Reset()
{
	m_currX = m_startX;
	m_currY = m_startY;
	m_complete = false;
}

bool CTextureMove::GetComplete()
{
	return m_complete;
}