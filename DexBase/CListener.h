

#pragma  once

#include <list>
#include <deque>

#define  VALIDE_EVENT -1

using namespace std; 
class CEventSender;


#define  EVENT_NAME_KEY_DOWN        "键盘按钮按下"
#define  EVENT_NAME_KEY_UP          "键盘按钮弹起"
#define  EVENT_NAME_KEY_CHAR        "字符输入"
#define  EVENT_NAME_MOUSE_MOVE      "鼠标移动"
#define  EVENT_NAME_MOUSE_L_UP      "鼠标左键弹起"
#define  EVENT_NAME_MOUSE_L_DOWN    "鼠标左键按下"
#define  EVENT_NAME_MOUSE_R_UP      "鼠标右键弹起"
#define  EVENT_NAME_MOUSE_R_DOWN    "鼠标右键按下"
#define  EVENT_NAME_MOUSE_M_UP      "鼠标滚轮弹起"
#define  EVENT_NAME_MOUSE_M_DOWN    "鼠标滚轮按下"
#define  EVENT_NAME_MOUSE_WHEEL_F   "鼠标滚轮前滚"
#define  EVENT_NAME_MOUSE_WHEEL_B   "鼠标滚轮后滚"
#define  EVENT_NAME_MOUSE_L_DRAG    "鼠标左键拖拽"
#define  EVENT_NAME_MOUSE_R_DRAG    "鼠标右键拖拽"

enum
{
	EVENT_ID_INPUT = 100,
	EVENT_ID_KEY_DOWN,
	EVENT_ID_KEY_UP,
	EVENT_ID_KEY_CHAR,
	EVENT_ID_MOUSE_MOVE,
	EVENT_ID_MOUSE_L_UP,
	EVENT_ID_MOUSE_L_DOWN,
	EVENT_ID_MOUSE_R_UP,
	EVENT_ID_MOUSE_R_DOWN,
	EVENT_ID_MOUSE_M_UP,
	EVENT_ID_MOUSE_M_DOWN,
	EVENT_ID_MOUSE_WHEEL_F,
	EVENT_ID_MOUSE_WHEEL_B,
	EVENT_ID_MOUSE_L_DRAG, 
	EVENT_ID_MOUSE_R_DRAG,
};
typedef struct _Args
{
	union
	{
		int i_member;
		float f_member;
		void* str_member;
	};
	_Args()  {} 
	_Args(int i)           { i_member = i;};
	_Args(float f)         { f_member = f;};
	_Args(double f)         { f_member = (float)f;};
	_Args(const char* data){ str_member = reinterpret_cast<void*>((char*)data);};
}stArgs;


typedef struct _stEvent
{
	string name;
	int    id;
protected:
	//参数先进后出
	std::deque<stArgs> args;
public:
	_stEvent() { name = string("无效事件");id = VALIDE_EVENT;}
	_stEvent(string nm, int Id):name(nm), id(Id){}
	void push_args(stArgs arg)
	{
		args.emplace_back(arg);
	}
	void pop_args(stArgs& arg)
	{
		if(!args.empty())
		{
			arg = args.back();
		    args.pop_back();
		}
	}
	size_t args_count()
	{
		return args.size();
	}
	stArgs back_args()
	{
		stArgs ret;
		if (!args.empty())
			ret = args.back();
		return ret;
	}
}stEvent;

class CListener
{//所有需要监听其他对象的类都从此派生
protected:
	std::list<CEventSender*> m_vSender; //监听的对象

public:
	CListener();
	virtual ~CListener();
public:
	virtual void ShutDown();
public:
	virtual bool ListenThis(CEventSender* obj){return true;}; 
	virtual bool OnEvent(stEvent event){ return false;};	 //监听到事件，
	virtual bool EventValid(stEvent event) { return true;};   //决定是否处理该事件
};
