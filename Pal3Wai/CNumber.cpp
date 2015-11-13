



#include "CNumber.h"


CNumber::CNumber(LPDIRECT3DDEVICE9 device)
{
	m_device = device; 
	m_number = 0;
	m_OpenSign = false;
	m_offSet = 0;
	m_startPosX = m_startPosY = 0;
	m_texture = NULL;
}

CNumber::~CNumber()
{
	ShutDown();
}

void CNumber::ShutDown()
{
	m_numberVec.clear();
	_SafeDelete(m_texture); 

}

void CNumber::SetNumber(int number)
{
	m_number = number;
	m_numberVec.clear(); //清除之前的坐标
	int temp = 0;
	int tempX;
	while(number!=0)
	{
		int last = number;
		number = number/10;
		temp = last - number*10;
		tempX = temp * m_NumberWidth;  //起始X坐标
		m_numberVec.insert(m_numberVec.begin(), tempX);	  //从前往后插入
	}
}

void CNumber::SetPos(TSHORT posX, TSHORT posY)
{
	m_startPosX = posX;
	m_startPosY = posY;
}


bool CNumber::LoadTexture(char* filename)
{
	if(filename == NULL || m_device == NULL)
		return false;
	_SafeDelete(m_texture); 
	m_texture = new CTextureEx();
	if(!m_texture->Load(m_device, filename))
		return false;
	m_NumberWidth = m_texture->GetSrcWidth()/11;
	m_offSet = m_NumberWidth;
	return true;
}
void CNumber::SetOpenSign(bool openSign)
{
	m_OpenSign = openSign;
}

void CNumber::Update()
{
}
void CNumber::Render()
{
	if(m_texture == NULL)
		return;	 
	if(m_OpenSign)
		m_texture->DrawPart(m_device, m_startPosX , m_startPosY, m_NumberWidth*10, 0, m_NumberWidth, m_texture->GetSrcHeight());
	for(size_t i = 0; i < m_numberVec.size() ;i++)
	{
		if(m_OpenSign)
			//如果开启符号，则各位都向后移动一个宽度
		    m_texture->DrawPart(m_device, m_startPosX + (i + 1) * m_offSet, m_startPosY, m_numberVec[i], 0, m_NumberWidth, m_texture->GetSrcHeight());
		else
			m_texture->DrawPart(m_device, m_startPosX + (i + 1) * m_offSet, m_startPosY, m_numberVec[i], 0, m_NumberWidth, m_texture->GetSrcHeight());
	}
}
//==================================================================================
CNumberAni::CNumberAni(LPDIRECT3DDEVICE9 device):CNumber(device)
{
	m_type = NUMBER_PIC;
	m_currPosX = m_startPosX;
	m_currPosY = m_startPosY;
	m_endPosX = m_startPosY;
	m_endPosY = m_startPosX;
	m_speed = 5;
	m_alphaSpeed = 0;
	m_alpha = 255;
	m_complete = false;
}
CNumberAni::~CNumberAni()
{
	ShutDown();
}
void CNumberAni::Update()
{
	if(m_complete)
		return;
	if(m_startPosX > m_endPosX)
		m_currPosX -= m_speed;
	else if(m_startPosX < m_endPosX)  
		m_currPosX += m_speed;

	if(m_startPosY> m_endPosY)
		m_currPosY -= m_speed;
	else if(m_startPosY < m_endPosY)  
		m_currPosY += m_speed;   

	//根据当前位置到结束位置的距离来设置当前alpha值
	int length = (m_endPosX-m_startPosX)*(m_endPosX-m_startPosX) + (m_endPosY-m_startPosY)*(m_endPosY-m_startPosY);
	int currLength = (m_endPosX-m_currPosX)*(m_endPosX-m_currPosX) + (m_endPosY-m_currPosY)*(m_endPosY-m_currPosY);
	m_alpha = float(currLength)/(float)length *255;
	if(abs(m_currPosX - m_endPosX) <= m_speed
		&& abs(m_currPosY - m_endPosY) <= m_speed)
		m_complete = true;
}

void CNumberAni::SetEndPos(TSHORT endX, TSHORT endY)
{
	m_endPosX = endX;
	m_endPosY = endY;
}

void CNumberAni::ResetAlpha()
{
	m_alpha = 255;
}

bool CNumberAni::GetComplete()
{
	return m_complete;
}

void CNumberAni::SetSpeed(int speed)
{
	m_speed = speed;
}
void CNumberAni::SetPos(TSHORT posX, TSHORT posY)
{
	m_startPosX = posX; 
	m_startPosY = posY;
	m_currPosX = m_startPosX;
	m_currPosY = m_startPosY;
}
void CNumberAni::Render()
{  
	if(m_texture == NULL)
		return;	 
	if(m_type == NUMBER_DEFAUT)
	{
	}
	else if(m_type == NUMBER_PIC)
	{
		m_texture->SetAlpha(m_alpha);
		if(m_OpenSign)
			m_texture->DrawPart(m_device, m_currPosX , m_currPosY, m_NumberWidth*10, 0, m_NumberWidth, m_texture->GetSrcHeight());
		for(size_t i = 0; i < m_numberVec.size() ;i++)
		{
			if(m_OpenSign)
				//如果开启符号，则各位都向后移动一个宽度
				m_texture->DrawPart(m_device, m_currPosX + (i + 1) * m_offSet, m_currPosY, m_numberVec[i], 0, m_NumberWidth, m_texture->GetSrcHeight());
			else
				m_texture->DrawPart(m_device, m_currPosX + (i + 1) * m_offSet, m_currPosY, m_numberVec[i], 0, m_NumberWidth, m_texture->GetSrcHeight());
		}
	}
}

bool CNumberAni::LoadIni(char* filename, char*filed)
{
	char file[64];
	m_OpenSign = CIni::GetInt(filename, filed, "opensign");
	CIni::GetString(filename, filed, "tex", file);
	if(!LoadTexture(file))
		return false;
	return true;
}
