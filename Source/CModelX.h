/*************************************************************
****DemoEx����X�ļ���װ��CModelX
****���ߣ�������(BruceKen)
****���ڣ�2012-10-27
*************************************************************/


#ifndef _CMODELX_H
#define _CMODELX_H


#include "CModel.h"
class CModelX: public CModel
{
protected:
	
	LPD3DXMESH        m_model;            //����ģ�ͽӿ�
	DWORD             m_numMaterials;	  
	LPD3DXBUFFER      m_matBuffer;	      //���ʻ���
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