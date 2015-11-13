/*************************************************************
****DemoEx引擎自定义字体类
****作者：阚正杰(BruceKen)
    注：类的主要架构同2D引擎Demo的Font类，待完善
*************************************************************/


#ifndef _CMYFONT_H
#define _CMYFONT_H


#define     CMYFONT_FONTWIDTH     1280							 //字体默认输出宽度
#define     CMYFONT_FONTHEIGHT    768							 //字体默认输出高度

#include "../DexBase/stdafx.h"
#include "../DexBase/typedefine.h"



class CMyFont
{
protected:
	LPDIRECT3DDEVICE9 m_device;
	LPD3DXFONT     m_Font;			 //字体对象
	int            m_textSize;           //字体大小
	D3DXFONT_DESCW m_stFont;         //字体结构
	DexColor       m_crFontColor;	 //字体颜色argb
	DexColor       m_useColor;       //暂时另用别的颜色输出
	bool           m_bUseColor;
	RECT           m_Rect;			 //字体的输出矩形
	bool           m_bCenter;	     //居中输出`
	TSHORT         m_iId;			 //字体编号

protected:
	void ResetFont();
public:
	CMyFont(LPDIRECT3DDEVICE9 pD3DDevice);
	~CMyFont();
	void ShutDown();

public:
	DexColor GetColor()   { return m_crFontColor ;};
	TSHORT   GetId()      { return m_iId;};

	void     UseColor(DexColor color) { m_useColor = color; m_bUseColor = true;};
	void     SetColor(DexColor color) { m_crFontColor = color;};
	void     ResetColor(){ m_bUseColor = false;};
	void     SetId(TSHORT id)                  { m_iId = id;};
	void     SetPos(int x, int y);
	void     SetSize(int w, int h);
	void     SetTextSize(int size);	  //设置字体大小
	void     SetRect(RECT rect);  //设置字体显示区域的位置信息
	void     SetRect(int x, int y, int w, int h);
	RECT     GetRect()    { return m_Rect;};
	void     Print(char * ch,...);
};



#endif