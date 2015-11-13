

#include "CRoad.h"

CRoad::CRoad(LPDIRECT3DDEVICE9 device, D3DXVECTOR3 pos, float width, float length, int id, int dir, float tileX, float tileY):CSoundObject()
{
	m_device = device;
	m_pos = pos;
	m_width = width;
	m_length = length;
	m_dir = dir;
	m_tileX = tileX;
	m_tileY = tileY;
	m_id   = id;
	CreateVB();
}

bool CRoad::CreateVB()
{
	if(FAILED(m_device->CreateVertexBuffer(CROAD_VERTEX * sizeof(stVertex1),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, FVF_XYZ_DIFF_TEX1, D3DPOOL_DEFAULT,
		&m_vb, NULL))) 
		return false;
	stVertex1 point0, point1, point2, point3;
	point0.m_pos =  m_pos;
	point1.m_pos =  m_pos;
	point2.m_pos =  m_pos;	  
	point3.m_pos =  m_pos;	
	point1.m_pos.z = m_pos.z + m_length; 
	point2.m_pos.x = m_pos.x + m_width;
	point3.m_pos.x = m_pos.x + m_width;
	point3.m_pos.z = m_pos.z + m_length;
	if(m_dir == RD_EW)
	{//东西
		point0.m_u = m_tileY;
		point0.m_v = m_tileX;
		point1.m_u = 0.0f;
		point1.m_v = m_tileX;
		point2.m_u = m_tileY;
		point2.m_v = 0.0f;
		point3.m_u = 0.0f;
		point3.m_v = 0.0f;
	}
	else
	{//南北
		point0.m_u = 0.0f;
		point0.m_v = m_tileY;   	
		point1.m_u = 0.0f;
		point1.m_v = 0.0f;
		point2.m_u = m_tileX;
		point2.m_v = m_tileY;
		point3.m_u = m_tileX;
		point3.m_v = 0.0f;
	}
	stVertex1 temp[4] = {point0, point1, point2, point3};

	void *ptr;
	if( FAILED(m_vb->Lock( 0, CROAD_VERTEX * sizeof(stVertex1), (void**)&ptr, 0 ) ) )
		return false;	
	memcpy(ptr, temp, CROAD_VERTEX * sizeof(stVertex1));
	if( FAILED(m_vb->Unlock() ) ) 
		return false; 
	return true;
}

CRoad::~CRoad()
{
	ShutDown();
}

void CRoad::ShutDown()
{
	_SafeRelease(m_vb);
	_SafeRelease(m_texture);
}

void CRoad::Update()
{
}

void CRoad::Render()
{
	if(!m_render)
		return;
	if(m_device == NULL)
		return;
	if(m_texture == NULL)
		return;
	m_device->SetTexture(0, m_texture);
	m_device->SetRenderState(D3DRS_LIGHTING, false);
	m_device->SetStreamSource( 0, m_vb, 0, sizeof(stVertex1));	
	m_device->SetFVF(FVF_XYZ_DIFF_TEX1);
	m_device->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
	m_device->SetTexture(0, NULL);
}

bool CRoad::IsOn(D3DXVECTOR3 pos)
{
	if(pos.x > m_pos.x && pos.x < (m_pos.x + m_width)
		&& pos.z > m_pos.z && pos.z < (m_pos.z + m_length))
	{
		return true;
	}
	return false;
}

float CRoad::GetHeight(D3DXVECTOR3 pos)
{
	return m_pos.y; //始终返回地面高度值
}