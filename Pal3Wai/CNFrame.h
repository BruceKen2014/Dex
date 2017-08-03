/*************************************************************
****DemoEx����Ի���ͼƬ��
****���ߣ�BruceKen
2012.12
*************************************************************/

#pragma  once

#include "../DexBase/typedefine.h"
#include "../Source/CTexture.h"

enum
{
	TOP_LEFT,  //����
	TOP,       //��
	TOP_RIGHT, //����
	LEFT,	   //��
	CENTER,	   //��
	RIGHT,		//��
	BOTTOM_LEFT,  //����
	BOTTOM,		  //��
	BOTTOM_RIGHT  //����
};
#define PIC_COUNT    9
class CNFrame
{
protected:

protected:
	LPDIRECT3DDEVICE9 m_device;
	CTextureEx* m_pic[PIC_COUNT];    //9��ͼƬ
	DInt16 m_width;	   //������
	DInt16 m_height;
	DInt16 m_posX;	   //���Ƶ���ʼ��
	DInt16 m_posY;
	float m_horizonScale; //��������(�ϡ��С�������ͼ)
	float m_verticalScale;//��������(�����С�������ͼ)
	bool  m_render;
	bool  m_successRes;//��Դ״̬�Ƿ�����

public:
	CNFrame();
	CNFrame(LPDIRECT3DDEVICE9 device);
	~CNFrame();
protected:
	virtual bool CheckRes();
	virtual bool Check();
public:
	virtual void ShutDown();
	virtual void Draw(); 
	virtual bool LoadTexture(DInt16 index, char* filename);	

	virtual void SetPos(DInt16 x, DInt16 y);
	virtual D3DXVECTOR2 GetPos();
	
	virtual void SetSize(DInt16 width, DInt16 height);
	virtual D3DXVECTOR2 GetSize();
	
	virtual void SetRender(bool b);
	virtual bool GetRender();
};