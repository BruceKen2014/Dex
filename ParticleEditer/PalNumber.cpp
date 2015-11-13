
#include "state/DexGameEngine.h"
#include "PalNumber.h"

PalNumber::PalNumber()
{
	m_number = 0;
	m_OpenSign = true;
	m_offSet = 0;
	m_startPosX = m_startPosY = 0;
	m_texture = NULL;
}

PalNumber::~PalNumber()
{
	ShutDown();
}

void PalNumber::ShutDown()
{
	m_numberVec.clear();
	_SafeDelete(m_texture); 

}

void PalNumber::SetNumber(int number)
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

void PalNumber::SetPos(TSHORT posX, TSHORT posY)
{
	m_startPosX = posX;
	m_startPosY = posY;
}


bool PalNumber::LoadTexture(const char* filename)
{
	if(filename == NULL)
		return false;
	_SafeDelete(m_texture); 
	m_texture = new CTextureEx();
	if(!m_texture->Load(DexGameEngine::getEngine()->GetDevice(),(char*) filename))
		return false;
	m_NumberWidth = m_texture->GetSrcWidth()/11;
	m_offSet = m_NumberWidth;
	return true;
}
void PalNumber::SetOpenSign(bool openSign)
{
	m_OpenSign = openSign;
}

void PalNumber::Update()
{
}
void PalNumber::Render()
{
	if(m_texture == NULL)
		return;	 
	if(m_OpenSign)
		m_texture->DrawPart(DexGameEngine::getEngine()->GetDevice(), m_startPosX , m_startPosY, m_NumberWidth*10, 0, m_NumberWidth, m_texture->GetSrcHeight());
	for(size_t i = 0; i < m_numberVec.size() ;i++)
	{
		if(m_OpenSign)
			//如果开启符号，则各位都向后移动一个宽度
			m_texture->DrawPart(DexGameEngine::getEngine()->GetDevice(), m_startPosX + (i + 1) * m_offSet, m_startPosY, m_numberVec[i], 0, m_NumberWidth, m_texture->GetSrcHeight());
		else
			m_texture->DrawPart(DexGameEngine::getEngine()->GetDevice(), m_startPosX + (i + 1) * m_offSet, m_startPosY, m_numberVec[i], 0, m_NumberWidth, m_texture->GetSrcHeight());
	}
}
//==================================================================================
PalNumberAni::PalNumberAni():PalNumber()
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
PalNumberAni::~PalNumberAni()
{
	ShutDown();
}
void PalNumberAni::Update()
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

void PalNumberAni::SetEndPos(TSHORT endX, TSHORT endY)
{
	m_endPosX = endX;
	m_endPosY = endY;
}

void PalNumberAni::ResetAlpha()
{
	m_alpha = 255;
}

bool PalNumberAni::GetComplete()
{
	return m_complete;
}

void PalNumberAni::SetSpeed(int speed)
{
	m_speed = speed;
}
void PalNumberAni::SetPos(TSHORT posX, TSHORT posY)
{
	m_startPosX = posX; 
	m_startPosY = posY;
	m_currPosX = m_startPosX;
	m_currPosY = m_startPosY;
}
void PalNumberAni::Render()
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
			m_texture->DrawPart(DexGameEngine::getEngine()->GetDevice(), m_currPosX , m_currPosY, m_NumberWidth*10, 0, m_NumberWidth, m_texture->GetSrcHeight());
		for(size_t i = 0; i < m_numberVec.size() ;i++)
		{
			if(m_OpenSign)
				//如果开启符号，则各位都向后移动一个宽度
				m_texture->DrawPart(DexGameEngine::getEngine()->GetDevice(), m_currPosX + (i + 1) * m_offSet, m_currPosY, m_numberVec[i], 0, m_NumberWidth, m_texture->GetSrcHeight());
			else
				m_texture->DrawPart(DexGameEngine::getEngine()->GetDevice(), m_currPosX + (i + 1) * m_offSet, m_currPosY, m_numberVec[i], 0, m_NumberWidth, m_texture->GetSrcHeight());
		}
	}
}

bool PalNumberAni::LoadIni(const char* filename, char*filed)
{
	char file[64];
	m_OpenSign = CIni::GetInt((char*)filename, filed, "opensign");
	CIni::GetString((char*)filename, filed, "tex", file);
	if(!LoadTexture(file))
		return false;
	return true;
}

//////////////////////////
PalNumberManager::PalNumberManager()
{

}

PalNumberManager::~PalNumberManager()
{
	foreach(std::list<PalNumberAni*>, ite, g_listNumbers)
	{
		_SafeDelete(*ite);
		ite = g_listNumbers.erase(ite);
	}
}
PalNumberAni* PalNumberManager::createNumberAni(EPlayerType type, bool skill_good, EPalPlayerHpType hpType)
{
	string filename;
	if(type == EPT_ENEMY)
	{
		if(hpType == EPHP_HP)
		{
			if(skill_good)
			{//怪物加精
				filename = "res\\number_addhp.png";
			}
			else
			{//怪物减精
				filename = "res\\number_losehp.png";
			}
		}
		else if(hpType == EPHP_MP)
		{
			if(skill_good)
			{//怪物加神
				filename = "res\\number_addmp.png";
			}
			else
			{//怪物减神
				filename = "res\\number_losemp.png";
			}
		}
	}
	else
	{
		if(hpType == EPHP_HP)
		{
			if(skill_good)
			{//我方加精
				filename = "res\\number_addhp.png";
			}
			else
			{//我方减精
				filename = "res\\number_losehp.png";
			}
		}
		else if(hpType == EPHP_MP)
		{
			if(skill_good)
			{//我方加神
				filename = "res\\number_addmp.png";
			}
			else
			{//我方减神
				filename = "res\\number_losemp.png";
			}
		}
	}
	PalNumberAni *number = new PalNumberAni();
	if(!number->LoadTexture(filename.c_str()))
	{
		_SafeDelete(number);
		return NULL;
	}
	else
	{
		g_listNumbers.push_back(number);
		return number;
	}
}

bool PalNumberManager::Update(int delta)
{
	foreach(std::list<PalNumberAni*>, ite, g_listNumbers)
	{
		(*ite)->Update();
		if((*ite)->GetComplete())
		{
			_SafeDelete(*ite);
			ite = g_listNumbers.erase(ite);
			if(ite == g_listNumbers.end())
				break;
		}
	}
	return true;
}

bool PalNumberManager::Render()
{
	foreach(std::list<PalNumberAni*>, ite, g_listNumbers)
	{
		(*ite)->Render();
	}
	return true;
}
