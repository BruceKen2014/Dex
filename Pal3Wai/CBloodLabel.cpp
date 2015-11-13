

#include "CBloodFlag.h"

CBloodLabel::CBloodLabel(LPDIRECT3DDEVICE9 device)
{
	m_device = device;
	m_texture = NULL;
	m_texture = new CTextureEx();
	m_font = NULL;
	m_number = 0;
	m_currNum = 0;
	m_posX = 0;
	m_posY = 0;
	m_posXF = 0;
	m_posYF = 0;
}

CBloodLabel::~CBloodLabel()
{
	ShutDown();
}

void CBloodLabel::ShutDown()
{
	_SafeDelete(m_texture);
	m_font = NULL;
}

bool CBloodLabel::LoadIni(char* filename, char*filed)
{
	m_posX = CIni::GetInt(filename, filed, "posx");
	m_posY = CIni::GetInt(filename, filed, "posy");
	m_posXF = CIni::GetInt(filename, filed, "posxf");
	m_posYF = CIni::GetInt(filename, filed, "posyf");
	char file[64];
	CIni::GetString(filename, filed, "tex", file);
	if(!m_texture->Load(m_device, file))
		return false;
	return true;
}

void CBloodLabel::Update()
{
}

void CBloodLabel::Render()
{
	if(m_device== NULL)
		return;
	if(m_font != NULL)
	{
		m_font->SetPos(m_posXF, m_posYF); 
		m_font->Print("%d/%d", m_currNum, m_number);
	}
	if(m_texture != NULL)
	{
		float width = m_texture->GetWidth()* float(m_currNum)/float(m_number);
		float height = m_texture->GetHeight();
		m_texture->DrawPart(m_device, m_posX, m_posY, 0, 0, width, height);
	}
}

void CBloodLabel::SetNumber(int number)
{
	m_number = number;
	m_currNum = m_number;
}
void CBloodLabel::SetCurrNum(int number)
{
	m_currNum = number;
}

void CBloodLabel::AddNum(int number)
{
	m_currNum += number;
	if(m_currNum > m_number)
		m_currNum = m_number;
}

void CBloodLabel::ReduceNum(int number)
{
	m_currNum -= number;
	if(m_currNum < 0)
		m_currNum = 0;
}

void CBloodLabel::SetFont(CMyFont *font)
{
	m_font = font;
}