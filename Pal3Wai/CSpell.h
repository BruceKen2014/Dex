
/*
Dex引擎法阵文件
作者：BruceKen
2013.1
*/


/*
能够转动的法阵，如战斗状态下人物脚踩的法阵、传送阵等
*/	  

#pragma once

#include "../DexBase/typedefine.h"
#include "../DexBase/CReference.h"


/*

1    3

0    2
*/
class CSpell:public CRefCount
{
protected:
	LPDIRECT3DDEVICE9 m_device;
	LPDIRECT3DTEXTURE9 m_texture;  //法阵纹理
	LPDIRECT3DVERTEXBUFFER9 m_vb ;  //顶点缓冲
	D3DXVECTOR3 m_pos;  //位置
	bool        m_clock; //是否顺时针转动
	float       m_width, m_length;  //长、宽
	float       m_speed;  //转动速度
	float       m_yRotate;  //当前绕Y轴的旋转角度

	D3DXMATRIX  m_matrix; //矩阵

protected:
    
	virtual void ShutDown();
public:
	CSpell(LPDIRECT3DDEVICE9 device);
	~CSpell();			 

public:
	virtual void Update();
	virtual void Render();

public:		
	virtual bool CreateVB(); 
	virtual bool LoadTexture(char* filename);

	virtual void SetPos(float x, float y ,float z);
	virtual void SetPos(D3DXVECTOR3 pos);
	virtual D3DXVECTOR3 GetPos();

	virtual void SetClockRotate(bool b);
	virtual bool GetClockRotate();

	virtual void SetSpeed(float f);
	virtual float GetSpeed();

	virtual void SetSize(float width, float length);
};
//typedef CPointRef<CSpell> RCSpell;