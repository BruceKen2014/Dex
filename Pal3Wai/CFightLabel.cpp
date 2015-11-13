


#include "CFightLabel.h"

CFightHead::CFightHead(LPDIRECT3DDEVICE9 device)
{
	m_device = device;
	m_texture = NULL;
	m_enmyPoint = NULL;
	m_speed = 0;
	m_offsetX = m_offsetY = 0;
	m_posX = m_posY = 0;
	m_player = false;

	m_texture = new CTextureEx();
}

CFightHead::~CFightHead()
{
	ShutDown();
}

void CFightHead::ShutDown()
{
	_SafeDelete(m_texture);
	m_device = NULL;
	m_enmyPoint = NULL;
}
void CFightHead::Update()
{
	m_posX += m_speed;
}

void CFightHead::Render()
{
	if(m_device != NULL && m_texture != NULL)
	{
		m_texture->Draw(m_device, m_posX, m_posY);
	}
}

void CFightHead::SetEnemy(CEnemy* enmy)
{
	m_enmyPoint = enmy;
	if(m_enmyPoint !=NULL)
		m_enmyPoint->SetFightHead(this);
}

CEnemy* CFightHead::GetEnemy()
{
	return m_enmyPoint;
}

CTextureEx* CFightHead::GetTex()
{
	return m_texture;
}

void CFightHead::SetPos(float x, float y)
{
	m_posX = x;
	m_posY = y;
}

void CFightHead::SetOffset(TSHORT x, TSHORT y)
{
	m_offsetX = x;
	m_offsetY = y;
}

void CFightHead::SetSpeed(float speed)
{
	m_speed = speed;
}

void CFightHead::SetIsPlayer(bool b)
{
	m_player = b;
}

bool CFightHead::GetIsPlayer()
{
	return m_player;
}

TSHORT CFightHead::GetPosX()
{
	return m_posX;
}

TSHORT CFightHead::GetPosY()
{
	return m_posY;
}

TSHORT CFightHead::GetDownCenterX()
{
	return m_posX + m_texture->GetWidth()/2;
}

TSHORT CFightHead::GetDownCenterY()
{
	return m_posY + m_texture->GetHeight();
}


TSHORT CFightHead::GetOffSetX()
{
	return m_offsetX;
}

bool CFightHead::LoadTexture(char* filename)
{
	if(m_device == NULL || m_texture == NULL)
		return false;
	if(!m_texture->Load(m_device, filename))
	{
		return false;
	}
	m_offsetX = m_texture->GetWidth()/2; 
	return true;
}

//战斗进度条

CFightLabel::CFightLabel(LPDIRECT3DDEVICE9 device)
{
	m_device = device;
	m_back = NULL;
	m_selectHead = NULL;
	m_figher = NULL;
	m_pointHead = NULL;
	m_length = 10;
	m_posX = m_posX = 0;
	m_offsetX = m_offsetY = 0;
	m_offsetXM = m_offsetYM = 0;
	m_endOffsetX = m_endOffsetY = 0;

	m_back = new CTextureEx();
	m_pointHead = new CTextureEx();
}

CFightLabel::~CFightLabel()
{
	ShutDown();
}

void CFightLabel::ShutDown()
{
	m_device = NULL;
	m_figher = NULL;
	_SafeClearVector(m_heads);
	_SafeDelete(m_back);
	_SafeDelete(m_pointHead);
}

void CFightLabel::Update()
{
	for(size_t i = 0; i < m_heads.size(); i++)
	{
		if(m_heads[i] != NULL)
		{
			if(m_heads[i]->GetEnemy() != NULL)
			{
				if(m_heads[i]->GetEnemy()->GetCurrHp() > 0)
					_SafeUpdate(m_heads[i]);
			}
		}
	}
}

void CFightLabel::Render()
{
	if(m_device == NULL)
		return;
	if(m_back != NULL)
		m_back->Draw(m_device, m_posX, m_posY);
	for(size_t i = 0; i < m_heads.size(); i++)
	{
		if(m_heads[i]->GetEnemy() != NULL)
		{
			if(m_heads[i]->GetEnemy()->GetCurrHp() > 0)
				_SafeRender(m_heads[i]);
		}
	}
	if(m_pointHead != NULL && m_selectHead != NULL)
	{
		if(m_selectHead->GetTex() != NULL)
			m_pointHead->Draw(m_device, m_selectHead->GetDownCenterX(), m_selectHead->GetDownCenterY(), DRAW_TOP_CENTER);
	}
}

void CFightLabel::AddHead(CFightHead* head)
{
	if(head == NULL)
		return;
	if(head->GetIsPlayer())
		head->SetPos(m_posX + m_offsetXM - head->GetOffSetX(), m_posY + m_offsetYM);
	else
	    head->SetPos(m_posX + m_offsetX - head->GetOffSetX(), m_posY + m_offsetY);
	m_heads.push_back(head);
}

void CFightLabel::SetSelectHead(CFightHead *head)
{
	m_selectHead  = head;
}

void CFightLabel::SetPos(TSHORT x, TSHORT y)
{
	m_posX = x;
	m_posY = y;
}
void CFightLabel::SetOffset(TSHORT x, TSHORT y)
{
	m_offsetX = x;
	m_offsetY = y;
}

void CFightLabel::SetOffsetM(TSHORT x, TSHORT y)
{
	m_offsetXM = x;
	m_offsetYM = y;
}

void CFightLabel::SetEndOffset(TSHORT x, TSHORT y)
{
	m_endOffsetX = x;
	m_endOffsetY = y;
}

void CFightLabel::SetLength(TSHORT length)
{
	m_length = length;
}

bool CFightLabel::LoadBackTexture(char* filename)
{
	if(m_device == NULL || m_back == NULL)
		return false;
	if(!m_back->Load(m_device, filename))
	{
		return false;
	}
	return true;
}

bool CFightLabel::LoadPointHeadTex(char* filename)
{
	if(m_device == NULL || m_pointHead == NULL)
		return false;
	if(!m_pointHead->Load(m_device, filename))
	{
		return false;
	}
	return true;
}
bool CFightLabel::ThereIsComplete()
{
	for(size_t i = 0; i < m_heads.size(); i++)
	{
		if(m_heads[i] == NULL)
			continue;
		if(m_heads[i]->GetEnemy() != NULL)
		{
			if(m_heads[i]->GetEnemy()->GetCurrHp() <= 0)
				continue;
		}
		TSHORT curr = m_heads[i]->GetPosX() + m_heads[i]->GetOffSetX();
		TSHORT ed = (m_posX + m_length - m_endOffsetX); 
		if(curr >= ed)
		{//到尽头了
			m_figher = m_heads[i];  //更改指针
			return true;
		}
	}
	return false;
}

void CFightLabel::ResetFighteHead()
{
	if(m_figher != NULL)
	{
		if(m_figher->GetIsPlayer())
			m_figher->SetPos(m_posX + m_offsetXM - m_figher->GetOffSetX(), m_posY + m_offsetYM);
		else
			m_figher->SetPos(m_posX + m_offsetX - m_figher->GetOffSetX(), m_posY + m_offsetY);
	}
}

CFightHead*  CFightLabel::GetFighterHead()
{
	return m_figher;
}