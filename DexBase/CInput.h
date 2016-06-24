/*************************************************************
****DemoEx���������豸������
****��飺��ʱֻ֧�ּ������
****���ߣ�������(BruceKen)
****���ڣ�2012-7-21
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
	MMD_LEFT,	       //��
	MMD_RIGHT,	       //��
	MMD_UP,			   //��
	MMD_DOWN,          //��
	MMD_LEFT_UP,       //����
	MMD_LEFT_DOWN,	   //����
	MMD_RIGHT_UP,	   //����
	MMD_RIGHT_DOWN	   //����
};
class CMouse:public CEventSender
{
protected:
	  LPDIRECTINPUTDEVICE8 m_device;
      DIMOUSESTATE m_mouseState;
      DIMOUSESTATE m_oldMouseState;
	 
	  //������豸�е�λ��
	  long m_xDPrePos;  //�����һ�̵�λ��
	  long m_yDPrePos;
	  long m_xDPos;   
	  long m_yDPos; 

	  //�������Ļ�ϵ�λ��
	  long m_xPrePos;  //�����һ�̵�λ��
	  long m_yPrePos;
      long m_xPos;
      long m_yPos;
	  int  m_iDeltaX;
	  int  m_iDeltaY;

      long m_zPos;     //����
private:
	  void ShutDown();
public:
	  CMouse(LPDIRECTINPUT8 input, HWND hwnd, bool exclusive = false);
	  ~CMouse();

	  void Update(HWND hwnd);
	  virtual void NotifyEvent(stEvent event); //���ط�����ֻ����������¼���ʱ���ٽ����¼�֪ͨ
	  
	  //��������Ļ������
	  long GetXPosition() { return m_xPos;};
	  long GetYPosition() { return m_yPos;};

	  //����豸�е��������
	  long GetXDPosition() { return m_xDPos;};
	  long GetYDPosition() { return m_yDPos;};

	  bool WheelFront();   //����ǰ��
	  bool WheelBack();	   //���ֺ��

	  bool MouseMove(Mouse_Move_Dir & mmd, int flag = 0);    //����Ƿ��ƶ���0:��Ļ�ϵ����	 1:�豸�е����

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
	bool WheelFront();   //����ǰ��
	bool WheelBack();	   //���ֺ��
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