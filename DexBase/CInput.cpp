




//#include "stdafx.h"
#include "CInput.h"
//#include "CFunction.h"
#include "../widget/DexDesktop.h"
#include "../widget/DexWidget.h"
#include "../DexBase/CDexDraw2D.h"
#include "../state/DexGameEngine.h"


using namespace DexGUI;
//=====================================����=========================================

//-----------------���졢������˽�м���������--------------------------
CKeyboard::CKeyboard(LPDIRECTINPUT8 input, HWND hwnd)
{
	m_device = NULL;
    // ���ü�������
    memset(m_keys, 0, sizeof(m_keys));
	memset(m_oldKeys, 0, sizeof(m_oldKeys));
    if(input->CreateDevice(GUID_SysKeyboard, &m_device, NULL) != DI_OK)
	{
		_Message("�����豸����ʧ��!");
	}
    if(m_device->SetDataFormat(&c_dfDIKeyboard) != DI_OK)
	{
		_Message("�����������ݸ�ʽʧ��!");
	}
	if(m_device->SetCooperativeLevel(hwnd,DISCL_FOREGROUND |DISCL_NONEXCLUSIVE) != DI_OK)
	{
		_Message("�������ú����ȼ�ʧ��!");
	}		  
	m_device->Acquire();

}

CKeyboard::~CKeyboard()
{
	ShutDown();
}

void CKeyboard::ShutDown()
{
   if(m_device)
   {
      m_device->Unacquire();
      m_device->Release();
      m_device = NULL;
   }
}

//--------------------------���з���--------------------------------------

void CKeyboard::Update()
{
    if(m_device == NULL)
		return;
    //�����ϴμ���״̬
    memcpy(m_oldKeys, m_keys, sizeof(m_keys));

    //��ȡ�豸״̬ʧ�������»�ȡ
    if(FAILED(m_device->GetDeviceState(sizeof(m_keys), (LPVOID)m_keys)))
    {
       if(FAILED(m_device->Acquire())) 
	  	  return ;
       if(FAILED(m_device->GetDeviceState(sizeof(m_keys), (LPVOID)m_keys)))
          return ;
    }

	if(m_Listeners.size() != 0)
	{//������������Ϊ0���ٽ����жϣ�Ϊ��Ч�ʿ���
		for(int i = 0; i < CINPUT_KEY_SIZE; i++)
		{

		}
	}
}

BOOL CKeyboard::KeyDown(DUInt32 key)
{
	return m_keys[key] & 0x80;
}

BOOL CKeyboard::KeyUp(DUInt32 key)
{
	//��״̬�ǰ��¶���״̬���ǰ��£���˴���Ϊ�ǰ�������
	return (m_oldKeys[key] & 0x80) && !(m_keys[key] & 0x80);
}
int CKeyboard::KeyDown()
{
	//TODO
	//�Ƿ���Ҫ������ÿһ�������������ж��أ�0.0
	return -1;
}
int CKeyboard::KeyUp()
{
	return -1;
}



//=====================================���=========================================

//-----------------���졢������˽�м���������--------------------------
CMouse::CMouse(LPDIRECTINPUT8 input, HWND hwnd, bool exclusive)
{
   DWORD flags;
   memset(&m_mouseState, 0, sizeof(m_mouseState));
   memset(&m_oldMouseState, 0, sizeof(m_oldMouseState));

   m_mouseState.lX = 0;
   m_mouseState.lY = 0;

   m_xDPrePos = 0;
   m_yDPrePos = 0;
   m_xDPos = 0;
   m_yDPos = 0;
   m_iDeltaX = 0;
   m_iDeltaY = 0;

   m_xPrePos = 0;
   m_yPrePos = 0;
   m_xPos = 0;
   m_yPos = 0;
   m_zPos = 0;

   if(input->CreateDevice(GUID_SysMouse, &m_device, NULL) != DI_OK)
   {
	   _Message("����豸����ʧ��!");
   }
   if(m_device->SetDataFormat(&c_dfDIMouse) != DI_OK)
   {
	   _Message("����������ݸ�ʽʧ�ܣ�");
   }		
   if(exclusive) 
	   flags = DISCL_FOREGROUND | DISCL_EXCLUSIVE | DISCL_NOWINKEY;
   else
	   flags = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE;

   if(m_device->SetCooperativeLevel(hwnd, flags) != DI_OK)
   {
	   _Message("������ú����ȼ�ʧ�ܣ�");
   }	
   m_device->Acquire();
}

CMouse::~CMouse()
{
	ShutDown();
}

