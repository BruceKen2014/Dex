/*************************************************************
****DemoEx�����Զ���������
****���ߣ�������(BruceKen)
    ע�������Ҫ�ܹ�ͬ2D����Demo��Font�࣬������
*************************************************************/


#ifndef _CMYFONT_H
#define _CMYFONT_H


#define     CMYFONT_FONTWIDTH     1280							 //����Ĭ��������
#define     CMYFONT_FONTHEIGHT    768							 //����Ĭ������߶�

#include "../DexBase/stdafx.h"
#include "../DexBase/typedefine.h"



class CMyFont
{
protected:
	LPDIRECT3DDEVICE9 m_device;
	LPD3DXFONT     m_Font;			 //�������
	int            m_textSize;           //�����С
	D3DXFONT_DESCW m_stFont;         //����ṹ
	DexColor       m_crFontColor;	 //������ɫargb
	DexColor       m_useColor;       //��ʱ���ñ����ɫ���
	bool           m_bUseColor;
	RECT           m_Rect;			 //������������
	bool           m_bCenter;	     //�������`
	TSHORT         m_iId;			 //������

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
	void     SetTextSize(int size);	  //���������С
	void     SetRect(RECT rect);  //����������ʾ�����λ����Ϣ
	void     SetRect(int x, int y, int w, int h);
	RECT     GetRect()    { return m_Rect;};
	void     Print(char * ch,...);
};



#endif