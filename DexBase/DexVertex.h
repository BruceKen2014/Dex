
#ifndef _DEX_VERTEX_H
#define _DEX_VERTEX_H
#include <d3dx9math.h>
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
} stVertex1;

struct stVertex2
{
	D3DXVECTOR3 m_pos;
	float m_u, m_v;
	D3DXVECTOR3 m_normal;
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
};

#endif // !_DEX_VERTEX_H
