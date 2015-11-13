/*************************************************************
****DemoEx������պ���CSkyBox.h
****��飺CSky     ��պл���
          CSkyBox  ��������պ��� ������ CSky
		  �պ�ɴ�CSky����չ��������༰���Ӹ���Ч��
****@���ߣ�������(BruceKen)
****@����:2012-11-16 
*************************************************************/

#ifndef  _CSKYBOX_H
#define  _CSKYBOX_H

#include "typedefine.h"
#include "../Source/CTexture.h"


class CSky
{
protected:
	LPDIRECT3DDEVICE9       m_device;        //�豸ָ��	
	LPDIRECT3DVERTEXBUFFER9 m_VertexBuffer;  //��պж��㻺��	

    D3DXVECTOR3             m_position;	     //λ��
	bool                    m_render;        //�Ƿ�����Ⱦ

public:
	CSky();
	virtual ~CSky();
public:
	virtual bool Check() = 0;
	virtual void ShutDown() = 0;

	virtual bool Initialize(LPDIRECT3DDEVICE9 device) = 0;
	
	virtual bool GetRender();
	virtual void SetRender(bool render); 
	
	virtual D3DXVECTOR3 GetPosition();
	virtual void SetPosition(D3DXVECTOR3 position);
	virtual void SetPosition(float posX, float posY,float posZ);

	virtual void Render() = 0;
};


typedef enum{
	SF_FRONT = 0,
	SF_BACK,
	SF_LEFT,
	SF_RIGHT,
	SF_TOP,
	SF_BOTTOM
} SKY_FACE;		//��������պе�������


/*	   ��������պж���ͼʾ
			4
             ---------------------7
	        /|				    /|
		   / |				   / |
	      /	 |				  /	 |
pos��� 0/---|---------------/3	 |
		 |	 |5				 |	 |
		 |	 |---------------|---|6
		 |	/				 |	/
		 | /				 | /
	   1 |/------------------|/	2
*/


#define SKY_FACE_COUNT    6
#define SKY_VERTEX_COUNT  8
#define SKY_XYZ_TEX1 (D3DFVF_XYZ| D3DFVF_TEX1 )	  
typedef struct _Sky_Vertex
{
	D3DXVECTOR3 m_pos;
	float tu, tv;

public:
	_Sky_Vertex()
	{
		m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		tu = tu = 0.0f;
	}
}Sky_Vertex;
class CSkyBox: public CSky
{
protected:


protected:
	Sky_Vertex      m_vertex[SKY_VERTEX_COUNT];     //������İ˸�����
	LPDIRECT3DTEXTURE9  m_texture[SKY_FACE_COUNT];  //��������ͼƬ
	float m_width, m_length, m_height;

public:
	CSkyBox();
	~CSkyBox();

protected:
	void CalCoor();  //����λ�ü��趨�Ĵ�С���㶥������꣬�����ö����u v����
public:
	virtual bool Check();
	virtual void ShutDown();

	virtual bool Initialize(LPDIRECT3DDEVICE9 device);
    virtual void SetSize(float width, float height, float length);	
	virtual void SetFace(SKY_FACE sky_face, char * filename);	   //�����������������
	virtual void Render();
};


 //�򵥵�����
/*	
  1 ------------- 3
	|			|
	|			|
	|			|
	|			|
  0	|-----------| 2
  */
class CGround   
{
protected:	  
	LPDIRECT3DDEVICE9       m_device;        //�豸ָ��	
	LPDIRECT3DVERTEXBUFFER9 m_VertexBuffer;  //�춥�㻺��	
	LPDIRECT3DTEXTURE9      m_texture;
	Sky_Vertex      m_vertex[4];   

	bool                    m_render;        //�Ƿ�����Ⱦ
	float m_xPos, m_yPos, m_zPos;
	float m_Width, m_Length;

public:
	CGround();
	virtual ~CGround();

public:
	virtual void ShutDown();

	virtual bool Initialize(LPDIRECT3DDEVICE9 device, D3DXVECTOR3 pos, D3DXVECTOR2 size);
	virtual void SetTexture(char * filename);
	virtual void Render();
};
#endif