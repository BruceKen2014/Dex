
#ifndef _DEX_DELEGATE_H
#define _DEX_DELEGATE_H
#include <list>
#include <map>
#include "CListener.h"
//using namespace std;
class CEventHandler;
class CDexObject;
class CDelegate
{
public:
	CDelegate();
	virtual ~CDelegate();

public:
	virtual void Handle(CDexObject* object, stEvent event) = 0;
};
class CDelegateG:public CDelegate
{//挂接全局函数
public:
	typedef void  (*EventCallBack)(CDexObject*,stEvent);
	EventCallBack m_fun;
public:
	CDelegateG();
	CDelegateG(EventCallBack fun);
	virtual ~CDelegateG();

public:
	CDelegateG& operator = (const CDelegateG& dgt);
	virtual void Handle(CDexObject* object,stEvent event);

};
class CDelegateM:public CDelegate
{//挂接成员函数
public:
	typedef void  (CEventHandler::*EventCallBack)(CEventHandler* sys, CDexObject*, stEvent);
	CEventHandler* m_gameSys; //处理事件的系统
	EventCallBack  m_fun;	  //系统内函数  

public:
	CDelegateM()
/*	{
		m_gameSys = NULL;
		m_fun = NULL;
	}*/;
	CDelegateM( CEventHandler* sys, EventCallBack fun )
/*	{
		m_gameSys = sys;
		m_fun = fun;
	}*/;
	virtual ~CDelegateM()
/*	{

	}*/;
	CDelegateM& operator = (const CDelegateM& dgt);

	void Handle(CDexObject* object,stEvent event);
};

class CEventDispather
{//一个UI一个事件可以有多个函数来进行处理
	//如一个面板中，当一个按钮按下时，一个函数进行一个功能处理，另一个函数进行另一个功能处理
private:
	std::list<CDelegate*> m_delegates;

public:
	CEventDispather();
	~CEventDispather();

public:
	void Handle(CDexObject* object,stEvent& event);
	CEventDispather& operator += (CDelegate* dlg);
	void Add(CDelegate* dlg); //效果等同重载的+=
};

class CEventSystem
{
private:
	typedef std::map<int, CEventDispather*> TMapEvent;
	TMapEvent m_mapEvent;
public:
	static CEventSystem* getEventSystem();
	bool registerEvent(int id,CDelegate* dlg);
	bool sendEvent(int eventId);
	bool sendEvent(stEvent event);
};
#define SYSTEM_EVENT_REGISTER_M(id, gamesys, clas, fun)\
{\
	CDelegateM* dlg = new CDelegateM(gamesys, (CDelegateM::EventCallBack)(&clas::fun));\
	CEventSystem::getEventSystem()->registerEvent(id, dlg);\
}
#define SYSTEM_EVENT_REGISTER_G(fun)\
{\
	CDelegateG* dlg = new CDelegateG((CDelegateG::EventCallBack)(&fun));\
	CEventSystem::getEventSystem()->registerEvent(id, dlg);\
}


#define UI_DELEGATE(gamesys, clas, fun) \
	(new CDelegateM(gamesys, (CDelegateM::EventCallBack)(&clas::fun)))

#define UI_DELEGATE_G(fun)       \
	(new CDelegateG((CDelegateG::EventCallBack)(&fun)))

#endif