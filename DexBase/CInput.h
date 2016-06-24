/*************************************************************
****DemoEx引擎输入设备处理类
****简介：暂时只支持键盘鼠标
****作者：阚正杰(BruceKen)
****日期：2012-7-21
*************************************************************/



#ifndef _CINPUT_H
#define _CINPUT_H


#define      CINPUT_KEY_SIZE       256

#include "typedefine.h"
#include "CEventSender.h"


//=============================================================================
//----------------------------------------------------------
class CKeyboard:public CEventSender
{
protected:
	  LPDIRECTINPUTDEVICE8 m_device;
	  char m_keys[CINPUT_KEY_SIZE];
	  char m_oldKeys[CINPUT_KEY_SIZE];
private:
	  void ShutDown();
public:
	  CKeyboard(LPDIRECTINPUT8 input, HWND hwnd);
	  ~CKeyboard();

	  void Update();
	  BOOL KeyUp(unsigned int key);
	  BOOL KeyDown(unsigned int key);

	  int  KeyDown();
	  int  KeyUp();
};

//----------------------------------------------------------
typedef enum Mouse_Move_Dir
{
	MMD_LEFT,	       //左
	MMD_RIGHT,	       //右
	MMD_UP,			   //上
	MMD_DOWN,          //下
	MMD_LEFT_UP,       //左上
	MMD_LEFT_DOWN,	   //左下
	MMD_RIGHT_UP,	   //右上
	MMD_RIGHT_DOWN	   //右下
};
class CMouse:public CEventSender
{
protected:
	  LPDIRECTINPUTDEVICE8 m_device;
      DIMOUSESTATE m_mouseState;
      DIMOUSESTATE m_oldMouseState;
	 
	  //鼠标在设备中的位置
	  long m_xDPrePos;  //鼠标上一刻的位置
	  long m_yDPrePos;
	  long m_xDPos;   
	  long m_yDPos; 

	  //鼠标在屏幕上的位置
	  long m_xPrePos;  //鼠标上一刻的位置
	  long m_yPrePos;
      long m_xPos;
      long m_yPos;
	  int  m_iDeltaX;
	  int  m_iDeltaY;

      long m_zPos;     //滚轮
private:
	  void ShutDown();
public:
	  CMouse(LPDIRECTINPUT8 input, HWND hwnd, bool exclusive = false);
	  ~CMouse();

	  void Update(HWND hwnd);
	  virtual void NotifyEvent(stEvent event); //重载方法，只有在有鼠标事件的时候再进行事件通知
	  
	  //获得鼠标屏幕的坐标
	  long GetXPosition() { return m_xPos;};
	  long GetYPosition() { return m_yPos;};

	  //获得设备中的鼠标坐标
	  long GetXDPosition() { return m_xDPos;};
	  long GetYDPosition() { return m_yDPos;};

	  bool WheelFront();   //滚轮前滚
	  bool WheelBack();	   //滚轮后滚

	  bool MouseMove(Mouse_Move_Dir & mmd, int flag = 0);    //鼠标是否移动，0:屏幕上的鼠标	 1:设备中的鼠标

	  BOOL LeftButtonDown();
	  BOOL RightButtonDown();
  	  BOOL MiddleButtonDown(); 
	  BOOL LeftButtonUp();
	  BOOL RightButtonUp();
  	  BOOL MiddleButtonUp();

	  bool MouseLDrag();
	  int  getDeltaX();
	  int  getDeltaY();

	  virtual void ShowCursor(bool show){};

	  void SetXPos(int x, int y)  {m_mouseState.lX = x , m_mouseState.lY = y; m_oldMouseState.lX = x; m_oldMouseState.lY = y;};
};
inline bool CMouse::WheelFront()
{
	if(m_zPos > 0)
	{
		//m_zPos = 0;
		return true;
	}
	return false;
}
inline bool CMouse::WheelBack()
{
	if(m_zPos < 0)
	{
		//m_zPos = 0;
		return true;
	}
	return false;
}
//=============================================================================


class CInputSystem
{
protected:
    LPDIRECTINPUT8 m_system;
    CKeyboard *   m_keyboard;
    CMouse *      m_mouse;

public:
	static bool m_bExclusive;
private:
	void ShutDown();
private:
	CInputSystem();
	~CInputSystem();

public:
	static CInputSystem& GetInputSingleton()
	{
		static CInputSystem g_InputSystem;
		return g_InputSystem; 
	}
	void Update(HWND hwnd);
	BOOL KeyUp(unsigned int key);
	BOOL KeyDown(unsigned int key);
	long GetMouseXPos() ;
	long GetMouseYPos() ;
	long GetMouseXDPos();
	long GetMouseYDPos();
	bool WheelFront();   //滚轮前滚
	bool WheelBack();	   //滚轮后滚
	bool MouseMove(Mouse_Move_Dir &mmd, int flag = 0);
	BOOL LeftButtonDown();
	BOOL RightButtonDown();
  	BOOL MiddleButtonDown(); 
	BOOL LeftButtonUp();
	BOOL RightButtonUp();
  	BOOL MiddleButtonUp();
	int  MouseDeltaX();
	int  MouseDeltaY();
public:
	void RegisterMouseListener(CListener* listener)
	{
		DEX_ENSURE(m_mouse);
		m_mouse->AddListener(listener);
	}
	void UnRegisterMouseListener(CListener* listener)
	{
		DEX_ENSURE(m_mouse);
		m_mouse->RemoveListener(listener);
	}
	void RegisterKeyboradListener(CListener* listener)
	{
		DEX_ENSURE(m_keyboard);
		m_keyboard->AddListener(listener);
	}
	void UnRegisterKeyboardListener(CListener* listener)
	{
		DEX_ENSURE(m_keyboard);
		m_keyboard->RemoveListener(listener);
	}
};




#endif