/*************************************************************
****DemoEx����GUI�����ļ�CGUI.h
****��飺CGui�������
    �����������ı��࣬ͼƬ�࣬��ť��
****���ߣ�������(BruceKen)
****���ڣ�2012-7-21
*************************************************************/

#ifndef _CGUI_H
#define _CGUI_H

#include <iostream>
#include <vector>
//#include "CFunction.h"
#include "../DexBase/typedefine.h"
#include "../DexBase/CInput.h"
#include "../DexBase/CDelegate.h"
#include "../DexBase/CListener.h"
#include "../DexBase/DexObject.h"

#include "CTextureMove.h"
#include "CMyFont.h"
#include <map>

#include "CCommandScript.h"


#define     CGUI_TEXTCOLOR     DexColor(0.0f ,0.0f ,1.0f)        //����GUIĬ��������ɫ
#define     CGUI_TEXTWIDTH     1280							   //����GUIĬ�Ͽ��
#define     CGUI_TEXTHEIGHT    768							   //�ļ�GUIĬ�ϸ߶�
#define     CGUI_TEXTBYTE      128                             //����GUI����ֽ���

typedef   BYTE      GUI_KEYWORD ;                  //GUI�ؼ��ؼ���,��8λ,�ɱ�ʾ256��GUI����
const GUI_KEYWORD    GK_Flag      = 255;           //�ؼ�������
const GUI_KEYWORD    GK_BaseGui   = 0  ;           //����Gui
const GUI_KEYWORD    GK_TextGui   = 1  ;           //�ı�Gui
const GUI_KEYWORD    GK_PicGui    = 2  ;           //ͼƬGui
const GUI_KEYWORD    GK_ButtonGui = 3  ;           //��ťGui
const GUI_KEYWORD    GK_ButtonText= 4  ;           //�ı���ťGui
const GUI_KEYWORD    GK_PicMvGui= 5  ;             //�ƶ�ͼƬGui

typedef   BYTE      MouseFlag;                  //GUI��OnMouse�Ĳ�����������
const MouseFlag      MF_Flag         = 0xFF;    //����Ϣ�������ֶ�
const MouseFlag      MF_MouseMove    = 0x01;    //��������ƶ���Ϣ
const MouseFlag      MF_MouseLDown   = 0x02;    //��������������������Ϣ
const MouseFlag      MF_MouseLUp     = 0x03;    //��������������������Ϣ
const MouseFlag      MF_MouseRUp     = 0x04;    //��������Ҽ�����������Ϣ

typedef   TSHORT    GuiReturnValue;  	         //������GUI����ֵ,���ݷ��ز�ͬ��ֵ���в�ͬ�Ĵ���
const GuiReturnValue  GRV_Flag       = 0x0FFF;   //����ֵ,����
const GuiReturnValue  GRV_ButtonOn   = 0x0000;   //����ֵ,����һ���ƶ�����ť��
const GuiReturnValue  GRV_ButtonDown = 0x0001;   //����ֵ,����ڰ�ť�ϰ���
const GuiReturnValue  GRV_ButtonUp   = 0x0002;   //����ֵ,����ڰ�ť�ϵ���
const GuiReturnValue  GRV_ButtonDownUseless = 0x0003;   //����ֵ,����ڰ�ť����Ч����

typedef   BYTE   ButtonState;	        //��ť״̬
const ButtonState  BS_NORMAL  = 0;      //һ��״̬
const ButtonState  BS_ON      = 1;	    //͹��״̬
const ButtonState  BS_DOWN    = 2;	    //����״̬
const ButtonState  BS_USELESS = 3;	    //��Ч״̬


enum
{
	mouse_l_down = 1 <<0,
	mouse_l_up   = 1 <<1,
	mouse_r_down = 1 <<2,
	mouse_r_up   = 1 <<3,
	mouse_move   = 1 <<4,
	mouse_l_drag = 1 <<5,
	mouse_r_drag = 1 <<6,


	mouse_none   = 0,
};


//=============================================================================
//CGui:GUI����
class CGui:public DexObject// CListener
{
	//TODO,�ڻ����м��������ļ��ȳ�Ա��������������������в���
	//����Ӧ�¼�ʱ��������bool��ʶ�������Ƿ�Ҫ������Ч�ļ�

