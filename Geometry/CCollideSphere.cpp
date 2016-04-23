

#include "CCollideShpere.h"

CCollideSphere::CCollideSphere(LPDIRECT3DDEVICE9 device, D3DXVECTOR3 center, float radius, bool render)
{
	m_device = device;
	memcpy(&m_center, &center, sizeof(D3DXVECTOR3));
	m_radius = radius;
	m_render = render;
#ifdef DEX_DEBGU
	m_mesh = NULL;
	D3DXCreateSphere(m_device, m_radius, 10, 10, &m_mesh, NULL);
#endif
}

CCollideSphere::~CCollideSphere()
{
	Shutdown();
}

void CCollideSphere::Shutdown()
{
#ifdef DEX_DEBGU
	_SafeRelease(m_mesh);
#endif
}

void CCollideSphere::Render()
{
	if(!m_render)
		return ;
#ifdef DEX_DEBGU
	D3DXMATRIX translation;
	D3DXMatrixTranslation(&translation, m_center.x, m_center.y, m_center.z);
	m_device->SetTransform(D3DTS_WORLD, &translation);
	if(m_mesh != NULL)
		m_mesh->DrawSubset(0);
	D3DXMatrixTranslation(&translation, 0, 0, 0); 
	m_device->SetTransform(D3DTS_WORLD, &translation);
#endif
}

void CCollideSphere::SetRender(bool render)
{
	m_render = render;
}

bool CCollideSphere::GetRender()
{
	return m_render;
}