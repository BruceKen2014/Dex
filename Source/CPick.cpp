


#include "CPick.h"

CPick::CPick()
{
	m_device = NULL;
}

CPick::~CPick()
{
	ShutDown();
}

bool CPick::Initial(LPDIRECT3DDEVICE9 device)
{
	m_device = device;
	return true;
}

stRay CPick::GetRay(D3DXMATRIX projection, D3DXMATRIX view, short x, short y)
{
	float px=0;
	float py=0;
	D3DVIEWPORT9 viewport;
	m_device->GetViewport(&viewport);
	//视区、投影变换
	px=(((2.0f*x)/viewport.Width)-1.0f)/projection(0,0);
	py=(((-2.0f*y)/viewport.Height)+1.0f)/projection(1,1);
	m_Ray.m_origin = D3DXVECTOR3(0,0,0);
	m_Ray.m_vector = D3DXVECTOR3(px,py,1);

	D3DXMATRIX oppo_view;
	//矩阵求逆
	D3DXMatrixInverse(&oppo_view, 0, &view);

	D3DXVec3TransformCoord(&m_Ray.m_origin, &m_Ray.m_origin, &oppo_view);
	D3DXVec3TransformNormal(&m_Ray.m_vector, &m_Ray.m_vector, &oppo_view);

	D3DXVec3Normalize(&m_Ray.m_vector, &m_Ray.m_vector);
	return m_Ray;
}

stRay CPick::GetRay(D3DXMATRIX projection, D3DXMATRIX view,  D3DXVECTOR2 _point)
{
	return GetRay(projection, view, _point.x, _point.y);
}

POINT CPick::GetXY(D3DXMATRIX projection, D3DXMATRIX view, D3DXVECTOR3 point)
{
	POINT p;
	stRay ray;
	ray.m_origin = g_camera.GetPosition();
	ray.m_vector = ray.m_origin - point;  
	D3DXVec3Normalize(&ray.m_vector, &ray.m_vector);

	D3DXMATRIX oppo_view;
	//矩阵求逆
	D3DXMatrixInverse(&oppo_view, 0, &view);

	//D3DXVec3TransformCoord(&ray.m_origin, &ray.m_origin, &oppo_view);
	//D3DXVec3TransformNormal(&ray.m_vector, &ray.m_vector, &oppo_view);

	D3DXVec3TransformCoord(&ray.m_origin, &ray.m_origin, &view);
	D3DXVec3TransformNormal(&ray.m_vector, &ray.m_vector, &view);
	float px = ray.m_vector.x/ray.m_vector.z;
	float py = ray.m_vector.y/ray.m_vector.z;
	D3DVIEWPORT9 viewport;
	m_device->GetViewport(&viewport);
	//px=(((2.0f*x)/viewport.Width)-1.0f)/projection(0,0);
	//py=(((-2.0f*y)/viewport.Height)+1.0f)/projection(1,1);
	p.x = (px * projection(0,0) + 1.0f) * viewport.Width / 2.0f;
	p.y = (py * projection(1,1) - 1.0f) * viewport.Height / (-2.0f); 
	return p;
}