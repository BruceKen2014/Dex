/*************************************************************
****DemoEx引擎X文件封装类CModelX
****作者：阚正杰(BruceKen)
****日期：2012-10-27
*************************************************************/


#ifndef _CMODELX_H
#define _CMODELX_H


#include "CModel.h"
class CModelX: public CModel
{
protected:
	
	LPD3DXMESH        m_model;            //网格模型接口
	DWORD             m_numMaterials;	  
	LPD3DXBUFFER      m_matBuffer;	      //材质缓存
	D3DMATERIAL9*     m_matList;
	LPDIRECT3DTEXTURE9* m_textureList;
public:
	CModelX();
	CModelX(LPDIRECT3DDEVICE9 device, char* filename);
	virtual ~CModelX();
	virtual void ShutDown();
public:
	virtual void Update();
	virtual void Render();
};
#endif