/*************************************************************
****DemoEx�������ʰȡ��
****���ߣ�������(BruceKen)
****���ڣ�2012-12-6
*************************************************************/

#ifndef _CPICK_H
#define _CPICK_H


#include "../DexBase/typedefine.h"
#include "../Geometry/CRay.h"
#include "CCamera.h"


extern CCamera g_camera;
class CPick
{
protected:
	LPDIRECT3DDEVICE9 m_device;
	stRay m_Ray;
public:
	CPick();
	~CPick();
	void ShutDown(){};

public:
	bool   Initial(LPDIRECT3DDEVICE9 device);
	//������Ļ���������x��y����3D�ռ��е�����
	stRay  GetRay(D3DXMATRIX projection,  D3DXMATRIX view, short x, short y); 
	stRay  GetRay(D3DXMATRIX projection,  D3DXMATRIX view, D3DXVECTOR2 _point);

	POINT  GetXY(D3DXMATRIX projection,  D3DXMATRIX view, D3DXVECTOR3 point); //���ݿռ��еĵ㷵�ض�Ӧ����Ļ�ϵ�����
};

#endif