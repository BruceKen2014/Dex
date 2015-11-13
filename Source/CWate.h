

#pragma once 

#include "../DexBase/typedefine.h"

class CWater
{
public:
	CWater(){};
	~CWater(){};
public:
	virtual void Update()= 0;
	virtual void Render()= 0;
	virtual void ShutDown()= 0;
};

#define  WATER_DELAY 6
class CWaterF: public CWater
{
protected:
	LPDIRECT3DDEVICE9       m_device;        //�豸ָ��	
	LPDIRECT3DVERTEXBUFFER9 m_VertexBuffer;  //���㻺��	

	stVertex2               m_vectex[4];

	D3DXVECTOR3             m_position;	     //λ��
	vector<stTexture*>        m_pTexList;    //�����б�
	int                     m_width; //ˮ����
	int                     m_length;
	int                     m_curr;  //��ǰ��������
	int                     m_delay;

public:
	CWaterF();
	~CWaterF();

	bool Init(LPDIRECT3DDEVICE9 device, const char *filename, int width, int length);
    void SetPos(const D3DXVECTOR3& pos);
	void AddTexture(const char*filename);
	virtual void Update();
	virtual void Render();
	virtual void ShutDown();
};