/*************************************************************
****DemoEx���湫������
****@���ߣ�������(BruceKen)
****@Date��2012-12-10
*************************************************************/

#ifndef  _CBILLBOARD_H
#define  _CBILLBOARD_H

#include "../DexBase/typedefine.h"
#include "CTexture.h"

/*  
���㼰λ��ʾ�⣺
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
	//D3DXCOLOR                m_transColor;  //͸��ɫ
	stBVertex                m_vertexs[4]; //4������
	bool                     m_render;  //�Ƿ�����Ⱦ
	float                    m_xScale;
	float                    m_yScale;
	float                    m_yRotate;  //��Y����ת�ĽǶ�
	float                    m_width;   //���
	float                    m_height;  //�߶�
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
	virtual void  SetScale(float scale);  //��������
	virtual float GetXScale();
	virtual float GetYScale();

	//���
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


//��������ָ����ⲿ���룬������ڲ��½��Ļ������������ʱ�򣬽��ķѴ������ڴ�
class CBillBoardAni: public CBillboard
{//���ж�֡�Ķ�̬billboard
protected:
	enum
	{
		MUL, //��֡ͼƬλ��һ��ͼ��
		ONE, //��֡ͼƬ�ɶ���ͼƬ���
	}; 
	CDexTex* m_tex;
	bool  m_changeByCamera;  //�Ƿ����������λ�õĸı���ı䣬���Ƿ�ʼ����������
	short m_type;  //���ͣ�ȡֵ��ö��
	short m_xFrame;  //������ͬһ��ͼƬ�У�����֡��
	short m_yRows;    //�ж�����
	short m_frames;	  //��֡��
	short m_currFrame;  //��ǰ֡

	int   m_aniSpeed;	  //�����ٶ�
	int   m_currCount;	  //��ǰ����

	D3DXVECTOR3 m_dir;  //�����������������ı䣬��ô�洢����λ��


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
	virtual void SetFrame(int frames);     //������֡��
	virtual void SetFrameX(int xFrame);		
	virtual void SetFrame(int total_frame, int xframe); 
	virtual void SetDir(D3DXVECTOR3 dir);
	virtual void SetDir(float x, float y , float z);
	virtual void SetSpeed(int speed);
};
#endif