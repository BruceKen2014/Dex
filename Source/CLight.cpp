




#include "CLight.h"

CLight::CLight()
{}

CLight::CLight(D3DLIGHTTYPE _type, D3DCOLORVALUE diffuse, D3DCOLORVALUE specular)
{
	m_light.Type = _type;
	m_light.Diffuse = diffuse;
	m_light.Specular = specular;

	m_light.Attenuation0 = 1.0f;
	m_light.Attenuation1 = 0.0f; 
	m_light.Attenuation2 = 0.0f;

}

CLight::~CLight()
{
	ShutDown();
}

void CLight::ShutDown()
{
}
D3DLIGHT9 CLight::GetLight() const
{
	return m_light;
}

D3DXVECTOR3 CLight::GetPosition()
{
	return m_light.Position;
}
D3DXVECTOR3 CLight::GetDirection()
{
	return m_light.Direction;
}
float CLight::GetRange()
{
	return m_light.Range;
}
float CLight::GetFallof()
{
	return m_light.Falloff;
}
float CLight::GetTheta()
{
	return m_light.Theta;
}
float CLight::Getphi()
{
	return m_light.Phi;
}
void CLight::SetId( DInt32 _id)
{
	m_id = _id;
}
DInt32 CLight::GetId() const
{
	return m_id;
}

void CLight::SetPostion(float x, float y, float z)
{
	m_light.Position.x = x;
	m_light.Position.y = y;
	m_light.Position.z = z;
}
void CLight::SetPosition(D3DXVECTOR3 position)
{
	m_light.Position = position;
}
void CLight::SetDirection(D3DXVECTOR3 direction)
{
	 m_light.Direction = direction; 
}
void CLight::SetRange(float range)
{
	m_light.Range = range;
}
void CLight::SetFallof(float fallof)
{
	m_light.Falloff = fallof;
}
void CLight::SetThpi(float theta, float phi)
{
	m_light.Theta = theta;
	m_light.Phi   = phi;
}
void CLight::SetTheta(float theta)
{
	m_light.Theta = theta;
}
void CLight::SetPhi(float phi)
{
	m_light.Phi   = phi;
}