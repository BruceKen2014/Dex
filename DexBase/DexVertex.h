
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
	_stVertex0();
	void SetColor(const DexColor& color);
	void SetPos(const DexVector3& pos);
} stVertex0;
typedef struct _stVertex1
{
	D3DXVECTOR3 m_pos;
	D3DCOLOR    m_color;
	float m_u, m_v;
public:
	_stVertex1();
	_stVertex1(D3DXVECTOR3 pos, DexColor color, float u, float v);
	void SetUV(float u, float v);
	void SetColor(const DexColor& color);
	void SetPos(const DexVector3& pos);
} stVertex1;

struct stVertex2
{
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_normal;
	float m_u, m_v;
public:
	stVertex2();

	void SetUV(float u, float v);
	void SetPos(const DexVector3& pos);
	void SetNormal(const DexVector3& normal);
};

struct stVertex3
{
	DexVector3 m_pos;
	DexVector3 m_normal;
	D3DCOLOR color;
	float m_u, m_v;
public:
	stVertex3();
	stVertex3(const DexVector3& pos, const DexVector3& normal, float u, float v);
	void SetColor(const DexColor& _color);
	void SetUV(float u, float v);
	void SetPos(const DexVector3& pos);
	void SetNormal(const DexVector3& normal);
	bool operator == (const stVertex3& p) const;
	bool operator != (const stVertex3& p)const;
	bool operator > (const stVertex3& p)const;
	bool operator < (const stVertex3& p)const;
};

#endif // !_DEX_VERTEX_H
