#pragma  once
#include <list>
#include "CListener.h"
class CEventSender
{//�����ܹ������¼���Ϣ�Ķ��󶼴Ӵ�����
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