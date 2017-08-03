
//#include "CEventSender.h"
#include "CDelegate.h"
#include "CEventHandler.h"
#include "DexLog.h"


CDelegate::CDelegate()
{

}
CDelegate::~CDelegate()
{

}

//==================================================

CDelegateG::CDelegateG()
{
	m_fun = NULL;
}

CDelegateG::CDelegateG(EventCallBack fun)
{
	m_fun = fun;
}
CDelegateG::~CDelegateG()
{
}


void CDelegateG::Handle(CDexObject* object, stEvent event )
{
	if(m_fun != NULL)
		m_fun(object, event);
}

CDelegateG& CDelegateG::operator=( const CDelegateG& dgt )
{
	m_fun = dgt.m_fun;
	return *this;
}


CDelegateM::CDelegateM()
{
	m_gameSys = NULL;
	m_fun = NULL;
}

CDelegateM::CDelegateM( CEventHandler* sys, EventCallBack fun )
{
	m_gameSys = sys;
	m_fun = fun;
}
CDelegateM::~CDelegateM()
{

}
CDelegateM& CDelegateM::operator=( const CDelegateM& dgt )
{
	m_gameSys = dgt.m_gameSys;
	m_fun = dgt.m_fun;
	return *this;
}
void CDelegateM::Handle(CDexObject* object, stEvent event )
{
	if(m_gameSys != NULL && m_fun != NULL)
	{
		(m_gameSys->*m_fun)(m_gameSys, object,event);
	}
}


//-------------------------------
CEventDispather::CEventDispather()
{

}
CEventDispather::~CEventDispather()
{
	for(std::list<CDelegate*>::iterator it = m_delegates.begin(); it != m_delegates.end();)
	{
		if(*it != NULL)
		{
			delete *it;
			*it = NULL;
			m_delegates.erase(it);
			it = m_delegates.begin();
		}
	}
}

void CEventDispather::Handle(CDexObject* object,stEvent& event)
{
	for(std::list<CDelegate*>::iterator it = m_delegates.begin(); it != m_delegates.end(); it++)
	{
		if(*it != NULL)
		{
			(*it)->Handle(object, event);
		}
	}
}
CEventDispather& CEventDispather::operator +=(CDelegate* dlg)
{
	if(dlg != NULL)
	{	
		bool isin = false;
		for(std::list<CDelegate*>::iterator it = m_delegates.begin(); it != m_delegates.end(); it++)
		{
			if(*it == dlg)
			{
				isin = true;
				break;
			}
		}
		if(!isin)
			m_delegates.push_back(dlg);
	}
	return *this;
}
void CEventDispather::Add(CDelegate* dlg)
{
	if(dlg == NULL)
		return;
	bool isin = false;
	for(std::list<CDelegate*>::iterator it = m_delegates.begin(); it != m_delegates.end(); it++)
	{
		if(*it == dlg)
		{
			isin = true;
			break;
		}
	}
	if(!isin)
		m_delegates.push_back(dlg);
}

CEventSystem* CEventSystem::getEventSystem()
{
	static CEventSystem* system = NULL;
	if(system == NULL)
		system = new CEventSystem;
	return system;
}

bool CEventSystem::registerEvent(int id,CDelegate* dlg)
{
	//是不要要用object工厂管理呢？暂时算了，基本一次性的东西，就不管理了
	//Dex_QueryObject();
	if(m_mapEvent.find(id) == m_mapEvent.end())
	{
		m_mapEvent[id] = new CEventDispather;
	}
	m_mapEvent[id]->Add(dlg);
	return true;
}

bool CEventSystem::sendEvent(int eventId)
{
	stEvent event;
	event.id = eventId;
	return sendEvent(event);
}

bool CEventSystem::sendEvent(stEvent event)
{
	if(m_mapEvent.find(event.id) == m_mapEvent.end())
	{
		DexLog::getSingleton()->Log(log_allert, "事件系统发送了一个没有处理系统的事件");
		return false;
	}
	m_mapEvent[event.id]->Handle(NULL, event);
	return true;
}