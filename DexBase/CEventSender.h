#pragma  once
#include <list>
#include "CListener.h"
class CEventSender
{//所有能够发出事件信息的对象都从此派生
protected:
	std::list<CListener*> m_Listeners;

public:
	CEventSender();
	~CEventSender();

public:
	virtual void AddListener(CListener* listener);
	virtual void RemoveListener(CListener* listener);
	virtual void NotifyEvent(stEvent event);
};