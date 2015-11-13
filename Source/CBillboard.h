/*************************************************************
****DemoEx引擎公告牌类
****@作者：阚正杰(BruceKen)
****@Date：2012-12-10
*************************************************************/

#ifndef  _CBILLBOARD_H
#define  _CBILLBOARD_H

#include "../DexBase/typedefine.h"
#include "CTexture.h"

/*  
顶点及位置示意：
  --------------
  | 0          | 1
  |			   |
  | 2          |
  |----pos-----| 3
       
*/
#define  BILLBOARD_FVF  (D3DFVF_XYZ | D3DFVF_DIFFUSE| D3DFVF_TEX1)	 
typedef struct _stBVertex
	{
		D3DXVECTOR3 m_pos;
		unsigned long   m_color;
		float m_u, m_v;
		//_stBVertex()
		//{
		//	m_pos = D3DXVECTOR3(0,0,0);
		//	m_color = D3DXCOLOR(255, 255, 255, 255);
		//	m_u = 0.0f;
		//	m_v = 0.0f;
		//}
	} stBVertex; 

class CBillboard
{
protected:
	LPDIRECT3DDEVICE9        m_device;
	LPDIRECT3DVERTEXBUFFER9  m_vertexBuffer;
	D3DXMATRIX               m_matrix;
	LPDIRECT3DTEXTURE9       m_texture;
	D3DXVECTOR3              m_pos;	
	//D3DXCOLOR                m_transColor;  //透明色
	stBVertex                m_vertexs[4]; //4个顶点
	bool                     m_render;  //是否开启渲染
	float                    m_xScale;
	float                    m_yScale;
	float                    m_yRotate;  //绕Y轴旋转的角度
	float                    m_width;   //宽度
	float                    m_height;  //高度
	float                    m_alpha;	
	
public:
	CBillboard();
	CBillboard(LPDIRECT3DDEVICE9 device);
	virtual ~CBillboard();

protected:
	virtual void SetMatrix();
	virtual void UpdateVertex();
public:	 
	virtual void CreateVertex();
	virtual void ShutDown();
	virtual void Update(D3DXVECTOR3 camera_pos);
	virtual void Render();

public:
	//texture
	virtual bool  LoadTexture(char *filename);
	//alpha
	virtual void  SetAlpha(float alpha);
	virtual float GetAlpha();

	virtual void  SetRender(bool _render);
	virtual bool  GetRender();

	//scale
	virtual void  SetXScale(float scale);
	virtual void  SetYScale(float scale);
	virtual void  SetScale(float xScale, float yScale);
	virtual void  SetScale(D3DXVECTOR2 scale);
	virtual void  SetScale(float scale);  //整体缩放
	virtual float GetXScale();
	virtual float GetYScale();

	//宽高
	virtual void  SetWidth(float width);
	virtual void  SetHeight(float height);
	virtual void  SetSize(float width, float height);
	virtual void  SetSize(D3DXVECTOR2 size);
	virtual void  SetSize(float size);
	virtual float GetWidth();
	virtual float GetHeight();

	//pos
	virtual void  SetPos(float xPos, float yPos, float zPos);
	virtual void  SetPos(D3DXVECTOR3 pos);
	virtual D3DXVECTOR3 GetPos();
};


//这里纹理指针从外部传入，如果在内部新建的话，当数量多的时候，将耗费大量的内存
class CBillBoardAni: public CBillboard
{//具有多帧的动态billboard
protected:
	enum
	{
		MUL, //多帧图片位于一张图中
		ONE, //多帧图片由多张图片组成
	}; 
	CDexTex* m_tex;
	bool  m_changeByCamera;  //是否随着摄像机位置的改变而改变，即是否始终面对摄像机
	short m_type;  //类型，取值见枚举
	short m_xFrame;  //对于在同一张图片中，横向帧数
	short m_yRows;    //有多少行
	short m_frames;	  //总帧数
	short m_currFrame;  //当前帧

	int   m_aniSpeed;	  //动画速度
	int   m_currCount;	  //当前计数

	D3DXVECTOR3 m_dir;  //如果不随着摄像机而改变，那么存储朝向位置


public:
	CBillBoardAni();
	CBillBoardAni(LPDIRECT3DDEVICE9 device);
	virtual ~CBillBoardAni();

protected:
//	virtual void SetMatrix();
	virtual void UpdateVertex();
public:	
	virtual void CreateVertex();
	virtual void ShutDown();
	virtual void Update(D3DXVECTOR3 camera_pos);
	virtual void Render();

public:
	virtual void SetTex(CDexTex* tex);
	virtual void SetChangeByCamera(bool b);
	virtual void SetFrame(int frames);     //设置总帧数
	virtual void SetFrameX(int xFrame);		
	virtual void SetFrame(int total_frame, int xframe); 
	virtual void SetDir(D3DXVECTOR3 dir);
	virtual void SetDir(float x, float y , float z);
	virtual void SetSpeed(int speed);
};
#endif