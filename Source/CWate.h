

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
	LPDIRECT3DDEVICE9       m_device;        //设备指针	
	LPDIRECT3DVERTEXBUFFER9 m_VertexBuffer;  //顶点缓存	

	stVertex2               m_vectex[4];

	D3DXVECTOR3             m_position;	     //位置
	vector<stTexture*>        m_pTexList;    //纹理列表
	int                     m_width; //水面宽度
	int                     m_length;
	int                     m_curr;  //当前纹理索引
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