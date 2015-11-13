
#include "CSoundObject.h"

CSoundObject::CSoundObject()
{
	m_device = NULL;
	m_vb     = NULL;
	m_texture = NULL;
	m_pos    = D3DXVECTOR3(0, 0, 0);
	m_id = 0;
	m_render = true;
}

CSoundObject::~CSoundObject()
{
}

bool CSoundObject::LoadTexture(char* filename)
{ 
	_SafeRelease(m_texture);
    if(FAILED(D3DXCreateTextureFromFile(m_device, filename, &m_texture)))
	{
		_Message("º”‘ÿŒ∆¿Ì%s ß∞‹!", filename);
		return false;
	}
    return true;
}

void CSoundObject::SetSoundFile(char* filename)
{
	strcpy(m_sndFile, filename);
}

void CSoundObject::SetRender(bool render)
{
	m_render = render;
}
bool CSoundObject::GetRender()
{
	return m_render;
}

void CSoundObject::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

void CSoundObject::SetPos(float x, float y, float z)
{
	m_pos.x = x;
	m_pos.y = y;
	m_pos.z = z;
}
D3DXVECTOR3 CSoundObject::GetPos()
{
	return m_pos;
}


void CSoundObject::SetId(int id)
{
	m_id = id;
}

int CSoundObject::GetId()
{
	return m_id;
}


void CSoundObject::GetSndFile(char* file)
{
	strcpy(file, m_sndFile);
}