void CMouse::ShutDown()
{
   if(m_device)
   {
      m_device->Unacquire();
      m_device->Release();
      m_device = NULL;
   }
}
//------------------------------���з���----------------------------------
void CMouse::Update(HWND hwnd)
{

    if(m_device == NULL)
		return;
    //�����״̬
    memcpy(&m_oldMouseState, &m_mouseState, sizeof(m_mouseState));
    //�����ȡʧ�������»�ȡһ��
    if(FAILED(m_device->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseState)))
    {
       if(FAILED(m_device->Acquire()))
		   return ;
       if(FAILED(m_device->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseState)))
           return ;
    }
	POINT pt;
	GetCursorPos(&pt); 
	ScreenToClient(hwnd , &pt);
	m_xDPos  += m_mouseState.lX;
	m_yDPos  += m_mouseState.lY;
	m_xPos   = pt.x;
	m_yPos   = pt.y;
    m_zPos = m_mouseState.lZ;

	//m_xPos += get2DDrawer()->GetCameraPos().x;
	//m_yPos -= get2DDrawer()->GetCameraPos().y;

	if(LeftButtonDown())
	{//�������
		stEvent _event;
		if(m_xPos == m_xPrePos && m_yPos == m_yPrePos)
		{//���δ�ƶ� Ϊ����¼�
			_event.name = string(EVENT_NAME_MOUSE_L_DOWN);
			_event.id = EVENT_ID_MOUSE_L_DOWN;	 
			_event.push_args(stArgs(m_xPos));
			_event.push_args(stArgs(m_yPos));
			getDesktop()->SetUiEvent(true); //Ŀǰ����opengl����Ϊ2DĿǰ��Ȼ����DX��������ʱ�Ȱ�UIע��
			NotifyEvent(_event);
		}
		else
		{//����ƶ��� Ϊ�����ק�¼�
			if(0)//getDesktop()->getDragingWgt() == NULL)
			{
				_event.name = string(EVENT_NAME_MOUSE_L_DRAG);
				_event.id = EVENT_ID_MOUSE_L_DRAG;
				_event.push_args(stArgs(m_xPos - m_xPrePos));
				_event.push_args(stArgs(m_yPos - m_yPrePos)); 
				_event.push_args(stArgs(m_xPos));
				_event.push_args(stArgs(m_yPos));
				//getDesktop()->SetUiEvent(true);
				NotifyEvent(_event);
			}
			else
			{//���������קһ���ؼ���û��Ҫ�ٵ���NotifyEvent(_event);
				//getDesktop()->getDragingWgt()->OffSet(DexPoint(m_xPos - m_xPrePos, m_yPos - m_yPrePos));
			}
		}
		
	}
	else if(LeftButtonUp())
	{
		//getDesktop()->SetDragingWgt(NULL);
		stEvent _event(EVENT_NAME_MOUSE_L_UP, EVENT_ID_MOUSE_L_UP);
		_event.push_args(stArgs(m_xPos));
		_event.push_args(stArgs(m_yPos));
		//getDesktop()->SetUiEvent(true);
		NotifyEvent(_event);
	}
	else if(RightButtonDown())
	{
		stEvent _event;
		if(m_xPos == m_xPrePos && m_yPos == m_yPrePos)
		{//���δ�ƶ� Ϊ�Ҽ��¼�
			_event.name = string(EVENT_NAME_MOUSE_R_DOWN);
			_event.id = EVENT_ID_MOUSE_R_DOWN;	 
			_event.push_args(stArgs(m_xPos));
			_event.push_args(stArgs(m_yPos));
			//getDesktop()->SetUiEvent(true);
			NotifyEvent(_event);
		}
		else
		{//����ƶ��� Ϊ�Ҽ���ק�¼�
			if(0)//getDesktop()->getDragingWgt() == NULL)
			{
				_event.name = string(EVENT_NAME_MOUSE_R_DRAG);
				_event.id = EVENT_ID_MOUSE_R_DRAG;
				_event.push_args(stArgs(m_xPos - m_xPrePos));
				_event.push_args(stArgs(m_yPos - m_yPrePos)); 
				_event.push_args(stArgs(m_xPos));
				_event.push_args(stArgs(m_yPos));
				//getDesktop()->SetUiEvent(true);
				NotifyEvent(_event);
			}
			else
			{
				//getDesktop()->getDragingWgt()->OffSet(DexPoint(m_xPos - m_xPrePos, m_yPos - m_yPrePos));
			}
		}
	}
	else if(RightButtonUp())
	{
		//getDesktop()->SetDragingWgt(NULL);
		stEvent _event(EVENT_NAME_MOUSE_R_UP, EVENT_ID_MOUSE_R_UP);
		_event.push_args(stArgs(m_xPos));
		_event.push_args(stArgs(m_yPos));
		//getDesktop()->SetUiEvent(true);
		NotifyEvent(_event);
	}
	else if(WheelFront())
	{
		stEvent _event(EVENT_NAME_MOUSE_WHEEL_F, EVENT_ID_MOUSE_WHEEL_F);
		//_event.push_args(stArgs(m_xPos));
		//_event.push_args(stArgs(m_yPos));
		//getDesktop()->SetUiEvent(true);	      
		NotifyEvent(_event);
	}
	else if(WheelBack())
	{
		stEvent _event(EVENT_NAME_MOUSE_WHEEL_B, EVENT_ID_MOUSE_WHEEL_B);
		//_event.push_args(stArgs(m_xPos));
		//_event.push_args(stArgs(m_yPos));
		//getDesktop()->SetUiEvent(true);
		NotifyEvent(_event);
	}
	else if(m_xPos != m_xPrePos || m_yPos != m_yPrePos)
	{
		stEvent _event;
		_event.name = string(EVENT_NAME_MOUSE_MOVE);
		_event.id = EVENT_ID_MOUSE_MOVE;
		_event.push_args(stArgs(m_xPos));
		_event.push_args(stArgs(m_yPos));
		getDesktop()->SetUiEvent(true);
		NotifyEvent(_event);
	}
	m_iDeltaX = m_xPos - m_xPrePos;
	m_iDeltaY = m_yPos - m_yPrePos;
	m_xPrePos = m_xPos;
	m_yPrePos = m_yPos;
}
void CMouse::NotifyEvent(stEvent event)
{
	for(std::list<CListener*>::iterator it = m_Listeners.begin(); it != m_Listeners.end(); it++)
	{
		if(!getDesktop()->getUiEvent())
			break; //���û������¼���������¼��ѱ���ȡ�������ٶ�����ؼ��ַ��¼�
		if(*it != NULL)
		{
			(*it)->OnEvent(event);
		}
	}
}
//-----------------------------------------
bool CMouse::MouseMove(Mouse_Move_Dir &mmd, int flag )
{
	if(flag == 0)
	{
		if(   abs(m_xPos - m_xPrePos) > 1
			||abs(m_yPos - m_yPrePos) > 1
			)
		{
			m_xPrePos = m_xPos;
			m_yPrePos = m_yPos;
			return true;
		}
		return false;
	}
	if(flag == 1)
	{
		if(m_xDPos - m_xDPrePos > 0)
		{
			if(m_yDPos - m_yDPrePos > 0)
				mmd = MMD_RIGHT_DOWN;
			else if(m_yDPos - m_yDPrePos == 0)
				mmd = MMD_RIGHT;
			else
				mmd = MMD_RIGHT_UP;
			m_xDPrePos = m_xDPos;
			m_yDPrePos = m_yDPos;
			return true;
		}
		else if(m_xDPos - m_xDPrePos < 0)
		{
			if(m_yDPos - m_yDPrePos > 0)
				mmd = MMD_LEFT_DOWN;
			else if(m_yDPos - m_yDPrePos == 0)
				mmd = MMD_LEFT;
			else
				mmd = MMD_LEFT_UP;
			m_xDPrePos = m_xDPos;
			m_yDPrePos = m_yDPos;
			return true;
		}
		else if(m_yDPos - m_yDPrePos > 0)
		{
			mmd = MMD_DOWN;
			m_xDPrePos = m_xDPos;
			m_yDPrePos = m_yDPos;
			return true;
		}
		else if(m_yDPos - m_yDPrePos < 0)
		{
			mmd = MMD_UP;
			m_xDPrePos = m_xDPos;
			m_yDPrePos = m_yDPos;
			return true;
		}
		else 
			return false;
	}
	return false;
}
BOOL CMouse::LeftButtonDown()
{
    return m_mouseState.rgbButtons[0] & 0x80;
}
BOOL CMouse::RightButtonDown()
{
    return m_mouseState.rgbButtons[1] & 0x80;
}
BOOL CMouse::MiddleButtonDown()
{
    return m_mouseState.rgbButtons[2] & 0x80;
}
BOOL CMouse::LeftButtonUp()
{  
	return (!(m_mouseState.rgbButtons[0] & 0x80) && (m_oldMouseState.rgbButtons[0] & 0x80));
}
BOOL CMouse::RightButtonUp()
{  
	return (!(m_mouseState.rgbButtons[1] & 0x80) && (m_oldMouseState.rgbButtons[1] & 0x80));
}
BOOL CMouse::MiddleButtonUp()
{  
	return (!(m_mouseState.rgbButtons[2] & 0x80) && (m_oldMouseState.rgbButtons[2] & 0x80));
}

