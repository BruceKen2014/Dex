
/*
���ߣ�BruceKen
*/
#pragma  once
//����alpha���Ե�ģ��

#include "CModelX.h"


class CModelAlpha: public CModelX
{
protected:
	float m_ZLength;   //��������ľ���

public:
	CModelAlpha(LPDIRECT3DDEVICE9 device, char* filename);
	~CModelAlpha();

public:
	virtual void UpdateLength(D3DXVECTOR3 camera_pos);

	virtual float GetZLength();
};