


#include "CCollideBox.h"

CCollideBox::CCollideBox(LPDIRECT3DDEVICE9 device, D3DXVECTOR3 pos, D3DXVECTOR3 _size, bool render)
{
	Initialize(DexVector3(pos.x, pos.y, pos.z), DexVector3(_size.x, _size.y, _size.z));
	m_device = device;
	m_render = render;
#ifdef DEX_DEBGU
	m_mesh = NULL;
	D3DXCreateBox(m_device, _size.x, _size.y, _size.z, &m_mesh, NULL);
#endif
}


CCollideBox::~CCollideBox()
{
	Shutdown();
}

void CCollideBox::Shutdown()
{
#ifdef DEX_DEBGU
	_SafeRelease(m_mesh);
#endif
}

void CCollideBox::Render()
{
	if(!m_render)
		return ;
#ifdef DEX_DEBGU
	D3DXMATRIX translation;     //坐标转换,之前我将原点设在顶点0，结果发现D3D是设在下面的中心
	D3DXMatrixTranslation(&translation, m_posX + m_width/2, m_posY + m_height/2, m_posZ + m_depth/2); 
	m_device->SetTransform(D3DTS_WORLD, &translation);
	if(m_mesh != NULL)
		m_mesh->DrawSubset(0);
	D3DXMatrixTranslation(&translation, 0, 0, 0); 
	m_device->SetTransform(D3DTS_WORLD, &translation);
#endif
}

void CCollideBox::SetRender(bool render)
{
	m_render = render;
}

bool CCollideBox::GetRender()
{
	return m_render;
}