bool CMouse::MouseLDrag()
{
	return (m_mouseState.rgbButtons[0] & 0x80)
		  &&(m_iDeltaX != 0 || m_iDeltaX != 0);
}

int CMouse::getDeltaX()
{
	return m_iDeltaX;
}

int CMouse::getDeltaY()
{
	return m_iDeltaY;
}


//=====================================����ϵͳ=========================================
//-----------------���졢������˽�м���������--------------------------

bool CInputSystem::m_bExclusive = false;
CInputSystem::CInputSystem()
{
   m_keyboard = NULL;
   m_mouse = NULL;

   if(DirectInput8Create(DexGameEngine::getEngine()->GetHInstance(), DIRECTINPUT_VERSION,
      IID_IDirectInput8, (void **)&m_system, NULL) == DI_OK)
   {
      m_keyboard = new CKeyboard(m_system, DexGameEngine::getEngine()->GetHwnd());
      m_mouse = new CMouse(m_system, DexGameEngine::getEngine()->GetHwnd(), m_bExclusive);
   }
}

CInputSystem::~CInputSystem()
{
	ShutDown();
}

void CInputSystem::ShutDown()
{
   if(m_keyboard)
   {
      delete m_keyboard;
      m_keyboard = NULL;
   }

   if(m_mouse)
   {
      delete m_mouse;
      m_mouse = NULL;
   }

   if(m_system)
   {
      m_system->Release();
      m_system = NULL;
   }
}

