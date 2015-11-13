/*************************************************************
****DemoEx引擎天空盒类CSkyBox.h
****简介：CSky     天空盒基类
          CSkyBox  立方体天空盒类 派生于 CSky
		  日后可从CSky再扩展球形天空类及附加各种效果
****@作者：阚正杰(BruceKen)
****@日期:2012-11-16 
*************************************************************/

#ifndef  _CSKYBOX_H
#define  _CSKYBOX_H

#include "typedefine.h"
#include "../Source/CTexture.h"


class CSky
{
protected:
	LPDIRECT3DDEVICE9       m_device;        //设备指针	
	LPDIRECT3DVERTEXBUFFER9 m_VertexBuffer;  //天空盒顶点缓存	

    D3DXVECTOR3             m_position;	     //位置
	bool                    m_render;        //是否开启渲染

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
} SKY_FACE;		//立方体天空盒的六个面


/*	   立方体天空盒顶点图示
			4
             ---------------------7
	        /|				    /|
		   / |				   / |
	      /	 |				  /	 |
pos起点 0/---|---------------/3	 |
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
	Sky_Vertex      m_vertex[SKY_VERTEX_COUNT];     //立方体的八个顶点
	LPDIRECT3DTEXTURE9  m_texture[SKY_FACE_COUNT];  //六幅纹理图片
	float m_width, m_length, m_height;

public:
	CSkyBox();
	~CSkyBox();

protected:
	void CalCoor();  //根据位置及设定的大小计算顶点的坐标，并设置顶点的u v坐标
public:
	virtual bool Check();
	virtual void ShutDown();

	virtual bool Initialize(LPDIRECT3DDEVICE9 device);
    virtual void SetSize(float width, float height, float length);	
	virtual void SetFace(SKY_FACE sky_face, char * filename);	   //设置立方体的六个面
	virtual void Render();
};


 //简单地面类
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
	LPDIRECT3DDEVICE9       m_device;        //设备指针	
	LPDIRECT3DVERTEXBUFFER9 m_VertexBuffer;  //天顶点缓存	
	LPDIRECT3DTEXTURE9      m_texture;
	Sky_Vertex      m_vertex[4];   

	bool                    m_render;        //是否开启渲染
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