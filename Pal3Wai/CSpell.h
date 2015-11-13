
/*
Dex���淨���ļ�
���ߣ�BruceKen
2013.1
*/


/*
�ܹ�ת���ķ�����ս��״̬������Ųȵķ��󡢴������
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
	LPDIRECT3DTEXTURE9 m_texture;  //��������
	LPDIRECT3DVERTEXBUFFER9 m_vb ;  //���㻺��
	D3DXVECTOR3 m_pos;  //λ��
	bool        m_clock; //�Ƿ�˳ʱ��ת��
	float       m_width, m_length;  //������
	float       m_speed;  //ת���ٶ�
	float       m_yRotate;  //��ǰ��Y�����ת�Ƕ�

	D3DXMATRIX  m_matrix; //����

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