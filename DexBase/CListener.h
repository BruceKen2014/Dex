

#pragma  once

#include <list>
#include <deque>

#define  VALIDE_EVENT -1

using namespace std; 
class CEventSender;


#define  EVENT_NAME_KEY_DOWN        "���̰�ť����"
#define  EVENT_NAME_KEY_UP          "���̰�ť����"
#define  EVENT_NAME_KEY_CHAR        "�ַ�����"
#define  EVENT_NAME_MOUSE_MOVE      "����ƶ�"
#define  EVENT_NAME_MOUSE_L_UP      "����������"
#define  EVENT_NAME_MOUSE_L_DOWN    "����������"
#define  EVENT_NAME_MOUSE_R_UP      "����Ҽ�����"
#define  EVENT_NAME_MOUSE_R_DOWN    "����Ҽ�����"
#define  EVENT_NAME_MOUSE_M_UP      "�����ֵ���"
#define  EVENT_NAME_MOUSE_M_DOWN    "�����ְ���"
#define  EVENT_NAME_MOUSE_WHEEL_F   "������ǰ��"
#define  EVENT_NAME_MOUSE_WHEEL_B   "�����ֺ��"
#define  EVENT_NAME_MOUSE_L_DRAG    "��������ק"
#define  EVENT_NAME_MOUSE_R_DRAG    "����Ҽ���ק"

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
	//�����Ƚ����
	std::deque<stArgs> args;
public:
	_stEvent() { name = string("��Ч�¼�");id = VALIDE_EVENT;}
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
{//������Ҫ��������������඼�Ӵ�����
protected:
	std::list<CEventSender*> m_vSender; //�����Ķ���

public:
	CListener();
	virtual ~CListener();
public:
	virtual void ShutDown();
public:
	virtual bool ListenThis(CEventSender* obj){return true;}; 
	virtual bool OnEvent(stEvent event){ return false;};	 //�������¼���
	virtual bool EventValid(stEvent event) { return true;};   //�����Ƿ�����¼�
};
