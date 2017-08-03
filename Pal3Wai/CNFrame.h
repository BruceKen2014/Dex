/*************************************************************
****DemoEx引擎对话框图片类
****作者：BruceKen
2012.12
*************************************************************/

#pragma  once

#include "../DexBase/typedefine.h"
#include "../Source/CTexture.h"

enum
{
	TOP_LEFT,  //左上
	TOP,       //上
	TOP_RIGHT, //右上
	LEFT,	   //左
	CENTER,	   //中
	RIGHT,		//右
	BOTTOM_LEFT,  //坐下
	BOTTOM,		  //下
	BOTTOM_RIGHT  //右下
};
#define PIC_COUNT    9
class CNFrame
{
protected:

protected:
	LPDIRECT3DDEVICE9 m_device;
	CTextureEx* m_pic[PIC_COUNT];    //9副图片
	DInt16 m_width;	   //整体宽高
	DInt16 m_height;
	DInt16 m_posX;	   //绘制的起始点
	DInt16 m_posY;
	float m_horizonScale; //横向缩放(上、中、下三副图)
	float m_verticalScale;//竖向缩放(坐、中、右三副图)
	bool  m_render;
	bool  m_successRes;//资源状态是否正常

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