	//TODO:��gui�Ͽ��Թҽ��ӿؼ�
	//TODO����xml�����ļ��м���UI���ؼ�������������־���UI�����λ����Ϣ�������ļ���Ϣ
protected:
	TSHORT      m_iXPos, m_iYPos;		         //GUI����Ļ�ϵ�����
	TSHORT       m_iWidth;
	TSHORT       m_iHeight;
	bool        m_bCommunicate;                  //GUI�Ƿ���Խ���,�Ӵ����������İ�ť�ཫ���Ժ���꽻��
	GUI_KEYWORD m_keyword;                       //GUI�ؼ���
	TSHORT      m_iId;                           //GUI���
	bool        m_render;                        //�Ƿ���Ⱦ
	bool        m_active;                        //�Ƿ���Ч����m_render������ͻ(��Чʱ���ܲ���Ⱦ����ȾʱҲ������Ч)
	//TSHORT m_iWidth, m_iHeight;			//GUI����
	int         m_UiFlag;

public:	
	enum
	{
		Add_Flag,
		Minus_Flag
	};
	CEventDispather  m_MouseMove;
	CEventDispather  m_MouseLUp;
	CEventDispather  m_MouseLDown;
	CEventDispather  m_MouseRUp;
	CEventDispather  m_MouseRDown;
	CEventDispather  m_MouseLDrag;
	CEventDispather  m_MouseRDrag;
public:
	CGui();
	virtual ~CGui();

public:
	virtual GuiReturnValue OnMouse(TSHORT x, TSHORT y, MouseFlag f);
	virtual void Render() = 0;
	virtual void ShutDown() = 0;				   
	virtual void LoadIni(char * filename) {};	     //�������ļ��м�������
	virtual bool OnEvent(stEvent event);
	virtual void ModifyFlag(int i_o, int flag);
	virtual bool EventValid(stEvent event){ return false;};//�ж��¼��Ƿ���Ч����������¼���������������������GUI֮������Ч��������Ч

	virtual TSHORT GetXposition() {      return m_iXPos;};
	virtual TSHORT GetYposition() {      return m_iYPos;};
	virtual bool   GetIsCommunicate() {  return m_bCommunicate;};
	virtual TSHORT GetId()        {      return m_iId;};
	virtual GUI_KEYWORD   GetKeyword() { return m_keyword;};
	virtual bool   GetRender()    {      return m_render; }; 
	virtual bool   GetActive()    {      return m_active;};

	virtual TSHORT GetWidth()  { return m_iWidth;};
	virtual TSHORT GetHeight() { return m_iHeight;};
	
	virtual void  SetSize(TSHORT w, TSHORT h)     { m_iWidth = w;  m_iHeight = h;};
	virtual void  SetXposition(TSHORT xPosition)  {  m_iXPos = xPosition;};
	virtual void  SetYposition(TSHORT yPosition)  {  m_iYPos = yPosition;};
	virtual void  SetPosition(TSHORT x, TSHORT y) {  m_iXPos = x;  m_iYPos = y;};
	virtual void  SetIsCommunicate(bool b)		  {  m_bCommunicate = b;};
	virtual void  SetId(TSHORT id)                {  m_iId = id;};
	virtual void  SetKeyword(GUI_KEYWORD keyword) {  m_keyword = keyword;};
	virtual void  SetText(char * text)            {};
	virtual void  SetFont(CMyFont *)              {};
	virtual void  SetRender(bool render)          {  m_render = render;}
	virtual void  SetActive(bool active)          {  m_active = active;}

};

//=============================================================================
//CTextGui:����GUI��
class CTextGui:public CGui
{
protected:
	char *    m_pText;		   //text
	CMyFont*  m_pFont;	       //��Gui��Ӧ������ָ��
	DexColor  m_crTextColor;   //������ɫ
												
public:
	CTextGui();
	CTextGui(char * pText, CMyFont * pFont);
	virtual ~CTextGui();

public:
	virtual void Render();
	virtual void ShutDown();
	virtual void LoadIni(char * filename) {};

public:
	virtual void SetColor(DexColor color)      { m_crTextColor = color;};
	virtual void SetFont(CMyFont * pFont) { m_pFont = pFont ;};
	virtual void SetText(char * text);
};


//=============================================================================
//CPictureGui:ͼƬGUI��
class CPictureGui:public CGui
{
protected:
	stTexture    m_stTexture;
	LPD3DXSPRITE m_pSprite ;

