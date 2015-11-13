
/*************************************************************
****Dex引擎空间碰撞立方体
****作者：阚正杰(BruceKen)
****日期：2012-12-25
*************************************************************/
#pragma  once

#include "CBox.h"

class CCollideBox:public CBox
{
protected:
	LPDIRECT3DDEVICE9 m_device;
#ifdef DEX_DEBGU  
	LPD3DXMESH m_mesh;
#endif
	bool       m_render;

public:
	CCollideBox(LPDIRECT3DDEVICE9 device, D3DXVECTOR3 pos, D3DXVECTOR3 _size, bool render);
	~CCollideBox();

public:
	virtual void Shutdown();
	virtual void Render();

	virtual void SetRender(bool render);
	virtual bool GetRender();
};