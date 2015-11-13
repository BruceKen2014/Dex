
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
{//�ҽ�ȫ�ֺ���
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
{//�ҽӳ�Ա����
public:
	typedef void  (CEventHandler::*EventCallBack)(CEventHandler* sys, CDexObject*, stEvent);
	CEventHandler* m_gameSys; //�����¼���ϵͳ
	EventCallBack  m_fun;	  //ϵͳ�ں���  

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
{//һ��UIһ���¼������ж�����������д���
	//��һ������У���һ����ť����ʱ��һ����������һ�����ܴ�����һ������������һ�����ܴ���
private:
	std::list<CDelegate*> m_delegates;

public:
	CEventDispather();
	~CEventDispather();

public:
	void Handle(CDexObject* object,stEvent& event);
	CEventDispather& operator += (CDelegate* dlg);
	void Add(CDelegate* dlg); //Ч����ͬ���ص�+=
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