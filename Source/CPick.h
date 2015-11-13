/*************************************************************
****DemoEx引擎鼠标拾取类
****作者：阚正杰(BruceKen)
****日期：2012-12-6
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
	//根据屏幕上鼠标点击的x、y返回3D空间中的射线
	stRay  GetRay(D3DXMATRIX projection,  D3DXMATRIX view, short x, short y); 
	stRay  GetRay(D3DXMATRIX projection,  D3DXMATRIX view, D3DXVECTOR2 _point);

	POINT  GetXY(D3DXMATRIX projection,  D3DXMATRIX view, D3DXVECTOR3 point); //根据空间中的点返回对应于屏幕上的坐标
};

#endif