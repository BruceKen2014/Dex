
#ifndef _DEX_VERTEX_H
#define _DEX_VERTEX_H
#include <d3dx9math.h>
#include "../DexMath/DexVector3.h"
#include "DexColor.h"
typedef struct _stVertex0
{
	D3DXVECTOR3 m_pos;
	D3DCOLOR    m_color;
public:
	_stVertex0()
	{
		m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_color = getD3DColor(DexColor(1.0f, 1.0f, 1.0f));
	}
	void SetColor(const DexColor& color)
	{
		m_color = getD3DColor(color);
	}
	void SetPos(const DexVector3& pos)
	{
		m_pos.x = pos.x;
		m_pos.y = pos.y;
		m_pos.z = pos.z;
	}
} stVertex0;
typedef struct _stVertex1
{
	D3DXVECTOR3 m_pos;
	D3DCOLOR    m_color;
	float m_u, m_v;
public:
	_stVertex1()
	{
		m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_color = getD3DColor(DexColor(1.0f, 1.0f, 1.0f));
		m_u = m_v = 0.0f;
	}
	_stVertex1(D3DXVECTOR3 pos, DexColor color, float u, float v)
	{
		m_pos = pos;
		m_color = getD3DColor(color);
		m_u = u;
		m_v = v;
	}
	void SetUV(float u, float v)
	{
		m_u = u;
		m_v = v;
	}
	void SetColor(const DexColor& color)
	{
		m_color = getD3DColor(color);
	}
	void SetPos(const DexVector3& pos)
	{
		m_pos.x = pos.x;
		m_pos.y = pos.y;
		m_pos.z = pos.z;
	}
} stVertex1;

struct stVertex2
{
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_normal;
	float m_u, m_v;
public:
	stVertex2()
	{
		m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		m_u = m_v = 0.0f;
	}

	void SetUV(float u, float v)
	{
		m_u = u;
		m_v = v;
	}
	void SetPos(const DexVector3& pos)
	{
		m_pos.x = pos.x;
		m_pos.y = pos.y;
		m_pos.z = pos.z;
	}
	void SetNormal(const DexVector3& normal)
	{
		m_normal.x = normal.x;
		m_normal.y = normal.y;
		m_normal.z = normal.z;
	}
};

struct stVertex3
{
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_normal;
	D3DCOLOR color;
	float m_u, m_v;
	
	
public:
	stVertex3()
	{
		m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		m_u = m_v = 0.0f;
	}
	void SetColor(const DexColor& _color)
	{
		color = getD3DColor(_color);
	}
	void SetUV(float u, float v)
	{
		m_u = u;
		m_v = v;
	}
	void SetPos(const DexVector3& pos)
	{
		m_pos.x = pos.x;
		m_pos.y = pos.y;
		m_pos.z = pos.z;
	}
	void SetNormal(const DexVector3& normal)
	{
		m_normal.x = normal.x;
		m_normal.y = normal.y;
		m_normal.z = normal.z;
	}
};

#endif // !_DEX_VERTEX_H