//------------------------------���з���----------------------------------
void CInputSystem::Update(HWND hwnd)
{
   if(m_keyboard != NULL)
	   m_keyboard->Update();
   if(m_mouse != NULL)
	   m_mouse->Update(hwnd);
}
//-----------------------------
BOOL CInputSystem::KeyDown(DUInt32 key)
{
	if(m_keyboard == NULL)
		return FALSE;
	return m_keyboard->KeyDown(key);
}

BOOL CInputSystem::KeyUp(DUInt32 key)
{
	if(m_keyboard == NULL)
		return FALSE;
	return m_keyboard->KeyUp(key);
}

long CInputSystem::GetMouseXPos()
{
	if(m_mouse == NULL)
		return 0;
	return m_mouse->GetXPosition();
}

long CInputSystem::GetMouseYPos()
{
	if(m_mouse == NULL)
		return 0;
	return m_mouse->GetYPosition();
}
long CInputSystem::GetMouseXDPos()
{
	if(m_mouse == NULL)
		return 0;
	return m_mouse->GetXDPosition();
}

long CInputSystem::GetMouseYDPos()
{
	if(m_mouse == NULL)
		return 0;
	return m_mouse->GetYDPosition();
}

BOOL CInputSystem::LeftButtonDown()
{
	if(m_mouse == NULL)
		return FALSE;
	return m_mouse->LeftButtonDown();
}

BOOL CInputSystem::LeftButtonUp()
{
	if(m_mouse == NULL)
		return FALSE;
	return m_mouse->LeftButtonUp();
}

BOOL CInputSystem::RightButtonDown()
{
	if(m_mouse == NULL)
		return FALSE;
	return m_mouse->RightButtonDown();
}

BOOL CInputSystem::RightButtonUp()
{
	if(m_mouse == NULL)
		return FALSE;
	return m_mouse->RightButtonUp();
}

BOOL CInputSystem::MiddleButtonDown()
{
	if(m_mouse == NULL)
		return FALSE;
	return m_mouse->MiddleButtonDown();
}

BOOL CInputSystem::MiddleButtonUp()
{
	if(m_mouse == NULL)
		return FALSE;
	return m_mouse->MiddleButtonUp();
}

int CInputSystem::MouseDeltaX()
{
	int iDeltaX = 0;
	if (m_mouse != nullptr)
		iDeltaX = m_mouse->getDeltaX();
	return iDeltaX;
}
int CInputSystem::MouseDeltaY()
{
	int iDeltaY = 0;
	if (m_mouse != nullptr)
		iDeltaY = m_mouse->getDeltaY();
	return iDeltaY;
}

bool CInputSystem::WheelFront()
{
	if(m_mouse == NULL)
		return false;
	return m_mouse->WheelFront();
}

bool CInputSystem::WheelBack()
{
	if(m_mouse == NULL)
		return false;
	return m_mouse->WheelBack();
}

bool CInputSystem::MouseMove(Mouse_Move_Dir &mmd, int flag ) 
{
	if(m_mouse == NULL)
		return false;
	return m_mouse->MouseMove(mmd, flag);
}



