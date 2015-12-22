

#include "DexVertex.h"

_stVertex0::_stVertex0()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_color = getD3DColor(DexColor(1.0f, 1.0f, 1.0f));
}
void _stVertex0::SetColor(const DexColor& color)
{
	m_color = getD3DColor(color);
}
void _stVertex0::SetPos(const DexVector3& pos)
{
	m_pos.x = pos.x;
	m_pos.y = pos.y;
	m_pos.z = pos.z;
}

_stVertex1::_stVertex1()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_color = getD3DColor(DexColor(1.0f, 1.0f, 1.0f));
	m_u = m_v = 0.0f;
}
_stVertex1::_stVertex1(D3DXVECTOR3 pos, DexColor color, float u, float v)
{
	m_pos = pos;
	m_color = getD3DColor(color);
	m_u = u;
	m_v = v;
}
void _stVertex1::SetUV(float u, float v)
{
	m_u = u;
	m_v = v;
}
void _stVertex1::SetColor(const DexColor& color)
{
	m_color = getD3DColor(color);
}
void _stVertex1::SetPos(const DexVector3& pos)
{
	m_pos.x = pos.x;
	m_pos.y = pos.y;
	m_pos.z = pos.z;
}
//vertex2-----------------------------------------------------
stVertex2::stVertex2()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_u = m_v = 0.0f;
}

void stVertex2::SetUV(float u, float v)
{
	m_u = u;
	m_v = v;
}
void stVertex2::SetPos(const DexVector3& pos)
{
	m_pos.x = pos.x;
	m_pos.y = pos.y;
	m_pos.z = pos.z;
}
void stVertex2::SetNormal(const DexVector3& normal)
{
	m_normal.x = normal.x;
	m_normal.y = normal.y;
	m_normal.z = normal.z;
}

//vertex3----------------------------------------------------
stVertex3::stVertex3()
{
	m_pos = DexVector3(0.0f, 0.0f, 0.0f);
	m_normal = DexVector3(0.0f, 0.0f, 1.0f);
	m_u = m_v = 0.0f;
	SetColor(DEXCOLOR_WHITE);
}
stVertex3::stVertex3(const DexVector3& pos, const DexVector3& normal, float u, float v) :m_u(u), m_v(v)
{
	SetPos(pos);
	SetNormal(normal);
	SetColor(DEXCOLOR_WHITE);
}
void stVertex3::SetColor(const DexColor& _color)
{
	color = getD3DColor(_color);
}
void stVertex3::SetUV(float u, float v)
{
	m_u = u;
	m_v = v;
}
void stVertex3::SetPos(const DexVector3& pos)
{
	m_pos.x = pos.x;
	m_pos.y = pos.y;
	m_pos.z = pos.z;
}
void stVertex3::SetNormal(const DexVector3& normal)
{
	m_normal.x = normal.x;
	m_normal.y = normal.y;
	m_normal.z = normal.z;
}
bool stVertex3::operator == (const stVertex3& p) const
{
	return m_pos == p.m_pos && m_normal == p.m_normal
		&& m_u == p.m_u && m_v == p.m_v;
}
bool stVertex3::operator != (const stVertex3& p)const
{
	return m_pos != p.m_pos ||
		m_normal != p.m_normal ||
		!DexMath::Equal(m_u, p.m_u) ||
		!DexMath::Equal(m_v, p.m_v);
}
bool stVertex3::operator > (const stVertex3& p)const
{
	return (m_pos > p.m_pos) ||
		(m_pos == p.m_pos && m_normal > p.m_normal) ||
		(m_pos == p.m_pos && m_normal == p.m_normal && m_u > p.m_u) ||
		(m_pos == p.m_pos && m_normal == p.m_normal && DexMath::Equal(m_u, p.m_u) && m_v > p.m_v);
}
bool stVertex3::operator < (const stVertex3& p)const
{
	bool posMin = (m_pos < p.m_pos);
	if (posMin)
		return true;
	bool normalMin = (m_pos == p.m_pos && m_normal < p.m_normal);
	if (normalMin)
		return true;
	bool uMin = (m_pos == p.m_pos && m_normal == p.m_normal && m_u < p.m_u);
	if (uMin)
		return true;
	return false;
	//return (m_pos < p.m_pos) ||
	//(m_pos == p.m_pos && m_normal < p.m_normal) ||
	//(m_pos == p.m_pos && m_normal == p.m_normal && m_u < p.m_u) ||
	//(m_pos == p.m_pos && m_normal == p.m_normal && DexMath::Equal(m_u, p.m_u) && m_v < p.m_v);
}