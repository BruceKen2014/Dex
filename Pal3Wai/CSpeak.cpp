



#include "CSpeak.h"


CSpeak::CSpeak(LPDIRECT3DDEVICE9 device)
{
	m_device = device;
	m_words = NULL;
	m_words = (char *)malloc(sizeof(char) * SPEAK_MAX_WORDS);
	memset(m_words, 0, SPEAK_MAX_WORDS);
	m_length = 0;
	m_CurLength = 0;
	m_xOffset = 0;
	m_yOffset = 0;
	m_speed = 100;
	m_headPos = 0;//默认左边
	m_thisTime = m_thisTime = 0;

	m_font = new CMyFont(m_device);
	m_font->SetColor(DexColor(1.0f,1.0f,1.0f));
	m_backPic = new CNFrame(m_device);
	m_pPersonHead = NULL;
}

CSpeak::~CSpeak()
{
	ShutDown();
}

bool CSpeak::LoadBackPic(DInt16 index, char* filename)
{
	if(!m_backPic->LoadTexture(index, filename))
		return false;
	return true;
}
void CSpeak::ShutDown()
{
	_SafeFree(m_words);
	_SafeDelete(m_font);
	_SafeDelete(m_backPic);
}

void CSpeak::Show()
{	
	if(strlen(m_words) == 0)
		return ;
	//显示背景
	if(m_backPic != NULL)
		m_backPic->Draw();
	//输出对于的文字
	if(m_words != NULL)
	{	
		m_thisTime = GetTickCount();
		if(m_thisTime - m_lastTime > m_speed)
		{
			m_lastTime = m_thisTime;
			if(m_CurLength < m_length)
				m_CurLength +=2;
		}
		if(m_CurLength < m_length)
		{
			char temp[1024] = {0};
			strncpy(temp, m_words, m_CurLength);
			sprintf(temp, "%s%c", temp, '\0');
			RECT rect = m_font->GetRect();
			char c= temp[m_CurLength - 1];
			m_font->Print(temp);
		}
		else
		{
			m_font->Print(m_words);
		}
	}
	//显示头像
	if(m_pPersonHead != NULL)
	{
		if(m_headPos == 0)
			m_pPersonHead->Draw(m_device, m_startX, m_startY + m_height, DRAW_BOTTOM_LEFT);
		else if(m_headPos == 1)
			m_pPersonHead->Draw(m_device, m_startX + m_width, m_startY + m_height, DRAW_BOTTOM_RIGHT);
	}
}

void CSpeak::SetHead(CTexture* head, short pos)
{
	m_pPersonHead = head;
	m_headPos = pos;
	//m_pPersonHead->SetScale(2.0f, 1.0f);
	if(m_headPos == 0)
	{
		//设置头像镜像
		m_pPersonHead->SetXMirro(true);
		//移动文字位置
		m_font->SetPos(m_startX + m_pPersonHead->GetWidth() +m_xOffset, m_startY + m_yOffset);
	}
	else
	{
		m_pPersonHead->SetXMirro(false);
		m_font->SetPos(m_startX + m_xOffset, m_startY + m_yOffset);
	}
}

void CSpeak::SetWords(char *words)
{
	//先释放之前的资源
	if(m_words != NULL)
	{
		free(m_words);
		m_words = NULL;
		m_words = (char *)malloc(sizeof(char) * SPEAK_MAX_WORDS);
		memset(m_words, 0, SPEAK_MAX_WORDS);
	}
	strcpy(m_words, words);
	m_length = strlen(words);
}

void CSpeak::AddWords(char * words)
{
	m_length = m_length + strlen(words);
	strcat(m_words, words);
}

void CSpeak::SetSpeed(short speed)
{
	m_speed = speed;
}

void CSpeak::SetWordsOffsetX(short x)
{
	m_xOffset = x;
	if(m_font != NULL)
		m_font->SetPos(m_font->GetRect().left + m_xOffset, m_font->GetRect().top);
}

void CSpeak::SetWordsOffsetY(short y)
{
	m_yOffset = y;
	if(m_font != NULL)
		m_font->SetPos(m_font->GetRect().left, m_font->GetRect().top + m_yOffset);
}


void CSpeak::SetWordsOffset(short x, short y)
{
	m_xOffset = x;
	m_yOffset = y;
	if(m_font != NULL)
	{
		float left = m_font->GetRect().left;
		float width = m_font->GetRect().right - left;
		float top =	m_font->GetRect().top;
		float height =   m_font->GetRect().bottom - top;
		//m_font->SetPos(m_font->GetRect().left + m_xOffset, m_font->GetRect().top + m_yOffset);
		m_font->SetRect(left + m_xOffset, top + m_yOffset, width - m_xOffset, height- m_yOffset);
	}
}

void CSpeak::SetStartPos(short x, short y)
{
	m_startX = x;
	m_startY = y;
	if(m_font != NULL)
		m_font->SetPos(m_startX, m_startY);
	if(m_backPic!= NULL)
		m_backPic->SetPos(m_startX, m_startY);
}

void CSpeak::SetSize(short w, short h)
{
	m_width = w;
	m_height = h;
	if(m_font != NULL)
	{
		if(m_pPersonHead != NULL)
		m_font->SetSize(m_width - m_pPersonHead->GetWidth(), m_height);
	}
	m_backPic->SetSize(m_width, m_height);
}
void CSpeak::ReSetCurrLength()
{
	m_CurLength = 0;
}

bool CSpeak::IsComplete()
{
	return (m_CurLength == m_length);
}

void CSpeak::SetEnd()
{
	m_CurLength = m_length;
}