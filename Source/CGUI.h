/*************************************************************
****DemoEx引擎GUI处理文件CGUI.h
****简介：CGui抽象基类
    从中派生出文本类，图片类，按钮类
****作者：阚正杰(BruceKen)
****日期：2012-7-21
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


#define     CGUI_TEXTCOLOR     DexColor(0.0f ,0.0f ,1.0f)        //文字GUI默认字体颜色
#define     CGUI_TEXTWIDTH     1280							   //文字GUI默认宽度
#define     CGUI_TEXTHEIGHT    768							   //文件GUI默认高度
#define     CGUI_TEXTBYTE      128                             //字体GUI最大字节数

typedef   BYTE      GUI_KEYWORD ;                  //GUI控件关键字,共8位,可表示256种GUI类型
const GUI_KEYWORD    GK_Flag      = 255;           //控件保留字
const GUI_KEYWORD    GK_BaseGui   = 0  ;           //基类Gui
const GUI_KEYWORD    GK_TextGui   = 1  ;           //文本Gui
const GUI_KEYWORD    GK_PicGui    = 2  ;           //图片Gui
const GUI_KEYWORD    GK_ButtonGui = 3  ;           //按钮Gui
const GUI_KEYWORD    GK_ButtonText= 4  ;           //文本按钮Gui
const GUI_KEYWORD    GK_PicMvGui= 5  ;             //移动图片Gui

typedef   BYTE      MouseFlag;                  //GUI中OnMouse的参数传入属性
const MouseFlag      MF_Flag         = 0xFF;    //无信息，保留字段
const MouseFlag      MF_MouseMove    = 0x01;    //传入鼠标移动信息
const MouseFlag      MF_MouseLDown   = 0x02;    //传入鼠标左键单击击下信息
const MouseFlag      MF_MouseLUp     = 0x03;    //传入鼠标左键单击弹起信息
const MouseFlag      MF_MouseRUp     = 0x04;    //传入鼠标右键单击弹起信息

typedef   TSHORT    GuiReturnValue;  	         //交互性GUI返回值,根据返回不同的值进行不同的处理
const GuiReturnValue  GRV_Flag       = 0x0FFF;   //返回值,保留
const GuiReturnValue  GRV_ButtonOn   = 0x0000;   //返回值,鼠标第一次移动到按钮上
const GuiReturnValue  GRV_ButtonDown = 0x0001;   //返回值,鼠标在按钮上按下
const GuiReturnValue  GRV_ButtonUp   = 0x0002;   //返回值,鼠标在按钮上弹起
const GuiReturnValue  GRV_ButtonDownUseless = 0x0003;   //返回值,鼠标在按钮上无效按下

typedef   BYTE   ButtonState;	        //按钮状态
const ButtonState  BS_NORMAL  = 0;      //一般状态
const ButtonState  BS_ON      = 1;	    //凸起状态
const ButtonState  BS_DOWN    = 2;	    //按下状态
const ButtonState  BS_USELESS = 3;	    //无效状态


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
//CGui:GUI基类
class CGui:public DexObject// CListener
{
	//TODO,在基类中加入声音文件等成员变量，用声音单例类进行播放
	//在相应事件时根据声音bool标识来决定是否要播放音效文件

	//TODO:在gui上可以挂接子控件
	//TODO：从xml配置文件中加载UI面板控件，里面包括各种具体UI所需的位置信息，声音文件信息
protected:
	TSHORT      m_iXPos, m_iYPos;		         //GUI在屏幕上的坐标
	TSHORT       m_iWidth;
	TSHORT       m_iHeight;
	bool        m_bCommunicate;                  //GUI是否可以交互,从此类派生出的按钮类将可以和鼠标交互
	GUI_KEYWORD m_keyword;                       //GUI关键字
	TSHORT      m_iId;                           //GUI编号
	bool        m_render;                        //是否渲染
	bool        m_active;                        //是否有效，和m_render并不冲突(有效时可能不渲染，渲染时也可能无效)
	//TSHORT m_iWidth, m_iHeight;			//GUI宽、高
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
	virtual void LoadIni(char * filename) {};	     //从配置文件中加载数据
	virtual bool OnEvent(stEvent event);
	virtual void ModifyFlag(int i_o, int flag);
	virtual bool EventValid(stEvent event){ return false;};//判断事件是否有效，如鼠标点击事件发生后，如果鼠标坐标落在GUI之内则有效，否则无效

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
//CTextGui:字体GUI类
class CTextGui:public CGui
{
protected:
	char *    m_pText;		   //text
	CMyFont*  m_pFont;	       //该Gui对应的字体指针
	DexColor  m_crTextColor;   //文字颜色
												
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
//CPictureGui:图片GUI类
class CPictureGui:public CGui
{
protected:
	stTexture    m_stTexture;
	LPD3DXSPRITE m_pSprite ;

    D3DXMATRIX         m_mat;			      //变换矩阵
	D3DXVECTOR2        m_scale;
	D3DXVECTOR2        m_center;
public:
	CPictureGui();
	CPictureGui(LPDIRECT3DDEVICE9 pD3DDevice, char *filename, const DexColor& TransColor = NULL);	//最后一个参数为NULL表示不选择透明色
	virtual ~CPictureGui();

protected:
	void CaculateMatrix();     //GUI的位置若是改变，则调用此方法重新计算图片的变换矩阵
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
//CPicMvGui:具有移动状态的图片GUI
enum
{
CPICMVGUI_FLAG,
CPICMVGUI_ONCE = 1,   //一次移动到位
CPICMVGUI_CYCLE = 2,  //到位后从头循环
};
class CPicMvGui:public CGui
{
protected:
	LPDIRECT3DDEVICE9 m_device;
	CTextureMove* m_tex;
	int           m_flag;  //动态方式        

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

	//virtual void Reset(); //重置到初始状态

	//virtual bool GetComplete();
};




//=============================================================================
//CButtonGui:按钮GUI类(派生自图片GUI类)
class CButtonGui:public CPictureGui
{
protected:
	ButtonState m_bsState;

public:
	CButtonGui();
	CButtonGui(LPDIRECT3DDEVICE9 pD3DDevice, char *filename, DexColor TransColor = NULL);
	virtual ~CButtonGui();

public:
	virtual void ShutDown();	 /*直接沿用父类ShutDown即可*/
	virtual GuiReturnValue OnMouse(TSHORT x, TSHORT y, MouseFlag f);
	virtual void Render();
	virtual bool OnEvent(stEvent event);
	virtual bool EventValid(stEvent event);

public:
	virtual void        HaveInvalide();   //表面一个按钮含有无效状态 
    virtual	ButtonState GetState()                  {return m_bsState;};
	virtual void        SetState(ButtonState state) { m_bsState = state;};

};

//文字按钮类
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
//CGuiSystem:系统GUI类
class CGuiSystem
{
protected:
	TFontVector	m_vFont;             //Gui系统字体容器,其中第一个是默认字体
	TGuiVector  m_vGuis;			 //GUI向量
	std::map<int, CGui*> m_vGuiUnique;   //不批量渲染的GUI列表
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
	void SetUniqueId(int id);//设置该id对应的GUI不渲染
	void Render(int id);     //渲染该id对应的GUI
};



#endif