    D3DXMATRIX         m_mat;			      //�任����
	D3DXVECTOR2        m_scale;
	D3DXVECTOR2        m_center;
public:
	CPictureGui();
	CPictureGui(LPDIRECT3DDEVICE9 pD3DDevice, char *filename, const DexColor& TransColor = NULL);	//���һ������ΪNULL��ʾ��ѡ��͸��ɫ
	virtual ~CPictureGui();

protected:
	void CaculateMatrix();     //GUI��λ�����Ǹı䣬����ô˷������¼���ͼƬ�ı任����
public:
	virtual void   SetXposition(TSHORT xPosition);
	virtual void   SetYposition(TSHORT yPosition);
	virtual void   SetPosition(TSHORT x, TSHORT y);

public:
	virtual void ShutDown();
    virtual void Render();
	virtual void LoadIni(char * filename){};
};
//=============================================================================
//CPicMvGui:�����ƶ�״̬��ͼƬGUI
enum
{
CPICMVGUI_FLAG,
CPICMVGUI_ONCE = 1,   //һ���ƶ���λ
CPICMVGUI_CYCLE = 2,  //��λ���ͷѭ��
};
class CPicMvGui:public CGui
{
protected:
	LPDIRECT3DDEVICE9 m_device;
	CTextureMove* m_tex;
	int           m_flag;  //��̬��ʽ        

public:
	CPicMvGui(LPDIRECT3DDEVICE9 pD3DDevice);
	virtual ~CPicMvGui();

public:
	virtual void ShutDown();
	virtual void Render();   

	virtual bool LoadTexture(char* filename);
	virtual void SetStartPos(TSHORT x, TSHORT y);
	virtual void SetEndPos(TSHORT x, TSHORT y);
	virtual void SetSpeed(float speed);
	virtual void SetFlag(int flag);

	//virtual void Reset(); //���õ���ʼ״̬

	//virtual bool GetComplete();
};




//=============================================================================
//CButtonGui:��ťGUI��(������ͼƬGUI��)
class CButtonGui:public CPictureGui
{
protected:
	ButtonState m_bsState;

public:
	CButtonGui();
	CButtonGui(LPDIRECT3DDEVICE9 pD3DDevice, char *filename, DexColor TransColor = NULL);
	virtual ~CButtonGui();

public:
	virtual void ShutDown();	 /*ֱ�����ø���ShutDown����*/
	virtual GuiReturnValue OnMouse(TSHORT x, TSHORT y, MouseFlag f);
	virtual void Render();
	virtual bool OnEvent(stEvent event);
	virtual bool EventValid(stEvent event);

public:
	virtual void        HaveInvalide();   //����һ����ť������Ч״̬ 
    virtual	ButtonState GetState()                  {return m_bsState;};
	virtual void        SetState(ButtonState state) { m_bsState = state;};

};

//���ְ�ť��
class CButtonText:public CTextGui
{
protected:
	ButtonState m_bsState;

public:
	CButtonText();
	CButtonText(char * pText, CMyFont * pFont);
	virtual ~CButtonText();

public:
	virtual void Render();
	virtual void ShutDown();
	virtual GuiReturnValue OnMouse(TSHORT x, TSHORT y, MouseFlag f);
};


/////////////////////////////////////////////////////////
typedef  std::vector<CGui *>   TGuiVector; 
typedef  std::vector<CMyFont *>   TFontVector; 
//typedef  std::vector<CTextGui>   TTextVector; 
////////////////////////////////////////////////////////

//=============================================================================
//CGuiSystem:ϵͳGUI��
class CGuiSystem
{
protected:
	TFontVector	m_vFont;             //Guiϵͳ��������,���е�һ����Ĭ������
	TGuiVector  m_vGuis;			 //GUI����
	std::map<int, CGui*> m_vGuiUnique;   //��������Ⱦ��GUI�б�
public:
   CGuiSystem(LPDIRECT3DDEVICE9 pD3DDevice);
   virtual ~CGuiSystem();
   virtual void ShutDown();

public:
	TGuiVector GetGuiVector()   { return m_vGuis;};


public:
	bool AddGui(CGui * Gui);
	void Render();
	void LoadFont(LPDIRECT3DDEVICE9 pD3DDevice, char * filename);
	void LoadGui(LPDIRECT3DDEVICE9 pD3DDevice, char * filename);
	void SetUniqueId(int id);//���ø�id��Ӧ��GUI����Ⱦ
	void Render(int id);     //��Ⱦ��id��Ӧ��GUI
};



#endif