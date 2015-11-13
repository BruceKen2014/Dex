
/*
作者：BruceKen
*/
#pragma  once
//带有alpha属性的模型

#include "CModelX.h"


class CModelAlpha: public CModelX
{
protected:
	float m_ZLength;   //距摄像机的距离

public:
	CModelAlpha(LPDIRECT3DDEVICE9 device, char* filename);
	~CModelAlpha();

public:
	virtual void UpdateLength(D3DXVECTOR3 camera_pos);

	virtual float GetZLength();
};