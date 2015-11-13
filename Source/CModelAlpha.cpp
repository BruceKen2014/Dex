


#include "CModelAlpha.h"

CModelAlpha::CModelAlpha(LPDIRECT3DDEVICE9 device, char* filename):CModelX(device, filename)
{
	m_ZLength = 0.0f;
}

CModelAlpha::~CModelAlpha()
{
	ShutDown();
}

void CModelAlpha::UpdateLength(D3DXVECTOR3 camera_pos)
{
	m_ZLength = D3DXVec3Length(&(D3DXVECTOR3(m_posX, m_posY, m_posZ) - camera_pos));
}

float CModelAlpha::GetZLength()
{
	return m_ZLength;
}